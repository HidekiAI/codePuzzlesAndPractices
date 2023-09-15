#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'isBalanced' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */

string isBalanced(string s) {
  vector<tuple<char, char>> kBrackets = {
      make_tuple('(', ')'), make_tuple('{', '}'), make_tuple('[', ']')};

  // determine whether the bracket is open or close
  auto evaluate_bracket = [](char bracket,
                             vector<tuple<char, char>> kBrackets) {
    for (auto &kPaired_bracket : kBrackets) {
      if (bracket == get<0>(kPaired_bracket)) {
        // true means open bracket
        return make_tuple(true, kPaired_bracket);
      } else if (bracket == get<1>(kPaired_bracket)) {
        // false means close bracket
        return make_tuple(false, kPaired_bracket);
      }
    }
    throw invalid_argument("bracket is not in the list of kBrackets");
  };

  // 1. The string {[()]} meets both criteria for being a balanced string.
  // 2. The string {[(])} is not balanced because the brackets enclosed by the
  // matched pair { and } are not balanced: [(]).
  // 3. The string {{[[(())]]}} meets both criteria for being a balanced string.
  // we'll use the stack to track the opening brackets, this way, if we get '(',
  // '(', combo, we know that when ')' is popped, next expected is ')' (unless
  // another open brackets are encounted and new one is pushed to stack)
  vector<char> stack;

  do {
    // grab head, and pop it (move to next char)
    auto ch = s.front();
    s.erase(s.begin());

    auto [is_open_bracket, kPaired_bracket] = evaluate_bracket(ch, kBrackets);
    if (is_open_bracket) {
      // push to stack
      stack.push_back(ch);
    } else {
      auto expected_bracket = get<0>(kPaired_bracket);
      if (stack.size() == 0) {
        //return "NO (stack is empty, but we're expecting a closing bracket)";
        return "NO";
      }
      // pop from stack and verify that it should be the MATCHING bracket
      auto popped_bracket = stack.back();
      stack.pop_back();
      if (popped_bracket != expected_bracket) {
        //return string("NO (popped bracket is not the expected bracket - "
        //              "Expected='") +
        //       string(1, expected_bracket) + string("', Popped='") +
        //       string(1, popped_bracket) + string("')");
        return "NO";
      }
    }
  } while (s.size() > 0);
  // if stack is not empty, then we have an open bracket that is not closed
  return stack.size() == 0 ? "YES" : "NO";
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string t_temp;
  getline(cin, t_temp);

  int t = stoi(ltrim(rtrim(t_temp)));

  for (int t_itr = 0; t_itr < t; t_itr++) {
    string s;
    getline(cin, s);

    string result = isBalanced(s);

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
