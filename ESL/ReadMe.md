[TOC]

# Extend Stochastic Logic

## 專案說明
查看SC_ESL.md

---
## 設計說明
* SNG 為 8-bits B-SNG，對比INT8的精度
  * 原架構為Unipolar SNG，改用Bipolar SNG
    * 該架構長度為 $2^n - 1$ ，這邊為對齊精準度，調整為 $2^n$
      * n 為 LFSR bits數
      * 原架構設 $2^n - 1$是為了避免LFSR誤差（僅1 bit）
* 激發函數tanh(x)，改採用FSM實現

---
## 檔案架構
```text
ESL/
├── rtl/                <-- 放置所有 Verilog/SystemVerilog 原始碼
│   ├── basic           <-- 基礎邏輯閘
│   ├── SNG             <-- Stochastic Number Generator，會改成Bipolar SNG
│   ├── arith           <-- ESL 算術運算
│   ├── act             <-- 激發函數
│   └── neuron          <-- 神經元
├── sim/                <-- 放置模擬相關檔案 (Testbench)
│   └── testbench.cpp
├── obj_dir/            <-- (由 Verilator 自動生成) 放置編譯出來的中繼檔與執行檔
├── wave.vcd            <-- 波型圖
└── ReadMe.md           <-- 專案說明文件
```

---
## 編譯方式
```bash=