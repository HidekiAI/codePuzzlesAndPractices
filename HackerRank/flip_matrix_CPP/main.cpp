#include <bits/stdc++.h>
#define DBGOUT ;

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'flippingMatrix' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts 2D_INTEGER_ARRAY matrix as parameter.
 */

int flippingMatrix(vector<vector<int>> matrix) {

  // function to sum each quadrant for evaluations (returns tuple<long, long,
  // long, long>, input is matrix)
  auto sum_quadrants = [](const vector<vector<int>> &matrix) {
    const auto height = matrix.size();
    const auto mid_height = height / 2;
    const auto width = matrix[0].size();
    const auto mid_width = width / 2;
    long q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (auto row = 0; row < mid_height; ++row) {
      for (auto col = 0; col < mid_width; ++col) {
        q1 += matrix[row][col];
        q2 += matrix[row][col + mid_width];
        q3 += matrix[row + mid_height][col];
        q4 += matrix[row + mid_height][col + mid_width];
      }
    }
    return make_tuple(q1, q2, q3, q4);
  };

  // dump matrix to stdout
  auto dump_matrix = [](const vector<vector<int>> &matrix) {
    cout << "---" << endl
         << "Matrix height:" << matrix.size() << endl
         << "Matrix width:" << matrix[0].size() << endl;
    for (auto row = 0; row < matrix.size(); ++row) {
      for (auto col = 0; col < matrix[0].size(); ++col) {
        cout << matrix[row][col] << " ";
      }
      cout << endl;
    }
  };
  auto dump_quadrants = [](const tuple<long, long, long, long> &quadrants) {
    cout << "---" << endl
         << "Quadrants:" << endl
         << "Q1: " << get<0>(quadrants) << endl
         << "Q2: " << get<1>(quadrants) << endl
         << "Q3: " << get<2>(quadrants) << endl
         << "Q4: " << get<3>(quadrants) << endl;
  };
  // function to flip Q2 to Q1 modifying matrix inplace
  auto flip_q2_to_q1 = [](vector<vector<int>> &matrix) {
    const auto height = matrix.size();
    const auto mid_height = height / 2;
    const auto width = matrix[0].size();
    const auto mid_width = width / 2;
    for (auto row = 0; row < mid_height; ++row) {
      auto sumq1 = 0L;
      auto sumq2 = 0L;
      for (auto col = 0; col < mid_width; ++col) {
        sumq1 += matrix[row][col];
        sumq2 += matrix[row][col + mid_width];
      }
      // if sumq2 > sumq1, flip row
      if (sumq2 > sumq1) {
        // for horizontal flip, all we need to do is reverse the row
        reverse(matrix[row].begin(), matrix[row].end());
      }
    }
  };
  // function to flip Q4 to Q3 modifying matrix inplace
  auto flip_q4_to_q3 = [](vector<vector<int>> &matrix) {
    const auto height = matrix.size();
    const auto mid_height = height / 2;
    const auto width = matrix[0].size();
    const auto mid_width = width / 2;
    for (auto row = 0; row < mid_height; ++row) {
      auto sumq3 = 0L;
      auto sumq4 = 0L;
      for (auto col = 0; col < mid_width; ++col) {
        sumq3 += matrix[row + mid_height][col];
        sumq4 += matrix[row + mid_height][col + mid_width];
      }
      // if sumq3 > sumq4, flip row
      if (sumq3 > sumq4) {
        // for horizontal flip, all we need to do is reverse the row
        reverse(matrix[row + mid_height].begin(),
                matrix[row + mid_height].end());
      }
    }
  };
  // function to flp (vertically) Q4 to Q2 modifying matrix inplace
  auto flip_q4_to_q2 = [](vector<vector<int>> &matrix) {
    const auto height = matrix.size();
    const auto mid_height = height / 2;
    const auto width = matrix[0].size();
    const auto mid_width = width / 2;
    for (auto col = 0; col < mid_width; ++col) {
      auto sumq2 = 0L;
      auto sumq4 = 0L;
      for (auto row = 0; row < mid_height; ++row) {
        sumq2 += matrix[row][col + mid_width];
        sumq4 += matrix[row + mid_height][col + mid_width];
      }
      // if sumq4 > sumq2, flip column
      if (sumq4 > sumq2) {
        // for vertical flip, we need to swap the values in the column
        // and the easiest way is to start at each ends and meet in the middle
        for (auto row = 0; row < mid_height; ++row) {
          swap(matrix[row][col + mid_width],
               matrix[(height - 1) - row][col + mid_width]);
        }
      }
    }
  };

  auto q1_is_biggest = [](tuple<long, long, long, long> &quadrants) {
    auto q1 = get<0>(quadrants);
    auto q2 = get<1>(quadrants);
    auto q3 = get<2>(quadrants);
    auto q4 = get<3>(quadrants);
    return q1 > q2 && q1 > q3 && q1 > q4 && q2 > q4 && q4 > q3;
  };

  vector<int> flip_order; // Current flip order (binary representation)
  vector<int> best_order; // Best flip order resulting in the maximum Q1 sum
  long max_sum = 0;       // Maximum sum in Q1
  // Define the explore_flip_orders lambda function
  // Recursive function to explore different flip orders and find the maximum Q1
  // sum
  // NOTE: For recusive functions, we need to pass the function itself as a
  // parameter  (i.e. explore_flip_orders) to be able to call it recursively
  function<void(int)> explore_flip_orders =
      [&matrix, &flip_order, &best_order, &max_sum, &explore_flip_orders,
       &sum_quadrants, &q1_is_biggest, &flip_q2_to_q1, &flip_q4_to_q3,
       &flip_q4_to_q2](int index) -> void {
    if (index == 8) {
      // Base case: All flip orders have been explored
      auto quadrants = sum_quadrants(matrix);
      long current_sum = get<0>(quadrants);

      // If Q1 is the largest and the current sum is greater than the maximum
      // sum, update the maximum sum and best order
      if (q1_is_biggest(quadrants) && current_sum > max_sum) {
        max_sum = current_sum;
        best_order = flip_order;
      }
      return;
    }

    // Apply flips based on the current flip order
    flip_order.push_back(index & 1);
    flip_order.push_back((index >> 1) & 1);
    flip_order.push_back((index >> 2) & 1);

    if (flip_order[0])
      flip_q2_to_q1(matrix);
    if (flip_order[1])
      flip_q4_to_q3(matrix);
    if (flip_order[2])
      flip_q4_to_q2(matrix);

    // Recursively explore the next flip order
    explore_flip_orders(index + 1);

    // Undo the flips for the next iteration
    if (flip_order[2])
      flip_q4_to_q2(matrix);
    if (flip_order[1])
      flip_q4_to_q3(matrix);
    if (flip_order[0])
      flip_q2_to_q1(matrix);

    // Remove the last three elements from flip_order to reset it
    flip_order.pop_back();
    flip_order.pop_back();
    flip_order.pop_back();
  };

  explore_flip_orders(0); // Call the function with initial argument
  // Apply the best flip order to maximize Q1 sum
  for (size_t i = 0; i < best_order.size(); i += 3) {
    if (best_order[i])
      flip_q2_to_q1(matrix);
    if (best_order[i + 1])
      flip_q4_to_q3(matrix);
    if (best_order[i + 2])
      flip_q4_to_q2(matrix);
  }

  // Recalculate Q1 sum
  max_sum = get<0>(sum_quadrants(matrix));
  cout << "Maximum sum in the first quadrant: " << max_sum << endl;
  return max_sum;
}

