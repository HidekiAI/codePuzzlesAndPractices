// If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.
// Find the sum of all the multiples of 3 or 5 below 1000.

#include <iostream> // include for cin ant cout

bool is_multiple_of_3_or_5(int number) {
    return number % 3 == 0 || number % 5 == 0;
}

int main() {
    int sum = 0;
    for (int i = 0; i < 1000; i++) {
        if (is_multiple_of_3_or_5(i)) {
            sum += i;
        }
    }
    std::cout << sum << std::endl;
    return 0;
}