#include <bits/stdc++.h>
#define DBGOUT

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Post-mortem (past 30 minutes):
// There is an error in the example description, and
// that caused me to get delayed in solving this problem.
// Due to only about 2 minutes left, I only was able to
// submit it once, in which an input of k=7 A=[1 2 3 9 10 12]
// gve me a result back of 1 operation when it was actually
// expected to be 2.  The issue was because
// my do/while loop was not checking for the condition to be
// false, but rather true.  And the second bug was because
// I wasn't returning -1 on invalid cases such as when
// k=200 A=[5, 6, 7] it should return -1
// Finally, some of the test cases were timing out, and
// that was because I was sorting the list each time
// There was a test case in which it had 1,000,000 x 1's
// and k=1,000,000,000.

/*
 * Complete the 'cookies' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER k
 *  2. INTEGER_ARRAY A
 */

int cookies(int k, vector<int> A) {
#ifdef DBGOUT
  cout << "[cookies] k: " << k << " - A.size(): " << A.size() << endl;
#endif
  // example1:
  // sweetness threshold k=9
  // A = [2, 7, 3, 6, 4, 6]
  // Sort least to most sweet: A = [2, 3, 4, 6, 6, 7]
  // least sweet cookies are level 2 and 3
  // combine them to get 2 + 2 * 3 = 8 (sweetness = 1x least_sweet + 2x
  // 2nd_least_sweet) 8 is less than k=9, so we combine the next least sweet
  // cookies
  //
  // A = [4, 6, 6, 7, 8]
  // 4 and 6 are the least sweet cookies
  // combine them to get 4 + 2 * 6 = 16 (sweetness = 1x least_sweet + 2x
  // 2nd_least_sweet)
  //
  // A = [6, 7, 8, 16], 6, 7, and 8 are still < 9 (but 16 is now > 9)
  // take the next least sweet cookies of 6 and 7
  // combine them to get 6 + 2 * 7 = 20 (sweetness = 1x least_sweet + 2x
  // 2nd_least_sweet)
  //
  // A = [8, 16, 20]
  // take the next least sweet cookies of 8 and 16
  // combine them to get 8 + 2 * 16 = 40 (sweetness = 1x least_sweet + 2x
  // 2nd_least_sweet)
  //
  // A = [20, 40]
  // now that all sweetness are >= 9, we return the number of operations it took
  // to get here which is 4 operations

  // NOTE: Because k can be some insanely huge number, and the array can be
  // incredibly large (but presorted from least to most),
  // it is optimal to opt out as soon as we find sweetness level that is
  // less than k.  For example list is 100,000 elements long with all 1's, and k
  // is 100. that means that as long as those "1's" remain in the the list, very
  // first element are going to be "1" for many many iterations, hence we can
  // opt out very quickly!
  auto are_all_cookies_sweet_enough = [&A, k]() {
    for (const auto &cookie : A) {
      if (cookie < k) {
        return false;
      }
    }
    return true;
  };
  // Note that std::map is already presorted by key, so the beauty of it is that
  // we can just pick the head of the map to get the least sweet cookie and
  // ++head to get the second least sweet cookie
  auto distinct_sweetness_levels = map<int, size_t>(); // key: sweetness level
  // decrement the count of the key in the map
  auto decrement_distinct_list = [&distinct_sweetness_levels](const int key) {
    auto it = distinct_sweetness_levels.find(key);
    if (it != distinct_sweetness_levels.end()) {
      // found it, so decrement the value
      it->second--;
      if (it->second == 0) {
        distinct_sweetness_levels.erase(it);
      }
    }
  };
  // incremnt or add key to the map in case there was an issue and needs an
  // undo...
  auto increment_distinct_list = [&distinct_sweetness_levels,
                                  &A](const int key) {
#ifdef DBGOUT
    // NOTE: Make sure to pass as &A instead of A, for the later will make a
    // copy that may not have/contain updated values
    auto it_in_A = find(A.begin(), A.end(), key);
    if (it_in_A == A.end()) {
      cout << "ERROR: key=" << key << " not found in A" << endl;
      for (const auto &cookie : A) {
        cout << cookie << " ";
      }
      cout << endl;
      throw runtime_error("key not found in A");
    }
#endif
    // NOTE: we do not check whether the key is greater than k
    auto it = distinct_sweetness_levels.find(key);
    // check if key (cookie) already exists in map
    // if it does, then increment the value, else add it to the map
    if (it != distinct_sweetness_levels.end()) {
      // found it, so increment the value
      it->second++;
    } else {
      // not found, so add it to the map
      distinct_sweetness_levels.insert(make_pair(key, 1));
    }
#ifdef DBGOUT
    cout << "distinct_sweetness_levels: ";
    for (const auto &sweetness_level : distinct_sweetness_levels) {
      cout << sweetness_level.first << "(" << sweetness_level.second << ")"
           << endl;
    }
#endif
  };
  auto build_distincts = [&A, k, &distinct_sweetness_levels,
                          &increment_distinct_list]() {
    for (const auto &cookie : A) {
      // only add to the map if the sweetness level is less than k
      increment_distinct_list(cookie);
    }
  };
  // find 2 least sweet cookies
  auto find_least_sweet_cookies = [&distinct_sweetness_levels]() {
#ifdef DBGOUT
    cout << "distinct_sweetness_levels: ";
    for (const auto &sweetness_level : distinct_sweetness_levels) {
      cout << sweetness_level.first << "(" << sweetness_level.second << "), ";
    }
    cout << endl;
#endif
    // Note: std::map is already presorted by key, so just pick the head
    if (distinct_sweetness_levels.size() == 0) {
      // we have couple approaches here, one is just fail, and the other is to
      // iterate through the A list and find the 2 least sweet cookies
      throw runtime_error("1: no distinct sweetness levels found");
    } else if (distinct_sweetness_levels.size() == 1) {
      // we have only 1 distinct sweetness level, but as long as count is 2 or
      // more, then we can use it
      if (distinct_sweetness_levels.begin()->second > 1) {
        auto least_sweet_cookie = distinct_sweetness_levels.begin()->first;
        return make_pair(least_sweet_cookie, least_sweet_cookie);
      } else {
        // we have only 1 distinct sweetness level, and count is 1, so we need
        // to find the second least sweet cookie
        throw runtime_error("2: no distinct sweetness levels found");
      }
    };
    // locate 2 least sweet cookies
    // Note: std::map is already presorted by key, so just pick the head
    // and if the head count is 2 or more, then make the second cookie same
    // as the first cookie
    auto current = distinct_sweetness_levels.begin();
    auto least_sweet_cookie = current->first;
    // find the second least sweet cookie
    auto second_least_sweet_cookie = least_sweet_cookie;
    if (current->second < 2) {
      // not enough, so move to the next one
      ++current;
      if (current == distinct_sweetness_levels.end()) {
        throw runtime_error("3: no distinct sweetness levels found");
      }
      second_least_sweet_cookie = current->first;
    }
    return make_pair(least_sweet_cookie, second_least_sweet_cookie);
  };

  // NOTE: Due to huge size of the list, we no longer will sort it each update
  // hence, we assume that the very initial list will be pre-sorted; when
  // we calculate the result, if it's less than k, then we'll push it to the
  // head of the array, and if it's greater than or equal to k, then we'll
  // push it to the tail of the array.  we'll also remember the last known
  // smallest value, so we can quickly seek to it and combine it with the
  // next smallest value (which should be the second element in the list)
  auto combine_least_sweet_cookies =
      [&A, k, increment_distinct_list](
          const std::pair<int, int> &last_known_smallest_sweetness) {
#ifdef DBGOUT
        cout << "A.size()=" << A.size()
             << " last_known_smallest_sweetness.first="
             << last_known_smallest_sweetness.first
             << " last_known_smallest_sweetness.second="
             << last_known_smallest_sweetness.second << endl;
        if (A.size() < 256) {
          for (size_t i = 0; i < A.size(); i++) {
            cout << A[i] << " ";
          }
          cout << endl;
        }
#endif
        if (A.size() < 2) {
          return;
        }
        // locate the least sweet cookie in A so that we can remove it
        auto it_smallest =
            find(A.begin(), A.end(), last_known_smallest_sweetness.first);
        auto val1 = *it_smallest;
        if (it_smallest == A.end()) {
#ifdef DBGOUT
          cout << "Could not find value " << last_known_smallest_sweetness.first
               << " for current A.size()=" << A.size() << endl;
#endif
          throw runtime_error("Could not find value");
        }
        A.erase(it_smallest);

        // start again from the head because we just removed the first element
        it_smallest =
            find(A.begin(), A.end(), last_known_smallest_sweetness.second);
        auto val2 = *it_smallest;
        if (it_smallest == A.end()) {
          // put back the first element before we bail
          A.insert(A.begin(), val1);
#ifdef DBGOUT
          cout << "it_smallest2 == A.end()" << endl;
#endif
          return;
        }
        A.erase(it_smallest);

        auto least_sweet_cookie = val1 < val2 ? val1 : val2;
        auto second_least_sweet_cookie = val2 >= val1 ? val2 : val1;
        auto result = least_sweet_cookie + 2 * second_least_sweet_cookie;

        // and make to to resort it, so it's always sorted least to most sweet
        // note that this is not the most efficient way to do this, if the
        // list is very large (i.e. one of the sample data was 100,000 x 1's)
        // if result >= k, then we'll push to the tail
        // if result < k, then we'll push to the head
        if (result < k) {
          // append to the head so we can std::find it quickly
          A.insert(A.begin(), result);
          // also need to add this to the distinct list
        } else {
          // we no longer care about this value, so we'll just append it to tail
          A.push_back(result);
        }
        increment_distinct_list(result);
#ifdef DBGOUT
        cout << "result: " << result << " (k=" << k
             << ") least_sweet_cookie: " << least_sweet_cookie
             << " second_least_sweet_cookie: " << second_least_sweet_cookie
             << " A.size(): " << A.size() << endl;
#endif
      };

  auto operations = 0L;

  // sort least to most sweet
  sort(A.begin(), A.end());

  // deal with edge-case where you have 1,000,000 x 1's and k=1,000,000,000
  // i.e. you have 10 sequences of 1's:
  // O1: A[1,1,1,1,1,1,1,1,1,1], k=10
  // O2: A[1,1,1,1,1,1,1,1,2]
  // O3: A[1,1,1,1,1,1,2,2]
  // O4: A[1,1,1,1,2,2,2]
  // O5: A[1,1,2,2,2,2]
  // O6: A[2,2,2,2,2], k=2 is minimum
  // O7: A[2,2,2,4]
  // O8: A[2,4,4]
  // O9: A[4,6], k=4 is minimum
  // 10: A[10], k=10 is minimum
  // took 10 operation to get here
  // because the list is presroted by here, we can just count all the 1's up to
  // or equal to k, and replace those 1's with the total count of 1's
  auto have_ones = A[0] == 1;
  while (have_ones) {
    auto count_ones = 0;
    for (auto it = A.begin(); it != A.end(); ++it) {
      if ((*it == 1) && (count_ones <= k)) {
        ++count_ones;
      } else {
        // opt out as soon as we find a value that is greater than 1
        break;
      }
    }
    if (count_ones > 0) {
      // erase all the 1's up to count_ones and append to tail th total count
      // of 1's
#ifdef DBGOUT
      cout << "count_ones: " << count_ones << endl;
#endif
      operations += count_ones - 1;
      A.erase(A.begin(), A.begin() + count_ones);
      A.push_back(count_ones);
    }
    have_ones = A[0] == 1;
  }
  // now generate a pre-sorted list of all UNIQUE sweetness levels
  build_distincts();
  while (false == are_all_cookies_sweet_enough() && A.size() > 1) {
    operations++;
    auto last_known_smallest_sweetness = find_least_sweet_cookies();
    combine_least_sweet_cookies(last_known_smallest_sweetness);
  };

  return (A.size() > 0) ? (operations > 0 ? operations : -1) : -1;
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string first_multiple_input_temp;
  getline(cin, first_multiple_input_temp);

  vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

  int n = stoi(first_multiple_input[0]);

  int k = stoi(first_multiple_input[1]);

  string A_temp_temp;
  getline(cin, A_temp_temp);

  vector<string> A_temp = split(rtrim(A_temp_temp));

  vector<int> A(n);

  for (int i = 0; i < n; i++) {
    int A_item = stoi(A_temp[i]);

    A[i] = A_item;
  }

  int result = cookies(k, A);

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
