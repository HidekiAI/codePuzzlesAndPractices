// Work out the first ten digits of the sum of the following one-hundred
// 50-digit numbers. (see data.txt for the numbers)

// Whiteboard:
// - I am assuming that each line represents a "50-digit"s number (huge number)
// - There are 100 lines
// - sum(100_lines) is what I think it means
// Most likely, the trick here is that summation of the numbers is too large to
// fit in a single number i.e. max 64-bit is 2^64-1 = 18446744073709551615,
// which is less than 50-digits The most primitive way to solve this is to do it
// the way we've learned how to add numbers in elementary school, which is to
// add each digit from right to left and carry over the overflow to the next
// digit. Fortunately, each lines are fixed length of 50-digits, so the
// assumption is is that all numbers aligns to from the right without needing to
// shift the numbers.
// 1. Read the file line by line as a string; just for optmizations, we'll
// preallocate 50 chars, and
//    if the line does not have 50 chars, we'll return an error (in future,
//    we'll pad left with 0s)
// 2. From right-most char-index, we'll add the column values and carry over the
// overflow to the next
//    column. We'll keep the result in a vector of 50 elements.
// Data-structure-wise, it will be:
// * an array of 50 elements per line (100 lines)
// * an array of u64 (i.e. Vec<u64>) to keep the sum of the columns, where
// Vec[0] is the right-most
//   digit of the sum

#include <algorithm> // std::reverse
#include <cassert>
#include <chrono>   // std::chrono
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <string>   // std::string
#include <vector>   // std::vector

const size_t MAX_DIGITS = 50;

std::vector<uint64_t> reverse_64(const std::vector<uint64_t> &data) {
  // Note that using std::reverse() will increase performance from 550µs to 380µs (30% faster!) compared to using std::for_each(rbegin,rend)
  auto rev = data;
  std::reverse(rev.begin(), rev.end());
  return rev;
}

std::string reverse_line(const std::string &line) {
  //std::string rev;
  //std::for_each(line.rbegin(), line.rend(),
  //              [&rev](const auto &c) { rev.push_back(c); });
  //return rev;
  
  // Note that using std::rever() will increase performance from 550µs to 380µs (30% faster!)
  std::string rev = line;
  std::reverse(rev.begin(), rev.end());
  return rev;
}

std::vector<std::string>
read_file_reversed_lines(const std::string &filename = "../data.txt") {
  std::ifstream file(filename);                     // Open the file
  std::vector<std::string> lines_reversed_per_line; // Vector to store the lines
  std::string line;                  // Temp string to store each line
  while (std::getline(file, line)) { // Read each line
    if (line.size() != MAX_DIGITS) { // Check if the line is 50 chars
      std::cerr << "Line is not 50 chars: " << line << std::endl;
      throw std::runtime_error("Line is not 50 chars"); // Return error
    }

    // iterators from rbegin() (last element) down to rend(), build a rev-string
    lines_reversed_per_line.push_back(reverse_line(line));
  }
  return lines_reversed_per_line; // Return the vector
}

// NOTE: Unlike the rust version, we'll just use vector<string> since digit
// conversion in C++ is fast! We will assume that the read_file() verified that
// each line is 50 chars (critical when summing from right-most to left-most)
// We'll also assume that the lines are in reverse order (i.e. right-most digit
// is at index 0) because (unlike rust version) it'll make it easier to just
// append carry-overs to tails of the sum vector.
std::vector<uint64_t>
sum_columns(const std::vector<std::string> &lines_reversed) {
  long long carry_over = 0;
  auto sum_reversed = std::vector<uint64_t>(
      MAX_DIGITS,
      0); // Initialize the sum vector with 50 zeros (will grow wider than 50)

  // Right most digit (10th digit) are at index 0 (reversed) for convenience
  // for each column, we'll sum the digits and carry over the overflow
  for (size_t col = 0; col < MAX_DIGITS; ++col) { // For each digit
    auto column_sum = carry_over;
    for (const auto &row_line : lines_reversed) {
      auto digit = row_line[col] - '0'; // Convert the digit to integer
      column_sum += digit;              // Add the digit to the column sum
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

// Expected result: 5537376230390876637302048746832985971773659831892672
// Calculated result: 5537376230390876637302048746832985971773659831892672
// Duration: 531µs  <--- (rust unoptimized version is 5.371384ms - 10x slower!)
// First 10 digits of the sum (should be '5537376230'): 5537376230
int main() {
  auto start_time =
      std::chrono::high_resolution_clock::now(); // Start the clock
  auto lines =
      read_file_reversed_lines(); // Read the file, each lines are reversed
  if (lines.empty()) {            // Check if the file is empty
    return 1;                     // Return error
  }

  // Final sum should be 52 digits;
  // 5537376230390876637302048746832985971773659831892672 Upper 10 digits are
  std::cout
      << "Expected result: 5537376230390876637302048746832985971773659831892672"
      << std::endl
      << "Calculated result: ";
  auto sum_vec = sum_columns(lines); // Sum the columns
  std::for_each(sum_vec.begin(), sum_vec.end(),
                [](const auto &digit) { std::cout << digit; });
  std::cout << std::endl;

  // 5537376230 slice first 10 digits
  auto sum_str =
      std::string(sum_vec.size(), '0'); // Initialize the string with 50 zero
  auto index = 0; // unfortunately, unlike other iterators, STL does not have a

  // (index,value) paired (i.e. into_iter().enumerate() in rust)
  std::for_each(
      sum_vec.begin(), sum_vec.end(),
      [&sum_str, &index](const auto &digit) { // For each digit in the sum
        sum_str[index++] = digit + '0';       // Add the digit to the string
      });
  auto first_10_digits = sum_str.substr(0, 10); // Get the first 10 digits

  auto end_time = std::chrono::high_resolution_clock::now(); // End the clock
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time); // Calculate the duration
  std::cout << "Duration: " << duration.count() << "µs ("
            << duration.count() / 1000.0 << "ms)"
            << std::endl; // Print the duration

  std::cout
      << "First 10 digits of the sum (should be '5537376230'): "; // Print the
                                                                  // message
  std::cout << first_10_digits << std::endl; // Print the first 10 digits
  assert(first_10_digits ==
         "5537376230"); // Assert the first 10 digits (should be '5537376230'

  return 0; // Return success
}