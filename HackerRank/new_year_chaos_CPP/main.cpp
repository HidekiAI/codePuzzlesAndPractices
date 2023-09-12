#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'minimumBribes' function below.
 *
 * The function accepts INTEGER_ARRAY q as parameter.
 */
void minimumBribes(vector<int> q) {
  auto total_bribes = 0;
  const auto kMax_bribe_count_per_person = 2;
  const auto kToo_chaotic =
      "Too chaotic"; // if single person bribed more than max bribe count
  auto bribes_per_person = vector<int>(q.size()); // track bribes per person

  auto find_index = [](const vector<int> &q, int value) {
    for (size_t i = 0; i < q.size() / 2; ++i) {
      if (q[i] == value) {
        return i;
      }
      if (q[q.size() - i - 1] == value) {
        return q.size() - i - 1;
      }
    }
    return (size_t) -1;
  };

  // sample: 5 1 2 3 7 8 6 4 - return: "Too chaotic"
  // sample: 1 2 5 3 7 8 6 4
  // expected: 7 bribes
  // [0]1
  // [1]2
  // [2]5 bribed 3 and 4 (2)
  // [3]3
  // [4]7 bribed 6 and 4 (2)
  // [5]8 bribed 6 and 4 (2)
  // [6]6 bribed 4 (1)
  // [7]4
  // 6 - 1:0, 2:0, 3:0, 4:0, 5:2, 6:0, 7:2, 8:2,

  // count all the person_id that are less than current person_id in the end of
  // the queue
  auto count_bribes = [](const vector<int> &q, int current_person_queue_index) {
    auto bribes = 0;
    const auto current_person_id = q[current_person_queue_index];
    if (current_person_queue_index + 1 >= q.size()) {
      return bribes;
    }
    // search from the next person in the queue to the end of the queue
    for (int i = current_person_queue_index + 1; i < q.size(); ++i) {
      // if the person_id in the queue is less than the current person_id, then
      // it has bribed the that person that is now behind
      if (q[i] < current_person_id) {
        ++bribes;
      }
    }
    return bribes;
  };

  string debugString = "";

  for (auto expected_index = 0; expected_index < q.size(); ++expected_index) {
    auto person_id = expected_index + 1;
    // seek current person index to see if it has bribed person in front
    auto queue_index = find_index(q, person_id);
    // check by looking at expected index position and whether
    // the queue_position found is less than the expected index position
    // if so, most likely the person has bribed someone in front
    auto bribes = count_bribes(q, queue_index);
    if (bribes > kMax_bribe_count_per_person) {
      cout << kToo_chaotic << endl;
      return;
    }
    // if here, the person has bribed someone in front
    total_bribes += bribes;
    // track bribes per person
    bribes_per_person[person_id - 1] = bribes;
    debugString += to_string(person_id) + ":" + to_string(bribes) + ",";
  }

  // cout << total_bribes << " - " << debugString << endl;
  cout << total_bribes << endl;
}

int main()
{
    string t_temp;
    getline(cin, t_temp);

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string n_temp;
        getline(cin, n_temp);

        int n = stoi(ltrim(rtrim(n_temp)));

        string q_temp_temp;
        getline(cin, q_temp_temp);

        vector<string> q_temp = split(rtrim(q_temp_temp));

        vector<int> q(n);

        for (int i = 0; i < n; i++) {
            int q_item = stoi(q_temp[i]);

            q[i] = q_item;
        }

        minimumBribes(q);
    }

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
