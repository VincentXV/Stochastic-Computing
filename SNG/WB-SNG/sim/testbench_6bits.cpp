#include <stdio.h>
#include <stdlib.h>
#include "VWBG.h"
#include "verilated_vcd_c.h"

vluint64_t main_time = 0;                   // 模擬計時器
int time_len = 128;                         // 模擬時間長度 64 clock

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
    int stop_per_5[13] = {0};               // 用於計算每 5 個 clock 的 SNG 輸出為 1 的次數
    int round = 0;                          // 用於計算模擬的輪數

    top -> clk = 0;                         // 時鐘初始值
    top -> rst = 1;                         // 設定模組的重置訊號為高電位

    int input_bn = 0b101100;                // 設定輸入的二進位數字
    int input_lfsr = 0b000010;              // 設定 LFSR 的初始值

    if (argc > 1){
        try{
            printf("輸入的 1st BN參數為: %s\n", argv[1]);
            input_bn = std::stoi(argv[1], nullptr, 2);      // 將輸入的二進位字串轉換為整數

            printf("解析後的十進位值為: %d\n", input_bn);
            printf("還原為六位元二進位: 0b%s\n\n", std::bitset<6>(input_bn).to_string().c_str());

            printf("輸入的 2nd LFSR參數為: %s\n", argv[2]);
            input_lfsr = std::stoi(argv[2], nullptr, 2);    // 將輸入的二進位字串轉換為整數

            printf("解析後的十進位值為: %d\n", input_lfsr);
            printf("還原為六位元二進位: 0b%s\n", std::bitset<6>(input_lfsr).to_string().c_str());

        } catch (const std::invalid_argument& e) {
            printf("無效的輸入，請輸入兩個二進位字串\n");
            return 1;
        }
    }
    else{
        printf("使用預設初始值:\n");
        printf("Binary Number = 0b101100 = 44\n");
        printf("LFSR = 0b000010 = 2\n");

    }

    printf("-------------------------------------------\n");
    printf("模擬開始 \n");

    top -> bn = input_bn;                   // 設定模組的輸入值
    top -> seed_in = input_lfsr;               // 設定模組的 LFSR 初始值

    printf("Binary Number = %d\n", top -> bn);
    printf("BN in bits = 0b%s\n", std::bitset<6>(top -> bn).to_string().c_str());
    printf("LFSR seed = %d\n", top -> seed_in);
    printf("seed in bits = 0b%s\n", std::bitset<6>(top -> seed_in).to_string().c_str());

    // ==========================================
    // 階段一：開始階段 (Start Phase)
    // 獨立執行前 3 個 tick (包含 clock 反轉與波形紀錄)
    // ==========================================
    for (int i = 0; i < 3; i++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(main_time);
        
        if(main_time & 1){                  // 每兩個時間單位翻轉一次時鐘, 相當於 main_time % 2 == 1
            //printf("time=%llu, SNG output = %d\n", main_time/2+1, top -> sn);
            sum_of_sn += top -> sn;         // 累加 SNG 輸出為 1 的次數
        }

        main_time++;
    }

    // 開始階段結束，正式放開 rst 訊號
    top->rst = 0;

    // ==========================================
    // 階段二：主模擬階段 (Main Simulation Phase)
    // 核心迴圈不再包含多餘的 if 判斷
    // ==========================================
    while( sc_time_stamp(time_len + 1) < time_len && !Verilated::gotFinish() ){
        top -> clk = !top -> clk;           // 時鐘翻轉
        top -> eval();                      // 計算模組內部訊號狀態
        tfp -> dump(main_time);             // 記錄波形資料

        if(main_time & 1){                  // 每兩個時間單位翻轉一次時鐘, 相當於 main_time % 2 == 1
            //printf("time=%llu, SNG output = %d\n", main_time/2+1, top -> sn);
            sum_of_sn += top -> sn;         // 累加 SNG 輸出為 1 的次數
        }
        if(main_time % 10 == 0){              // 每 5 個 clock 紀錄一次 SNG 輸出
            stop_per_5[main_time/10] = sum_of_sn;
        }

        main_time++;                        // 時間遞增
        
    }

    printf("-------------------------------------------\n");
    printf("模擬結束");

    printf("SNG output 為 1 的次數 = %d\n", sum_of_sn);
    printf("理論值 = %d/%d = %f\n", top -> bn, 64, (double)(top -> bn) / 64);
    printf("實際值 = %d/%d = %f\n", sum_of_sn, 64, (double)sum_of_sn / 64); 
    
    stop_per_5[12] = sum_of_sn;
    double algo_result = (double)(top -> bn) / 64;
    double test_result = 0;

    for(int i = 0; i < 13; i++){
        printf("停止在第 %2d 的 SNG output 為 1 的次數 = %d\n", (i+1)*5, stop_per_5[i]);
        test_result = (double)stop_per_5[i] / 64;
        printf("測試值 = %d/%d = %f\n", stop_per_5[i], 64, test_result);
        printf("Accuracy = %f\n", 1.0 - abs(test_result - algo_result) / algo_result);
    }

    top -> final();                         // 模組結束前的清理工作
    tfp -> close();                         // 關閉波形檔案
    delete tfp;                             // 釋放 trace wave 物件
    delete top;                             // 釋放模組實例
    return 0;
}