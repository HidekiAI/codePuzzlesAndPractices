#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'palindromeIndex' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING s as parameter.
 */
int palindromeIndex(string s) {
  // compare left and right string
  auto are_equal = [](const string &left, const string &right) {
    if (left.size() != right.size()) {
      return false;
    }
    // from left, we go from index=0, for right, we go from index=right.size()-1
    // down to 0
    for (int i = 0; i < left.size(); ++i) {
      if (left[i] != right[right.size() - 1 - i]) {
        // immediately opt out soon as we find a mismatch
        return false;
      }
    }
    return true;
  };

  // in nature of palindrome, we have following characteristics:
  // * if the string is odd, the middle character is not important and we only
  // compare left and right
  // * if the string is even, we compare left and right
  auto make_left_and_right = [](const string &s, string &left, string &right) {
    if (s.size() % 2 == 0) {
      left = s.substr(0, s.size() / 2);
      right = s.substr(s.size() / 2, s.size() / 2);
    } else {
      left = s.substr(0, s.size() / 2);
      right = s.substr(s.size() / 2 + 1, s.size() / 2);
    }
  };

  // the edge case is when the string is aleady a palindrome:
  {
    string left, right;
    make_left_and_right(s, left, right);
    if (are_equal(left, right)) {
      // if already one, return -1
      return -1;
    }
  }

  // opmital of any single character will make the string a palindrome but we'll
  // opt out on the first found.  We'll traverse from left to right and omit
  // one character, make left and right string, compare, and return the index
  for (int current_index = 0; current_index < s.size(); ++current_index) {
    string left, right;
    make_left_and_right(
        s.substr(0, current_index) + s.substr(current_index + 1), left, right);
    if (are_equal(left, right)) {
      return current_index;
    }
  }
  return -1; // could not find any
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  cout << "Enter number of test cases: ";
  string q_temp;
  getline(cin, q_temp);

  int q = stoi(ltrim(rtrim(q_temp)));

  for (int q_itr = 0; q_itr < q; q_itr++) {
    cout << "Enter string: ";
    string s;
    getline(cin, s);

    int result = palindromeIndex(s);

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
