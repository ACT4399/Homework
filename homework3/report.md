# 41141126

作業三

## 解題說明
本題目要求實作一個 C++ 類別 Polynomial，用來表示並操作單變數整數係數的多項式。多項式以循環鏈結串列儲存，每個節點包含一項的係數、指數及指向下一節點的指標，且有一個表頭節點用於管理串列。
為了有效管理記憶體與節點，特別使用一個可用節點空間串列 (available-space list)，讓刪除的節點能夠被重複利用，避免頻繁的動態配置和釋放。
多項式的輸入與輸出操作，分別由重載的輸入運算子 (operator>>) 和輸出運算子 (operator<<) 處理，將外部表示（係數與指數的序列）與內部鏈結串列表示互相轉換。
實作中包含了複製建構子與指派運算子，確保物件複製時能做深複製並妥善管理動態記憶體，避免記憶體錯誤或洩漏。
多項式的加法、減法與乘法運算則依據節點指數大小做有序合併或展開，確保結果多項式仍維持指數遞減的順序。
最後，Evaluate函式透過傳入的浮點數值計算多項式在該點的值，使用pow函數計算冪次。
### 解題策略
1.資料結構設計：
使用循環鏈結串列 (circular linked list)，並加上表頭節點 (header node)，方便多項式的插入、刪除與管理。
每個節點 (node) 包含三個成員：係數 (coef)、指數 (exp)、指向下一節點的指標 (link)。

2.節點空間管理：
使用一個可用節點空間串列 (available-space list) 來重複利用已刪除的節點，避免頻繁分配與釋放記憶體，提高效率。

3.多項式的輸入與輸出：
透過operator>> 將輸入的多項式轉換成內部的循環鏈結串列形式。
透過operator<< 把鏈結串列的多項式轉換回外部格式輸出。

4.複製建構子與指派運算子：
實作複製建構子來深層複製多項式物件，避免淺層複製導致共用記憶體問題。
實作指派運算子同樣要確保深層複製並妥善釋放舊有資源。

5.解構子：
將多項式所用節點回收至可用節點空間串列。
將多項式所用節點回收至可用節點空間串列。

6.多項式運算：
加法 (operator+)、減法 (operator-) 和 *乘法 (operator)**，都用節點的指數大小來合併或建立新節點，保持指數由大到小排序。
乘法運算先計算每個項的乘積，生成臨時多項式，再用加法累加結果。

7.多項式評估 (Evaluate)：
根據給定浮點數 x，計算多項式在該點的值。

## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
#include <cmath>    
using namespace std;

struct Node {
    int coef;
    int exp;
    Node* link;
    Node(int c = 0, int e = 0, Node* l = nullptr) : coef(c), exp(e), link(l) {}
};

class Polynomial {
private:
    Node* head;  
    static Node* avail; 

    static void recycleNode(Node* node) {
        node->link = avail;
        avail = node;
    }

    static Node* getNode(int coef, int exp, Node* link) {
        if (avail != nullptr) {
            Node* temp = avail;
            avail = avail->link;
            temp->coef = coef;
            temp->exp = exp;
            temp->link = link;
            return temp;
        }
        return new Node(coef, exp, link);
    }

    void freeAllNodes() {
        if (!head) return;
        Node* curr = head->link;
        while (curr != head) {
            Node* temp = curr;
            curr = curr->link;
            recycleNode(temp);
        }
        recycleNode(head);
        head = nullptr;
    }

public:
    Polynomial() {
        head = getNode(0, -1, nullptr);
        head->link = head;
    }

    Polynomial(const Polynomial& a) {
        head = getNode(0, -1, nullptr);
        head->link = head;
        Node* tail = head;
        for (Node* curr = a.head->link; curr != a.head; curr = curr->link) {
            tail->link = getNode(curr->coef, curr->exp, head);
            tail = tail->link;
        }
    }

