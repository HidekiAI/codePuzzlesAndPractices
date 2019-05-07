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
/*
John works at a clothing store. He has a large pile of socks that he must pair 
by color for sale. Given an array of integers representing the color of each 
sock, determine how many pairs of socks with matching colors there are.

For example, there are n=7 socks with colors ar=[1,2,1,2,1,3,2]. There is one pair of color 1 and one 
of color 2. There are three odd socks left, one of each color. The number of pairs is 2.

Function Description

Complete the sockMerchant function in the editor below. It must return an integer 
representing the number of matching pairs of socks that are available.

sockMerchant has the following parameter(s):

* n: the number of socks in the pile
* ar: the colors of each sock

Input Format

The first line contains an integer n, the number of socks represented in ar. 
The second line contains n space-separated integers describing the colors ar[i] of the 
socks in the pile.

Constraints
* 1 <= n <= 100
* 1 <= ar[i] <= 100 where0 <= i < n

Output Format

Return the total number of matching pairs of socks that John can sell.

Sample Input

9
10 20 20 10 10 30 50 10 20

Sample Output

3
Explanation

sock.png

John can match three pairs of socks.
*/



class Solution
{

    // Complete the sockMerchant function below.
    static int sockMerchant(int n, int[] ar)
    {
        // Expensive way with Tuple.Create:
        var sums1 = ar.GroupBy(s => s)
            .Select(g => Tuple.Create(g.Key, g.Count() / 2));
        var sum = 0;
        foreach (var g in sums1)
        {
            sum += g.Item2;
        }

        // Correct way:
        var sums = ar.GroupBy(s => s)
            .Select(g => g.Count() / 2);
        sum = 0;
        foreach (var g in sums)
        {
            sum += g;
        }
        return sum;
    }

    static void Main(string[] args)
    {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int n = Convert.ToInt32(Console.ReadLine());

        int[] ar = Array.ConvertAll(Console.ReadLine().Trim().Split(' '), arTemp => Convert.ToInt32(arTemp));
        //        textWriter.WriteLine($"#Begin - arr.Len={ar.Count()} (Expected:{n}), '{(string.Join(",", ar))}'");
        //        var zzz = ar.GroupBy(s => s)
        //                    .Select(g =>
        //                    {
        //                        textWriter.WriteLine($"#1-Key={g.Key}, Array={(string.Join(",", g.Select(v => v)))}");
        //                        return g;
        //                    }).Select(g => Tuple.Create(g.Key, g.Count(), g.Count() / 2))
        //                    .Select(g =>
        //                    {
        //                        //textWriter.WriteLine($"#2-Key={g.Item1}, Count={g.Item2}, IsEven={g.Item3}");
        //                        return g;
        //                    }).Where(g => g.Item3 > 0)
        //                    .Select(g =>
        //                    {
        //                        textWriter.WriteLine($"#3-Key={g.Item1}, Count={g.Item2}, IsEven={g.Item3}");
        //                        return g;
        //                    })
        //                ;
        //        if (zzz.Count() > 0)
        //        {
        //            textWriter.WriteLine($"Count()={zzz.Count()}");
        //        }
        //        textWriter.WriteLine("#End");

        int result = sockMerchant(n, ar);

        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}
