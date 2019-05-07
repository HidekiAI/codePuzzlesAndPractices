// Lilah has a string, , of lowercase English letters that she repeated infinitely many times.
//
// Given an integer, , find and print the number of letter a's in the first  letters of Lilah's infinite string.
//
// For example, if the string  and , the substring we consider is , the first  characters of her infinite string. There are  occurrences of a in the substring.
//
// Function Description
//
// Complete the repeatedString function in the editor below. It should return an integer representing the number of occurrences of a in the prefix of length  in the infinitely repeating string.
//
// repeatedString has the following parameter(s):
//
// s: a string to repeat
// n: the number of characters to consider
// Input Format
//
// The first line contains a single string, . 
// The second line contains an integer, .
//
// Constraints
//
// For  of the test cases, .
// Output Format
//
// Print a single integer denoting the number of letter a's in the first  letters of the infinite string created by repeating infinitely many times.
//
// Sample Input 0
//
// aba
// 10
// Sample Output 0
//
// 7
// Explanation 0 
// The first  letters of the infinite string are abaabaabaa. Because there are  a's, we print  on a new line.
//
// Sample Input 1
//
// a
// 1000000000000
// Sample Output 1
//
// 1000000000000
// Explanation 1 
// Because all of the first n=1000000000000 letters of the infinite string are a, we print 1000000000000 on a new line.

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

class Solution
{

    // Complete the repeatedString function below.
    static long repeatedString(string s, long n)
    {
        var aCountInS = s.Where(l => l == 'a').Count();
		var fit = n / s.Count();
		var lastSubString = (n % s.Count());
		var aCountInLast = s.Substring(0, (int)lastSubString)
				.Where(l => l == 'a')
				.Count();
Console.WriteLine($"s='{s}', n={n}, fit={n}/{s.Count()}={fit}, last={n}%{s.Count()}[{n%s.Count()}]={lastSubString}, sub={s.Substring(0, (int)lastSubString)}, count={aCountInLast}, +{fit*aCountInS} = {(fit * aCountInS) + aCountInLast}");
		return (fit * aCountInS) + aCountInLast;
    }

    static void Main(string[] args)
    {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        string s = Console.ReadLine();

        long n = Convert.ToInt64(Console.ReadLine());

        long result = repeatedString(s, n);

        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}
