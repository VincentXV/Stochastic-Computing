# Weighted Binary SNG
Reference : https://hackmd.io/@Vincent1999/BJUiP_zrgg
[TOC]

---
## 專案概述

隨機運算（Stochastic Computing）使用位元流中 「1」 出現的機率來表示數值。
透過一個 4 位元的線性反饋移位暫存器（LFSR）生成偽隨機序列，並透過權重分配邏輯（Weight-Based Generator, WBG）確保各權重信號互斥，最後與輸入的二進位值進行比對以產生精確的隨機流輸出。

---
## 編譯方式
:::info
在撰寫Makefile，將編譯方式簡化如下
:::
```text
make            <--- 編譯
make run        <--- 執行
make run BITS=6bits ARGS="101100 000010" // ARGS 為指定BN, LFSR的參數
make clean      <--- 清除編譯後產生的檔案
```

---
## 硬體架構
----
### 1. 4-bit LFSR (`LFSR.v`)
- **特點**：4 位元線性反饋移位暫存器。
- **特徵多項式反饋**：`feedback = LFSR[3] ^ LFSR[2]`。
- **初始值**：`4'b0010=B=11`。

### 2. Weight-Based Generator (`WBG.v`)
將 LFSR 的 4 位元輸出（$l_3, l_2, l_1, l_0$）轉換為互斥的權重信號（$w_3, w_2, w_1, w_0$）：
- $w_3 = l_3$
- $w_2 = l_2 \cdot \overline{l_3}$
- $w_1 = l_1 \cdot \overline{l_2} \cdot \overline{l_3}$
- $w_0 = l_0 \cdot \overline{l_1} \cdot \overline{l_2} \cdot \overline{l_3}$

最後與輸入的二進位數 `bn`（預設為 `4'b1011`，即十進位的 11）進行 AND 運算後 OR 輸出 `sn`：
$$\text{sn} = (w_3 \cdot bn[3]) \mid (w_2 \cdot bn[2]) \mid (w_1 \cdot bn[1]) \mid (w_0 \cdot bn[0])$$

----
### 3. 6-bits base Arch.
- 撰寫、測試緣由：
  - 之前Meeting時，有看到一篇論文，在比較SNG提前截斷（以此減少運算週期）的準確度論文
  - <img alt="Image" src="https://github.com/user-attachments/assets/2c3d1f81-8252-439d-b454-fb8a1941b0fa" />
  - 因數據推測論文在比較時，是使用最Classical SNG來針對RNG的部分做變化，但LFSR提出時，是有配套SNG-WBG(Weighted Binary SNG)，因此想測試其在提前截斷時的準確率
- 測試方式與結果：
  - 方式：
    - 重新調整WBG長度為６bits LFSR及產生$2^6=64$ bits 的SN
    - 並且進行量測，截斷各長度的準確度，並與論文得到的數據進行比較
    - Note: 繪圖需要一些python的套件，我自己是只將其裝在虛擬環境(此處環境名稱實為draw)上
      - 指令：
      - ```text
        1. 建立虛擬環境 (名稱以 myenv 為例)
        python3 -m venv myenv

        2. 啟動虛擬環境
        source myenv/bin/activate

        3. 正常使用 pip 安裝套件
        pip install pandas matplotlib numpy
        
        4. 關閉（退出）目前正在執行的 Python 虛擬環境
        deactivate
    - 繪圖指令：
      - `./run_sim.sh`
        - note: 需要有執行權限`chmod +x run_sim.sh`
      - `python3 plot_accuracy.py`
        - note: 需要再有相關套件的環境
        - `source draw/bin/activate`
        - `deactivate`
  - 結果：
    - <img alt="Image" src="https://github.com/user-attachments/assets/b35dfffb-6c64-42fb-93a9-66816a9e40d4" />

---
## 檔案結構
```text
WB-SNG/
├── rtl/                <-- 放置所有 Verilog/SystemVerilog 原始碼
│   ├── 4bits           <-- LFSR bit數
│       ├── LFSR.v          
│       └── WBG.v           
│   ├── 6bits           <-- LFSR bit數
│       ├── LFSR.v          
│       └── WBG.v  
├── sim/                <-- 放置模擬相關檔案 (Testbench)
│   └── testbench_4bits.cpp
│   └── testbench_6bits.cpp
├── obj_dir/            <-- (由 Verilator 自動生成) 放置編譯出來的中繼檔與執行檔
├── run_sim.sh          <-- 多次執行不同參數的腳本，並將準確度紀錄至lfsr_results.csv
├── lfsr_results.csv    <-- 精確度數據紀錄檔
├── plot_accuracy.py    <-- 將數據繪製成圖表
└── ReadMe.md           <-- 專案說明文件
```

---
## 後續可改進內容

1. [x] 目前LFSR與BN都是直接寫死在verilog，可改成由verilator參數輸入
    - Note：目前只調整6-bits的testbench
2. [x] 簡化編譯方式（採用Makefile）
3. [x] 量化測試其準確度