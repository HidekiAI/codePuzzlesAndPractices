
//In this challenge, the task is to debug the existing code to successfully execute all provided test files.
//
//Given an array of  distinct integers, transform the array into a zig zag sequence by permuting the array elements. A sequence will be called a zig zag sequence if the first  elements in the sequence are in increasing order and the last  elements are in decreasing order, where . You need to find the lexicographically smallest zig zag sequence of the given array.
//
//Example.
//
//
//Now if we permute the array as , the result is a zig zag sequence.
//
//Debug the given function findZigZagSequence to return the appropriate zig zag sequence for the given input array.
//
//Note: You can modify at most three lines in the given code. You cannot add or remove lines of code.
//
//To restore the original code, click on the icon to the right of the language selector.
//
//Input Format
//
//The first line contains  the number of test cases. The first line of each test case contains an integer , denoting the number of array elements. The next line of the test case contains  elements of array .
//
//Constraints
//
//
// ( is always odd)
//
//Output Format
//
//For each test cases, print the elements of the transformed zig zag sequence in a single line.



// WARNING: TEST WILL FAIL if you ADD or REMOVE lines of code!
#include <bits/stdc++.h>
using namespace std;

void findZigZagSequence(vector < int > a, int n){
    sort(a.begin(), a.end());
    int mid = ((n + 1)/2) - 1;  // added -1
    swap(a[mid], a[n-1]);

    int st = mid + 1;
    int ed = n - 2; // was -1
    while(st <= ed){
        swap(a[st], a[ed]);
        st = st + 1;
        ed = ed - 1;    // was +1
    }
    for(int i = 0; i < n; i++){
        if(i > 0) cout << " ";
        cout << a[i];
    }
    cout << endl;
}

int main() {
    int n, x;
    int test_cases;
    cin >> test_cases;

    for(int cs = 1; cs <= test_cases; cs++){
        cin >> n;
        vector < int > a;
        for(int i = 0; i < n; i++){
            cin >> x;
            a.push_back(x);
        }
        findZigZagSequence(a, n);
    }
}



