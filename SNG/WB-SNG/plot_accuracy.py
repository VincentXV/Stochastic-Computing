import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def main():
    # 1. 讀取 CSV 檔案
    csv_file = 'accuracy_results.csv'
    try:
        df = pd.read_csv(csv_file)
    except FileNotFoundError:
        print(f"錯誤：找不到 {csv_file}，請先執行 shell script。")
        return

    # 清理異常值 (過濾掉可能出現的無限大或空值)
    df = df.replace([np.inf, -np.inf], np.nan).dropna()

    # 將 testbench 中第 13 個測點的 65 clock 強制修正為 64，以利圖表 x 軸對齊
    df['Length'] = df['Length'].replace(65, 64)

    # 2. 將數據根據 Length 進行分組，並計算 Accuracy 的平均值
    avg_data = df.groupby('Length')['Accuracy'].mean().reset_index()

    # 3. 準備繪圖
    plt.figure(figsize=(8, 6))

    # 使用散點圖繪製平均準確率 (參考目標圖表風格)
    plt.scatter(
        avg_data['Length'], 
        avg_data['Accuracy'], 
        color='red', 
        s=25, 
        label='WBG with LFSR gen'
    )

    # 設定圖表視覺細節
    plt.xlabel('Partial Bitstream Length (bits)', fontsize=14)
    plt.ylabel('Accuracy', fontsize=14)
    plt.title('Accuracy of WBG using 6-bits LFSR Generator', fontsize=16)

    # 設定 X 與 Y 軸的範圍
    plt.xlim(0, 70)
    plt.ylim(0, 1.05)

    # 加入圖例
    plt.legend(loc='lower right', fontsize=12)

    # 調整佈局並儲存圖片
    plt.tight_layout()
    output_img = 'wbg_lfsr_accuracy.png'
    plt.savefig(output_img, dpi=300)
    print(f"繪圖完成！已儲存至 {output_img}")

    # 顯示圖片
    plt.show()

if __name__ == "__main__":
    main()