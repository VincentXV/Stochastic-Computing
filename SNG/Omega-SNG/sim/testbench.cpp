#include <stdio.h>
#include <stdlib.h>
#include "Vtop.h"
#include "verilated_vcd_c.h"

/* Omega-Flip 模組測試檔案
// 可使用參數輸入Binary的值，若未輸入則使用預設值0b100
// 並使用波形檔案wave.vcd記錄模擬過程
// 及使用record陣列記錄每次的結果，用於分析correlation，來判斷此模組結果是否夠亂
*/

vluint64_t main_time = 0;                        // 模擬計時器
int time_len = 32;                               // 模擬時間長度

double sc_time_stamp(int len)
{
    return (double)(main_time % len);
}

int main(int argc, char **argv){
    Verilated::commandArgs(argc, argv);     // 解析 command line 參數
    Verilated::traceEverOn(true);           // 開啟 trace 功能

    VerilatedVcdC* tfp = new VerilatedVcdC; // 建立 trace wave 物件
    Vtop* top = new Vtop;                   // 建立模組實例
    top -> trace(tfp, 0);                   // 將模組訊號掛載到波形物件上，0 表示記錄所有層級
    tfp -> open("wave.vcd");                // 開啟波形檔案

    int c = 0;                              // 記錄1的個數
    int record[15]={};                      // 記錄每次的結果
    int sum = 0;                            // 記錄correlation的1的總數
    int table[8] = {0};                     // 記錄乘法結果

    int input = 0b100;                      // 預設初始值 = 4

    if (argc > 1){
        try{
            printf("輸入的參數為: %s\n", argv[1]);
            input = std::stoi(argv[1], nullptr, 2); // 將輸入的二進位字串轉換為整數

            printf("輸入參數字串為: %s\n", argv[1]);
            printf("解析後的十進位值為: %d\n", input);
            printf("還原為八位元二進位: 0b%s\n\n", std::bitset<3>(input).to_string().c_str());

        } catch (const std::invalid_argument& e) {
            printf("無效的輸入，請輸入二進位字串\n");
            return 1;
        }
    }
    else{
        printf("使用預設初始值:input = 0x4 = 0b100\n");
    }
    printf("-------------------------------------------\n");
    printf("模擬開始\n");
    top -> clk = 0;                                 // 時鐘初始值
    top -> Binary = input;                          // 設定模組的輸入值

    while( sc_time_stamp(time_len + 1) < time_len && !Verilated::gotFinish() ){
        top -> clk = !top -> clk;                   // 時鐘翻轉
        top -> eval();                              // 計算模組內部訊號狀態
        tfp -> dump(main_time);                     // 記錄波形資料
        if((int)(main_time % 2) == 1 && main_time < 30) {                    // 在時鐘上升沿時，記錄模組的輸出值
            record[main_time / 2] = top -> SN;      // 記錄每次的結果
            printf("""第 %2d 次模擬結果: SN = 0b %s\n", (int)(main_time / 2)+1, std::bitset<8>(top -> SN).to_string().c_str());
        }
        main_time++;                                // 時間遞增
    }
    /*
    printf("output = ");                            // 輸出結果
    for (int i = 0; i < 8; i++) {
        printf("%d", (top->SN >> i) & 1);           // 輸出每個位元的值
        if((top->SN >> i) & 1) {
            c++; // 計算1的個數
        }
    }

    printf("\n");
    printf("1的個數為: %d\n", c);                // 輸出1的個數
    */
    printf("-------------------------------------------\n");
    printf("模擬結束");

    top -> final();                             // 模組結束前的清理工作
    tfp -> close();                             // 關閉波形檔案
    delete tfp;                                 // 釋放 trace wave 物件
    delete top;                                 // 釋放模組實例

    printf("Correlatin 驗算\n");
    printf("input = %d\n", input);
    printf("運算結果理論值為: 0.5 * 0.5 = 0.25 = 4/8 = SN有4個1\n");
    printf("實驗結果驗算如下：\n");
    for (int i = 0; i < 15; i++) {
        for(int j = i + 1; j < 15; j++){
            sum = 0;                            // 重置sum計數器
            printf("-------------------------------------------\n");

            printf("第%2d 個數字 %s 與第%2d 個數字 %s 的 correlation = \n", i + 1, std::bitset<8>(record[i]).to_string().c_str(), j + 1, std::bitset<8>(record[j]).to_string().c_str());

            for (int k = 0; k < 8; k++) {
                if((record[i] >> k) & 1 && (record[j] >> k) & 1) { // AND 為 SN 乘法
                    sum++; // 計算1的個數
                }
            }

            table[sum]++;                      // 統計乘法結果
            printf("左式 = %d\n", sum);
            printf("右式 = (%d * %d ) / 8 = %d\n", input, input, ((input * input) / 8));
        }
    }

    printf("-------------------------------------------\n");
    printf("乘法結果統計\n");
    printf("(%d * %d ) / 8 = \n", input, input);
    for (int i = 0; i < 8; i++) {
        printf("為 %d 的組合有 %d 組\n", i, table[i]);
    }
    return 0;
}