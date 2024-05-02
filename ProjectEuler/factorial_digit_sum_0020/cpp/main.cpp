// n! means n × (n − 1) × ... × 3 × 2 × 1
// For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
// and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0
// = 27. Find the sum of the digits in the number 100!

// Whiteboard:
// - a method to turn n! into array of digits (i.e. string char[])
// - does not matter from left to right or right to left, just sum them up
// Hindsight: it turns out 100! does not fit into 64-bit integer; it's 158
// digits long according to the calculator (9.3e+157).  So, we need to use
// array of digits to store the number.
// factorial is done recursively (pseudo code):
//      factorial(n) {
//       if n == 0 { return 1 }
//       return n * factorial(n - 1)
//      }
// But now, we will have to store the result into array of digits.
// First thing first, we need to come up with ways to multiply a number with
// another number, and store the result into array of digits.
// There there are probably other shortcuts similar to the power of 2
// multiplication excercise (the one about 2^1000), as a starter, will
// brute-force by iteration.
// If n = 3, then 3! = 1 * 2 * 3 = 2 * 3 = 6
// or (1 + 1) * 3 = 2 * 3 = 2 + 2 + 2 = 6
// meaning, we just use the next number to iterate n+1 times of addition.
// So for n=10: 10! = 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10
//      x = 2
//      m = x + 1 // the next number
//      s = for i in 1..m { x += x } // if m = 3, and x = 2, then x = 2 + 2 + 2
//      = 6 and next x is 2 + 1 = 3, repeat...
// To optimize, the highest factorial that will fit in a 64-bit unsigned integer
// is 20! = 2432902008176640000, which is 19 digits long.  So, we can just
// test to see if n < 20, then we can just use the regular factorial() function
// that returns a 64-bit unsigned integer.  Otherwise, we will have to use
// the array of digits to store the result which starts from 21! (n=21).

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

// 100! = 9.3e+157 (158 digits) -> sum of digits = 648
const uint16_t N_FACTORIAL_BASE = 100;

// Though we can just bunch it up into single function, will split functions
// into:
// - get factorial(n)
// - get digits(f)
std::vector<std::uint8_t> get_digits(std::uint64_t f) {
  std::vector<std::uint8_t> digits;
  while (f > 0) {
    digits.push_back(f % 10);
    f /= 10;
  }
  std::reverse(digits.begin(), digits.end()); // swaps in place
  return digits;
}

// when n < 20, we can just use the regular factorial() recursive function
std::uint64_t factorial(std::uint16_t n) {
  if (n == 0) {
    return 1;
  }
  return n * factorial(n - 1);
}

void dump_digits(std::vector<std::uint8_t> &digits) {
  for (auto d : digits) {
    std::cout << (int)d;
  }
}

std::vector<std::uint8_t> add_large_numbers(std::vector<std::uint8_t> &a,
                                            std::vector<std::uint8_t> &b) {
  std::vector<std::uint8_t> sum_rev;
  auto left_rev = a; // copy so we can reverse in place
  auto right_rev = b;
  std::reverse(left_rev.begin(), left_rev.end());   // reverse in place
  std::reverse(right_rev.begin(), right_rev.end()); // reverse in place

  // we cannot assume a and b are both of same width!
  auto max_width = std::max(left_rev.size(), right_rev.size());
  std::uint16_t carry = 0;
  // from left to right, add each column
  for (auto col = 0; col < max_width; ++col) {
    // DO NOT assume that values on l and r are of same width!
    // NOTE that this was the bug that I had spent about 40 minutes to find...
    // if this was Rust, it would have caught it either during static analysis or runtime
    // because Rust does not allow you to access an array element that is out of bounds
    // this is why I want to quit using C++!
    auto l = col < left_rev.size() ? left_rev[col] : 0;
    auto r = col < right_rev.size() ? right_rev[col] : 0;
    uint16_t column_sum = l + r + carry;
    carry = column_sum / 10;
    sum_rev.push_back(column_sum % 10);
  }
  while (carry > 0) {
    sum_rev.push_back(carry % 10);
    carry /= 10;
  }
  std::reverse(sum_rev.begin(), sum_rev.end());

  // dump_digits(b);
  // std::cout << " + ";
  // dump_digits(a);
  // std::cout << " = ";
  // dump_digits(sum_rev);
  // std::cout << endl;

  return sum_rev;
}

// for n > 20, we will have to use array of digits
// unlike the regular factorial() function which goes down to 0 (n - 1)
// this version will go up to n starting from 20!
// 3! = 1 * 2 * 3
//    = (1 * 2) * 3 = 2 * 3
// 100! = (1 * 2 * 3 ... 20) * 21 * (22 * ... 99 * 100)
//      current = [1 * 2 * 3 ... 20]
//      i = 21
//      x = (22 * ... 99 * 100)
// 100! = current * i * x
// new_current = current * i
//             = current + current + ... + current (i times)
// 10! = 10 * 9 * (8 *... * 2 * 1)
//     = (10 + 10 + ... + 10 [9 times]) * (8 * ... * 2 * 1)
std::vector<std::uint8_t> factorial_21(std::uint64_t n) {
  auto current_n = 20; // start with 20!
  auto current = get_digits(factorial(current_n));

  while (current_n < n) {
    current_n += 1;

    // std::cout << " current_n(" << current_n << ") x ";
    // dump_digits(current);
    // std::cout << std::endl;

    // i.e. if current_n = 21, then we will add current 21 times (21 * current)
    auto new_current = current;
    for (auto i = 0; i < current_n - 1; ++i) {
      // std::cout << i << ": ";
      new_current = add_large_numbers(new_current, current);
    }

    current = new_current;
  }

  // now, return the final result
  return current;
}

