#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include "VBSNG.h"
#include "verilated_vcd_c.h"

vluint64_t main_time = 0;                    // 模擬計時器
int time_len = 32;                           // 模擬時間長度
int bit_len = 4;                             // 模擬輸入位元長度

double sc_time_stamp(int len)
{
    return (double)(main_time % len);
}

int main(int argc, char **argv){
    Verilated::commandArgs(argc, argv);     // 解析 command line 參數
    Verilated::traceEverOn(true);           // 開啟 trace 功能

    VerilatedVcdC* tfp = new VerilatedVcdC; // 建立 trace wave 物件
    VBSNG* top = new VBSNG;                 // 建立模組實例
    top -> trace(tfp, 0);                   // 將模組訊號掛載到波形物件上，0 表示記錄所有層級
    tfp -> open("wave.vcd");                // 開啟波形檔案

    int sum_of_sn = 0;                      // 用於計算 SNG 輸出為 1 的次數

    int input = 0b0101;                     // （有號數）BN 預設初始值 = 5

    if (argc > 1){
        try{
            size_t input_len = 0;
            printf("輸入的參數為: %s\n", argv[1]);
            input = std::stoi(argv[1], &input_len, 2); // 將輸入的二進位字串轉換為整數

            // 讀取有號數
            if(input_len != bit_len){
                printf("輸入的二進位字串長度不正確，請輸入 %d 位元的二進位字串\n", bit_len);
                return 1;
            }

            int sign_mask = 1 << (bit_len - 1); // 4-bit 時為 1000 (8)
            if (input & sign_mask) {
                input -= (1 << bit_len); // 將輸入轉換為有號數，4-bit 時為 16 ＝ 10000
            }

            printf("解析後的十進位值為: %d\n", input);
            printf("還原為%d位元二進位: 0b%s\n\n", bit_len, std::bitset<4>(input).to_string().c_str());

        } catch (const std::invalid_argument& e) {
            printf("無效的輸入，請輸入二進位字串\n");
            return 1;
        }
    }
    else{
        printf("使用預設初始值:input = 0b0101 = 5\n");
    }
    
    printf("-------------------------------------------\n");
    printf("開始模擬\n");
    top -> clk = 0;                         // 時鐘初始值
    top -> bn = input;                  // 設定模組的輸入值

    printf("SN = ");

    while( sc_time_stamp(time_len + 1) < time_len && !Verilated::gotFinish() ){
        top -> clk = !top -> clk;           // 時鐘翻轉
        top -> eval();                      // 計算模組內部訊號狀態
        tfp -> dump(main_time);             // 記錄波形資料

        main_time++;                        // 時間遞增

        if( (int)main_time & 1 && main_time < 30 ){
            printf("%d", top -> sn);
            if(top -> sn == 1){
                sum_of_sn++;
            }
        }
    }

    printf("\n-------------------------------------------\n");
    printf("模擬結束\n");
    printf("SN = 1 的次數: %d\n", sum_of_sn);
    printf("實際值 p  = %d/%d\n", sum_of_sn, 16);
    printf("       p* = 2p - 1 = (%d - %d) / %d = %d/%d\n", (2 * sum_of_sn), 16, 16, (2 * sum_of_sn - 16), 16);
    printf("理論值 p*  = %d/%d\n", input, 8);
    printf("       p = (p* + 1)/2 = ((%d / %d) + 1) / 2 = %d / %d\n", input, 8, input + 8, 16);

    top -> final();                         // 模組結束前的清理工作
    tfp -> close();                         // 關閉波形檔案
    delete tfp;                             // 釋放 trace wave 物件
    delete top;                             // 釋放模組實例
    return 0;
}