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
// 20 digits 18 quintillion can overflow so we'll cap at 19 digits
// if at worst case, if all digits are '9's in which summing each
// digits that needs to fit inside a 64-bits unsigned long long int
// would be 18quintrillion / 9 = 2 billion digits
// and I'm hoping nobody is cruel enough to make the string that long!
// AND at the same time, k can be very large as well (i.e. 10,000)
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
  //  * n="9875";
  //  * k=4;
  //  The number p is created by concatenating the string n, k times
  //  so the initial p=9875987598759875
  //  super_digit(p) = 9+8+7+5+9+8+7+5+9+8+7+5+9+8+7+5 = 116
  //  super_digit(p) = super_digit(116) = 1+1+6=8
  //  super_digit(8) = 8
  //  All of the digits of p sum to 116.  digits of 116 sum to 8.
  //  8 is the only one figit, so it is the super digit.
  //  NOTE: Alternatively, it's better to just calulcate super_digit(n) and
  // then multiply by k and take super_digit of that result.
  // For example:
  // super_digit(9875) = 2 (see above)
  // multiply that by k=4, we get 2 * 4 = 8
  // Note that if k=5, we'd have to sd that result again, so
  // we get 2 * 5 = 10 => super_digit(10) = 1
  // Proof: 29*5=145 => sd(1+4+5) = sd(10) = sd(1+0) = sd(1) = 1
  //
  // Examples when we need to partition the string into blocks of 4 digits for
  // super_digit(123456789024680): block 1: super_digit(12345) = sd(15) = 6
  // block 2: sd(67890) = sd(30) = 3
  // block 3: sd(24680) = sd(20) = 2
  // block 1, 2 and 3 combined: = sd(6 + 3 + 2) = sd(11) = 2
  // Note that it is actually doing superdigit of 4 digits, which then is
  // superdigit of 2 digits, which then is superdigit of 1 digit proof:
  // 123456789024680 = sd(1+2+3+4+5+6+7+8+9+0+2+4+6+8+0) = sd(65) = sd(11) = 2
  // Example 2 digits for sd(123456789024680):
  // block 1: 12 = 3 ; block 2: 34 = 7 ; block 3: 56 = 11 = 2
  // block 4: 78 = 15 = 6 ; block 5: 90 = 9 ; block 6: 24 = 6 ; block 7: 68 = 14
  // = 5 block 8: 0 = 0 block 1, 2, 3, 4, 5, 6, 7 and 8 combined: sd(3 + 7 + 2 +
  // 6 + 9 + 6 + 5 + 0) = sd(38) = sd(11) = 2
  // Examples when we need to partition the string into blocks of 4 digits for
  // 1234 repeated 16 times
  // which is basically super_digit(1234) * 16 = sd(10) * 16 = sd(1) * 16 = 16
  // => sd(16) = sd(7) Proof: sd(1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4
  // +1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4 +1+2+3+4
  // +1+2+3+4 +1+2+3+4 +1+2+3+4) = sd(160) = sd(7)

  // partition the string into blocks of 19 digits (if greater than eighteen
  // quintillion, it'll overlflow still, so we'll limit to 19 digits instead of
  // 20 digits of eighteen quintillion)
  auto partition_string = [](const string &s, int partition_size) {
    vector<string> partitions;
    for (int i = 0; i < s.size(); i += partition_size) {
      partitions.push_back(s.substr(i, partition_size));
    }
    return partitions;
  };

  std::function<uint64_t(const std::string &)> recursive_super_digit =
      [&](const string &string_block) -> uint64_t {
    // if the string is only one character, return as is
    if (string_block.size() == 1) {
      // by here, we know the string is a single digit so we can use stoi()
      // safely
      return stoi(string_block);
    }

    // we'll do the divide-and-conquer approach of binary partitioning until
    // we get to the reasonable amount of digit counts that we can sum up
    // without overflowing
    uint16_t sum = 0;

    // try to optimize by iterating over the string and sum up the digits if
    // within the max_digits limit
    if (string_block.size() < max_digits) {
      // otherwise, sum up all the digits
      for (const auto &c : string_block) {
        // assume char is 0..9, so subtract '0' to get the actual value
        sum += c - '0';
      }
    }

    // half the size
    auto partitions = partition_string(string_block, string_block.size() / 2);
    // for each halves, break it down again by halving it (recursive)
    for_each(partitions.begin(), partitions.end(),
             [&sum, &recursive_super_digit](const string &s) {
               sum += recursive_super_digit(s);
             });

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

int main() {
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
