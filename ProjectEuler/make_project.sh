#!/bin/bash

_TARGET=$1

if [ -z $_TARGET ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

if [ -d $_TARGET ]; then
    echo "Project $_TARGET already exists"
    exit 1
fi

mkdir $_TARGET
cd $_TARGET
cargo new $_TARGET
mv $_TARGET rust

mkdir cpp
touch cpp/main.cpp

echo '
// My essentials that I always include:
#define _HAS_CXX20 1
#if _HAS_CXX20
#include <algorithm> // std::sort, std::transform, std::find (std::find - make sure to override operator==)
#include <array>
#include <cassert>    // assert()
#include <chrono>     // for start/end time measurement
#include <cstdint>    // std::uint16_t, etc - I''m too used to rust types...
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
// but without using static_assert() because it''s not available until C++17
#error This code requires at least C++17
#endif // !_HAS_CXX20 || !_HAS_CXX17

using namespace std;
' > cpp/main.cpp

git add .