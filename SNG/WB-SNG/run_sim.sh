#!/bin/bash

# 定義輸出的 CSV 檔案名稱
CSV_FILE="accuracy_results.csv"

# 初始化 CSV 標題
echo "Length,Accuracy" > "$CSV_FILE"

# 1. 產生 100 組隨機不重複的 (BN, SEED) 組合，從小到大排序
# 範圍皆為 1 ~ 63，避開 0 值導致的無效模擬與計算錯誤
echo "正在抽取 100 組隨機參數..."
awk 'BEGIN {
    srand();
    for(b=1; b<=63; b++) {
        for(s=1; s<=63; s++) {
            print rand() "\t" b "\t" s;
        }
    }
}' | sort -k1,1n | head -n 100 | cut -f2,3 | sort -k1,1n -k2,2n > selected_pairs.txt

# 2. 確保專案已編譯為最新狀態
echo "開始編譯 6-bits 架構..."
make BITS=6bits compile > /dev/null 2>&1

echo "開始執行 100 次模擬..."
count=1

# 3. 讀取清單並反覆執行 testbench
while read -r bn seed; do
    # 將十進位轉為 6-bits 二進位字串 (利用 bc 轉換，並用 awk 補齊前導 0)
    bn_bin=$(echo "obase=2; $bn" | bc | awk '{printf "%06d\n", $0}')
    seed_bin=$(echo "obase=2; $seed" | bc | awk '{printf "%06d\n", $0}')

    echo -ne "正在執行第 $count/100 組 (BN=$bn_bin, SEED=$seed_bin)...\r"

    # 執行 testbench 並透過 awk 抓取 Length 與 Accuracy
    make run BITS=6bits ARGS="$bn_bin $seed_bin" | awk '
        /停止在第/ { len=$2 }
        /Accuracy =/ { print len "," $3 }
    ' >> "$CSV_FILE"

    count=$((count + 1))
done < selected_pairs.txt

echo -e "\n模擬完成！結果已精簡並儲存至 $CSV_FILE"
rm selected_pairs.txt