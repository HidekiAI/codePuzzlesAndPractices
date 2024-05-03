// The Fibonacci sequence is defined by the recurrence relation:
//      Fn = Fn−1 + Fn−2, where F1 = 1 and F2 = 1.
// Hence the first 12 terms will be:
//    F1 = 1
//    F2 = 1
//    F3 = 2
//    F4 = 3
//    F5 = 5
//    F6 = 8
//    F7 = 13
//    F8 = 21
//    F9 = 34
//    F10 = 55
//    F11 = 89
//    F12 = 144
// The 12th term, F12, is the first term to contain three digits.
//
// What is the index of the first term in the Fibonacci sequence to contain 1000
// digits?

// Whiteboard:
// First, as always, declare a constant for the maximum number of digits of 1000
// Fibonacci is a recursive function, that we've written before many times.
//     uint64_t fibonacci(uint64_t n) {
//          if (n == 0) return 0;    // strive for tail-recursion
//          if (n == 1) return 1;
//          return fibonacci(n - 1) + fibonacci(n - 2);
//     }
// I think what I'd like to do, because I have no clue when it'll turn into 4
// digits so I'll randomly pick a number and see if it's 2 digits (we know
// fib(12) is the first 3 digit number) and when it's 3 digits, take mid-point
// and try again (binary search'ish approach)
// Hindsight: After re-reading the problem, I realized that it's asking for
// index with 1000 digits, not the number itself...
// I now need a method in which the fibonacci function returns a vector of
// digits, and I can just check the size of the vector.

// My essentials that I always include:
#define _HAS_CXX20 1
#if _HAS_CXX20
#include <algorithm> // std::sort, std::transform, std::find (std::find - make sure to override operator==)
#include <array>
#include <cassert>    // assert()
#include <chrono>     // for start/end time measurement
#include <cstdint>    // std::uint16_t, etc - Im too used to rust types...
#include <fstream>    // for reading in file
#include <functional> // lambdas!
#include <iostream>   // std::cout
#include <memory>     // std::shared_ptr, std::make_shared
#include <optional>   // a bit different from Rust Option<T> but still, useful!
#include <string>
#include <tuple>
#include <unordered_map> // use map if need keys to be ordered, but generally, I just need key to be hashed...
#include <unordered_set>
#include <utility> // std::pair, etc
#include <vector>
#else
// fail compiler if C++ version is less than C++20
// but without using static_assert() because its not available until C++17
#error This code requires at least C++17
#endif // !_HAS_CXX20 || !_HAS_CXX17

using namespace std;

const std::uint16_t MAX_DIGITS = 10^1000;
const std::uint16_t FIRST_3_DIGIT_INDEX = 12;

vector<std::uint8_t> fibonacci_large(vector<std::uint8_t> n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

//// given 2 numbers n and m, return index that results to 3 digits
//uint32_t find_index(uint32_t n, uint32_t m,
//                    uint32_t smallest_3_digits_index_sofar) {
//  assert(n > FIRST_3_DIGIT_INDEX);
//  assert(m > FIRST_3_DIGIT_INDEX);
//  std::cout << "Testing range [" << n << ", " << m
//            << "], smallest so far =" << smallest_3_digits_index_sofar
//            << " (fibonacci: " << fibonacci(smallest_3_digits_index_sofar)
//            << ")" << std::endl;
//  if (n == m)
//    return smallest_3_digits_index_sofar;
//
//  uint32_t mid = (n + m) / 2; // should be > FIRST_3_DIGIT_INDEX
//  uint64_t fib = fibonacci(mid);
//  if ( fib < MAX_DIGITS) {
//    // smallest is now mid
//    return find_index(mid + 1, m, mid);
//  } else if (fib >= MAX_DIGITS) {
//    return find_index(n, mid, smallest_3_digits_index_sofar);
//  } else {
//    return find_index(mid + 1, m, smallest_3_digits_index_sofar);
//  }
//}


// Brute force: Iterate through fibonacci numbers until we find one with 1000
// digits
int main() {
  auto start = std::chrono::high_resolution_clock::now();

  // just as a test, we'll start with 12 and 24 and narrow
  uint32_t index =
      find_index(FIRST_3_DIGIT_INDEX + 1, FIRST_3_DIGIT_INDEX * 2, 0) + 1;
  std::cout << "Index: " << index << std::endl;
  std::cout << "Fibonacci: " << index - 1 << " = " << fibonacci(index - 1)
            << std::endl;
  std::cout << "Fibonacci: " << index << " = " << fibonacci(index) << std::endl;
  std::cout << "Fibonacci: " << index + 1 << " = " << fibonacci(index + 1)
            << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Duration: " << diff.count() << " s" << std::endl;

  return 0;
}
