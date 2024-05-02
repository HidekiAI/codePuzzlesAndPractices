// A permutation is an ordered arrangement of objects. For example, 3124 is one
// possible permutation of the digits 1, 2, 3 and 4. If all of the permutations
// are listed numerically or alphabetically, we call it lexicographic order. The
// lexicographic permutations of 0, 1 and 2 are:
//      012   021   102   120   201   210
//
// What is the millionth lexicographic permutation of the digits 0, 1, 2, 3, 4,
// 5, 6, 7, 8 and 9?

// Whiteboard:
// A method to find permutations of a vector<char>, for example
// vector<char> v = { '0', '1', '2' };
// In which case, the permutations would be 012, 021, 102, 120, 201, 210
// For a 3 digits set, there are 3! = 6 permutations
// so fir N digits, there are N! permutations
// But knowing how many permutations there are is not enough, we need to
// sort them in lexicographic order (numerically or alphabetically ordered)
// in which case, it will be 012, 021, 102, 120, 201, 210 and 3rd permutation
// is 102, 6th permutation is 210.
// The question asked what is the millionth lexicographic permutation of the
// digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9.  As stated, number of permutations
// for 10 digits is 10! = 3,628,800.  So we need to find the 1,000,000th
// permutation should exist. Note: It turns out that there is an STL function
// called std::next_permutation <algorithm> that can be used to find the next
// permutation of a set of elements. As a starter, I will use this so that I can
// just do "get_next_permutation" and not have to worry about pre-permutating
// 3Mil elements! Once that is implemented, I'll go back and implement the
// permutation algorithm that will dynamically generate the "next" permutations
// to be able to iterate without memory hogging...

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
// The 1000000th permutation is: 2783915460
// Elapsed time: 0.0941085s
//
// real    0m0.177s
// user    0m0.000s
// sys     0m0.000s
// Wed, May  1, 2024 23:35:50
// Implementation time using std::next_permutation (cheating): 00:21:23.99 (~21
// minutes)
const std::vector<char> MyDigits = {'0', '1', '2', '3', '4',
                                    '5', '6', '7', '8', '9'};
const std::uint64_t MyPermutation = 1'000'000;

// using std::next_permutation...
std::string get_nth_permutation(const std::vector<char> &digits,
                                std::uint64_t n) {
  std::vector<char> v = digits;
  for (std::uint64_t i = 1; i < n; ++i) {
    std::next_permutation(v.begin(), v.end());
  }
  return std::string(v.begin(), v.end());
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  std::string result = get_nth_permutation(MyDigits, MyPermutation);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "The " << MyPermutation << "th permutation is: " << result
            << std::endl;
  std::cout << "Elapsed time: " << diff.count() << "s" << std::endl;
  return 0;
}