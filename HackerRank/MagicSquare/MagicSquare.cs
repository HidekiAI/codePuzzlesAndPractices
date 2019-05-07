// We define a magic square to be an  matrix of distinct positive integers from 1 to n^2 where the sum of any row,
// column, or diagonal of length n is always equal to the same number: the magic constant.
//
// You will be given a 3x3  matrix s of integers in the inclusive range [1,9]. We can convert any digit a to any other digit b in
// the range [1,9] at cost of |a - b|. Given s, convert it into a magic square at minimal cost. Print this cost on a new line.
//
// Note: The resulting magic square must contain distinct integers in the inclusive range [1,9].
//
// For example, we start with the following matrix :
//
// 5 3 4
// 1 5 8
// 6 4 2
// We can convert it to the following magic square:
//
// 8 3 4
// 1 5 9
// 6 7 2
// This took three replacements at a cost of |5-8| + |8-9| + |4-7| = 7.
//
// Function Description
//
// Complete the formingMagicSquare function in the editor below. It should return an integer that represents the minimal total
// cost of converting the input square to a magic square.
//
// formingMagicSquare has the following parameter(s):
// * s: a  array of integers
//
// Input Format
//
// Each of the lines contains three space-separated integers of row s[i].
//
// Constraints
//  * s[i][j] e [1,9]
//
// Output Format
//
// Print an integer denoting the minimum cost of turning matrix s into a magic square.
//
// Sample Input 0
// 4 9 2
// 3 5 7
// 8 1 5
// Sample Output 0
// 1
// Explanation 0
// If we change the bottom right value, s[2][2], from 5 to 6 at a cost of |6-5|=1, s becomes a magic square at the
// minimum possible cost.
// 4 9 2
// 3 5 7
// 8 1 6
//
// Sample Input 1
// 4 8 2
// 4 5 7
// 6 1 6
// Sample Output 1
// 4
// Explanation 1
// Using 0-based indexing, if we make
// * s[0][1]->9 at a cost of |9-8|=1
// * s[1][0]->3 at a cost of |3-4|=1
// * s[2][0]->8 at a cost of |8-6|=2,
// then the total cost will be 1+1+2=4.
// 4 9 2
// 3 5 7
// 8 1 6
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
	//static int calcMagicSum(int dimension)
	//{
	//	// From Wikipedia: "Magic constant"
	//	// M = n(n^2 + 1) / 2
	//	// i.e. n=3
	//	//   = 3 * (3^2 + 1) / 2 = 3 * 10 / 2 = 30 / 2 = 15
	//	return (dimension * ((dimension * dimension) + 1)) / 2;
	//}

	// From Wikipedia:
	// 1 is at the bottom and 2 is in the the upper right corner.  Every normaal magic square of order three is
	// obtained from the Lo Shu by rotation and reflection.
	// Odd and even numbers alternate in the periphery of the Lo Shu pattern; the 4
	// even numbers are at the fou corners, and the 5 odd numbers (outnumbering the even numbers by one)
	// from a cross in the center of the square.  Since 5 is in the center cell,
	// the sum of any two other cells that are directly through the 5 from each other is 10 (9+1 and 3+7).
	// 4 9 2
	// 3 5 7
	// 8 1 6
	// Excluding rotations and reflections, there is EXACTLY one 3x3 magic square, exactly 880 4x4 squares, etc...
	static int[][] getLuoShuSquare()
	{
		return new int[][]{
			new int[] {4, 9, 2},	// all corners are even numbers and the diagnal sums to 10 + 5 (4+6 and 2+8)
			new int[] {3, 5, 7},	// 5 is always at the center, and sum on cross is 10 (9+1 and 3+7)
			new int[] {8, 1, 6}
		};
	}
	//static int getLuoShuAdjacent(int num)
	//{
	//	if (num <= 0 || num >= 10)
	//	{
	//		return 0;
	//	}
	//	// it's easy to provide a map of adjacent values, i.e. if 4, return 6, if 9, return 1
	//	// but for a Luo Shu, it's always 10 (including middle 5) is the sum as long as
	//	// it is from the assumed position.  For example, if 2 is looked up, we know:
	//	// * because it's even number, it's at the corner
	//	// * The adjacent is not vertical (6) or horizontal (4), it's diagnal
	//	// If 9 is inquired:
	//	// * It's an odd number, so adjacent is vertical (or horizontal, depending on rotations)
	//	// * Its adjacent is always going to form the sequence of 9,5,1
	//	return 10 - num;
	//}

	static int[][] rotateLuoShu(int[][] s)
	{
		return new int[][] {
						new int[] {s[2][0], s[1][0], s[0][0]},
						new int[] {s[2][1], s[1][1], s[0][1]},	// not that center never changes
						new int[] {s[2][2], s[1][2], s[0][2]}
		};
	}

	static Tuple<int[][], int> diffLuoShu(int[][] s)
	{
		var ls = getLuoShuSquare();	// const/static?
		var absDiffs = new int[][]{
				new int[] { Math.Abs(ls[0][0] - s[0][0]), Math.Abs(ls[0][1] - s[0][1]), Math.Abs(ls[0][2] - s[0][2]) },
				new int[] { Math.Abs(ls[1][0] - s[1][0]), Math.Abs(ls[1][1] - s[1][1]), Math.Abs(ls[1][2] - s[1][2]) },
				new int[] { Math.Abs(ls[2][0] - s[2][0]), Math.Abs(ls[2][1] - s[2][1]), Math.Abs(ls[2][2] - s[2][2]) },
			};
		var sum = 0;
		for(var i = 0; i < 3 ; ++i)
		{
			for(var j = 0; j < 3; ++j)
			{
				sum += absDiffs[i][j];
			}
		}
Console.WriteLine($"\t{absDiffs[0][0]} {absDiffs[0][1]} {absDiffs[0][2]}");
Console.WriteLine($"\t{absDiffs[1][0]} {absDiffs[1][1]} {absDiffs[1][2]}");
Console.WriteLine($"\t{absDiffs[2][0]} {absDiffs[2][1]} {absDiffs[2][2]}\n");
Console.WriteLine($"\t\tSum: {sum}");
		return Tuple.Create(absDiffs, sum);
	}

	// Rotate 3 times to determine which has the smallest diffs, and assume that to be the newest array
	// Fortunately, the problem only wants the smallest diff, so we do not need to track by how much to
	// unwind back to origianl orientations.
	static Tuple<int[][], int> reorient(int[][] original)
	{
		var s1 = rotateLuoShu(original);
		var s2 = rotateLuoShu(s1);
		var s3 = rotateLuoShu(s2);
		// there's only going to be ONE correct rotation
		return (new Tuple<int[][], int>[] {
				diffLuoShu(original),
				diffLuoShu(s1),
				diffLuoShu(s2),
				diffLuoShu(s3)
			}).OrderBy(d => d.Item2)
			.First();
	}

	//----------------------------------------------------------------------------------------------

	// Complete the formingMagicSquare function below.
	static int formingMagicSquare(int[][] s) {
		var retTotalDiffs = 0;
		// NOTE: Assumes it's always 3x3 and we should be able to take the path of Luo Shu's magic square approach
Console.WriteLine($"{s[0][0]} {s[0][1]} {s[0][2]}");
Console.WriteLine($"{s[1][0]} {s[1][1]} {s[1][2]}");
Console.WriteLine($"{s[2][0]} {s[2][1]} {s[2][2]}\n\n");
		var newS = reorient(s);
		return newS.Item2;
	}

	static void Main(string[] args) {
		TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

		int[][] s = new int[3][];

		for (int i = 0; i < 3; i++) {
			s[i] = Array.ConvertAll(Console.ReadLine().Split(' '), sTemp => Convert.ToInt32(sTemp));
		}

		int result = formingMagicSquare(s);

		textWriter.WriteLine(result);

		textWriter.Flush();
		textWriter.Close();
	}
}