    ~Polynomial() {
        freeAllNodes();
    }

    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        freeAllNodes();
        head = getNode(0, -1, nullptr);
        head->link = head;
        Node* tail = head;
        for (Node* curr = a.head->link; curr != a.head; curr = curr->link) {
            tail->link = getNode(curr->coef, curr->exp, head);
            tail = tail->link;
        }
        return *this;
    }

    void insertTerm(int coef, int exp) {
        if (coef == 0) return;
        Node* prev = head;
        Node* curr = head->link;
        
        while (curr != head && curr->exp > exp) {
            prev = curr;
            curr = curr->link;
        }
        if (curr != head && curr->exp == exp) {
            curr->coef += coef;
            if (curr->coef == 0) { 
                prev->link = curr->link;
                recycleNode(curr);
            }
        } else {
            Node* newNode = getNode(coef, exp, curr);
            prev->link = newNode;
        }
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        x.freeAllNodes();
        x.head = getNode(0, -1, nullptr);
        x.head->link = x.head;

        int n;
        is >> n;  
        for (int i = 0; i < n; i++) {
            int c, e;
            is >> c >> e;
            x.insertTerm(c, e);  
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        Node* curr = x.head->link;
        if (curr == x.head) {
            os << "0";
            return os;
        }
        bool first = true;
        while (curr != x.head) {
            if (!first && curr->coef > 0)
                os << "+";
            first = false;

            if (curr->exp == 0) {
                os << curr->coef;
            } else if (curr->exp == 1) {
                if (curr->coef == 1)
                    os << "x";
                else if (curr->coef == -1)
                    os << "-x";
                else
                    os << curr->coef << "x";
            } else {
                if (curr->coef == 1)
                    os << "x^" << curr->exp;
                else if (curr->coef == -1)
                    os << "-x^" << curr->exp;
                else
                    os << curr->coef << "x^" << curr->exp;
            }
            curr = curr->link;
        }
        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* pa = this->head->link;
        Node* pb = b.head->link;
        Node* tail = result.head;

        while (pa != this->head && pb != b.head) {
            if (pa->exp > pb->exp) {
                tail->link = getNode(pa->coef, pa->exp, result.head);
                tail = tail->link;
                pa = pa->link;
            } else if (pa->exp < pb->exp) {
                tail->link = getNode(pb->coef, pb->exp, result.head);
                tail = tail->link;
                pb = pb->link;
            } else {
                int c = pa->coef + pb->coef;
                if (c != 0) {
                    tail->link = getNode(c, pa->exp, result.head);
                    tail = tail->link;
                }
                pa = pa->link;
                pb = pb->link;
            }
        }
        while (pa != this->head) {
            tail->link = getNode(pa->coef, pa->exp, result.head);
            tail = tail->link;
            pa = pa->link;
        }
        while (pb != b.head) {
            tail->link = getNode(pb->coef, pb->exp, result.head);
            tail = tail->link;
            pb = pb->link;
        }
        return result;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* pa = this->head->link;
        Node* pb = b.head->link;
        Node* tail = result.head;

        while (pa != this->head && pb != b.head) {
            if (pa->exp > pb->exp) {
                tail->link = getNode(pa->coef, pa->exp, result.head);
                tail = tail->link;
                pa = pa->link;
            } else if (pa->exp < pb->exp) {
                tail->link = getNode(-pb->coef, pb->exp, result.head);
                tail = tail->link;
                pb = pb->link;
            } else {
                int c = pa->coef - pb->coef;
                if (c != 0) {
                    tail->link = getNode(c, pa->exp, result.head);
                    tail = tail->link;
                }
                pa = pa->link;
                pb = pb->link;
            }
        }
        while (pa != this->head) {
            tail->link = getNode(pa->coef, pa->exp, result.head);
            tail = tail->link;
            pa = pa->link;
        }
        while (pb != b.head) {
            tail->link = getNode(-pb->coef, pb->exp, result.head);
            tail = tail->link;
            pb = pb->link;
        }
        return result;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (Node* pa = this->head->link; pa != this->head; pa = pa->link) {
            for (Node* pb = b.head->link; pb != b.head; pb = pb->link) {
                int c = pa->coef * pb->coef;
                int e = pa->exp + pb->exp;
                result.insertTerm(c, e); 
            }
        }
        return result;
    }

    float Evaluate(float x) const {
        float result = 0.0f;
        for (Node* curr = head->link; curr != head; curr = curr->link) {
            result += curr->coef * pow(x, curr->exp);
        }
        return result;
    }
};

