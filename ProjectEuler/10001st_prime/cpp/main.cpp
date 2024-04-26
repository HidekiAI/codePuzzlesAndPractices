// By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.
// What is the 10001st prime number?

// Whiteboard:
// Either a vector or counter to keep track of prime numbers up to the 10,001st prime number.
// Since the vector will just be wastefull, I will use a counter.
// I need a function that would take in a number and return true if it is a prime number.
// Though that is trivial, I'll just copy and paste the is_prime function from the largest_prime_factor snippet.
// And finally, just iterate from 2 to infinity and keep track of the number of prime numbers found
// up until the 10,001st prime number is found.

#include <iostream> // std::cout

const long long NUM = 10001;

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
    long long prime_count = 0;
    long long i = 2;
    while (true) {
        if (is_prime(i)) {
            prime_count++;
        }
        if (prime_count == NUM) {
            break;
        }
        i++;
    }
    std::cout << "The " << NUM << "st prime number is " << i << std::endl;
    return 0;
}   