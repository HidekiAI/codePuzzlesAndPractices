// The prime factors of 13195 are 5, 7, 13, and 29
// What is the largest prime factor of number 600851475143?

#include <iostream> // for cout
#include <cmath>    // for sqrt()

const long long NUM = 600851475143;


bool is_prime(long long number) {
    if (number < 2) {
        return false;
    }

    for (long long i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

int main() {
    long long largest_prime_factor = 0;
    const long long sqrt_number = sqrtl(NUM);   // round it to integer
    for (auto i = 2; i <= sqrt_number; i++) {
        if (NUM % i == 0 && is_prime(i)) {
            largest_prime_factor = i;
        }
    }

    std::cout << "The largest prime factor of number 600851475143 is: " << largest_prime_factor << std::endl;

    return 0;
}