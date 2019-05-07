/*
Gary is an avid hiker. He tracks his hikes meticulously, paying close attention to small details like topography. During his last hike he took exactly  steps. For every step he took, he noted if it was an uphill, , or a downhill,  step. Gary's hikes start and end at sea level and each step up or down represents a  unit change in altitude. We define the following terms:

A mountain is a sequence of consecutive steps above sea level, starting with a step up from sea level and ending with a step down to sea level.
A valley is a sequence of consecutive steps below sea level, starting with a step down from sea level and ending with a step up to sea level.
Given Gary's sequence of up and down steps during his last hike, find and print the number of valleys he walked through.

For example, if Gary's path is , he first enters a valley  units deep. Then he climbs out an up onto a mountain  units high. Finally, he returns to sea level and ends his hike.

Function Description

Complete the countingValleys function in the editor below. It must return an integer that denotes the number of valleys Gary traversed.

countingValleys has the following parameter(s):

n: the number of steps Gary takes
s: a string describing his path
Input Format

The first line contains an integer , the number of steps in Gary's hike. 
The second line contains a single string , of  characters that describe his path.

Constraints

Output Format

Print a single integer that denotes the number of valleys Gary walked through during his hike.

Sample Input

8
UDDDUDUU
Sample Output

1
Explanation

If we represent _ as sea level, a step up as /, and a step down as \, Gary's hike can be drawn as:

_/\      _
   \    /
    \/\/
He enters and leaves one valley.



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

class Solution
{

    // Complete the countingValleys function below.
    static int countingValleys(int n, string s)
    {
        // By here, we've either ended the journey, or have dropped below sea level
        // to begin counting valleys; we monitor the first "U" we'd encounter BELOW sea level
        // meaning, it doesn't count if it turns into a mountain instead of a valley.
        var currentLevel = 0;   // assume we start at sea level always
        var step = 0;
        // So to detect that, after we encounter the sequences of "U"'s, adjust
        // the sea level, and soon as we encounter the first "D" AND sea level
        // is below, count that as a valley (valley level), and reset IF we're
        // back to the same level that we've started the survey from
        var valleys = 0;
        var startSurvey = false;
		var sawValley = false;
        // 12
        // DDUUDDUDUUUD
        // out: 2
        // 10
        // DDUUDDUUDUU
        // out: 1
        // 10
        // DUDDDUUDUU
        // out: 2
        // _           _
        //   \/\      /
        //      \  /\/
        //       \/
        // 10
        // DDUDDUUDUU
        // out: 1
        // _          _
        //  \        /
        //   \/\  /\/
        //      \/
System.Console.WriteLine($"n={n}, s='{s}'");
        while (step < n)
        {
            var myStep = s[step] == 'U' || s[step] == 'u' ? 1 : -1;
            currentLevel += myStep;
System.Console.WriteLine($"\t{currentLevel}: {s[step]}({myStep}) - ss={startSurvey}, sv={sawValley}, vc={valleys}");
            if (currentLevel <= 0)   // we're only interested if we're below sea level
            {
                // we're below sea level, see if we're descending, if so, it's
                // either to reach back to where the valley began, or
                // we've not yet started the valley.
				if ((currentLevel == 0) && (myStep == 1))
				{
System.Console.WriteLine($"\t\t{currentLevel}: {s[step]}({myStep}) - ss={startSurvey}, sv={sawValley}, vc={valleys}");
                    ++valleys;
					sawValley = true;
				}
                // We care about whether we're going to mark the completion
                // of the valley discovery, or whether we've not yet discovered
                // a new valley.  To determine, we observe if startSurvey==true
                // and if so, soon as we match the starting valley level,
                // we mark as valley found.
				else if ((myStep == -1) && sawValley)
                {
                    // we've reached the other end of the valley, so mark it
                    startSurvey = false; // we're done with this survey
					sawValley = false;
                }
                // if we've not yet started survey, and we're descending, we do
                // nothing.  If we were descending, and we're not about to ascend,
                // then as long as we're below sea level, let's begin our
                // survey.
                else if ((myStep == 1) && !startSurvey) // survey hasn't started, but we're ascending and we're below sea level
                {
                    startSurvey = true;
					sawValley = false;
                }
            }
            else
            {
                // we're above the sea level, if we thought we had a valley,
                // we were wrong, so stop surveying and wait to go below sea
                // level again
                startSurvey = false;    // this became a mountaing
				sawValley = false;
            }
            ++step;
        }
        return valleys;
    }

    static void Main(string[] args)
    {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);
/*
8
UDDDUDUU
Sample Output
out: 1
_/\      _
   \    /
    \/\/
10
DUDDDUUDUU
out: 2
_          _
 \/\      /
    \  /\/
	 \/

*/
//        int n = Convert.ToInt32(Console.ReadLine());
//        string s = Console.ReadLine();
        	var n = 10;
			string s = "DUDDDUUDUU"; 
        int result = countingValleys(n, s);
if(result != 2)
{
		throw new Exception();
}
        	n = 8;
			s = "UDDDUDUU"; 
        result = countingValleys(n, s);
if(result != 1)
{
		throw new Exception();
}
        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}
