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
    VWBG* top = new VWBG;               // 建立模組實例
    top -> trace(tfp, 0);                   // 將模組訊號掛載到波形物件上，0 表示記錄所有層級
    tfp -> open("wave.vcd");                // 開啟波形檔案

    top -> clk = 0;                         // 時鐘初始值
    printf("-------------------------------------------\n");
    while( sc_time_stamp(time_len + 1) < time_len && !Verilated::gotFinish() ){
        top -> clk = !top -> clk;           // 時鐘翻轉
        top -> eval();                      // 計算模組內部訊號狀態
        tfp -> dump(main_time);             // 記錄波形資料

        if(main_time & 1){                 // 每兩個時間單位翻轉一次時鐘, 相當於 main_time % 2 == 1
            unsigned char lfsr_val = top -> lfsr_out; // 取得 LFSR 的值
            printf("time=%llu, \nLFSR = \n", main_time); // 印出LFSR状态
            for(int i = 3; i >= 0; i--){
                printf("%d", (lfsr_val >> i) & 1);
            }
            printf("\n");
        }


        main_time++;                        // 時間遞增
    }

    printf("-------------------------------------------\n");
    printf("模擬結束");

    top -> final();                         // 模組結束前的清理工作
    tfp -> close();                         // 關閉波形檔案
    delete tfp;                             // 釋放 trace wave 物件
    delete top;                             // 釋放模組實例
    return 0;
}