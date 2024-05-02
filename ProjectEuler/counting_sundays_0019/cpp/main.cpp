#define _HAS_CXX20 1
// You are given the following information, but you may prefer to do some
// research for yourself.
// - 1 Jan 1900 was a Monday.
// - Thirty days has September,
//   April, June and November.
//   All the rest have thirty-one,
//   Saving February alone,
//   Which has twenty-eight, rain or shine.
//   And on leap years, twenty-nine.
// - A leap year occurs on any year evenly divisible by 4, but not on a century
// unless it is divisible by 400. How many Sundays fell on the first of the
// month during the twentieth century (1 Jan 1901 to 31 Dec 2000)?

// Whiteboard:
// First, create a table of days in a month.

// My essentials that I always include:
#if _HAS_CXX20
#include <algorithm> // std::sort, std::transform, std::find (std::find - make sure to override operator==)
#include <array>
#include <cassert>    // assert()
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

const std::array<std::uint16_t, 12> DaysInMonth = {
    31, // January
    28, // February - 29 on leap year
    31, // March
    30, // April
    31, // May
    30, // June
    31, // July
    31, // August
    30, // September
    31, // October
    30, // November
    31  // December
};

bool is_leap_year(std::uint16_t year) {
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0) {
        return true;
      }
      return false;
    }
    return true;
  }
  return false;
}

std::uint32_t num_days_since_1900(std::uint16_t year, std::uint16_t month,
                                  std::uint16_t day) {
  std::uint32_t days = 0;
  for (std::uint16_t y = 1900; y < year; y++) {
    days += is_leap_year(y) ? 366 : 365;
  }
  for (std::uint16_t m = 1; m < month; m++) {
    days += DaysInMonth[m - 1];
    if (m == 2 && is_leap_year(year)) {
      days++;
    }
  }
  days += day;
  return days;
}

bool is_sunday_on_first_of_month(std::uint16_t year, std::uint16_t month) {
  return num_days_since_1900(year, month, 1) % 7 == 0;
}

int main() {
  std::uint32_t num_sundays = 0;
  for (std::uint16_t year = 1901; year <= 2000; year++) {
    for (std::uint16_t month = 1; month <= 12; month++) {
      if (is_sunday_on_first_of_month(year, month)) {
        num_sundays++;
      }
    }
  }
  std::cout << num_sundays << std::endl;
  return 0;
}
