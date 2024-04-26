// The sum of pimes below 10 is 2 + 3 + 5 + 7 = 17
// Find the sum of all the primes below two million

// Whiteboard:
// Stealing is_prime from problem one of the previously worked on prime-related problems

#include <iostream>     // std::cout    
#include <cmath>        // std::sqrt

const int MAX = 2000000;

bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i <= std::sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    long long sum = 0;
    for (int i = 2; i < MAX; i++) {
        if (is_prime(i)) {
            sum += i;
        }
    }
    std::cout << sum << std::endl;
    return 0;
}