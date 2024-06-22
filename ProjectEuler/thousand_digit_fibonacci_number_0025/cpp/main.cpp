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
//
// Hindsight: After re-reading the problem, I realized that it's asking for
// index with 1000 digits, not the number itself...
// I now need a method in which the fibonacci function returns a vector of
// digits, and I can just check the size of the vector.
// At least from this work, I now have a library of functions to both
// Add and Subtract large numbers (fibonacci of large number is also good,
// but all I need for that is add large number)

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
#include <stack>  // commonly used when I need to convert recursive to iterative
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

// Fibonacci_rec(12) = 144
// Fibonacci_iter(12) = 144
// Expected: 144
// Result: 144
// Unit test duration: 0.0040736 s
// Final Index: 4782 - 1000 digits
// Duration: 426.793 s
const std::uint16_t MAX_DIGITS = 1000; // basically, 10 ^ 1000

// NOTE: I'm using vector<uint32_t> instead of string and/or vector<uint8_t>
// because uint8_t gives really unpredictable output routed to std::cout
// because cout << thinks it's a char, and I don't want to bother wasting
// time doing std::cout << static_cast<int>(some_uint8_t_value) every time
void dump_digits(std::vector<std::uint32_t> digits) {
  for (auto digit : digits) {
    std::cout << digit;
  }
}

std::vector<std::uint32_t> i64_to_digits(std::uint64_t index) {
  auto digits = std::vector<std::uint32_t>();
  while (index > 0) {
    digits.push_back(index % 10);
    index /= 10;
  }
  std::reverse(digits.begin(), digits.end());
  return digits;
}

// We need comparitors for the vector<uint32_t>
bool op_equal(vector<std::uint32_t> left, vector<std::uint32_t> right) {
  if (left != right) {
    // usually, the built-in version is optimized and faster than manual
    return false;
  }
  if (left.size() != right.size()) {
    return false;
  }
  for (int i = 0; i < left.size(); i++) {
    if (left[i] != right[i]) {
      return false;
    }
  }
  return true;
}
bool op_equal_tup(
    std::tuple<bool /*sign*/, vector<std::uint32_t> /*digits*/> left,
    std::tuple<bool /*sign*/, vector<std::uint32_t> /*digits*/> right) {
  if (std::get<1>(left).size() != std::get<1>(right).size()) {
    return false;
  }
  return op_equal(std::get<1>(left), std::get<1>(right));
}

bool op_less_than(vector<std::uint32_t> left, vector<std::uint32_t> right) {
  if (left < right) {
    // usually, the built-in version is optimized and faster than manual
    return true;
  }
  if (left.size() != right.size()) {
    return left.size() < right.size();
  }
  for (int i = 0; i < left.size(); i++) {
    if (left[i] != right[i]) {
      return left[i] < right[i];
    }
  }
  return false;
}
bool op_less_than_tup(
    std::tuple<bool /*sign*/, vector<std::uint32_t> /*digits*/> left,
    std::tuple<bool /*sign*/, vector<std::uint32_t> /*digits*/> right) {
  return op_less_than(std::get<1>(left), std::get<1>(right));
}

// subtaction based assumptions that left is >= right AND both
// left AND right are POSITIVE numbers
vector<std::uint32_t> op_subtract(vector<std::uint32_t> left,
                                  vector<std::uint32_t> right) {
  if (op_equal(left, right)) {
    return {0};
  }
  // first, clone so that we can reverse it
  auto left_clone = left;
  auto right_clone = right;
  std::reverse(left_clone.begin(), left_clone.end());   // reverses in-place
  std::reverse(right_clone.begin(), right_clone.end()); // reverses in-place
  // we won't eve throw, we will ASSUME left > right (we already checked if
  // equal)
  // 87654
  // - 780
  // =====
  // (4 - 0) - 0 = 4 (no borrow)
  // (5 - 0) - 8 = 7 (borrow from next digit)
  // (6 - 1) - 7 = 8 (borrow from next digit)
  // (7 - 1) - 0 = 6 (no borrow)
  // (8 - 0) - 0 = 8 (no borrow)
  // 87654 - 780 = 86874
  // we'll update left-clone in-place
  for (auto left_index = 0; left_index < left_clone.size(); left_index++) {
    auto left_digit = left_clone[left_index];
    auto right_digit =
        (left_index < right_clone.size()) ? right_clone[left_index] : 0;
    if (left_digit < right_digit) {
      // borrow from the next digit
      left_clone[left_index] += 10;
      left_clone[left_index + 1] -= 1;
    }
    left_clone[left_index] -= right_digit;
  }

  // reverse the result
  std::reverse(left_clone.begin(), left_clone.end());
  return left_clone;
}

