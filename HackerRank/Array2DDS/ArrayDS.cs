/*
Given a  2D Array, :

1 1 1 0 0 0
0 1 0 0 0 0
1 1 1 0 0 0
0 0 0 0 0 0
0 0 0 0 0 0
0 0 0 0 0 0
We define an hourglass in  to be a subset of values with indices falling in this pattern in 's graphical representation:

a b c
  d
e f g
There are  hourglasses in , and an hourglass sum is the sum of an hourglass' values. Calculate the hourglass sum for every hourglass in , then print the maximum hourglass sum.

For example, given the 2D array:

-9 -9 -9  1 1 1 
 0 -9  0  4 3 2
-9 -9 -9  1 2 3
 0  0  8  6 6 0
 0  0  0 -2 0 0
 0  0  1  2 4 0
We calculate the following  hourglass values:

-63, -34, -9, 12, 
-10, 0, 28, 23, 
-27, -11, -2, 10, 
9, 17, 25, 18
Our highest hourglass value is  from the hourglass:

0 4 3
  1
8 6 6
Note: If you have already solved the Java domain's Java 2D Array challenge, you may wish to skip this challenge.

Function Description

Complete the function hourglassSum in the editor below. It should return an integer, the maximum hourglass sum in the array.

hourglassSum has the following parameter(s):

arr: an array of integers
Input Format

Each of the  lines of inputs  contains  space-separated integers .

Constraints

Output Format

Print the largest (maximum) hourglass sum found in .

Sample Input

1 1 1 0 0 0
0 1 0 0 0 0
1 1 1 0 0 0
0 0 2 4 4 0
0 0 0 2 0 0
0 0 1 2 4 0
Sample Output

19
Explanation

 contains the following hourglasses:

image
The hourglass with the maximum sum () is:

2 4 4
  2
1 2 4
*/
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text.RegularExpressions;
using System.Text;
using System;

class Solution {

    // Complete the hourglassSum function below.
    static int hourglassSum(int[][] arr) {
		var maxSum = Int32.MinValue;
		var first = true;
		// whether traversed from to or bottom, need to be aware of the boundaries/edges
		// on an NxN, first is always at [0][0], and last is at [N-3][N-3] for the top
		// left corners
		// Pattern to search (offset):
		// [0][0], [0][1], [0][2], [1][1], [2][0], [2][1], [2][2]
		var hourGlass = new Tuple<int, int>[]
		{
					Tuple.Create(0, 0),
					Tuple.Create(0, 1),
					Tuple.Create(0, 2),
					Tuple.Create(1, 1),
					Tuple.Create(2, 0),
					Tuple.Create(2, 1),
					Tuple.Create(2, 2)
		};
		var N = arr.Count();
		for(var row = 0; row <= N - 3; ++row)
		{
			for(var col = 0; col <= N - 3; ++col)
			{
				var sum = 0;
				foreach(var offset in hourGlass)
				{
					sum += arr[row + offset.Item1][col + offset.Item2];
				}
Console.Write($"{sum},");				
				if ((maxSum < sum) || first)
				{
					maxSum = sum;
					first = false;
				}
			}
		}
Console.WriteLine($"\nMax: {maxSum}");		
		return maxSum;
    }

    static void Main(string[] args) {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int[][] arr = new int[6][];

        for (int i = 0; i < 6; i++) {
            arr[i] = Array.ConvertAll(Console.ReadLine().Split(' '), arrTemp => Convert.ToInt32(arrTemp));
        }

        int result = hourglassSum(arr);

        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}