// Test the flippingMatrix function
void test_flippingMatrix() {
  // Test case 1
  vector<vector<int>> matrix1 = {{1, 2}, {3, 4}};
  int expected1 = 4;
  int actual1 = flippingMatrix(matrix1);
  assert(actual1 == expected1);

  // Test case 3
  // Input (stdin)
  // 1
  // 2
  // 112 042 083 119        335 307
  // 056 125 056 049
  // 015 078 101 043        253 366
  // 062 098 114 108
  // Goal is to make quadrant 1 the biggest, followed by quadrant 2, then 4,
  // then 3 in clockwise order. Currently, the order is 366 (Q4), 335 (Q1), 307
  // (Q2), and 253 (Q3). Again, goal is to make it clockwise, so first we need
  // to locate the largest quadrant, which is 366(Q4). Clockwise means to: If Q1
  // is biggest, do nothing If Q2 is biggest, move it to Q1 If Q3 is biggest,
  // move it to Q4, and reevaluate If Q4 is biggest, move it Q3 and reevaluate
  // For each flips, we need to look at each row and column and see if the sum
  // of the two in that row/column is greater than the sum of the other two. If
  // so, flip. So, for this example, we need to move Q4 to Q3, so we need to
  // possibly flip both or either of column 2 and/or 3. For column 2, 083+056 <
  // 101+114, so we need to flip column 2. For column 3, 119+049 > 043+108, so
  // we do not need to flip column 3.
  //
  // First, flip column 2:
  // 112 042 114 119        335 383
  // 056 125 101 049
  // 015 078 056 043        253 290
  // 062 098 083 108
  // We now have 383 (Q2) to be the largest, so we need to move it to Q1.
  // Looking at the rows, we see that 112+42 < 114+119, so we need to flip row
  // 0. and 056+125 > 101+49, so we do not need to flip row 1.
  //
  // Then flip row 0:
  // 119 114 042 112        414 304
  // 056 125 101 049
  // 015 078 056 043        253 290
  // 062 098 083 108
  // We now have 414 (Q1) to be the largest, so we are done.
  //
  // Then return the sum of the upper-left quadrant:
  // 119 + 114 + 56 + 125 = 414
  //
  // Expected Output
  // 414
  vector<vector<int>> matrix3 = {{112, 42, 83, 119},
                                 {56, 125, 56, 49},
                                 {15, 78, 101, 43},
                                 {62, 98, 114, 108}};
  int expected3 = 414;
  int actual3 = flippingMatrix(matrix3);
  assert(actual3 == expected3);

  // Test case 4
  // Matix dim: 4x4
  // 0517 0037 0380 3727
  // 3049 1181 2690 1587
  // 3227 3500 2665 0383
  // 4041 2013 0384 0965
  auto matrix4 = vector<vector<int>>{{517, 37, 380, 3727},
                                     {3049, 1181, 2690, 1587},
                                     {3227, 3500, 2665, 383},
                                     {4041, 2013, 384, 965}};
  int expected4 = 7544;
  int actual4 = flippingMatrix(matrix4);
  assert(actual4 == expected4);
}