// Note: unlike the other excercise, we'll add IN-PLACE so we do
// not need to mess with any carry-over values
std::vector<std::uint32_t> op_add(std::vector<std::uint32_t> left,
                                  std::vector<std::uint32_t> right) {
  // first, clone so that we can reverse it
  auto left_clone = left.size() > right.size() ? left : right;
  auto right_clone = left.size() > right.size() ? right : left;
  std::reverse(left_clone.begin(), left_clone.end());   // reverses in-place
  std::reverse(right_clone.begin(), right_clone.end()); // reverses in-place

  // 87654
  // 91239
  // =====
  // (4 + 9) = 13 = 3 (carry-over 1)
  // (5 + 3) + 1 = 9 (carry-over 0)
  // (6 + 2) = 8 (carry-over 0)
  // (7 + 1) = 8 (carry-over 0)
  // 8 + 9 = 17 = 7 (carry-over 1)
  // 91239 (5 digits) + 87654 (5 digits) = 178893 (6 digits)
  for (auto left_index = 0; left_index < left_clone.size(); left_index++) {
    // add and carry-over if needed; Note that because we use u8 type instead of
    // char '0'..'9' we can actually add in place (i.e. 9 + 4 = 13) and then
    // just subtract 10 if >= 10
    if (left_index < right_clone.size()) {
      // make sure right[i] exists...
      left_clone[left_index] += right_clone[left_index];
    }

    // TODO: some reason, when I do 'if (left_clone[left_index] >= 10)' it
    //       doesn't work and I have to preset my carry over - figure out why
    auto carry_over = left_clone[left_index] / 10; // 23 / 10 = 2
    if (carry_over > 0) {
      left_clone[left_index] = left_clone[left_index] % 10; // 23 % 10 = 3
      // if it's the last digit, we have to add a new digit, else +1 to next
      // digit.  We shouldn't have to do the push_back since we resized it
      // (left_clone.size()) earlier, but just in case, we'll leave this extra
      // check so that when I port this to rust, it'l be a reminder...
      if (left_index + 1 < left_clone.size()) {
        left_clone[left_index + 1] += carry_over;
      } else {
        left_clone.push_back(carry_over);
      }
    }
  }

  // trim: remove the last digit if it's 0 (43210 , strip it to 4321)
  while (left_clone.back() == 0) {
    left_clone.pop_back();
  }

  // reverse the result
  std::reverse(left_clone.begin(), left_clone.end());
  return left_clone;
}

