﻿//We define the distance between two array values as the number of indices between the two values. Given , find the minimum distance between any pair of equal elements in the array. If no such value exists, print .
//
//For example, if , there are two matching pairs of values: . The indices of the 's are  and , so their distance is . The indices of the 's are  and , so their distance is .
//
//Function Description
//
//Complete the minimumDistances function in the editor below. It should return the minimum distance between any two matching elements.
//
//minimumDistances has the following parameter(s):
//
//a: an array of integers
//Input Format
//
//The first line contains an integer , the size of array . 
//The second line contains  space-separated integers .
//
//Constraints
//
//Output Format
//
//Print a single integer denoting the minimum  in . If no such value exists, print .
//
//Sample Input
//
//6
//7 1 3 4 1 7
//Sample Output
//
//3
//Explanation 
//Here, we have two options:
//
// and  are both , so .
// and  are both , so .
//The answer is .
//
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

    // Complete the minimumDistances function below.
    static int minimumDistances(int[] a) {


    }

    static void Main(string[] args) {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int n = Convert.ToInt32(Console.ReadLine());

        int[] a = Array.ConvertAll(Console.ReadLine().Split(' '), aTemp => Convert.ToInt32(aTemp))
        ;
        int result = minimumDistances(a);

        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}

