// By considering the terms in the Fibonacci sequence whose values do not exceed four million, find the sum of the even-valued terms.
#include <iostream> // cout

const int MAX = 4000000;

// recursive function for fibonacci
int fib_rec(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fib_rec(n - 1) + fib_rec(n - 2);
    }
}

int main() {
    auto sum = 0;
    auto i = 0;
    while (true) {
        auto fib = fib_rec(i);
        if (fib > MAX) {
            break;
        }
        if (fib % 2 == 0) {
            sum += fib;
        }
        i++;
    }
    std::cout << "Sum = " << sum << std::endl;
}