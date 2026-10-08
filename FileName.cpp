#include <iostream>
#include <cmath>
using namespace std;

// 定義節點結構
struct Node {
    int coef;  // 係數
    int exp;   // 指數
    Node* next; // 下一個節點指標

    Node(int c = 0, int e = 0, Node* n = nullptr) : coef(c), exp(e), next(n) {}
};

// 定義 Polynomial 類別
class Polynomial {
private:
    Node* head; // 頭結點 (環形鏈表的頭結點)

public:
    // 預設構造函數
    Polynomial() {
        head = new Node(); // 建立頭結點
        head->next = head; // 初始化為環形鏈表
    }

    // 複製構造函數
    Polynomial(const Polynomial& a) {
        head = new Node();
        head->next = head;
        Node* current = a.head->next;
        while (current != a.head) {
            addTerm(current->coef, current->exp);
            current = current->next;
        }
    }

    // 賦值運算符
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            clear();
            Node* current = a.head->next;
            while (current != a.head) {
                addTerm(current->coef, current->exp);
                current = current->next;
            }
        }
        return *this;
    }

    // 析構函數
    ~Polynomial() {
        clear();
        delete head;
    }

    // 輸入多項式
    friend istream& operator>>(istream& is, Polynomial& x) {
        int n; // 項數
        is >> n;
        for (int i = 0; i < n; i++) {
            int coef, exp;
            is >> coef >> exp;
            x.addTerm(coef, exp);
        }
        return is;
    }

    // 輸出多項式
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        Node* current = x.head->next;
        bool first = true;
        while (current != x.head) {
            if (!first) os << " ";
            os << current->coef << "x^" << current->exp;
            first = false;
            current = current->next;
        }
        return os;
    }

    // 加法運算符
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->next;
        Node* p2 = b.head->next;

        while (p1 != head && p2 != b.head) {
            if (p1->exp > p2->exp) {
                result.addTerm(p1->coef, p1->exp);
                p1 = p1->next;
            }
            else if (p1->exp < p2->exp) {
                result.addTerm(p2->coef, p2->exp);
                p2 = p2->next;
            }
            else {
                result.addTerm(p1->coef + p2->coef, p1->exp);
                p1 = p1->next;
                p2 = p2->next;
            }
        }

        while (p1 != head) {
            result.addTerm(p1->coef, p1->exp);
            p1 = p1->next;
        }
        while (p2 != b.head) {
            result.addTerm(p2->coef, p2->exp);
            p2 = p2->next;
        }

        return result;
    }

    // 減法運算符
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->next;
        Node* p2 = b.head->next;

        while (p1 != head && p2 != b.head) {
            if (p1->exp > p2->exp) {
                result.addTerm(p1->coef, p1->exp);
                p1 = p1->next;
            }
            else if (p1->exp < p2->exp) {
                result.addTerm(-p2->coef, p2->exp);
                p2 = p2->next;
            }
            else {
                result.addTerm(p1->coef - p2->coef, p1->exp);
                p1 = p1->next;
                p2 = p2->next;
            }
        }

        while (p1 != head) {
            result.addTerm(p1->coef, p1->exp);
            p1 = p1->next;
        }
        while (p2 != b.head) {
            result.addTerm(-p2->coef, p2->exp);
            p2 = p2->next;
        }

        return result;
    }

    // 乘法運算符
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->next;
        while (p1 != head) {
            Node* p2 = b.head->next;
            while (p2 != b.head) {
                result.addTerm(p1->coef * p2->coef, p1->exp + p2->exp);
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        return result;
    }

    // 求值函數
    float Evaluate(float x) const {
        float result = 0;
        Node* current = head->next;
        while (current != head) {
            result += current->coef * pow(x, current->exp);
            current = current->next;
        }
        return result;
    }

private:
    // 添加一項到多項式中
    void addTerm(int coef, int exp) {
        if (coef == 0) return;

        Node* prev = head;
        Node* current = head->next;
        while (current != head && current->exp > exp) {
            prev = current;
            current = current->next;
        }

        if (current != head && current->exp == exp) {
            current->coef += coef;
            if (current->coef == 0) {
                prev->next = current->next;
                delete current;
            }
        }
        else {
            Node* newNode = new Node(coef, exp, current);
            prev->next = newNode;
        }
    }

    // 清空多項式
    void clear() {
        Node* current = head->next;
        while (current != head) {
            Node* tmp = current;
            current = current->next;
            delete tmp;
        }
        head->next = head;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "Enter first polynomial: ";
    cin >> p1;
    cout << "Enter second polynomial: ";
    cin >> p2;

    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "p1 + p2: " << sum << endl;
    cout << "p1 - p2: " << diff << endl;
    cout << "p1 * p2: " << prod << endl;

    float x;
    cout << "Enter a value for x: ";
    cin >> x;

    cout << "p1 evaluated at x = " << x << ": " << p1.Evaluate(x) << endl;
    cout << "p2 evaluated at x = " << x << ": " << p2.Evaluate(x) << endl;

    return 0;
}
