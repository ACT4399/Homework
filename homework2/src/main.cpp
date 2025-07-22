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
