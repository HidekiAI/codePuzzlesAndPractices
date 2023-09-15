#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'noPrefix' function below.
 *
 * The function accepts STRING_ARRAY words as parameter.
 */

void noPrefix(vector<string> words) {
  // only one lower-case, a..j inclusive -> "GOOD SET"
  // assuming each set only has ['a'..'j'] in a "word",
  // given sets of "words" locate potential "BAD SET"
  // (if not found, output "GOOD SET") based on the rule
  // that no string is a prefix of another string.
  // i.e. words=['abcd', 'bcd', 'abcde', 'bcde'],
  // since 'abcd' is a prefix to 'abcde', and
  // 'bcd' is a prefix to 'bcde', we print that
  // it is a "BAD SET" followed by the first encounter
  // of the "BAD SET" which is 'abcde' (for 'abcd')

  // Problem:
  // on a first pass, I was checking from current index to look for first
  // encounter of subset, but this is not correct, since the "first encounter"
  // is based on the super-set, not the subset. i.e. if we have ['abcd', 'bcd',
  // 'bcde', 'abcde'], though 'abcd' is a subset of 'abcde', we should not print
  // 'abcd' as the first encounter of the "BAD SET", since 'bcde' appears before
  // 'abcde'...

  // sorting the "words" list would ideally make it easier, but this will break
  // the issue of wanting to print the first "BAD SET" encountered, since the
  // "words" list is sorted
  for (auto current_word_index = 0; current_word_index < words.size();
       ++current_word_index) {
    // this is with an assumption that smaller set is prior to larger set
    for (auto i = current_word_index + 1; i < words.size(); ++i) {
      // if the current word is a prefix of the next word, then we have a "BAD
      // SET"
      if (words[i].find(words[current_word_index]) ==
          0) // beauty of str::find() is that it's basically equivalent to
             // string.contains()
      {
        cout << "BAD SET" << endl;
        cout << words[i] << endl;
        return;
      }
    }
    // if here, we may need to go backwards to check if the current word is a
    // prefix of the previous word
    for (auto i = 0; i < current_word_index;
         ++i) // again, we start from 0 to current_word_index, not down from
              // current_word_index to 0
    {
      // if the current word is a prefix of the next word, then we have a "BAD
      // SET"
      if (words[i].find(words[current_word_index]) == 0) {
        cout << "BAD SET" << endl;
        cout << words[i] << endl;
        return;
      }
    }
  }
  cout << "GOOD SET" << endl;
}

int main() {
  string n_temp;
  getline(cin, n_temp);

  int n = stoi(ltrim(rtrim(n_temp)));

  vector<string> words(n);

  for (int i = 0; i < n; i++) {
    string words_item;
    getline(cin, words_item);

    words[i] = words_item;
  }

  noPrefix(words);

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
