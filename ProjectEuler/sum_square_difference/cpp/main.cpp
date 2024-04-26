// The sum of squares of the frist ten natural numbers is,
//      1^2 + 2^2 + ... + 10^2 = 385
// The square of the sum of the first ten natural numbers is,
//      (1 + 2 + ... + 10)^2 = 55^2 = 3025
// Hence the difference between the sum of the squares of the 
// first ten natural numbers and the square of the sum is 
// 3025 - 385 = 2640.
// Find the difference between the sum of the squares of the
// first one hundred natural numbers and the square of the sum.

// Whiteboard:
// define a function that takes in a number and returns the sum of the squares of the first n natural numbers
// define a function that takes in a number and returns the square of the sum of the first n natural numbers

#include <iostream> // std::cout
//#include <cmath> // std::pow

const int N = 100;

int sum_of_squares(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i * i;
    }
    return sum;
}

int square_of_sum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum * sum;
}

int main() {
    int sum_squares = sum_of_squares(N);
    int square_sum = square_of_sum(N);
    int difference = square_sum - sum_squares;
    std::cout << "The difference between the sum of the squares of the first " << N << " natural numbers and the square of the sum is " << difference << std::endl;
    return 0;
}
