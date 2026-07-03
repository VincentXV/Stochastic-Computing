#include <stdio.h>
#include <stdlib.h>
//#include "VOmega_L.h"
#include "VFlip_L.h"
#include "verilated_vcd_c.h"

// 模板文件，請根據實際情況修改
// 如：top 需改成要測試的模組名稱、sc_time_stamp 的參數len 需改成需要測試的時間週期
// Note: 目前clk的產生是一高一低，因此main_time += 2時，才產生一個完整的clk

vluint64_t main_time = 0;                       // 模擬計時器
int time_len = 2;                               // 模擬時間長度

double sc_time_stamp(int len)
{
    return (double)(main_time % len);
}

int main(int argc, char **argv){
    Verilated::commandArgs(argc, argv);         // 解析 command line 參數
    Verilated::traceEverOn(true);               // 開啟 trace 功能

    VerilatedVcdC* tfp = new VerilatedVcdC;     // 建立 trace wave 物件
    //VOmega_L* top = new VOmega_L;               // 建立模組實例
    VFlip_L* top = new VFlip_L;               // 建立模組實例
    top -> trace(tfp, 0);                       // 將模組訊號掛載到波形物件上，0 表示記錄所有層級
    tfp -> open("wave.vcd");                    // 開啟波形檔案

    
    int input = 0b11110000;                      // 預設初始值=0x78=120
    int sel = 0b0000;                            // 預設初始值=0x0=0
    if (argc > 1){
        try{
            printf("輸入的參數為: %s %s\n", argv[1], argv[2]);
            input = std::stoi(argv[1], nullptr, 2); // 將輸入的二進位字串轉換為整數
            sel = std::stoi(argv[2], nullptr, 2);   // 將選擇訊號的二進位字串轉換為整數
            printf("輸入參數字串為: %s\n", argv[1]);
            printf("解析後的十進位值為: %d\n", input);
            printf("解析後的十六進位為: 0x%X\n", input);
            printf("還原為八位元二進位: 0b%s\n\n", std::bitset<8>(input).to_string().c_str());

            printf("選擇參數字串為: %s\n", argv[2]);
            printf("解析後的十進位值為: %d\n", sel);
            printf("解析後的十六進位為: 0x%X\n", sel);
            printf("還原為八位元二進位: 0b%s\n", std::bitset<4>(sel).to_string().c_str());
        } catch (const std::invalid_argument& e) {
            printf("無效的輸入，請輸入兩個二進位字串\n");
            return 1;
        }
    }
    else{
        printf("使用預設初始值:input = 0x78 = 0b 1111 0000\n");
        printf("及sel = 0x0 = 0b0000\n");
    }
    top -> I = input & 0xFF;                    // 設定模組輸入訊號
    top -> sel = sel & 0xF;                     // 設定模組選擇訊號

    while( sc_time_stamp(time_len + 1) < time_len && !Verilated::gotFinish() ){
        top -> eval();                          // 計算模組內部訊號狀態
        tfp -> dump(main_time);                 // 記錄波形資料

        main_time++;                            // 時間遞增
    }

    printf("-------------------------------------------\n");
    printf("模擬結束");

    top -> final();                             // 模組結束前的清理工作
    tfp -> close();                             // 關閉波形檔案
    delete tfp;                                 // 釋放 trace wave 物件
    delete top;                                 // 釋放模組實例
    return 0;
}