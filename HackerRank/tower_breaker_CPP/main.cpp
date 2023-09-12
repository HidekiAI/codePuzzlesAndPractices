// Two players are playing a game of Tower Breakers! Player  always moves first,
// and both players always play optimally.The rules of the game are as follows:
//
// Initially there are n towers.
// * Each tower is of height m.
// * The players move in alternating turns.
// * In each turn, a player can choose a tower of height X and reduce its height
// to Y, where 1 <= Y < X and Y evenly divides X.
// * If the current player is unable to make a move, they lose the game.
// Given the values of n and m, determine which player will win. If the first
// player wins, return 1. Otherwise, return 2.
//
// Example.
// n = 2
// m = 6
//
// There are  2 towers, each 6  units tall. Player 1 has a choice of two moves:
// - remove 3 pieces from a tower to leave 3 as 6 % 3 = 0
// - remove 5 pieces to leave 1
//
// Let Player 1 remove 3. Now the towers are 3 and 6 units tall.
//
// Player  2 matches the move. Now the towers are both 3 units tall.
//
// Now Player 1  has only one move.
//
// Player 1 removes 2 pieces leaving 1. Towers are 1  and 2 units tall.
// Player 2  matches again. Towers are both 1 unit tall.
//
// Player 1  has no move and loses. Return 2.
//
// Function Description
//
// Complete the towerBreakers function in the editor below.
//
// towerBreakers has the following paramter(s):
//
// * int n: the number of towers
// * int m: the height of each tower
// Returns
//
// * int: the winner of the game (1 or 2)
// Input Format
//
// The first line contains a single integer t, the number of test cases.
// Each of the next t lines describes a test case in the form of 2
// space-separated integers, n and m.
//
// Constraints
//  1 <= t <= 100
//  1 <= n,m <= 10^6
//
// Sample Input
//  STDIN   Function
//  -----   --------
//  2       t = 2
//  2 2     n = 2, m = 2
//  1 4     n = 1, m = 4
// Sample Output
//  2
//  1
// Explanation
// We'll refer to player 1 as P1  and player 2 as P2
//
// In the first test case, P1 chooses one of the two towers and reduces it to 1.
// Then P2 reduces the remaining tower to a height of 1.
// As both towers now have height 1, P1 cannot make a move so P2 is the winner.
//
// In the second test case, there is only one tower of height 4.
// P1 can reduce it to a height of either 1 or 2.
// P1 chooses 1 as both players always choose optimally.
// Because P2 has no possible move, P1 wins.

#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'towerBreakers' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER m
 */

int towerBreakers(int number_of_towers, int each_tower_height) {
  // * Each tower is of height m.
  // * In each turn, a player can choose a tower of height X and reduce its
  // height to Y, where 1 <= Y < X and Y evenly divides X.
  // for example, if the height is 6, the player can choose either 3 (6 % 3 ==
  // 0) or 5 to reduce the height to 1 (player must play optimal moves)
  auto pick_tower_height = [](int tower_height) {
    // in order to make "optimal" moves, the evaluations should be in
    // high-to-low order and pick as much as possible
    for (int i = tower_height - 1; i > 0; --i) {
      if (tower_height % i == 0) {
        return i; // opt out immediately
      }
    }
    // no choice but to return tower_height - 1
    return tower_height - 1;
  };

  auto current_player = 1;
  do {
    // * The players move in alternating turns.
    // * If the current player is unable to make a move, they lose the game.
    auto new_tower_height = pick_tower_height(each_tower_height);
    if (new_tower_height <= 0) {
      // current player loses
      return current_player == 1 ? 2 : 1;
    }
    // next player's turn
    current_player = current_player == 1 ? 2 : 1;
    // since each turn is played with "optimal" moves, we can assume that tower
    // is done/removed and no more moves can be made on THAT tower
    --number_of_towers;
  } while (number_of_towers > 0);
  // if here, it was the last player win, not the next player win
  return current_player == 1 ? 2 : 1;
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

    int result = towerBreakers(n, m);

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
