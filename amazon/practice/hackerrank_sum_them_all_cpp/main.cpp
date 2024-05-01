// Calculate the sume of an array of integers

// Example
// numbers = [3, 13, 4, 11, 9]

// The sume is 3 + 13 + 4 + 11 + 9 = 40

// Function Description
// Complete the function arraySum in the editor below.

// arraySum has the following parameter(s):
//     int numbers[n]: an array of integers

// Returns
//     int: an integer that represents the sum of the elements in the array

// Constraints
// 1 <= n <= 10^4
// 1 <= numbers[i] <= 10^4

// Input Format For Custom Testing
// Input from stdin will be processed as follows and passed to the function.
// The first line contains an integer, n, that represents the number of elements in the array.
// The next n lines each contain an integer, numbers[i], where 0 <= i < n.

// Sample Case 0
// Sample Input 0
// STDIN    Function
// -----    --------
// 5        numbers[] size n = 5
// 1        numbers = [1, 2, 3, 4, 5]
// 2
// 3
// 4
// 5
// 
// Sample Output 0
// 15
//
// Explanation 0
// The sum of the elements in the array is 1 + 2 + 3 + 4 + 5 = 15
// 
// Sample Case 1
// Sample Input 1
// STDIN    Function
// -----    --------
// 2        numbers[] size n = 2
// 12       numbers = [12, 12]
// 12
// 
// Sample Output 1
// 24
// 
// Explanation 1
// The sum of the elements in the array is 12 + 12 = 24


#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'arraySum' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY numbers as parameter.
 */

int arraySum(vector<int> numbers) {
    auto sum = 0;
    for(auto i = 0; i < numbers.size(); i++) {
        sum += numbers[i];
    }
    return sum;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string numbers_count_temp;
    getline(cin, numbers_count_temp);

    int numbers_count = stoi(ltrim(rtrim(numbers_count_temp)));

    vector<int> numbers(numbers_count);

    for (int i = 0; i < numbers_count; i++) {
        string numbers_item_temp;
        getline(cin, numbers_item_temp);

        int numbers_item = stoi(ltrim(rtrim(numbers_item_temp)));

        numbers[i] = numbers_item;
    }

    int result = arraySum(numbers);

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
