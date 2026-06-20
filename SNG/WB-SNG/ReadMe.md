# Weighted Binary SNG
Reference : https://hackmd.io/@Vincent1999/BJUiP_zrgg

---

## 專案概述

隨機運算（Stochastic Computing）使用位元流中 「1」 出現的機率來表示數值。
透過一個 4 位元的線性反饋移位暫存器（LFSR）生成偽隨機序列，並透過權重分配邏輯（Weight-Based Generator, WBG）確保各權重信號互斥，最後與輸入的二進位值進行比對以產生精確的隨機流輸出。

---

## 硬體架構

### 1. 4-bit LFSR (`LFSR4.v`)
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

---

## 檔案結構

```text
WB-SNG/
├── LFSR4.v          # 4位元線性反饋移位暫存器模組
├── WBG.v            # 權重隨機數產生器頂層模組
└── testbench.cpp    # Verilator C++ 測試平台
```

---

## 後續可改進內容

1. 目前LFSR與BN都是直接寫死在verilog，可改成由verilator參數輸入
2. 延續項目１，皆由參數輸入，量測結果正確率