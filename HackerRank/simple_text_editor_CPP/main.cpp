#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

// Post mortem: This quiz took me 30 minutes to complete. 
// It took 3 trials to finally realize that I was passing
// const vector of undo_stack to do_undo, in which the realization
// came about when I realized I wasn't popping the undo stack
// and when I added the pop_back, I got a compiler error (because
// I was passing const vector to undo).
// Very careless mistake in which I seem to make often during
// these quizzes when I have to time myself.

enum Operation {
  Undefined = 0,
  Append = 1, // Append string W to the end of S
  Delete = 2, // Delete the last k characters of S
  Print = 3,  // Print the k-th character of S
  Undo = 4    // Undo the last (not previously undone) operation of type 1 or 2,
              // reverting S to the state it was in prior to that operation
};

int main() {
  // Append string W to the end of S
  auto do_append = [](string &S, const string &W, vector<string> &undo_stack) {
    undo_stack.push_back(S); // save the current state of S prior to appending W
    S.append(W);
  };
  // delete the last k characters of S
  auto do_delete = [](string &S, const int k, vector<string> &undo_stack) {
    undo_stack.push_back(S); // save the current state of S prior to deleting
    S.erase(S.end() - k, S.end());
  };
  // print the k-th character of S
  auto do_print = [](const string &S, const int k) {
    cout << S[k - 1] << endl;
  };
  // undo the last (not previously undone) operation of type 1 or 2,
  // reverting S to the state it was in prior to that operation
  auto do_undo = [](string &S, vector<string> &undo_stack) {
    if (undo_stack.empty()) {
      return;
    }
    // restore S to the state it was in prior to the last operation by popping the stack
    S = undo_stack.back();
    undo_stack.pop_back();
  };

  /* Enter your code here. Read input from STDIN. Print output to STDOUT */
  auto Q = 0;
  cin >> Q;
  vector<string> undo_stack;
  string S; // the initial string
  for (auto line = 0; line < Q; ++line) {
    auto op = 0;
    cin >> op;
    switch ((Operation)op) {
    case Append: {
      // read W to be appended
      string W;
      cin >> W;
      // append W to S
      do_append(S, W, undo_stack);

    } break;
    case Delete: {
      // read k
      auto k = 0;
      cin >> k;
      // delete the last k characters of S
      do_delete(S, k, undo_stack);

    } break;
    case Print: {
      // read k
      auto k = 0;
      cin >> k;
      // print the k-th character of S
      do_print(S, k);

    } break;
    case Undo: {
      do_undo(S, undo_stack);
    } break;

    default:
      break;
    }
  }
  return 0;
}