int main() {

  test_flippingMatrix();
  exit(0);

  ofstream fout(getenv("OUTPUT_PATH"));

  string q_temp;
  getline(cin, q_temp);

  int q = stoi(ltrim(rtrim(q_temp)));

  for (int q_itr = 0; q_itr < q; q_itr++) {
    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    vector<vector<int>> matrix(2 * n);

    for (int i = 0; i < 2 * n; i++) {
      matrix[i].resize(2 * n);

      string matrix_row_temp_temp;
      getline(cin, matrix_row_temp_temp);

      vector<string> matrix_row_temp = split(rtrim(matrix_row_temp_temp));

      for (int j = 0; j < 2 * n; j++) {
        int matrix_row_item = stoi(matrix_row_temp[j]);

        matrix[i][j] = matrix_row_item;
      }
    }

    int result = flippingMatrix(matrix);

    fout << result << "\n";
  }

  fout.close();

  return 0;
}

string ltrim(const string &str) {
  string s(str);

  s.erase(s.begin(),
          find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));

  return s;
}

string rtrim(const string &str) {
  string s(str);

  s.erase(
      find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
      s.end());

  return s;
}

vector<string> split(const string &str) {
  vector<string> tokens;

  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = str.find(" ", start)) != string::npos) {
    tokens.push_back(str.substr(start, end - start));

    start = end + 1;
  }

  tokens.push_back(str.substr(start));

  return tokens;
}
