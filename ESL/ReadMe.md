[TOC]

# 檔案架構
```text
ESL/
├── rtl/                <-- 放置所有 Verilog/SystemVerilog 原始碼
│   ├── basic           <-- 基礎邏輯閘
│   ├── SNG             <-- Stochastic Number Generator 
│   ├── arith           <-- ESL 算術運算
│   ├── act             <-- 激發函數
│   └── neuron          <-- 神經元
├── sim/                <-- 放置模擬相關檔案 (Testbench)
│   └── testbench.cpp
├── obj_dir/            <-- (由 Verilator 自動生成) 放置編譯出來的中繼檔與執行檔
└── ReadMe.md           <-- 專案說明文件
```
Note:
* SNG 基於 8-bits B-SNG，對比INT8
* 
---
# 編譯方式

---
# 專案說明
查看SC_ESL.md