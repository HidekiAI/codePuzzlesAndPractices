#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'superDigit' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. STRING n
 *  2. INTEGER k
 */



// WARNING: the string n can be very long length!
int superDigit(string n, int k) {
  // We define super digit of an integer  using the following rules:
  // Given an integer, we need to find the super digit of the integer.
  //  * If x has only 1 digit, then its super digit is x.
  //  * Otherwise, the super digit of x is equal to the super digit of the sum
  //  of the digits of x. For example, the super digit of 9875 will be
  //  calculated as:
  //  * super_digit(9875)  9+8+7+5 = 29
  //  * super_digit(29)    2+9 = 11
  //  * super_digit(11)    1+1=2
  //  * super_digit(2)     =2
  //  Example:
  //  n="9875";
  //  k=4;
  //  The number p is created by concatenating the string n, k times
  //  so the initial p=9875987598759875
  //  super_digit(p) = 9+8+7+5+9+8+7+5+9+8+7+5+9+8+7+5 = 116
  //  super_digit(p) = super_digit(116) = 1+1+6=8
  //  super_digit(8) = 8
  //  All of the digits of p sum to 116.  digits of 116 sum to 8.
  //  8 is the only one figit, so it is the super digit.

  std::function<uint64_t(const std::string &)> recursive_super_digit =
      [&](const string &s) -> uint64_t {
    // if the string is only one character, return as is
    if (s.size() == 1) {
      // by here, we know the string is a single digit so we can use stoi()
      // safely
      return stoi(s);
    }
    // otherwise, sum up all the digits
    uint64_t sum = 0;
    for (const auto &c : s) {
      // assume char is 0..9, so subtract '0' to get the actual value
      sum += c - '0';
    }
    // convert to string and call recursively
    return recursive_super_digit(to_string(sum));
  };

  // Note that rather than calculating p=k*n and then super_digit(p), we can
  // we'll just calculate super_digit(n) and then multiply by k and take
  // super_digit of that result.
  // i.e. super_digit(n) = 5, k=4, then super_digit(5*4) = super_digit(20) = 2
  auto super_digit_of_n = recursive_super_digit(n) * k;
  return recursive_super_digit(to_string(super_digit_of_n));
}



int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    string n = first_multiple_input[0];

    int k = stoi(first_multiple_input[1]);

    int result = superDigit(n, k);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

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
