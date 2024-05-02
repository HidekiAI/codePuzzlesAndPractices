// Using 'names.txt'
// (https://projecteuler.net/resources/documents/0022_names.txt), a 46K text
// file containing over five-thousand first names, begin by sorting it into
// alphabetical order. Then working out the alphabetical value for each name,
// multiply this value by its alphabetical position in the list to obtain a name
// score.
//
// For example, when the list is sorted into alphabetical order, COLIN, which is
// worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list. So, COLIN
// would obtain a score of 938 × 53 = 49714.
//
// What is the total of all the name scores in the file?

// Whiteboard:
// The order of appearance in the file does not seem to matter since in the end
// I would have to sort it in alphabetical order.  I'll make sure to write a
// unit-test so that the name "COLIN" does appear on the 938th index after it is
// sorted out. I'm also unsure if there are duplilcate names in the file, but
// that can probably be validated when we do the unit-test (if COLIN appears
// less than 938, then there are duplicates, else there are no duplicates).  I
// will first start with std::set but once all is working, I'd like to go back
// and write bubble-sort (because it's the easiest implementation to write) just
// in case the programming-test disallows libraries and I am forced to write my
// own sorting algorithm... Once they are pre-sorted, I can then calculate the
// name score by summing the alphabetical values of each character in the name,
// and then multiply it by its alphabetical position in the list.  I'll then sum
// all the name scores to get the total of all the name scores in the file. Note
// that the alphabetical value is 1-based, so A=1, B=2, ..., Z=26, or simply
//      value = tolower(ch) - 'a' + 1;  // 1-based, so that 'a' will count as 1

// My essentials that I always include:
#define _HAS_CXX20 1
#if _HAS_CXX20
#include <algorithm> // std::sort, std::transform, std::find (std::find - make sure to override operator==)
#include <array>
#include <cassert>    // assert()
#include <chrono>     // for start/end time measurement
#include <cstdint>    // std::uint16_t, etc - I'm too used to rust types...
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
// but without using static_assert() because it's not available until C++17
#error This code requires at least C++17
#endif // !_HAS_CXX20 || !_HAS_CXX17

using namespace std;

// Read 5163 names from file: ../0022_names.txt
// Elapsed time: 9 milliseconds
// names[937]: COLIN
// Total of all the name scores in the file: 871198282
// Elapsed time: 11 milliseconds
// 
// real    0m0.073s
// user    0m0.000s
// sys     0m0.000s
// Wed, May  1, 2024 22:43:49
// Total Implementation time: 00:32:42.28 (~33 minutes)
const string FILENAME = "../0022_names.txt";

// read names from file and return as vector of strings presorted in
// alphabetical order
std::vector<std::string> read_names(const std::string &filename) {
  auto start_timer = chrono::high_resolution_clock::now();
  std::vector<std::string> names;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return names;
  }
  std::string name;
  while (std::getline(file, name, ',')) {
    // remove quotes
    name.erase(std::remove(name.begin(), name.end(), '\"'), name.end());
    names.push_back(name);
  }
  file.close();
  // std::sort is combination of quicksort and heapsort
  std::sort(names.begin(), names.end()); // sorts in-place
  std::cout << "Read " << names.size() << " names from file: " << filename
            << std::endl;
  auto end_timer = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
  std::cout << "Elapsed time: " << duration.count() << " milliseconds"
            << std::endl;

  return names;
}

// calculate the alphabetical value of a name
std::uint16_t name_value(const std::string &name) {
  std::uint16_t sum = 0;
  for (const char &ch : name) {
    sum += tolower(ch) - 'a' + 1;
  }
  return sum;
}

// calculate the total of all the name scores in the file
std::uint64_t total_name_scores(const std::vector<std::string> &names) {
  std::uint64_t sum = 0;
  // iterate through all names and calculate the name value and multiply by the
  // alphabetical position in the list
  for (std::size_t name_index = 0; name_index < names.size(); name_index++) {
    sum += name_value(names[name_index]) * (name_index + 1);
  }
  return sum;
}

// unit-test to make sure that the name "COLIN" is at the 938th index
void test_name_value(const std::vector<std::string> &names) {
  // let's make sure there are at least 938 names in the list
  assert(names.size() > 938);
  std::cout << "names[937]: " << names[937] << std::endl;
  assert(names[937] == "COLIN");
  assert(name_value("COLIN") == 53);
  assert(name_value("COLIN") * 938 == 49714);
}

int main() {
  auto start_timer = chrono::high_resolution_clock::now();

  auto names = read_names(FILENAME);
  test_name_value(names);
  auto sum = total_name_scores(names);
  std::cout << "Total of all the name scores in the file: " << sum << std::endl;

  auto end_timer = chrono::high_resolution_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end_timer - start_timer);
  std::cout << "Elapsed time: " << duration.count() << " milliseconds"
            << std::endl;

  return 0;
}
