#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Post mortem (past 30 minutes):
// Though I've comprehended the problem, I've failed to come up with a solution
// in time. Possibly, I should have at least began with a brute force solution
// by first, writing functions that will check/test for the validity of a wall
// via looking for vertical breaks.
// I'd then come up with 3 basic types of rows:
// 1. first row is based on all 1x1 bricks
// 2. second type is based on combinations of widest bricks, i.e. if the width is 9, then lay 2 1x4 bricks and padd it with 1 1x1 
// 3. third type is based on combinations of 1x2 and 1x3 bricks
// in all cases, we would basically come up with basic rows Vector<Vector<brick_type>>
// we'd then iterate based on height(n) by laying down these row-types and then looking for vertical breaks
// against row above to determine if that row should be rejected. then next rows and test vertical breaks again,
// and so on...

/*
 * Complete the 'legoBlocks' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n (height of the wall)
 *  2. INTEGER m (width of the wall)
 */

int legoBlocks(int n, int m) {
  // depth	height	width
  // 1	    1	      1           []
  // 1	    1	      2           [..]
  // 1	    1	      3           [....]
  // 1	    1	      4           [......]
  const auto bricks =
      vector<vector<int>>{{1, 1, 1}, {1, 1, 2}, {1, 1, 3}, {1, 1, 4}};

  // - The wall should not have any holes in it.
  // - The wall you build should be one solid structure, so there SHOULD NOT be
  //    a straight vertical break across all rows of bricks.
  // - The bricks must be laid horizontally.
  // Example of "bad layouts" in which the wall has holes or a straight vertical
  // break for a n=2, m=3:
  // [][][]       [][..]
  // [][..]       [][..]
  // Note that for a n=2, m=3, there are total of 9 valid/good layouts.
  // All results needs to mod against (10^9 + 7) to avoid overflow.
  const auto kMod = 1000000007; // 10^9 + 7

  // sample input:
  // n = 2, m = 2 -> result = 3 valid cases modulo (10^9 + 7) = 3
  //  [..]    [][]    [..]
  //  [..]    [..]    [][]
  // n = 3, m = 2 -> result = 2 x 2 x 2 - 1 = 7 valid cases modulo (10^9 + 7) =
  // 7
  //  [..]    [][]    [..]    [..]    [..]    [][]    [][]
  //  [..]    [..]    [][]    [][]    [..]    [..]    [..]
  //  [..]    [..]    [..]    [..]    [][]    [..]    [][]
  // n = 2, m = 3 -> result = 9
  //  [....]  [....]  [][][]  [..][]  [....]  [][..]  [....]  [][..]  [..][]
  //  [....]  [][][]  [....]  [....]  [..][]  [....]  [][..]  [..][]  [][..]
  // n = 4, m = 4 -> result = 3375
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string t_temp;
  getline(cin, t_temp);

  int t = stoi(ltrim(rtrim(t_temp)));

  for (int t_itr = 0; t_itr < t; t_itr++) {
    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input =
        split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    int result = legoBlocks(n, m);

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
