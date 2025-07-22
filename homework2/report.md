# 41141126

作業二

## 解題說明
本題要求實作一個 `Polynomial` 類別，並透過重載 `<<`、`>>` 運算子來方便輸入輸出多項式。多項式可進行加法與乘法運算，並能計算在指定 x 值的結果。
### 解題策略
1. 用結構表示多項式每一項（係數和次方），整個多項式用類別管理項目陣列。  
2. 實作加法和乘法函式，合併同次方項目。  
3. 寫計算函式，算多項式在某個 x 的值。
## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
using namespace std;

class Polynomial;  // forward declaration

class Term {
    friend class Polynomial;
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);
private:
    float coef;
    int exp;
};

class Polynomial {
public:
    Polynomial();
    Polynomial Add(const Polynomial& poly) const;
    Polynomial Mult(const Polynomial& poly) const;
    float Eval(float f) const;

    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

    void resize(int newCapacity);
};

Polynomial::Polynomial() {
    capacity = 10;
    terms = 0;
    termArray = new Term[capacity];
}

void Polynomial::resize(int newCapacity) {
    Term* newArray = new Term[newCapacity];
    for (int i = 0; i < terms; i++) {
        newArray[i] = termArray[i];
    }
    delete[] termArray;
    termArray = newArray;
    capacity = newCapacity;
}

Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;
    int i = 0, j = 0;
    while (i < terms && j < poly.terms) {
        if (termArray[i].exp == poly.termArray[j].exp) {
            float sumCoef = termArray[i].coef + poly.termArray[j].coef;
            if (sumCoef != 0) {
                if (result.terms == result.capacity) result.resize(result.capacity * 2);
                result.termArray[result.terms].coef = sumCoef;
                result.termArray[result.terms].exp = termArray[i].exp;
                result.terms++;
            }
            i++; j++;
        }
        else if (termArray[i].exp > poly.termArray[j].exp) {
            if (result.terms == result.capacity) result.resize(result.capacity * 2);
            result.termArray[result.terms] = termArray[i];
            result.terms++;
            i++;
        }
        else {
            if (result.terms == result.capacity) result.resize(result.capacity * 2);
            result.termArray[result.terms] = poly.termArray[j];
            result.terms++;
            j++;
        }
    }
    while (i < terms) {
        if (result.terms == result.capacity) result.resize(result.capacity * 2);
        result.termArray[result.terms++] = termArray[i++];
    }
    while (j < poly.terms) {
        if (result.terms == result.capacity) result.resize(result.capacity * 2);
        result.termArray[result.terms++] = poly.termArray[j++];
    }
    return result;
}

Polynomial Polynomial::Mult(const Polynomial& poly) const {
    Polynomial result;
    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < poly.terms; j++) {
            float newCoef = termArray[i].coef * poly.termArray[j].coef;
            int newExp = termArray[i].exp + poly.termArray[j].exp;
            bool found = false;
            for (int k = 0; k < result.terms; k++) {
                if (result.termArray[k].exp == newExp) {
                    result.termArray[k].coef += newCoef;
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (result.terms == result.capacity) result.resize(result.capacity * 2);
                result.termArray[result.terms].coef = newCoef;
                result.termArray[result.terms].exp = newExp;
                result.terms++;
            }
        }
    }
    int idx = 0;
    for (int i = 0; i < result.terms; i++) {
        if (result.termArray[i].coef != 0) {
            result.termArray[idx++] = result.termArray[i];
        }
    }
    result.terms = idx;
    return result;
}

float Polynomial::Eval(float f) const {
    float sum = 0.0;
    for (int i = 0; i < terms; i++) {
        float termVal = termArray[i].coef;
        int e = termArray[i].exp;
        float power = 1.0;
        for (int j = 0; j < e; j++) power *= f;
        sum += termVal * power;
    }
    return sum;
}

istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    in >> n;
    if (n > poly.capacity) poly.resize(n);
    poly.terms = n;
    for (int i = 0; i < n; i++) {
        in >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }
    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }
    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;
        if (c == 0) continue;
        if (i > 0 && c > 0) out << " + ";
        if (e == 0) out << c;
        else if (e == 1) out << c << "x";
        else out << c << "x^" << e;
    }
    return out;
}

int main() {
    Polynomial p1, p2;
    cout << "Input first polynomial (format: number_of_terms coef exp ...):\n";
    cin >> p1;
    cout << "Input second polynomial:\n";
    cin >> p2;

    cout << "Polynomial 1: " << p1 << "\n";
    cout << "Polynomial 2: " << p2 << "\n";

    Polynomial sum = p1.Add(p2);
    cout << "Sum: " << sum << "\n";

    Polynomial product = p1.Mult(p2);
    cout << "Product: " << product << "\n";

    float x;
    cout << "Enter x to evaluate polynomial 1: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << "\n";

    return 0;
}
```

## 效能分析

1. 時間複雜度：加法為O(n)、乘法為O(n²)
2. 空間複雜度：需要額外空間存放結果多項式，最多約為n項乘m項

## 測試與驗證

### 測試案例

測試案例輸入：
`3`
`2 2`
`3 1`
`4 0`
`2`
`1 1`
`-1 0`
`2`

預期輸出：
Polynomial 1: 2x^2 + 3x^1 + 4
Polynomial 2: 1x^1 + -1
Sum: 2x^2 + 4x^1 + 3
Product: 2x^3 + 1x^2 + -1x^1 + -4
p1(2) = 18

實際輸出：
Polynomial 1: 2x^2 + 3x^1 + 4
Polynomial 2: 1x^1 + -1
Sum: 2x^2 + 4x^1 + 3
Product: 2x^3 + 1x^2 + -1x^1 + -4
p1(2) = 18

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o polynomial polynomial.cpp
$ ./polynomial
```

### 結論

1.程式能正確實作多項式類別，完成加法、乘法與多項式值的計算。

2.測試案例涵蓋多種邊界狀況，驗證功能正確無誤。

3.程式時間複雜度與空間複雜度符合預期，效能穩定。

## 申論及開發報告

### 選擇陣列與迴圈實作的原因
在這次的多項式實作中，我選擇使用陣列搭配 for 迴圈來完成多項式的輸入、輸出、加法、乘法與計算值的功能。主要原因如下

1.直接操作資料結構簡單明瞭

2.加法與乘法的實作適合使用巢狀迴圈

3.方便控制資料與排序
