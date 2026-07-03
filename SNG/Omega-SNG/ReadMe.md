[TOC]

paper: https://hackmd.io/@Vincent1999/r150twcrxx
Note: for test, this project is base on 3-bits（小數精確度，實際到SC為8-bits）

----
## 檔案架構
```text
📁 Omega_SNG/
│
├── 📁 src/                     
 存放所有 top.v 會呼叫到的實際電路
│   ├── top.v                   # 專案頂層模組
│   ├── 📁 basic/               # MUX_2x1.v
│   ├── 📁 bmc/                 # BMC_8.v, BMC_256.v
│   ├── 📁 network/             # Omega_L.v, Flip_L.v
│   └── 📁 generators/          # LFSR4.v, 4_Shift.v, BitShiftNet.v
│
└── 📁 sim/                     # 專門只放測試用的 Testbench
```

----
## 編譯
指令：`verilator --cc -f filelist.f --exe sim/testbench_xxx.cpp --build --trace`
* 參數說明：
  * `--cc`: 指定輸出格式為 C++
    * 告訴 Verilator 將你輸入的 Verilog 數位電路原始碼，轉換成 C++ 的類別（Classes）與標頭檔（.h 和 .cpp）。（若要轉換成 SystemC，則會使用 --sc）。
  * `-f filelist.f`: 從指定的文字檔案（此處為 filelist.f）中讀取編譯參數與檔案列表
  * `--exe`: 指定包含 main() 函式的 C++ 封裝檔案（Wrapper/Testbench）
  * `--build`: 自動觸發後續的 C++ 編譯與連結步驟
  * `--trace`: 開啟波形追蹤（Waveform Tracing）功能
  * 如果需要測試單一模組在增加下列參數：
    * `--top-module module_name`
    * `src/module.v`

## Big Picture
![image](https://hackmd.io/_uploads/HkbFGGWUgl.png)

### BMC
![image](https://hackmd.io/_uploads/rkbPzMWIeg.png)

### Omega-Flip Network
![image](https://hackmd.io/_uploads/BJNxffb8gl.png)
![image](https://hackmd.io/_uploads/r1OMkOMUxg.png)
![image](https://hackmd.io/_uploads/Hkbfx1W7Mg.png)

