
// Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.
// (see data.txt for the numbers)

// Whiteboard:
// - I am assuming that each line represents a "50-digit"s number (huge number)
// - There are 100 lines
// - sum(100_lines) is what I think it means
// Most likely, the trick here is that summation of the numbers is too large to fit in a single number
// i.e. max 64-bit is 2^64-1 = 18446744073709551615, which is less than 50-digits
// The most primitive way to solve this is to do it the way we've learned how to add numbers
// in elementary school, which is to add each digit from right to left and carry over the overflow
// to the next digit. Fortunately, each lines are fixed length of 50-digits, so the assumption is
// is that all numbers aligns to from the right without needing to shift the numbers.
// 1. Read the file line by line as a string; just for optmizations, we'll preallocate 50 chars, and
//    if the line does not have 50 chars, we'll return an error (in future, we'll pad left with 0s)
// 2. From right-most char-index, we'll add the column values and carry over the overflow to the next
//    column. We'll keep the result in a vector of 50 elements.
// Data-structure-wise, it will be:
// * an array of 50 elements per line (100 lines)
// * an array of u64 (i.e. Vec<u64>) to keep the sum of the columns, where Vec[0] is the right-most
//   digit of the sum

#include <iostream> // std::cout
#include <fstream> // std::ifstream
#include <vector> // std::vector
#include <string> // std::string
#include <algorithm> // std::reverse
#include <chrono> // std::chrono

std::vector<std::string> read_file(const std::string& filename  = "../data.txt") {
    std::ifstream file  (filename); // Open the file            
    std::vector<std::string> lines; // Vector to store the lines
    std::string line; // Temp string to store each line
    while (std::getline(file, line)) { // Read each line
        if (line.size() != 50) { // Check if the line is 50 chars
            std::cerr << "Line is not 50 chars: " << line << std::endl;
            return {};
        }
        lines.push_back(line); // Add the line to the vector
    }
    return lines; // Return the vector
}

std::vector<uint64_t> sum_columns(const std::vector<std::string>& lines) {
    std::vector<uint64_t> sum(50, 0); // Initialize the sum vector with 50 zeros
    for (const auto& line : lines) { // For each line
        for (size_t i = 0; i < line.size(); ++i) { // For each char in the line
            sum[i] += line[i] - '0'; // Add the char to the sum
        }
    }
    for (size_t i = 0; i < sum.size(); ++i) { // For each column
        if (sum[i] >= 10) { // If the column is greater than 10
            sum[i + 1] += sum[i] / 10; // Carry over the overflow
            sum[i] %= 10; // Keep the remainder
        }
    }
    return sum; // Return the sum
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now(); // Start the clock
    auto lines = read_file(); // Read the file
    if (lines.empty()) { // Check if the file is empty
        return 1; // Return error
    }
    auto sum = sum_columns(lines); // Sum the columns
    // Final sum should be 52 digits; 5537376230390876637302048746832985971773659831892672
    // Upper 10 digits are 5537376230

    std::cout << "First 10 digits of the sum (should be '5537376230'): "; // Print the message
    std::reverse(sum.begin(), sum.end()); // Reverse the sum
    for (size_t i = 0; i < 10; ++i) { // For the first 10 digits
        std::cout << sum[i]; // Print the digit
    }
    std::cout << std::endl; // Print newline

    auto end_time = std::chrono::high_resolution_clock::now(); // End the clock
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // Calculate the duration
    std::cout << "Duration: " << duration.count() << "µs" << std::endl; // Print the duration

    return 0; // Return success
}
