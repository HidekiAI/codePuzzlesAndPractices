// Let d(n) be defined as the sum of proper divisors of n (numbers less than n
// which divide evenly into n). If d(a) = b and d(b) = a, where a ≠ b, then a
// and b are an amicable pair and each of a and b are called amicable numbers.
//
// For example, the proper divisors of 220 are 1, 2, 4, 5, 10, 11, 20, 22, 44,
// 55 and 110; therefore d(220) = 284. The proper divisors of 284 are 1, 2, 4,
// 71 and 142; so d(284) = 220.
//
// Evaluate the sum of all the amicable numbers under 10000.

// Whiteboard:
// d(a) = b
// d(b) = a
// a != b
// a and b are amicable pair
// what does it mean by "numbers less than n which divide evenly into n"?
// which means: m % n == 0; where m < n
//      sum := 0
//      for m := 1, 2, 3, ..., n - 1
//          if n % m == 0, then
//              m is a proper divisor of n
//              sum += m
//      d(n) = sum
// Now find d(a) = b and d(b) = a where a != b
// suppose a=220
// d(220) = 284
// now find d(284) = 220
// if found, then a and b are amicable pair

// Total time complexity: O(n^2)
// Total space complexity: O(1)
// where n is the limit

// My essentials that I always include:
#define _HAS_CXX20 1
#if _HAS_CXX20
#include <algorithm> // std::sort, std::transform, std::find (std::find - make sure to override operator==)
#include <array>
#include <cassert>    // assert()
#include <chrono>     // for start/end time measurement
#include <cstdint>    // std::uint16_t, etc - I'm too used to rust types...
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
// but without using static_assert() because it's not available until C++17
#error This code requires at least C++17
#endif // !_HAS_CXX20 || !_HAS_CXX17

using namespace std;

// Implementation time: 17m 14.56s
// Sum of all amicable numbers under 10000 is 31626
// Elapsed time: 0.799584 seconds
const std::int64_t LIMIT = 10000;

// calculate d(n)
std::int64_t sum_of_proper_divisors(std::int64_t n) {
  std::int64_t sum = 0;
  for (std::int64_t m = 1; m < n; m++) {
    if (n % m == 0) {
      sum += m;
    }
  }
  return sum;
}

// find amicable pair
// d(a) = b and d(b) = a, where a != b
std::int64_t find_amicable_pair(std::int64_t n) {
  std::int64_t a = n;
  std::int64_t b = sum_of_proper_divisors(a);
  if (a == b) {
    return 0;
  }
  if (a == sum_of_proper_divisors(b)) {
    return a;
  }
  return 0;
}

// find all amicable numbers under limit and add it all up
std::int64_t sum_of_amicable_numbers(std::int64_t limit) {
  std::int64_t sum = 0;
  for (std::int64_t n = 1; n < limit; n++) {
    sum += find_amicable_pair(n);
  }
  return sum;
}

int main() {
  auto start = chrono::high_resolution_clock::now();

  std::int64_t sum = sum_of_amicable_numbers(LIMIT);
  std::cout << "Sum of all amicable numbers under " << LIMIT << " is " << sum
            << std::endl;

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

  return 0;
}