#define _HAS_CXX20 1
// By starting at the top of the triangle below and moving to adjacent numbers
// on the row below, the maximum total from top to bottom is 23.
//      3           3
//     7 4          7
//    2 4 6         4
//   8 5 9 3        9
// That is, 3 + 7 + 4 + 9 = 23.
// Find the maximum total from top to bottom of the triangle below:
//
//                      75
//                     95 64
//                    17 47 82
//                   18 35 87 10
//                  20 04 82 47 65
//                 19 01 23 75 03 34
//                88 02 77 73 07 63 67
//               99 65 04 28 06 16 70 92
//              41 41 26 56 83 40 80 70 33
//             41 48 72 33 47 32 37 16 94 29
//            53 71 44 65 25 43 91 52 97 51 14
//           70 11 33 28 77 73 17 78 39 68 17 57
//          91 71 52 38 17 14 91 43 58 50 27 29 48
//         63 66 04 68 89 53 67 30 73 16 69 87 40 31
//        04 62 98 27 23 09 70 98 73 93 38 53 60 04 23
//
// NOTE: As there are only 16384 routes, it is possible to solve this problem by
// trying every route. However, Problem 67, is the same challenge with a
// triangle containing one-hundred rows; it cannot be solved by brute force, and
// requires a clever method! ;o)

// Whiteboard:
// Firstly, my impression was breadth-first search, until I realized that I
// cannot just pick largest number of each row.  I have to chain its larger
// child. Initial pattern/thought WAS (not anymore) is just look for the largest
// number WITHOUT considerations of what is the children of that number.
// I'd most likely have to visit each children all the way to the bottom, and
// find the paths to the largest number.  One thought (though it won't work if I
// think about it) is while recursing (or if iterating, while iterating), if the
// sum of path on the right or left child is smaller than the currently known
// largest path (that is passed down as recursion function (of if iterating, in
// the stack as a tuple)) then immediately opt out of that branch.  But the
// problem is that the untaken path may end up having a total sum that is larger
// than the currently known largest path.  So we have NO CHOICE but to visit all
// children of each node until we reach the bottom.

// My essentials that I always include:
#if __cplusplus < 201703L
// paradox here is that without C++17, we cannot use static_assert(), and at
// the same time, it'll fail compilation because `static_assert()` will be
// undefined for older C++ versions. So, I'll use #error preprocessor directive
#error "This code requires at least C++17 or later"
// static_assert(false, "This code requires at least C++17");
#endif

// #if _HAS_CXX17
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

// expected: 1074
static const std::vector<std::vector<std::uint16_t>> MyTriangle = {
    {75},
    {95, 64},
    {17, 47, 82},
    {18, 35, 87, 10},
    {20, 4, 82, 47, 65},
    {19, 1, 23, 75, 3, 34},
    {88, 2, 77, 73, 7, 63, 67},
    {99, 65, 4, 28, 6, 16, 70, 92},
    {41, 41, 26, 56, 83, 40, 80, 70, 33},
    {41, 48, 72, 33, 47, 32, 37, 16, 94, 29},
    {53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14},
    {70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57},
    {91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48},
    {63, 66, 4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31},
    {04, 62, 98, 27, 23, 9, 70, 98, 73, 93, 38, 53, 60, 4, 23}};

class Vertex {
public:
  struct Position {
    std::uint16_t row;
    std::uint16_t col;
  };
  // for std::find() to work, we need to define equality operator
  bool operator==(const Position &other) const {
    return position.row == other.row && position.col == other.col;
  }

