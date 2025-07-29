# 41141126

作業一的問題一

## 解題說明
本題要求以以下數學定義實作Ackermann 函數
Ackermann(m, n) =
    n + 1                       當 m = 0 時
    Ackermann(m - 1, 1)        當 m > 0 且 n = 0 時
    Ackermann(m - 1, Ackermann(m, n - 1)) 當 m > 0 且 n > 0 時
### 解題策略
1.用遞迴依定義實作Ackermann函數

2.實作 `ackermann(int m, int n)` 函數並處理三種情況

3.注意不要輸入過大的m與n，避免堆疊溢位（stack overflow）

## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
using namespace std;

int ackermann(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann(m - 1, 1);
    else
        return ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << ackermann(m, n) << endl;
    return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 `O(A(m, n))`
2. 空間複雜度：空間複雜度為 `O(A(m, n))`

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (m, n) | 預期輸出 | 實際輸出 |
|----------|------------------|----------|----------|
| 測試一   | (0, 0)           | 1        | 1        |
| 測試二   | (1, 1)           | 3        | 3        |
| 測試三   | (2, 2)           | 7        | 7        |
| 測試四   | (3, 3)           | 61       | 61       |
| 測試五   | (3, 4)           | 125      | 125      |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
```

### 結論

1.程式能正確計算 1 到 n 的連加總和。

2.當輸入 n 為負數時，程式會成功拋出異常，符合設計預期。

3.測試案例涵蓋多種邊界狀況（n = 0、n = 1、n > 1、n < 0），驗證程式的正確性與健壯性。

## 申論及開發報告

### 選擇使用遞迴的原因
1.Ackermann函數本身就是遞迴定義，所以直接使用遞迴實作

2.程式碼較簡單

遞迴版本在m和n過大時容易造成記憶體堆疊溢位（Stack Overflow）

作業一的問題二

## 解題說明
本題要求寫一個遞迴函數來計算*powerset(S)*

### 解題策略
1.用包含或不包含來找出一個集合中的所有子集合
2.用遞迴的方式來構建所有可能的組合

## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
#include <string>

using namespace std;

void generatePowerset(const string& set, string current, int index) {
    if (index == set.length()) {
        cout << "{ ";
        for (int i = 0; i < current.length(); ++i) {
            cout << current[i] << " ";
        }
        cout << "}" << endl;
        return;
    }

    generatePowerset(set, current, index + 1);

    generatePowerset(set, current + set[index], index + 1);
}

int main() {
    string S = "abc";  // 測試集合 {a, b, c}
    cout << "Powerset of {" << S << "}:\n";
    generatePowerset(S, "", 0);
    return 0;
}
```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 `O(2ⁿ)`
2. 空間複雜度：空間複雜度為 `O(n)`

## 測試與驗證

### 測試案例

| 測試案例 | S | 預期輸出數量 | 實際輸出數量 |
|----------|------------|---------------------|---------------------|
| 測試一   | ""         | 1                   | 1                   |
| 測試二   | "a"        | 2                   | 2                   |
| 測試三   | "ab"       | 4                   | 4                   |
| 測試四   | "abc"      | 8                   | 8                   |


### 編譯與執行指令

```shell
g++ -std=c++17 -o powerset powerset.cpp
./powerset
```

### 結論

1.程式能正確列出任何集合所有的子集合

2.測試涵蓋空集合到三元素集合

## 申論及開發報告

### 選擇使用遞迴的原因
1.題目要求

2.易於理解與實現

3.程式比較簡單

4.接近數學的定義
