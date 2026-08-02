#include <stdio.h>
#include <stdlib.h>
#include "VWBG.h"
#include "verilated_vcd_c.h"

vluint64_t main_time = 0;                       // 模擬計時器
int time_len = 32;                              // 模擬時間長度

double sc_time_stamp(int len)
{
    return (double)(main_time % len);
}

int main(int argc, char **argv){
    Verilated::commandArgs(argc, argv);     // 解析 command line 參數
    Verilated::traceEverOn(true);           // 開啟 trace 功能

    VerilatedVcdC* tfp = new VerilatedVcdC; // 建立 trace wave 物件
    VWBG* top = new VWBG;                   // 建立模組實例
    top -> trace(tfp, 0);                   // 將模組訊號掛載到波形物件上，0 表示記錄所有層級
    tfp -> open("wave.vcd");                // 開啟波形檔案

    int sum_of_sn = 0;                      // 用於計算 SNG 輸出為 1 的次數

    top -> clk = 0;                         // 時鐘初始值
    printf("-------------------------------------------\n");

    printf("Binary Number = %d\n", top -> bn);
    printf("BN in bits = %d%d%d%d\n", (top->bn >> 3) & 1, (top->bn >> 2) & 1, (top->bn >> 1) & 1, (top->bn >> 0) & 1);
    while( sc_time_stamp(time_len + 1) < time_len && !Verilated::gotFinish() ){
        top -> clk = !top -> clk;           // 時鐘翻轉
        top -> eval();                      // 計算模組內部訊號狀態
        tfp -> dump(main_time);             // 記錄波形資料

        if(main_time & 1){                  // 每兩個時間單位翻轉一次時鐘, 相當於 main_time % 2 == 1
            printf("time=%llu, SNG output = %d\n", main_time, top -> sn);
            sum_of_sn += top -> sn;         // 累加 SNG 輸出為 1 的次數
        }


        main_time++;                        // 時間遞增
    }

    printf("-------------------------------------------\n");
    printf("模擬結束");
    printf("SNG output 為 1 的次數 = %d\n", sum_of_sn);
    printf("理論值 = %d/%d = %f\n", top -> bn, 16, (double)(top -> bn) / 16);
    printf("實際值 = %d/%d = %f\n", sum_of_sn, 16, (double)sum_of_sn / 16); 

    top -> final();                         // 模組結束前的清理工作
    tfp -> close();                         // 關閉波形檔案
    delete tfp;                             // 釋放 trace wave 物件
    delete top;                             // 釋放模組實例
    return 0;
}