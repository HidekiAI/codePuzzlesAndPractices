#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'caesarCipher' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts following parameters:
 *  1. STRING s
 *  2. INTEGER k
 */

string caesarCipher(string s, int k) {
  //  Caesar's cipher shifts each letter by a number of letters. If the shift
  //  takes you past the end of the alphabet, just rotate back to the front of
  //  the alphabet. In the case of a rotation by 3, w, x, y and z would map to
  //  z, a, b and c.
  // Example:
  // s = "There's-a-starman-waiting-in-the-sky";
  // k = 3;
  // The alphabet is rotated by 3, matching the mapping above. The encrypted
  // string is .
  // ret = "Wkhuh'v-d-vwdupdq-zdlwlqj-lq-wkh-vnb";
  // Note: The cipher only encrypts letters; symbols, such as -, remain
  // unencrypted.
  auto shift_char = [](char c, int shift) {
    const auto k_alphabet_size = 26;
    // if not a letter, return as is
    if (!isalpha(c)) {
      return c;
    }
    // if uppercase, shift by 'A'
    if (isupper(c)) {
      return (char)('A' + (c - 'A' + shift) % k_alphabet_size);
    }
    // if lowercase, shift by 'a'
    return (char)('a' + (c - 'a' + shift) % k_alphabet_size);
  };

  // iterate over each character and shift
  string ret_string = "";
  for (const auto &c : s) {
    ret_string += shift_char(c, k);
  }
  return ret_string;
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string n_temp;
  getline(cin, n_temp);

  int n = stoi(ltrim(rtrim(n_temp)));

  string s;
  getline(cin, s);

  string k_temp;
  getline(cin, k_temp);

  int k = stoi(ltrim(rtrim(k_temp)));

  string result = caesarCipher(s, k);

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
