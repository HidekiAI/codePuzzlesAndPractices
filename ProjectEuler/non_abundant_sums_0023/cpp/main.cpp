// A perfect number is a number for which the sum of its proper divisors is
// exactly equal to the number. For example, the sum of the proper divisors of
// 28 would be 1 + 2 + 4 + 7 + 14 = 28, which means that 28 is a perfect number.
//
// A number n is called deficient if the sum of its proper divisors is less than
// n and it is called abundant if this sum exceeds n.
//
// As 12 is the smallest abundant number, 1 + 2 + 3 + 4 + 6 = 16, the smallest
// number that can be written as the sum of two abundant numbers is 24. By
// mathematical analysis, it can be shown that all integers greater than 28123
// can be written as the sum of two abundant numbers. However, this upper limit
// cannot be reduced any further by analysis even though it is known that the
// greatest number that cannot be expressed as the sum of two abundant numbers
// is less than this limit.
//
// Find the sum of all the positive integers which cannot be written as the sum
// of two abundant numbers.
//
// Whiteboard:
// A small function to take a value n, which will find all its divisors, and
// then sum it all up, and return true if sum == n (a "perfect number"). i.e. n
// = 28, divisors = {1, 2, 4, 7, 14}, sum(1, 2, 4, 7, 14) == 28 (match!) Another
// function to check if n is deficient, abundant, or perfect.
// - A deficient number is when sum of divisors < n
// - An abundant number is when sum of divisors > n
// - A perfect number is when sum of divisors == n (above function)
// I want to prove that 12 is the smallest abundant number...
// 12's divisors are {1, 2, 3, 4, 6}, sum(1, 2, 3, 4, 6) == 16
// As defined, an abundant number is when sum of divisors 16 > 12, hence it is
// abundant. Next, I want to prove that 24 is the smallest number that can be
// written as the sum of two abundant numbers.
// 24's divisors are {1, 2, 3, 4, 6, 8, 12}, sum(1, 2, 3, 4, 6, 8, 12) == 36
// which means 24 is deficient because 36 > 24.
// Smallest number that can be written as the sum of two abundant numbers is 24,
// because 12 + 12 == 24, and 12 is the smallest abundant number.  I guess the
// rule does not say two unique abundant numbers, so 12 + 12 is valid.
// I want to prove that all integers greater than 28123 can be written as the
// sum of two abundant numbers.
// Because I need to find the sum of all positive integers which cannot be
// written as the sum of two abundant numbers, I need to find all abundant
// numbers first.  Then, I need to find all possible sums of two abundant
// numbers, and store them in a set.  Then, I need to find all positive integers
// that are not in the set of sums of two abundant numbers.
// That's just confusing...  oh well..

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

// Sum of all positive integers that cannot be written as the sum of two abundant numbers: 4179871
// Time taken: 4.46856s
// 
// real    0m4.566s
// user    0m0.000s
// sys     0m0.000s
// Wed, May  1, 2024 23:13:04
// Implementation time: 00:27:07.30 (~27 min)
const std::uint16_t MAX_NUMBER = 28123;

// function to find all divisors of n, and sum them up to see if it's a perfect
// number, abundant number, or deficient number
enum class NumberType { Deficient, Perfect, Abundant };
NumberType classify_number_by_summation(const std::uint16_t n) {
  std::uint16_t sum = 0;
  for (std::uint16_t i = 1; i < n; ++i) {
    if (n % i == 0) {
      sum += i;
    }
  }
  if (sum < n) {
    return NumberType::Deficient;
  } else if (sum > n) {
    return NumberType::Abundant;
  } else {
    // assume sum == n
    return NumberType::Perfect;
  }
}

// I need to find all abundant numbers first.  Then, I need to find all possible
// sums of two abundant numbers, and store them in a set.  Then, I need to find
// all positive integers that are not in the set of sums of two abundant numbers.
std::vector<std::uint16_t> find_abundant_numbers() {
  std::vector<std::uint16_t> abundant_numbers;
  for (std::uint16_t i = 1; i <= MAX_NUMBER; ++i) {
    if (classify_number_by_summation(i) == NumberType::Abundant) {
      abundant_numbers.push_back(i);
    }
  }
  return abundant_numbers;
}

// find all possible sums of two abundant numbers so that I can find all
// positive integers that are not in the set of sums of two abundant numbers
std::unordered_set<std::uint16_t> find_all_sums_of_two_abundant_numbers(
    const std::vector<std::uint16_t> &abundant_numbers) {
  std::unordered_set<std::uint16_t> sums;
  for (std::size_t i = 0; i < abundant_numbers.size(); ++i) {
    for (std::size_t j = i; j < abundant_numbers.size(); ++j) {
      sums.insert(abundant_numbers[i] + abundant_numbers[j]);
    }
  }
  return sums;
}

// find all positive integers that are NOT in the set of sums of two abundant
// numbers
std::uint64_t sum_of_all_positive_integers_not_sum_of_two_abundant_numbers(
    const std::vector<std::uint16_t> &abundant_numbers,
    const std::unordered_set<std::uint16_t> &sums_of_abundants) {
  std::uint64_t sum = 0;
  for (std::uint16_t i = 1; i <= MAX_NUMBER; ++i) {
    if (sums_of_abundants.find(i) == sums_of_abundants.end()) {
      sum += i;
    }
  }
  return sum;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  auto abundant_numbers = find_abundant_numbers();
  auto sums = find_all_sums_of_two_abundant_numbers(abundant_numbers);
  auto sum = sum_of_all_positive_integers_not_sum_of_two_abundant_numbers(
      abundant_numbers, sums);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Sum of all positive integers that cannot be written as the sum "
               "of two abundant numbers: "
            << sum << std::endl;
  std::cout << "Time taken: " << diff.count() << "s" << std::endl;
  return 0;
}