std::vector<std::uint8_t> get_digits_factorial(std::uint16_t n) {
  if (n < 20) {
    std::uint64_t f = factorial(n);
    return get_digits(f);
  }
  // else, we will have to use array of digits
  return factorial_21(n);
}

void unit_test(int test_n) {
  auto a = get_digits(2432902008176640000);
  auto loop_counter = 6; // something was/is broken, if counter is 5, it works,
                         // but fails on 6...
  auto b = a;
  for (auto i = 1; i < loop_counter; ++i) {
    a = add_large_numbers(a, b);
  }
  std::cout << "Sum: ";
  dump_digits(a);
  std::cout << std::endl;
  auto expected_product = get_digits(14597412049059840000);
  assert(a == expected_product);

  std::cout << "Unit test: n=" << test_n << std::endl;
  auto digits = get_digits_factorial(test_n);
  std::cout << "factorial(" << test_n << ") - There are " << digits.size()
            << " digits" << endl;
  std::cout << "Digits: ";
  dump_digits(digits);
  std::cout << endl;
  std::string digits_to_string = "";
  for (auto d : digits) {
    digits_to_string += std::to_string(d);
  }
  auto digits_sum = 0;
  for (auto d : digits) {
    digits_sum += d;
  }

  if (test_n == 21) {
    auto factorial_21 = "2432902008176640000"; //"2,432,902,008,176,640,000"
    auto expected_sum =
        2 + 4 + 3 + 2 + 9 + 0 + 2 + 0 + 0 + 8 + 1 + 7 + 6 + 6 + 4; //= 48
    assert(digits.size() == 19);
    assert(digits_to_string == factorial_21);
    assert(digits_sum == expected_sum);
  } else if (test_n == 22) {
    auto factorial_22 = "1124000727777607680000";
    auto expected_sum = 1 + 1 + 2 + 4 + 0 + 0 + 0 + 0 + 7 + 2 + 7 + 7 + 7 + 7 +
                        6 + 0 + 7 + 6 + 8; //  = 48
    assert(digits.size() == 22);
    assert(digits_to_string == factorial_22);
    assert(digits_sum == expected_sum);
  } else if (test_n == 23) {
    auto factorial_23 = "25852016738884976640000";
    auto expected_sum = 2 + 5 + 8 + 5 + 2 + 0 + 1 + 6 + 7 + 3 + 8 + 8 + 4 + 9 +
                        7 + 6 + 6 + 4; //  = 88
    assert(digits.size() == 23);
    assert(digits_to_string == factorial_23);
    assert(digits_sum == expected_sum);
  } else if (test_n == 24) {
    auto factorial_24 = "620448401733239439360000";
    auto expected_sum = 6 + 2 + 0 + 4 + 4 + 8 + 4 + 0 + 1 + 7 + 3 + 3 + 2 + 3 +
                        9 + 4 + 3 + 9 + 3 + 6; //  = 88
    assert(digits.size() == 26);
    assert(digits_to_string == factorial_24);
    assert(digits_sum == expected_sum);
  } else if (test_n == 25) {
    auto factorial_25 = "15511210043330985984000000";
    auto expected_sum = 1 + 5 + 5 + 1 + 1 + 2 + 1 + 0 + 0 + 4 + 3 + 3 + 3 + 0 +
                        9 + 8 + 5 + 9 + 8 + 4; //  = 88
    assert(digits.size() == 26);
    assert(digits_to_string == factorial_25);
    assert(digits_sum == expected_sum);
  } else if (test_n == 27) {
    auto factorial_27 = "10888869450418352160768000000";
    auto expected_sum = 1 + 0 + 8 + 8 + 8 + 8 + 6 + 9 + 4 + 5 + 0 + 4 + 1 + 8 +
                        3 + 5 + 2 + 1 + 6 + 0 + 7 + 6 + 8; //  = 88
    assert(digits.size() == 28);
    assert(digits_to_string == factorial_27);
    assert(digits_sum == expected_sum);
  } else if (test_n == 29) {
    auto factorial_29 = "8841761993739701954543616000000";
    auto expected_sum = 8 + 8 + 4 + 1 + 7 + 6 + 1 + 9 + 9 + 3 + 7 + 3 + 9 + 7 +
                        0 + 1 + 9 + 5 + 4 + 5 + 4 + 3 + 6 + 1 + 6; //  = 88
    assert(digits.size() == 31);
    assert(digits_to_string == factorial_29);
    assert(digits_sum == expected_sum);
  }
  std::cout << "Unit test passed!" << endl << endl;
}

int main() {
  // unit_test(21);
  // unit_test(22);
  // unit_test(23);
  // unit_test(24);
  // unit_test(25);

  auto start = std::chrono::high_resolution_clock::now();
  std::vector<std::uint8_t> digits = get_digits_factorial(N_FACTORIAL_BASE);
  std::cout << "factorial(" << N_FACTORIAL_BASE << ") - There are "
            << digits.size() << " digits" << endl;
  std::cout << "Digits: ";
  dump_digits(digits);
  std::cout << endl;
  std::int32_t sum = 0;
  for (auto d : digits) {
    sum += d;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Execution time: " << duration.count() << "ms" << std::endl;

  auto expected_sum = 648; // n=100 => 648
  std::cout << "Total sum: " << sum << " - Expected sum: " << expected_sum
            << std::endl;
  assert(sum == expected_sum);
  return 0;
}
