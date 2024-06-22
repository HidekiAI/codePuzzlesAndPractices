// 2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.
// What is the sum of the digits of the number 2^1000?

// Whiteboard:
// According to my calculator, 2^1000 = 1.07150860718626732094842504906e+301
// From binary standpoint, 2^15 means 16 bits, hence 2^1000 means 1001 bits.
// long-long is 64-bits, which means, it would require 16 long-longs to store
// 1001 bits. From the perspective of number of digits, 2^1000 will have 302
// digits. Once we have 300+ digits, the rest is easy (just sum each digits).
// The hard part is to figure out how to store 300+ digits, as well
// as how to calculate 2^1000.
// 2^15 basically means 2 arithmatically-shift-left 15 times, which
// is 2 * 2 * 2 * ... * 2 (15 times).
// But unfortunately, we can't do that with 2^1000 (iterating 1000 times still
// won't work).
// Summations on the other hand, specifically, large summations, have already
// been solved in problem #13 (Large Sum 0013); 2^15 for example can be
// broken down into summing:
//  2^15 = 32768
//       = 16384 + 16384  = 2^14 + 2^14 (2^14 x 2)
//       = 8192 + 8192 + 8192 + 8192 = 2^13 + 2^13 + 2^13 + 2^13 (2^13 x 4)
//       = 4096 + 4096 + 4096 + 4096 + 4096 + 4096 + 4096 + 4096 (2^12 x 8)
//         and so on, but also as can be (though pointless) broken down into:
//       = (16384 + 8192 + 4096 + 2048 + 1024 + 512 + 256 + 128 + 64 + 32 + 16 +
//       8 + 4 + 2 + 1) + 1 = (2^14 + 2^13 + 2^12 + 2^11 + 2^10 + 2^9 + 2^8 +
//       2^7 + 2^6 + 2^5 + 2^4 + 2^3 + 2^2 + 2^1 + 2^0) + 1
// If we can somehow break down 2^1000 into summations of each lines to be
// for example, 100 digits wide, we can just sum it up column-wise - actully,
// if all lines are same digits, you must multiple it by number of lines instead
// of iterating...  For example, if there are 300 lines, and the column has all
// 9's then digit = (300 * 9) % 10; and carry-over = (300 * 9) / 10;
// Note that we assume that each line is in reversed order for conviniences of
// digit alignments (i.e. right-most digit is at index 0).
// At best case scenario, if we can come up with each lines to be all the same,
// we can optimize by must having single line reversed, and multiple each digit
// by expected number of lines.  But for now, we care less, we just want to
// just come up with sets of lines that sums up to 2^1000, and after it is
// functional, we can optimize it later.
// 2^1000 is 302 digits long, so ideally, we want each line to be about 300
// digits wide.  But generating that 300 digits should NOT be a hand-typed
// constant. Instead, we need to come break down into coming up with X digits
// (i.e. 16) that sums up to 300 digits, and then we can just multiply it by
// 300, and so on. But all that has to fit all evenly (i.e. divisible by 2^x).
// First, break down 2^M (where M = 1000) into smaller parts:
//       2^1000 = 2^1000 * 1 = 2^1000 * 2^0
//              = 2^999 + 2^999 = 2^999 * 2 = 2^999 * 2^1
//              = 2^998 * 4 = 2^998 * 2^2
// Pattern: 2^M =  2^(M - n) * 2^n (but we won't use this formula directly since
// it involves multiplication, we only use it for shorthand, for summation is
// longer to type).
//     1024 = 2^10
//          = 2^9 * 2^1 = 512 * 2
//          = 2^5 * 2^5 = 32 * 32
//    16384 = 2^14 * 2^0
//    2^100 = 2^50 * 2^50 = 1125899906842624 * 1125899906842624
//    2^500 = 2^250 * 2^250
//          = 2^100 * 2^100 * 2^100 * 2^100 * 2^100
//   2^1000 = 2^500 * 2^500
//          = 2^100 * 2^100 * 2^100 * 2^100 * 2^100 * 2^100 * 2^100 * 2^100 *
//          2^100 * 2^100
// Though we now have a formula to break down 2^1000 into value that fits into
// 64-bits, it's multiplcation (not addition for large sum).  For example, 2^100
// * 2^100 means adding 2^100 2^100 times which is not practical (in fact,
// almost impossible). We need a balance, for example, 100 lines of 50 digits
// was managable in problem #13. First, let's figure out what will give us clost
// to 300 digits. Again, once we have that magic number of 2^x, we can juggle
// the values until we get 2^1000.

