#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'pairs' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER k
 *  2. INTEGER_ARRAY arr
 */

// NOTE: Time limit for this test was 33 minutes, I had about 1 minute to spare! (barely made it!)
// sadly, I didn't have time to optimize this code, and am disappointed mainly because
// the solution was very simple; Trying to remember what I've stumbled on was the fact that
// it took me 2 tries to realize that the nested inner loop should start from i+1 (not 0)
// since the left side of the array has already been evaluated against the right side!
// It was mainly due to the mindset of wanting to use for_each() and lambda rather
// than just doing it more simpler.  Lesson learned (I think/hope) is to not overthink
// and start off always in simple for() loop iteration and later consider lambda/for_each()
// const-aware approach since these kinds of tests are more about time-attack rather than
// code elegance (sadly).
int pairs(int k, vector<int> arr) {
  // k=1
  // arr=[1,2,3,4]
  // Result=3: There are 3 values that differ by k=1:
  // 2-1=2, 3-2=1, and 4-3=1
  auto calc_difference = [&k](int left, int right) -> bool {
    // cout << "k=" << k << ": " << left << "-" << right << "=" <<
    // abs(left-right)  << " (" << (abs(left-right) == k) << ")" << endl;
    return abs(left - right) == k;
  };

  // NOTE: each integer arr[i] will be unique!
  // this means, ideally if the array was sorted, we may be able
  // to somehow determine the distance (k) and reduce the iteration
  // for optimization (later)

  // first attempt, just go from current and scan against all
  // if we had 1,2,3,4 then we only need to evaluate 1 against 2,3,4
  // and 2 against 3,4 (because 1 has already been evaluated)
  // and 3 against 4
  auto found_count = 0;
  for (auto i = 0; i < arr.size(); ++i) {
    for (auto j = i + 1; j < arr.size(); ++j) {
      // because we can assume that each integers are unique, we can
      // probably  just skip, but even if we did the diff, it'll be 0 so
      // it won't match (k > 0 if all integers are unique)
      if (calc_difference(arr[i], arr[j])) {
        ++found_count;
      }
    }
  }
  return found_count;
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string first_multiple_input_temp;
  getline(cin, first_multiple_input_temp);

  vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

  int n = stoi(first_multiple_input[0]);

  int k = stoi(first_multiple_input[1]);

  string arr_temp_temp;
  getline(cin, arr_temp_temp);

  vector<string> arr_temp = split(rtrim(arr_temp_temp));

  vector<int> arr(n);

  for (int i = 0; i < n; i++) {
    int arr_item = stoi(arr_temp[i]);

    arr[i] = arr_item;
  }

  int result = pairs(k, arr);

  fout << result << "\n";

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
