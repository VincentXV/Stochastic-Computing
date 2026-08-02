# 預設編譯位元數 (如果在終端機只打 make，就會預設跑 4bits)
BITS ?= 4bits
ARGS ?= 

# 定義目錄
RTL_DIR = rtl/$(BITS)
SIM_DIR = sim
BUILD_DIR = obj_dir

# 頂層模組
TOP_MODULE = WBG

# 抓取對應位元資料夾下的所有 .v 檔，以及對應的 testbench
V_SRCS = $(wildcard $(RTL_DIR)/*.v)
CPP_SRC = $(SIM_DIR)/testbench_$(BITS).cpp

# Verilator 編譯參數
VERILATOR_FLAGS = --cc --exe --build --trace -Mdir $(BUILD_DIR) -I$(RTL_DIR)

# ==========================================
# 目標規則 (Targets)
# ==========================================

# 1. 預設目標：單純輸入 make 時，會對應到 compile
all: compile

# 2. 僅編譯：執行 Verilator 轉換與編譯
compile:
	@echo "開始編譯 $(BITS) 架構..."
	verilator $(VERILATOR_FLAGS) $(RTL_DIR)/$(TOP_MODULE).v $(CPP_SRC)

# 3. 僅執行：跑編譯出來的二進位檔
run:
	@echo "執行 $(BITS) 模擬..."
	./$(BUILD_DIR)/V$(TOP_MODULE) $(ARGS)

# 4. 清理編譯垃圾
clean:
	@echo "清理 $(BUILD_DIR) 與波形檔..."
	rm -rf $(BUILD_DIR) wave.vcd

# 宣告這些是「動作」而非「實體檔案」
.PHONY: all compile run clean