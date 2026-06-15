# LFSR 8 bits 架構程式碼
reference code: https://hackmd.io/BiB4yBbkTKyawxx9fLA5Lw?view#LFSR

---
## LFSR 程式調整
1. `LFSR8.v` shift調整
   * 原先的設計是將 LFSR[7] 作為回饋源，並由 LFSR[0] 向 LFSR[7] 移位。重構後的目標是將移位方向反轉，並調整回饋計算方式
   * 將原本 [0] -> shift ->[7]
     * `LFSR[0] <= feedback;`
     * `LFSR[2] <= LFSR[1] ^ feedback;`
       * Note: `<=`  為非阻塞賦值
   * 改為 [7] -> shilf -> [0]
     * `feedback = LFSR[0] ^ LFSR[1]`
     * `LFSR[7] <= feedback;`
   * 目的：
     * 為將輸出轉為常用邏輯
        ```bash
        time=1, out = 
        10001010
        time=3, out = 
        11000101
        time=5, out = 
        11100010 
        ```
2. testbench.cpp 邏輯
   * 大部分可以去看註解
   * 重點講解clock部分
     * 由於電路是positive edge trigger
     * time = 0 時，LFSR尚未賦予初始值，因此out = 0000 0000
     * time++，相當於clock invert，因此每兩個clock會有一次觸發
1. tesetbench.cpp 微小注意點（Warning）
   * `vluint64_t main_time = 0;` 
     * printf時，型別從`%ld` 改為 `%llu`


