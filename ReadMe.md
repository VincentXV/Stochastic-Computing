Reference:
https://hackmd.io/BiB4yBbkTKyawxx9fLA5Lw?view

Compile:
1. `verilator -Wall --cc --exe top.v testbench.cpp --trace --fno-dfg`
    * 可再加`--build`，省略後續的make(步驟2)
2. `make -C obj_dir -f Vtop.mk`
3. `./obj_dir/Vtop`
4. `gtkwave wave.vcd`

Note:
1. `top`要改成自己模組的名稱