// brute force:
// 2^n = 2 * 2 * 2 * ... * 2 (n times)
// so I'll just use Large_Numbers library, and multiply 1000 times.
//  for i in 1..1000 { result = result * 2; }
// and then just add the digits in result
#define _HAS_CXX20 1

#include <algorithm> // std::reverse
#include <cassert>   // assert
#include <chrono>    // std::chrono
#include <iostream>  // std::cout
#include <string>    // std::string
#include <vector>    // std::vector
#include "../../../hairev_utils/large_numbers/cpp/src/lib_large_numbers.hpp"

using namespace hairev::libs;

const long POWER = 1000;

std::vector<uint64_t> reverse_64(const std::vector<uint64_t> &data) {
  // Note that using std::reverse() will increase performance from 550µs to
  // 380µs (30% faster!) compared to using std::for_each(rbegin,rend)
  auto rev = data;
  std::reverse(rev.begin(), rev.end());
  return rev;
}

// Unlike the large-sum problem version, though we still assume that
// the 10th digit is on the 0th index (reversed), we DO NOT make
// an assumption that each line have same number of digits.  This small
// flexibility will require kmax_digits to be passed in, which should be
// the widest line (i.e. 50 digits wide) of the set of lines, so that
// we can iterate through each column all the way up to kmax_digits per
// line...
std::vector<uint64_t>
sum_columns(const std::vector<std::string> &lines_reversed,
            size_t &kmax_digits) {
  long long carry_over = 0;
  auto sum_reversed = std::vector<uint64_t>(
      kmax_digits,
      0); // Initialize the sum vector with 50 zeros (will grow wider than 50)

  // Right most digit (10th digit) are at index 0 (reversed) for convenience
  // for each column, we'll sum the digits and carry over the overflow
  for (size_t col = 0; col < kmax_digits; ++col) { // For each digit
    auto column_sum = carry_over;
    for (const auto &row_line : lines_reversed) {
      // allowing each line to have different number of digits would
      // require us to check if the column is within the line's width
      if (col < row_line.size()) {
        auto digit = row_line[col] - '0'; // Convert the digit to integer
        column_sum += digit;              // Add the digit to the column sum
      }
    }

    sum_reversed[col] = column_sum % 10; // i.e. sum=32767, result = 7
    carry_over = column_sum / 10; // i.e. sum=32767, new carry_over = 3276
    // std::cout << sum_reversed[col];
  }

  // we've done all the columns, but we still have carry-over, so we'll append
  // the carry-over to the sum vector
  while (carry_over > 0) {         // While there is carry-over
    auto digit = carry_over % 10;  // Get the last digit of the carry-over
    sum_reversed.push_back(digit); // tail of the vector
    carry_over /= 10;              // Carry over the overflow
    // std::cout << digit;
  }
  // std::cout << std::endl;

  // finally, we'll reverse the sum vector
  return reverse_64(sum_reversed);
}

// entry point
int main() {
  auto start = std::chrono::high_resolution_clock::now();

  // for now, just brute force the result
  Large_Numbers my_large_number = Large_Numbers("1");
  for (long i = 1; i < POWER; ++i) {
    my_large_number = my_large_number * Large_Numbers("2");
  }

  std::cout << "2^" << POWER << " = ";
  my_large_number.Dump();
  std::cout << " (" << my_large_number.Size() << " digits)" << std::endl;
  std::cout << "Sum of the digits: ";
  std::uint64_t sum = 0;
  for (const auto &digit : my_large_number.Get()) {
    sum += digit - '0'; // Convert the digit to integer
  }
  std::cout << sum << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Duration: " << diff.count() << " s" << std::endl;
}