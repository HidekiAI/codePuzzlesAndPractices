// A unit fraction contains 1 in the numerator. The decimal representation of
// the unit fractions with denominators 2 to 10 are given:
//      1/2	= 	0.5
//      1/3	= 	0.(3)
//      1/4	= 	0.25
//      1/5	= 	0.2
//      1/6	= 	0.1(6)
//      1/7	= 	0.(142857)
//      1/8	= 	0.125
//      1/9	= 	0.(1)
//      1/10	= 	0.1
// Where 0.1(6) means 0.166666..., and has a 1-digit recurring cycle. It can be
// seen that 1/7 has a 6-digit recurring cycle.
//
// Find the value of d < 1000 for which 1/d contains the longest recurring cycle
// in its decimal fraction part.
//

// Whiteboard: In a nutshell, from previous excerise, which I had to write a
// large number function for op_add and op_subtract, I will most likely have to
// write op_divide and op_multiply.  I may not need to write op_multiply, but
// just like op_add versus op_subtract, in which op_add was trivial, op_subtract
// was not, I would guess op_multiply would be trivial compared to op_divide.
// I'm also wondering if optimization of mult and div by 2 (shift left and
// right) are easy to integrate.  Obviously, quick case of mult and div by 0 and
// 1 are also trivial. lastly, I want to panic/throw for division_by_zero rather
// than fail with Result::Error or Option::None.

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