  // very simple check:
  // - a vertex can have 0, 1, or 2 children
  // - child row is current row + 1
  // - child col is current col or current col + 1 (yes, it's that simple)
public:
  // immutable/const: once they are declared, they won't change
  const Position position;
  const std::optional<Position> possible_left_child;
  const std::optional<Position> possible_right_child;

private:
  std::optional<Position> get_left_child() {
    // edge case: root has 2 children though it's at the edge
    if (position.row == 0) {
      return Position{1, 0}; // left child
    }
    // if we're at the bottom, we don't have any children (neither left or
    // right)
    if (position.row >= MyTriangle.size() - 1) {
      return std::nullopt;
    }
    // if we're at left edge, we don't have a left child
    if (position.col == 0) {
      return std::nullopt;
    }
    return Position{(std::uint16_t)(position.row + 1), position.col};
  }
  std::optional<Position> get_right_child() {
    // edge case: root has 2 children though it's at the edge
    if (position.row == 0) {
      return Position{1, 1}; // right child
    }
    // if we're at the bottom, we don't have any children (neither left or
    // right)
    if (position.row >= MyTriangle.size() - 1) {
      return std::nullopt;
    }
    // if we're at right edge, we don't have a right child
    if (position.col == MyTriangle[position.row].size() - 1) {
      return std::nullopt;
    }
    return Position{(std::uint16_t)(position.row + 1),
                    (std::uint16_t)(position.col + 1)};
  }

public:
  Vertex(Position pos)
      : position(pos), possible_left_child(get_left_child()),
        possible_right_child(get_right_child()) {}
};

// Since we have a triangle with (row,col), we probably do not need to keep
// track of the values; It's also a question of, by knowing the row and col, can
// we determine the left and right child, and its parents (a vertex can have 0,
// 1, or 2 parents).
// - parent vertex can have 0, 1, or 2 children
// - child vertex can have 0, 1, or 2 parents (0 parent: root, 1 parent: 2nd and
// edge vertices, 2 parents: middle vertices) this data structure seems similar
// to "largest product in a grid" problem (though building the grid was easier
// because it was square)
std::vector<Vertex> build_triangle() {
  std::vector<Vertex> vertices;
  for (std::uint16_t row = 0; row < MyTriangle.size(); ++row) {
    for (std::uint16_t col = 0; col < MyTriangle[row].size(); ++col) {
      auto vert = Vertex::Position{row, col};
      // std::cout << "row: " << row << " col: " << col
      //           << " value: " << MyTriangle[row][col] << " left child: "
      //           << (Vertex(vert).possible_left_child.has_value() ? "yes" :
      //           "no")
      //           << " right child: "
      //           << (Vertex(vert).possible_right_child.has_value() ? "yes"
      //                                                             : "no")
      //           << std::endl;
      vertices.push_back(vert);
    }
  }
  return vertices;
}

// Note that taking the largest child is not enough; we have to take the
// largest chain of children; so we have to recursively call this function
// until we reach the bottom of the triangle
const std::uint16_t
get_larger_children_rec(const std::vector<Vertex> &vertices,
                        const Vertex::Position &vertex,
                        const std::uint16_t &current_edge_sum) {
  // locate/find the current vertex
  const auto current_vertex =
      std::find(vertices.begin(), vertices.end(), vertex);
  const auto vert_value =
      MyTriangle[current_vertex->position.row][current_vertex->position.col];

  // if we're at the bottom, return the value of the vertex and be done
  if (current_vertex->position.row == MyTriangle.size() - 1) {
    return vert_value + current_edge_sum;
  }

  // current edge sum is the sum of all the edges it has visited so far
  auto left_child_value = 0; // if optional::None, child is 0
  auto right_child_value = 0;
  if (current_vertex->possible_left_child.has_value()) {
    auto left_child_vertex = current_vertex->possible_left_child.value();
    left_child_value = get_larger_children_rec(vertices, left_child_vertex,
                                               current_edge_sum + vert_value);
  }
  if (current_vertex->possible_right_child.has_value()) {
    auto right_child_vertex = current_vertex->possible_right_child.value();
    right_child_value = get_larger_children_rec(vertices, right_child_vertex,
                                                current_edge_sum + vert_value);
  }

  // return the larger chain of chilren of this edge
  return std::max(left_child_value, right_child_value);
}

int main() {
  auto expected_largest_path_sum = 1074;
  auto vertices = build_triangle();
  std::uint64_t largest_path_sum =
      get_larger_children_rec(vertices, Vertex::Position{0, 0}, 0);
  std::cout << "The largest path sum is: " << largest_path_sum << std::endl;

  assert(largest_path_sum == expected_largest_path_sum);
  return 0;
}