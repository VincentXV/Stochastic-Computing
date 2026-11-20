---
title: A New Stochastic Computing Methodology for Efficient Neural Network Implementation
tags: 碩士班 論文Ｍeeting
---
#  Stochastic Computing 隨機運算
論文名稱：[A New Stochastic Computing Methodology for Efficient Neural Network Implementation](https://ieeexplore.ieee.org/document/7093194)
Meeting 日期：01/22, 04/08, 06/03

[Survey of DL](https://hackmd.io/@Vincent1999/SurveyOfDL)

[AI 論文摘要](https://www.themoonlight.io/paper/share/e0dffa88-e850-485b-82d7-5b39cd9eed9a)

---
[TOC]

---
## I. 回顧 單極與雙極
![image](https://github.com/user-attachments/assets/3de1b95a-5207-4720-8782-82fe79939546)
* 上圖可視為
    * ( a ) - 單極 **乘法**
    * ( b ) - 單極與雙極 **加法**
    * ( c ) - 雙極 **乘法**
    * ( d ) - 與MUX組合，作為單極與雙極 **減法**
    * ( 額外 ) - XOR 可以做單極 **絕對值減法**
    * **除法** 參考論文-[Design of Division Circuits for Stochastic Computing](https://hackmd.io/POoPB2xESoCSsEO9DMC12w)

### 單極 unipolar
關於單極的詳細介紹可以回頭看之前的論文-[Survey of Stochastic Computing](https://hackmd.io/@Vincent1999/BJUiP_zrgg/edit)
* 數值映射：數值 $x = P(X)$，其中 $P(X)$ 是位元串中 $1$ 出現的機率
* 數值範圍：介於$[0:1]$之間
### 雙極 bipolar
* 數值映射：數值 $x = 2 \cdot P(X) - 1$
    * 位元串全是 1 代表 $+1$
    * 位元串全是 0 代表 $-1$
    * 位元串中 1 和 0 各半代表 $0$
    * 如：$11110000_{bitstream}=\frac{4}{8}_{unipolar}=\frac{4}{8}\times2-1=0_{bipolar}$
* 數值範圍：介於$[-1:+1]$之間
* 雙極會是$x = 2 \cdot P - 1$是有原因的
    * 跟電腦數字表示中的2的補數有關
    * 兩個P是表示負數與正數
    * -1是因為零不重疊
* 在同樣位元數下（如：256 bitstream），雙極的精度必定比單極低

---
## II. 比例編碼機制 (Ratio-based Encoding) /<br> 擴展隨機邏輯 (Extended Stochastic Logic ― ESL)
>支持實數運算的隨機計算算術單元

![image](https://github.com/user-attachments/assets/407dffb1-cb9b-4d25-af55-5c75107c83a5)
![image](https://github.com/user-attachments/assets/3eb0333b-c92e-4bd4-91e3-42884c73adb9)
Note：上圖所提到的$B2P\ \& \ P2B$，其實就是之前的SNG與Counter
* 圖三的波形圖
    * Clk (藍色波形)：系統時脈
    * Input (3bits) (綠色橫線)：這是輸入的二進位原始數據 $X_q$，在圖中的範例為 0x02
    * LFSR 3bits (紅色階梯波)：這是由隨機數產生器生成的序列 $R_n$
    * Output (紫色波形)：最終生成的 隨機位元串。
* 圖四的分佈圖，代表隨機數 $R_n$ 的統計特性
    * 物理意義：一個20-bits長的bitstream(SN)代表小數$p=0.5$，所累計到的「1」的總數的**發生機率**
    * 數據說明
        * 計數器所累計到的「1」的總數
        * 數值範圍：為 $0$ 到 $20$
        * X軸說明$p=0.5$、範圍$0\sim20$
    * 二項分佈 (Binomial Distribution)
        * 圖中曲線的峰值位於 $X=10$。這是因為實驗設定輸入機率 $p=0.5$，<br>根據期望值公式 $E(X) = M \cdot p = 20 \cdot 0.5 = 10$
---- 
* 數值映射：數值 $\huge x = \frac{P(p^*)}{P(q^*)}$
    * 其中 $p$ 與 $q$ 為雙極隨機數（bipolar stochastic number）
<span id = "ex01">![image](https://github.com/user-attachments/assets/9e37eadb-46f2-4860-9373-1fb2c123e4f4)</span>
* 數值範圍：理論上可以表示 **整個實數域 $(-\infty, +\infty)$**
    * 變數說明：
        * Extended Stochastic Number(簡稱ESN) - $x^*$
        * 單極SN - $p$
        * 雙極SN - $p^*$
        * 二進位整數 - $P$，為單極SN之機率分子
    * 二進位整數 $P$ 轉換為隨機機率 $p$（單極）：$p=\frac{P}{2^N-1}$
    * 上述 $p$ 代入雙極性公式 $p^* = 2p - 1$
        * $p^* = 2 \left( \frac{P}{2^N - 1} \right) - 1 = \frac{2P - (2^N - 1)}{2^N - 1}$
    * 可以直接使用二進位整數 $P$ 與 $Q$ 透過簡單的線性變換，就定義出一個在隨機計算領域中代表任意實數的數值 $x^*$
![image](https://github.com/user-attachments/assets/55b4e13a-8b06-43de-8384-d3918160939a)
* 優點：徹底解決了 SC 無法表示大於 1 的數值問題
* 缺點：硬體開銷比傳統單極/雙極略高，因為每個數值需要兩組位元串來表示
* 基本運算：
    * 乘法/除法
    ![image](https://github.com/user-attachments/assets/f47b7182-13d2-45d2-9d5b-ac04dcd43cd8)
        * ![image](https://github.com/user-attachments/assets/2b39a161-766e-4b4f-877b-f2ec1137e377)
        *  $out=x \times y$
            * $x^*=\frac{p^*}{q^*}$
            * $y^*=\frac{r^*}{s^*}$
        * $\Rightarrow$ The goal is to find: $out^* = x^* \cdot y^* = \frac{p^* \cdot r^*}{q^* \cdot s^*}$
        * 證明說明：(僅講乘法，除法同理，畢竟論文也沒講除法)
            1. 硬體機率層面 (Var_change 之前，也就是第一行)
                * 以分子為例，定義 XNOR Gate 在處理隨機位元串 $p$ 與 $r$ 時的輸出機率 $P(out_u)$
                    $$P(out_u) = p \cdot r + (1-p)(1-r)$$
                * XNOR 只有在輸入為 (1,1) 或 (0,0) 時輸出 1
            2. 數值映射與代換 (Var_change 過程與第二行)
                * `Var_change`是指「變數變換」或「變數替換」，這邊將變數$x\ \And\ y$轉換成bipolar stochastic $x^*\ \And\ y^*$
                    * 機率轉雙極性：$p^* = 2p - 1$
                    * 雙極性轉機率：$p = \frac{p^* + 1}{2}$
                * XNOR 的機率公式
                    * $P(A \overline{\oplus} B) = P(A)P(B) + (1-P(A))(1-P(B))$
                    * $(1-P(A))(1-P(B)) = 1 - P(B) - P(A) + P(A)P(B)$
                    * $P(A \overline{\oplus} B) = P(A)P(B) + [1 - P(B) - P(A) + P(A)P(B)]$
                    * $\Rightarrow P(A \overline{\oplus} B) = 1 - P(A) - P(B) + 2P(A)P(B)$
                * 後續僅是展開、通分、相消，便得到第三行等號之右式
                    * 不會或忘記怎麼做，請去問AI
            3. 輸出定義與對號入座 (第三行)
                * 等號之左式，為第2行$\Huge\frac{\frac{1+out_u^*}{2}}{\frac{1+out_v^*}{2}}$消除了共同分母"$/2$"
                * 等號之右式，為第2行最後簡化之結果
                * 最後$imply \rightarrow$得出$out_u^*=p^* \cdot r^*$



    * 加法/減法
    ![image](https://github.com/user-attachments/assets/a5d46d86-f3a7-4a53-8945-16dc4b879e19)
        * ![image](https://github.com/user-attachments/assets/a2001242-cd9e-4fde-bb60-4139d93dca37)
        * The goal is to find: $\Large out^* = x^* + y^* = \frac{p^*}{q^*} + \frac{r^*}{s^*}=\frac{p^*s^* + r^*q^*}{q^*s^*}$
        * 變數說明：
            * 分子項 A：$j = p \overline{\oplus} s$，其代表值 $j^* = p^* \cdot s^*$
            * 分子項 B：$k = r \overline{\oplus} q$，其代表值 $k^* = r^* \cdot q^*$
            * 分母項：先算 $l = q \overline{\oplus} s$，代表值 $l^* = q^* \cdot s^*$
            * $c$ (Selection Signal)：MUX（多工器）的選擇訊號，其機率通常設定為 $P(c) = 0.5$
                * 雙極性數值 $c^*$：根據 $2P-1$ 定義，$c^* = 2(0.5) - 1 = \mathbf{0}$。
            * $sig$ (Scaling/Sign Signal)：一個補償訊號，用於調整分母的密度
                * 在推導中，作者設定 $P(sig) = 0.75$，對應的雙極性數值 $sig^* = 2(0.75) - 1 = \mathbf{0.5}$
                * 它出現在分母的運算中，用來產生那個關鍵的 $0.5$ 倍率，抵消分子 MUX 帶來的縮放因子。
        * ::: spoiler 證明說明：
            1. 令$\large out^*=\frac{out_u^*}{out_v^*}$
                * 分子（電路之t*）- $P(out_u) = P(c)P(j) + (1-P(c))P(k)$
                * 分母（電路之u*）- $P(out_v) = out_v^* = l^* \overline{\oplus} sig^* = 1 - P(l) - P(sig) + 2P(l)P(sig)$
                    * XNOR「兩者相同」才輸出`1`，機率公式如下$(l^*\times sig^*)+(1-l^*)(1-sig^*)$
            2. `Var_change`將機率傳換成biploar stochastic number
                * Note: <span style="color:red;">
                    * 這邊可能有失誤將第三行的分母2，誤打成s
                    * 分子項最後係數應該是$0.25=\frac{1}{4}$
                    * 分母項變數轉換的展開結果，論文中電路圖使用xnor（同或），但於證明展開中使用的卻是xor（異或）</span>
                * 分子項
                    * 變數代入：<br>$\Rightarrow \frac{c^* + 1}{2}\frac{j^* + 1}{2}+\frac{k^* + 1}{2}-\frac{c^* + 1}{2}\frac{k^* + 1}{2}$
                    * 提出係數$\frac{1}{4}$：<br>$\Rightarrow \frac{1}{4}\{(j^*+1)(c^*+1)+2(k^*+1)-(k^*+1)(c^*+1)\}$<br>$\Rightarrow\frac{1}{4}\{(j^*c^*+j+c+1)+(2k^*+2)-(k^*c^*+k^*+c^*+1)\}$<br>$\Rightarrow\frac{1}{4}\{(j^*c^*+j)+(k^*+2)-(k^*c^*)\}$<br>$\Rightarrow\frac{1}{4}\{c^*(j^*-k^*)+j^*+k^*+2\}$
                    * 代入$c^*=0$：<br>$\Rightarrow\frac{out\_u^*+1}{2}=\frac{1}{4}(j^*+k^*+2)$
                    * 同乘2：
                    $\Rightarrow out\_u^*+1=0.5j^*+0.5k^*+1$
                * 分母項
                    * Var_change(異或展開)：
                    $\Rightarrow(1-\frac{l^*+1}{2})(1-\frac{sig^*+1}{2})+\frac{l^*+1}{2}\frac{sig^*+1}{2}$
                    $\Rightarrow 1-\frac{l^*+1}{2}-\frac{sig^*+1}{2}+2\frac{l^*+1}{2}\frac{sig^*+1}{2}$
                    $\Rightarrow \frac{1}{4}(4-2(l^*+1)-2(sig^*+1)+2(l^* sig^*+l^*+sig^*+1))$
                    $\Rightarrow \frac{1}{4}(2l^* sig^*+2)$
                    $\Rightarrow \frac{1}{2}(l^* sig^*+1)$
                    * 同乘2：
                    $\Rightarrow \frac{out\_v^*+1}{2}=\frac{l^* sig^*+1}{2}$
                    $\Rightarrow out\_v^*+1=l^* sig^*+1$
                    * 代入sig^*^=0.5
                    $\Rightarrow out\_v^*+1=0.5l^*+1$
            3. 消去左右項的1得出結果
            $out\_u^* = 0.5j^*+0.5k^*$
            $out\_u^* = 0.5(p^* s^* + r^* q^*)$
            $out\_v^*=0.5 l^*$
            $out\_v^*=0.5 q^* s^*$
          :::

----
## III. Additional Stochastic Blocks Used to Implement NN
![image](https://github.com/user-attachments/assets/71bdb987-f2de-4139-b9fb-dfa433047a82)
* 上圖是論文中提出可應用到類神經網路的架構 - ESL neuron(神經元)
    * ![image](https://github.com/user-attachments/assets/731ac007-2586-4483-9b51-ce4c0a5c1e49)
    * 此架構為1x2的神經元，包含矩陣乘法與激發函數，可對應上圖的數學模型
    * 左半邊是處理矩陣乘法
        * 輸入為：
            * $\large X_{i1}=\frac{X_{i1\_p}}{X_{i1\_q}}$
            * $\large X_{i2}=\frac{X_{i2\_p}}{X_{i2\_q}}$
            * $\large W_{i1}=\frac{W_{i1\_p}}{W_{i1\_q}}$
            * $\large W_{i2}=\frac{W_{i2\_p}}{W_{i2\_q}}$
            * $\large Bias_{i}=\frac{Bias_{i\_p}}{Bias_{i\_q}}$
        * 輸出為：$\large Out^*=\frac{Out\_p}{Out\_q}$
            * 相當於數學模型中的$z$
        * ![image](https://github.com/user-attachments/assets/4b978738-0799-4b27-b7e9-d37c12ad4d3b)
        * ![image](https://github.com/user-attachments/assets/2a724526-c25c-4f11-bb5a-69c31c605878)
            * 電路說明：
                * 4個 Xnor 運算 $X_{i1} \times W_{i1}$ 與 $X_{i2} \times W_{i2}$
                * 3to1 Mux運算$X_{i1}W_{i1}+X_{i2}W_{i2}+Bias_{i}$
                    * Note：這邊是三個運算元相加，因此補償的sig為$\frac{1}{3}$
        * 此架構可以在進行硬體擴展
            * <font color ="red">但有一個問題，硬體一旦確定便是寫死的，在實際應用上不可能限制使用者的輸入輸出數量。
            * 那麼該如何設計可應用到各種大小的神經網路呢？</font>
                * 論文是以MATLAB跑完AI訓練後，依據訓練完的模型下去寫HDL並燒錄近FPGA
                * Meeting討論結果：如果需要兩個以上的輸入，可以後面再加一個累加器
    * 右半邊是處理激發函數 - ESL 雙曲正切函數 (ESL Hyperbolic Tangent Function)
        * ![image](https://github.com/user-attachments/assets/bc68b1bf-dc78-41d1-92cb-dd9b887275e2)
            * 上圖有三個模組，用於組成完整的$\text{tanh(X)}$函數
                * ESL Multiplier
                    * 參考ESL 四則運算
                * Bipolar  Divisor - 雙極除法器
                    * ![image](https://github.com/user-attachments/assets/40152b40-2950-48dc-87d1-5f4aba69f5f7)
                    * ![image](https://github.com/user-attachments/assets/55c79a93-2669-410b-95d8-5a0b5e3b7c6f)
                    * 核心邏輯類似[ADDIE-base Divider](https://hackmd.io/POoPB2xESoCSsEO9DMC12w?view#ADDIE-base-Divider)
                        * ADDIE 雙極 邏輯與真值表
                          ![image](https://github.com/user-attachments/assets/c502e079-a8a7-482e-886c-ead1a2a3158a)
                          | $x_1$ | $x_2$ | $x_1 / x_2$ | $z_1$ | $x_1 x_2$ | $x_2^2$ | $-z_1$ | $-z_1 x_2^2$ |
                          | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
                          | $-$ | $-$ | $+$ | $+$ | $+$ | $+$ | $-$ | $-$ |
                          | $-$ | $+$ | $-$ | $-$ | $-$ | $+$ | $+$ | $+$ |
                          | $+$ | $-$ | $-$ | $-$ | $-$ | $+$ | $+$ | $+$ |
                          | $+$ | $+$ | $+$ | $+$ | $+$ | $+$ | $-$ | $-$ |
                             * Up/Down邏輯
                                 * 觸發 Up － 兩者同為 1
                                     * $X_1X_2 = 1$：表示當下 $X_1X_2$ 的抽樣為正值
                                     * $-X_2^2\hat{Z} = 1$：表示當下 $-X_2^2\hat{Z}$ 的抽樣為正值。
                                     * 反過來意味著，真正的 $X_2^2\hat{Z}$ 當下是負值（即位元為 0）
                                     * $\Rightarrow X_1X_2 > X_2^2\hat{Z}\Rightarrow$ 觸發 Up
                                 * 觸發 Down － 兩者同為 0
                                     * $X_1X_2 = 0$：表示當下 $X_1X_2$ 的抽樣為負值
                                     * $-X_2^2\hat{Z} = 0$：表示當下 $-X_2^2\hat{Z}$ 的抽樣為負值。
                                     * 反過來意味著，真正的 $X_2^2\hat{Z}$ 當下是正值（即位元為 1）
                                     *  $\Rightarrow X_1X_2 < X_2^2\hat{Z}\Rightarrow$ 觸發 Down
                        
                        * 靠中間16 Bits Counter去計算$\frac{p^*}{q^*}$
                        * 紅色XNOR計算$\frac{p^*}{q^*}\times q^*=p^{*'}$
                            * 從這個$p^{*'}$與input $p^{*}$比較來去判斷Counter的Up/Down
                    * 左半部判斷$\frac{p^*}{q^*}$屬於第幾象限
                        * 如果是１、４象限，由$p^*$控制Up/Down
                        * 反之，則由$Not(p^*)$控制
                    * 運算邏輯
                        * 中間偏上的兩個Counter
                            * 看得出來是要確保數值不要超過上下限
                            * 但怎麼做到的？
                                * 因為是雙極範圍是[-1,1]，以二的補數格式來說0x0000與0xFFFF並非是對應的值
                                * 再者Counter的值應該是二進位整數，是怎麼在後續B2P中轉成SN
                                    * 應該是有些電路被省略了？
                                    * 類似像$\frac{Counter}{N}$的電路
                                        * <font color ="#ADD8E6">沒有，因此Counter出來是整數，後續B2P的Random也是產生整數就好，這樣一來也就沒有問題了</font>
                                * <font color ="#ADD8E6">因為Counter出來的是一個正整數，所以可以用$0xFFFF,0x0000$來表示最大最小值，用此方式來判對是否達到極值</font>
                                * 電路中的16 bits是IEEE 754格式或是別的？
                                    * <font color ="#ADD8E6">整數</font>
                * Bipolar Hyperbolic Tangent - 雙極雙曲正切函數
                    * ![image](https://github.com/user-attachments/assets/381fbfa1-0cfc-4fc5-92ef-4145b9a01999)
                    * ![image](https://github.com/user-attachments/assets/7346894f-aeb3-4e78-9f7b-14dd360a5bdb)
                    * $\Large tanh(x)=\frac{sinh(x)}{cosh(x)}=\frac{e^x-e^{-x}}{e^x+e^{-x}}=\frac{e^{2x}-1}{e^{2x}+1}$
                        * ![image](https://github.com/user-attachments/assets/64c011e3-9ff7-401c-a59f-2e68b986f96c)

                        * $\large sinh(x)=\frac{e^x-e^{-x}}{2}$
                        * $\large cosh(x)=\frac{e^x+e^{-x}}{2}$
                        * 分子分母同乘$e^x$
                        * $\Large \Rightarrow \frac{e^x-e^{-x}}{e^x+e^{-x}}=\frac{(e^x-e^{-x})e^x}{(e^x+e^{-x})e^x}=\frac{e^{2x}-1}{e^{2x}+1}$
                    * 論文提出$tanh(Gx)$近似於累積分布函數（CDF - Cumulative Distribution Function）
                        * 二項式分布與CDF
                          ![image](https://github.com/user-attachments/assets/c1a75885-74e8-40ee-8b8e-d1c9e8f2b542)
                        * ![image](https://github.com/user-attachments/assets/a2cc02d8-1e07-4b94-aad9-59e84ed3eb72)
                            * ![image](https://github.com/user-attachments/assets/4cf065b8-fe70-4c23-ab96-ab3aef4e8d6a)
                            * $p^* \rightarrow p \rightarrow P(X=x) \rightarrow F(x)$
                                * $p^* \rightarrow p$
                                    * $p^* = 2p-1$
                                * $p \rightarrow P(X=x)$
                                    * $P(X=x)=f(x)=(^k_x)p^xp^{k-x}$
                    * 硬體設計說明
                        * The F(x) of a dichotomous process x with activity p∗ = 0 behaves similarly to the tanh(Gx) function (Fig. 9). Therefore, F(x) can be obtained using a P2B(k) converter (its output is governed by a binomial distribution), a binary comparator, a P2B(N) converter [to integrate the function f (x)], and a B2P(N) [to rerandomize the evaluated F(x) signal], as shown in Fig. 9.
                            * 1st P2B(k) - 輸出由二項分布控制的f(x)
                            * 2nd P2B(N) - 積分 f(x) 函數，產生F(x)
                            * B2P(N)     - 將F(x)轉成SN
                        *  1st P2B(k)：
                          ![截圖 2026-05-11 下午4.19.25](https://github.com/user-attachments/assets/a20d8bc4-7447-4f23-971f-3f6bf9a9abe5)
                            * 藉由Counter（P2B）將SN轉成Binary，然後每k clocks判斷一次，判斷條件與輸出如下
                            * ![image](https://github.com/user-attachments/assets/57d1bb0d-de71-4470-864b-7cc21b7097cb)
                                * $f(x)$為k clocks後，由x轉換成的Binary Number
                                    * Note: 此處並未完全轉換整個SN，論文中以k=41為例
                        * 2nd P2B(N) & B2P：
                          ![image](https://github.com/user-attachments/assets/025bb1d5-a805-4852-8cee-e6b4742827da)
                            * 利用一個Counter（P2B）將前一結果累加/積分
                            * 再將此訊號 F(X) 藉由B2P轉換成SN
                    * $tanh(x) = tanh(G \cdot \frac{X^*}{G})$
                        * $G = 0.0659 \cdot k + 1.3668, \quad k \in \mathbb{N}$
                            * k為第一個P2B的統計週期
                        * 因此在激發函數的架構中，會有一個乘法器用於將輸入$X^*\text{乘上}\frac{1}{G}$
                            * G在圖中是寫為k~1~，但與週期k不同
                            * > he function requires compressing the input range [−a, a] to [−a/G, a/G]. That isto say, multiplying the input ESL signal by a factor 1/G < 1 (this condition must be satisfied so that there is no loss of information in the range [−1/G, +1/G])
                            * ![image](https://github.com/user-attachments/assets/35407b02-dda1-42b0-bc4a-7690ef13bc1c)
                        * > In the case of this paper, we
choose a compression space factor G = 4.0687.
                            * 論文是以k=41為例
                    * 最後輸出
                        * ![image](https://github.com/user-attachments/assets/042cea24-369f-47f7-b2b1-2da2220d5d37)
                        * ![image](https://github.com/user-attachments/assets/6fc5d5b2-49c7-4ab7-8157-2bd526360612)
                    *  <font color = "grey">代理解項目：（剩實作）</font>
                        1. tanh的電路為什麼可以這麼簡單實作出二項式分布？
                            * 因為 P2B 其實就是一個隨機事件的計數器。
                            * 第一個 P2B 對隨機脈衝計數，得到了符合二項式分佈的鐘形曲線點 $f(x)$；
                            * 第二個 P2B 把這些點隨著時間加總起來（積分），二項式分佈的積分恰好形成了一條完美的 S 型累積曲線 $F(x)$
                        2. 第一個 P2B 的更新頻率對第二個 P2B 的影響
                            * 第一個 P2B(k) 轉換器每 $k$ 個時脈週期（clock cycles）才會提供一個新的機率值 $f(x)$ 
                            * 第二個負責積分的 P2B(N) 轉換器，其輸入端接收長達 $k$ 個週期的連續 1 或連續 0 的「區塊（Chunks）」
                            * 這是為什麼論文提出
                                * > As a result, it is necessary that $N \gg k$ to properly evaluate $F(x)$.
                                * 以圖中的設計為例，其數值建立在16-bits上，因此$N=2^{16}=65536$
                                * 收集到足夠多的 $k$ 週期樣本（約 1598 次）
                                    * 實作電路上有點太大了
                        3. 除法與tanh電路實作與模擬結果

                        
----
## 實做上需要解決的問題
1. 怎麼拆分實數？
    * 如[範例](#ex01)，$\frac{2}{3}$如何決定要拆成$p^*=\frac{-4}{8}\ \&\ q^*=\frac{-6}{8}$
    * 精度可以直接定出分母
    * 會需要一個**轉換器**從IEEE754轉成SC
        * 可以看看定佳那邊是不是就是將IEEE754轉成定點數再轉成SC
2. 精度代價：雖然數值表示範圍變大了，但當 $q^*$ 非常接近 0 時，微小的位元波動（隨機誤差）會被放大，導致數值不穩定。
    * 這就是為什麼論文中需要「Gain 增益單元」來精確控制這個比例。
3. 證明中的$\frac{1+out_u}{1+out_v}$，其中的分子與分母的1是否會影響運算結果之精度？
4. 最後$out^* = \frac{out_u^*}{out_v^*}$，$out_u$ & $out_v$合併才是問題
    * 論文主要是學術，專注在ESL架構的計算上，但SNG, Counter並沒有提到怎麼做出來
    * SNG有很多論文可以平行加速
    * 就目前看到的論文架構，實做上Counter才是速度的瓶頸
5. tanh實作上，採用時間換空間的做法，因此完成運算時間拉到非常之大
    * 以圖中架構為範例（16 bits），完成此運算要65536個clock cycle
    * 當然這是為了精度才這麼做的
    * 是否可以再更小可以再去研究一下
    * 或者是參考傅立葉轉換（欣佑報的論文），找一個近似法實作tanh函數

