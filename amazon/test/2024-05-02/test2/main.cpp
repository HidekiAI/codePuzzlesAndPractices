// Amazon has multiple delivery centers and delivery warehouses all over the world.  The world
// is represented by a number line from -10^9 to 10^9.  There are n delivery centers, the ith
// one at location center[i].  A location x is called a suitable location for a warehouse if it is possibel to bring
// all the products to that point by traveling a distance of no more than d.  At any one time, products can be
// brought from  one delivercy center and placed at point x.  Given the position of n delivery centers,
// calculate the number of suitable locations in the world.  That is, calculate the number of points x
// on the number line (-10^9 <= x <= 10^9) wher the travel distance required to bring all the products to that point is
// is less than or equal to do.
// Note: The distance between point x and center[i] is |x - center[i]|, their absolute difference.

// Sample Case 0
//   STDIN       Function
//   -----       --------
//   4          center[] size n = 4
//   2     center = [2, 0, 3, -4]
//   0
//   3
//   -4
//   22         d = 22
// Sample output:
//   5
// Explanation:
// There are 5 suitable locations, i.e. {-1, 0, 1, 2, 3}.
// - place a wherehouse at x=-1, total distance traveled is 2 * |-1 - 2| + 2 * | -1 - 3| + 2 * |-1 - (-4)| = 22 <= d
// - x = 0, total distance traveled is 2 * |0 - 2| + 2 * |0 - 0| + 2 * |0 - 3| + 2 * |0 - (-4)| = 18 <= d
// - x = 1, total distance traveled is 2 * |1 - 2| + 2 * |1 - 0| + 2 * |1 - 3| + 2 * |1 - (-4)| = 18 <= d
// - x = 2, total distance traveled is 2 * |2 - 2| + 2 * |2 - 0| + 2 * |2 - 3| + 2 * |2 - (-4)| = 18 <= d   
// - x = 3, total distance traveled is 2 * |3 - 2| + 2 * |3 - 0| + 2 * |3 - 3| + 2 * |3 - (-4)| = 22 <= d   

#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);



/*
 * Complete the 'suitableLocations' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY center
 *  2. LONG_INTEGER d
 */

int suitableLocations(vector<int> center, long d) {
    auto dmin = -1000000000;
    auto dmax = 1000000000;
    
    int count = 0;
    for(auto i = dmin; i <= dmax; ++i) {
        auto totalD = 0;
        for(auto j = 0; j < center.size(); ++j)
        {
            totalD += 2 * abs(i - center[j]);
        }
        if (totalD <= d) {
            ++count;
        }
    }
    return count;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string center_count_temp;
    getline(cin, center_count_temp);

    int center_count = stoi(ltrim(rtrim(center_count_temp)));

    vector<int> center(center_count);

    for (int i = 0; i < center_count; i++) {
        string center_item_temp;
        getline(cin, center_item_temp);

        int center_item = stoi(ltrim(rtrim(center_item_temp)));

        center[i] = center_item;
    }

    string d_temp;
    getline(cin, d_temp);

    long d = stol(ltrim(rtrim(d_temp)));

    int result = suitableLocations(center, d);

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
