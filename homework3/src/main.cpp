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
