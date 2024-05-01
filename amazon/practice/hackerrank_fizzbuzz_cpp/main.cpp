#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);



/*
 * Complete the 'fizzBuzz' function below.
 *
 * The function accepts INTEGER n as parameter.
 */

void fizzBuzz(int n) {
    // iterate from 1 to n (inclusive)
    for(auto i = 1; i <= n; i++) {
        // check if i is divisible by 3 and 5
        if(i % 3 == 0 && i % 5 == 0) {
            cout << "FizzBuzz" << endl;
        }
        // check if i is divisible by 3
        else if(i % 3 == 0) {
            cout << "Fizz" << endl;
        }
        // check if i is divisible by 5
        else if(i % 5 == 0) {
            cout << "Buzz" << endl;
        }
        // print i
        else {
            cout << i << endl;
        }
    }
}

int main()
{
    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    fizzBuzz(n);

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
