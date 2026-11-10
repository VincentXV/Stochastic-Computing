# Bipolar SNG

---
筆記連結：https://hackmd.io/@Vincent1999/BiopolarSNG
論文連結：https://ieeexplore.ieee.org/document/11465213

---
## 架構圖
<img alt="Image" src="https://github.com/user-attachments/assets/caef0d44-7187-4bcd-8d89-95668d754e59" />

---
## 專案架構
```text
WB-SNG/
├── rtl/                <-- 放置所有 Verilog/SystemVerilog 原始碼
│   ├── 4bits           <-- LFSR bit數
│       ├── LFSR.v          
│       └── WBG.v           
├── sim/                <-- 放置模擬相關檔案 (Testbench)
│   └── testbench_4bits.cpp
├── obj_dir/            <-- (由 Verilator 自動生成) 放置編譯出來的中繼檔與執行檔
└── ReadMe.md           <-- 專案說明文件
```

---
## 編譯方式
```bash=
// for test LFSR in SNG/B-SNG
$ verilator --cc --exe sim/testbench_LFSR.cpp --build --trace -Irtl/4bits/ rtl/4bits/LFSR4.v
$ ./obj_dir/VLFSR4
$ gtkwave wave.vcd

// for test BSNG in SNG/B-SNG
$ verilator -cc -exe sim/testbench_BSNG.cpp --build --trace -Irtl/4bits/ rtl/4bits/BSNG.v
$ ./obj_dir/VBSNG
or 
$ ./obj_dir/VBSNG <參數，如：1101>
$ gtkwave wave.vcd
```

---
## 待處理項目
- [ ] 模駔須加上rst
    -  目前設計是直接在宣告時給值，此作法雖在模擬與FPGA上大致合法
    -  但真到了ASIC實做晶片時，就會有bug
       -  實體晶片，通電那一瞬間電晶體沒有任何預載機制
- [ ] 