// operator subtract (to calculate n-1 and n-2)
std::tuple<bool /*is_positive*/, vector<std::uint32_t> /*digits*/>
op_subtract_tuple(
    std::tuple<bool /*is_positive*/, vector<std::uint32_t> /*digits*/> left,
    std::tuple<bool /*is_positive*/, vector<std::uint32_t> /*digits*/> right) {
  // first, clone so that we can reverse it
  auto left_clone = std::get<1>(left);
  auto right_clone = std::get<1>(right);
  auto left_is_positive = std::get<0>(left);
  auto right_is_positive = std::get<0>(right);
  std::reverse(left_clone.begin(), left_clone.end());   // reverses in-place
  std::reverse(right_clone.begin(), right_clone.end()); // reverses in-place
  auto result_is_positive = op_less_than_tup(left, right) ? false : true;

  // now we can subtract (paying attention to assumptions that
  // width/size/length of n.size() != m.size()) but also realizing that n - m
  // != m - n
  auto max_width = std::max(left_clone.size(), right_clone.size());
  auto result = vector<std::uint32_t>();
  // n - m = -m + n
  // 123 - 87654 = -87654 + 123 = -87531
  // 87654
  // - 123
  // =====
  //  4 - 3 = 1
  //  5 - 2 = 3
  //  6 - 1 = 5
  //  7 - 0 = 7
  //  8 - 0 = 8
  // sign it => -87531
  // very trivial elementary school math, but doing this in code makes it seem
  // complex :P
  // * (+l) - (+r) = subtract (l-r) or (r-l) and if r > l, negate;
  //   i.e. 10 - 5 = 5; 5 - 10 = -5
  // * (+l) - (-r) = (+l) + (+r) = just addition;
  //   i.e. 10 - -5 = 10 + 5 = 15; 5 - -10 = 5 + 10 = 15
  // * (-l) - (+r) = just add and negate;
  //   i.e. -10 - 5 = -10 + -5 = -15; -5 - 10 = -5 + -10 = -15
  // * (-l) - (-r) = (-l) + (+r) = subtract (r-l) or (l-r) and if r > l, negate
  //  i.e. -10 - -5 = -10 + 5 = -5; -5 - -10 = -5 + 10 = 5

  // * (+l) - (+r) = subtract (l-r) or (r-l) and if r > l, negate;
  //   i.e. 10 - 5 = 5; 5 - 10 = -5
  if (left_is_positive && right_is_positive) {
    result_is_positive = true;
    // if both are postive, first check if left < right, if so, swap and set
    // (and later negate)
    if (op_less_than(left_clone, right_clone)) {
      // std::swap(left_clone, right_clone); // alternatively, just r - l
      result_is_positive = false;
      result = op_subtract(right_clone, left_clone); // swap left and right
    } else {
      result = op_subtract(left_clone, right_clone);
    }
  }
  // * (+l) - (-r) = (+l) + (+r) = just addition;
  //   i.e. 10 - -5 = 10 + 5 = 15; 5 - -10 = 5 + 10 = 15
  else if (left_is_positive != right_is_positive) {
    // if right is negative, then it's just addition (n - -m = n + m)
    // if left is negative, swap, then add, and negate
    result_is_positive = left_is_positive;    // negate if left is negative
    result = op_add(left_clone, right_clone); // either way, just add...
  }
  // * (-l) - (+r) = just add and negate;
  //   i.e. -10 - 5 = -10 + -5 = -15; -5 - 10 = -5 + -10 = -15
  else if (!left_is_positive && right_is_positive) {
    result_is_positive = false;
    // if left is negative, then it's just addition (n - -m = n + m)
    // if right is negative, swap, then add, and negate
    result = op_add(left_clone, right_clone); // either way, just add...
  }
  // * (-l) - (-r) = (-l) + (+r) = subtract (r-l) or (l-r) and if r > l, negate
  //  i.e. -10 - -5 = -10 + 5 = -5; -5 - -10 = -5 + 10 = 5
  else if (!left_is_positive && !right_is_positive) {
    result_is_positive = true;
    // if both are negative, make right side positive and then
    // test to see if m > n, and if so, just subtract (-n - -m = m - n)
    // else, swap again, and then subtract and then negate (n - m)

    // first, make right side positive (i.e. -left + right)
    if (op_less_than(left_clone, right_clone)) {
      // now that right side is positive, we can subtract right - left
      // (basically, swapped) IF right > left
      result = op_subtract(right_clone, left_clone);
    } else {
      // because right < left, doing (right - left) will need swapped
      // to (left - right) and negate
      result = op_subtract(left_clone, right_clone); // left > right
      result_is_positive = false;
    }
  }

  // finally, reverse the result
  std::tuple<bool, vector<std::uint32_t>> result_tuple = {result_is_positive,
                                                          result};
  return result_tuple;
}