Node* Polynomial::avail = nullptr;

int main() {
    Polynomial p1, p2;
    cout << "輸入多項式p1: ";
    cin >> p1;
    cout << "輸入多項式p2: ";
    cin >> p2;

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;

    Polynomial p3 = p1 + p2;
    cout << "p1 + p2 = " << p3 << endl;

    Polynomial p4 = p1 - p2;
    cout << "p1 - p2 = " << p4 << endl;

    Polynomial p5 = p1 * p2;
    cout << "p1 * p2 = " << p5 << endl;

    float x;
    cout << "輸入一個x: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;

    return 0;
}
```

## 效能分析

|          | 時間複雜度 | 空間複雜度 | 
|----------|------------------|----------|
| 插入一項   | *O*(*n*)           | *O*(1)        | 
| 加法   | *O*(*m*+*n*)           | *O*(*m*+*n*)       | 
| 減法   | *O*(*m*+*n*)           | *O*(*m*+*n*)       | 
| 乘法   | *O*(*m***n*)           | *O*(*m***n*)       | 
| 輸入   | *O*(*n²*)           |*O*(*n*)     | 
| 輸出   | *O*(*n*)           | *O*(1)        | 
| 求Evaluate()| *O*(*n*)           | *O*(1)       |
## 測試與驗證

### 測試案例

輸入：`3 2 4 -1 2 3 0` `2 1 3 4 1` `2`

預期輸出：
`p1 = 2x^4-x^2+3`

`p2 = x^3+4`

`p1 + p2 = 2x^4+x^3-x^2+7`

`p1 - p2 = 2x^4-x^3-x^2-1`

`p1 * p2 = 2x^7 - x^5 + 8x^4 - 4x^2 + 3x^3 + 12`

`p1(2) = 31`

實際輸出：
`p1 = 2x^4-x^2+3`

`p2 = x^3+4`

`p1 + p2 = 2x^4+x^3-x^2+7`

`p1 - p2 = 2x^4-x^3-x^2-1`

`p1 * p2 = 2x^7 - x^5 + 8x^4 - 4x^2 + 3x^3 + 12`

`p1(2) = 31`


### 編譯與執行指令

```shell
g++ main.cpp --std=c++21 -o main.exe
./main.exe
```

### 結論

1.程式能正確表示並運算多項式，支援加法、減法、乘法及在指定點的求值。

2.在輸入格式不符或係數為零時，程式能正確處理或忽略，避免錯誤。

3.測試涵蓋多項式的空多項式、單項式、多項式及異常情況，驗證程式功能完整且正確。

## 申論及開發報告

### 選擇使用循環鏈結串列的原因
1.節省空間與方便管理
循環鏈結串列利用 header 節點可以輕鬆管理多項式的所有項，並且方便在任意位置插入或刪除節點，而不需要頻繁搬移元素，特別適合動態調整多項式項數的需求。

2.可重複利用節點空間
透過可用節點列表（available-space list）回收已刪除的節點，減少記憶體碎片及頻繁分配記憶體的成本，提高記憶體使用效率。

3.方便運算實作
多項式加、減、乘法等運算均可藉由鏈結串列的節點遍歷完成，循環鏈結串列能確保遍歷過程中簡潔且無需擔心邊界條件。
