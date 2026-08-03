Reference:
https://hackmd.io/BiB4yBbkTKyawxx9fLA5Lw?view

LFSR & WBG - https://hackmd.io/@Vincent1999/BJUiP_zrgg/edit



Compile:
1. `verilator -Wall --cc --exe top.v testbench.cpp --trace --fno-dfg`
    * 可再加`--build`，省略後續的make(步驟2)
2. `make -C obj_dir -f Vtop.mk`
3. `./obj_dir/Vtop`
    * 執行testbench，看terminal輸出結果
4. 看波型圖，依作業系統選擇與安裝對應軟體
   * Linux：`gtkwave wave.vcd`
   * MacOS：`sufer wave.vcd`
     * 操作上大同小異
  

Note:
1. `top`要改成自己模組的名稱
2. Compile後續專案會陸續改成用Makefile
   * 到時候直接`make`, `make run`就好