// ideally, I'd probably want to have functions jut for finding fibonacci
// that are within the range to fit in uint64_t, but it'll just cause
// code-pollutions, so we'll just handle all cases in one function...
// constraints: for fibonacci, n must be > 0
vector<std::uint32_t> fibonacci_large_rec(vector<std::uint32_t> n) {
  if (n.size() == 0) {
    throw std::invalid_argument("n must be > 0");
  }
  auto result = vector<std::uint32_t>();
  if ((n.size() == 1) && (n[0] == 0)) {
    result.push_back(0);
    return result;
  }
  if ((n.size() == 1) && (n[0] == 1)) {
    result.push_back(1);
    return result;
  }
  auto n_minus_1 = op_subtract(n, {1});
  auto n_minus_2 = op_subtract(n, {2});
  auto fib_n_minus_1 = fibonacci_large_rec(n_minus_1);
  auto fib_n_minus_2 = fibonacci_large_rec(n_minus_2);
  auto added = op_add(fib_n_minus_1, fib_n_minus_2);
  return added;
}

// iterative version of fibonacci because trying to achieve 1000 digits
// in recursive version is not only slow, but also stack-overflow prone
vector<std::uint32_t> fibonacci_large_iter(uint64_t n) {
  if (n == 0) {
    return {0};
  }
  if (n == 1) {
    return {1};
  }
  auto n_minus_1 = std::vector<std::uint32_t>{1};
  auto n_minus_2 = std::vector<std::uint32_t>{0};
  auto fib_n = std::vector<std::uint32_t>{};
  // begin iteration from 2..=n
  for (auto i = 2; i <= n; i++) {
    // fib(n) = fib(n-1) + fib(n-2); 34 = 21 + 13
    // fib(n-1) = fib(n-2) + fib(n-3) ; 21 = 13 + 8
    fib_n = op_add(n_minus_1, n_minus_2);
    n_minus_2 = n_minus_1;
    n_minus_1 = fib_n;
  }
  return fib_n;
}

void unit_test() {
  auto start = std::chrono::high_resolution_clock::now();
  // if lambda is recursive, it must be defined as a std::function separately
  // again, because of these "feels like a hack added to C++", I prefer
  // rust...
  std::function<std::uint64_t(std::uint64_t)> fn_fib_rec;
  fn_fib_rec = [&fn_fib_rec](std::uint64_t n) -> std::uint64_t {
    if (n == 0)
      return 0;
    if (n == 1)
      return 1;
    return fn_fib_rec(n - 1) + fn_fib_rec(n - 2);
  };

  auto fn_fib_iterator = [&fn_fib_rec](std::uint64_t n) -> std::uint64_t {
    if (n == 0)
      return 0;
    if (n == 1)
      return 1;
    auto n_minus_1 = 1;
    auto n_minus_2 = 0;
    auto result = 0;
    for (auto i = 2; i <= n; i++) {
      result = n_minus_1 + n_minus_2;
      n_minus_2 = n_minus_1;
      n_minus_1 = result;
    }
    return result;
  };

  auto index = 12;
  std::cout << "Fibonacci_rec(" << index << ") = " << fn_fib_rec(index)
            << std::endl;
  std::cout << "Fibonacci_iter(" << index << ") = " << fn_fib_iterator(index)
            << std::endl;
  auto expected = i64_to_digits(fn_fib_rec(index));
  std::cout << "Expected: ";
  dump_digits(expected);
  std::cout << std::endl;
  auto result = fibonacci_large_iter(index);
  std::cout << "Result: ";
  dump_digits(result);
  std::cout << std::endl;
  assert(op_equal(expected, result));

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Unit test duration: " << diff.count() << " s" << std::endl;
}

// Brute force: Iterate through fibonacci numbers until we find the very first
// index that are 1000 digits long
int main() {
  unit_test();

  auto start = std::chrono::high_resolution_clock::now();

  // start with 3 digits - fibonacci(12) = 144 (first index that are 3 digits)
  auto index = 11; // it does pre-increment, so start at 11
  auto result = std::vector<std::uint32_t>();
  // while (fibonacci_large_rec(i64_to_digits(index)).size() < MAX_DIGITS) {
  do {
    index++;
    result = fibonacci_large_iter(index);
  } while (result.size() < MAX_DIGITS);

  std::cout << "Final Index: " << index << " - " << result.size() << " digits"
            << endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Duration: " << diff.count() << " s" << std::endl;

  return 0;
}
