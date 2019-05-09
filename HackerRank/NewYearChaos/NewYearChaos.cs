//It's New Year's Day and everyone's in line for the Wonderland rollercoaster ride! There are a number of people queued up, and each person wears a sticker indicating their initial position in the queue. Initial positions increment by  from  at the front of the line to  at the back.
//
//Any person in the queue can bribe the person directly in front of them to swap positions. If two people swap positions, they still wear the same sticker denoting their original places in line. One person can bribe at most two others. For example, if  and  bribes , the queue will look like this: .
//
//Fascinated by this chaotic queue, you decide you must know the minimum number of bribes that took place to get the queue into its current state!
//
//Function Description
//
//Complete the function minimumBribes in the editor below. It must print an integer representing the minimum number of bribes necessary, or Too chaotic if the line configuration is not possible.
//
//minimumBribes has the following parameter(s):
//
//q: an array of integers
//Input Format
//
//The first line contains an integer , the number of test cases.
//
//Each of the next  pairs of lines are as follows: 
//- The first line contains an integer , the number of people in the queue 
//- The second line has  space-separated integers describing the final state of the queue.
//
//Constraints
//
//Subtasks
//
//For  score 
//For  score 
//
//Output Format
//
//Print an integer denoting the minimum number of bribes needed to get the queue into its final state. Print Too chaotic if the state is invalid, i.e. it requires a person to have bribed more than  people.
//
//Sample Input
//
//2
//5
//2 1 5 3 4
//5
//2 5 1 3 4
//Sample Output
//
//3
//Too chaotic
//Explanation
//
//Test Case 1
//
//The initial state:
//
//pic1(1).png
//
//After person  moves one position aheadIndex by bribing person :
//
//pic2.png
//
//Now person  moves another position ahead by bribing person :
//
//pic3.png
//
//And person  moves one position ahead by bribing person :
//
//pic5.png
//
//So the final state is  after three bribing operations.
//
//Test Case 2
//
//No person can bribe more than two people, so its not possible to achieve the input state.

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
    static bool testParts(ref int[] q, ref int[] bribeCounts, int headIndex, int tailIndex)
    {
        var maxBribesPerPerson = 2;
//Console.WriteLine($"\tEvaluate: {string.Join(", ", q.Skip(headIndex).Take((tailIndex + 1) - headIndex))} Head:{headIndex}:Tail:{tailIndex}");
        bool visitedAll = false;
		while(visitedAll == false)
		{
            for(var qIndex = tailIndex; qIndex > headIndex; --qIndex)	// traverse backwards
            {
//Console.WriteLine($"## Index:{qIndex}, {q[qIndex]} vs {q[qIndex - 1]} -> diff:{q[qIndex] - q[qIndex - 1]}");
                // 2, 1, 5, 3, 4
                // 4 > 3 - skip
                // 3 < 5 - swap 5:+1
                // 2, 1, 3, 5, 4
                // 4 < 5 - swap 5:+2
                // 2, 1, 3, 4, 5
                // 5 > 4, 4 > 3, 3 > 1
                // 1 < 2 - swap 2:+1
                // 1, 2, 3, 4, 5 - done
                if (q[qIndex] > q[qIndex-1])	// i==3, q[3]==3, q[3-1]==q[2]==5, is 3 > 5? No
                {
                    // we're in order
                    visitedAll = qIndex == (headIndex + 1); // head is 1 indexed, never 0!
                    continue;
                }
                // if here, q[qIndex-1] < q[qIndex], swap
                ++bribeCounts[q[qIndex - 1] - 1];
//Console.WriteLine($"\t\tSwap: {q[qIndex]} with {q[qIndex-1]} at index={qIndex}, bribeCount={bribeCounts[q[qIndex]]}");
                if (bribeCounts[q[qIndex - 1] - 1] > maxBribesPerPerson)
                {
                    return false;
                }

                // swap (xor in-place)
                q[qIndex-1] = q[qIndex-1] ^ q[qIndex];
                q[qIndex]   = q[qIndex-1] ^ q[qIndex];
                q[qIndex-1] = q[qIndex-1] ^ q[qIndex];
//Console.WriteLine($"\t\t\tQ: {string.Join(", ", q)}");
//Console.WriteLine($"\t\t\tBribes: {string.Join(", ", bribeCounts)}");
                break;	// restart from end again
            }
        }
//Console.WriteLine($"#{string.Join(",", q)}");
        return visitedAll;
    }

	// Complete the minimumBribes function below.
	static void minimumBribes(int[] q) {
//Console.WriteLine($"\n---------\n# New test: {string.Join(",", q)}");
		// Each value has a weight (distance) to it, in which
		// should be at the most 2; but depending on what goes
		// first, there are chances where it can shift by 3 or
		// more.  The weights are positive it needs go left
		// and negative if right.
		//var distance = q.Select((v, i) => Tuple.Create(v, ((i + 1) - v)))
		//		.ToDictionary(tup => tup.Item1, tup => tup.Item2);
		// It makes NO SENSE to bribe somebody so that s/he can
		// go towards the end; means that one would assume if
		// larger number is found, it had to have brided the
		// one in front.
		// For example, if you have 2,1,3,4,5 then 2 bribed
		// 1, not the other way around.
		// Brute force method, you walk from left to right,
		// and soon as you meet somebody who is greater than
		// the one behind, begin shifting until maxBribeCount
		var bribeCounts = new int[q.Count()];	// init all to 0 (default(int) == 0)
        // I believe new c#8 has slices, but assume not all compiler supports it yet
        // emulate slices by headIndex/tailIndexIndex indicies
        var inSeq = false;
        var chaos = false;
        var parts = q.Count() / 1024 == 0 ? q.Count() : q.Count() / 1024;   // index will never by more than 31-bits!
//parts = q.Count() / 2;  // for debug purpose!
        do
        {
            var headTailIndex = new List<Tuple<int, int>>();
            for(var headIndex = 0; headIndex < q.Count() - 1; headIndex += parts)
            {
                var tailIndex = headIndex + parts - 1;
                if (tailIndex > q.Count() - 1)
                {
                    tailIndex = q.Count() - 1;
                }
//Console.WriteLine($"\t\tCurrent: head={headIndex}, tail={tailIndex} (parts={parts}, qCount={q.Count()})");
                if (headIndex == tailIndex)
                {
                    var last = headTailIndex.Last();
                    headIndex = headIndex - parts >= 0 ? headIndex - parts : 0;
                    if (last != null)
                    {
                        headTailIndex = headTailIndex.Take(headTailIndex.Count() - 1).ToList();  // if take is 0 or negative, returns empty list
                        headIndex = last.Item1;
                    }
                }
                headTailIndex.Add(Tuple.Create(headIndex, tailIndex));
            }
//Console.WriteLine($"# IndexList: {string.Join(", ", headTailIndex.Select(t => t.Item1+":"+t.Item2))}");
            // fork and join
            chaos = false;
            object chaosMutex = new object();
            System.Threading.Tasks.Parallel.ForEach(headTailIndex, (index) =>
            {
                var headIndex = index.Item1;
                var tailIndexIndex = index.Item2;
                var tested = testParts(ref q, ref bribeCounts, headIndex, tailIndexIndex);
                if (tested == false)
                {
                    lock(chaosMutex)    // mainly so we don't print it more than once
                    {
                        if (!chaos)
                        {
                            Console.WriteLine("Too chaotic");
                        }
                    }
                    chaos = true;
                    return;
                }
            });
            // Join
            inSeq = true;
            for(var qI = 0; qI < q.Count() - 1; ++qI)
            {
                if (q[qI] > q[qI+1])
                {
                    inSeq = false;
                    parts *= 2; // make partition bigger until it's the whole array
                    if(parts > q.Count())
                    {
                        parts = q.Count();
                    }
                    break;
                }
            }
        } while((inSeq == false) && (chaos == false));
        if(!chaos)
        {
		    var sum = bribeCounts.Sum();
		    Console.WriteLine(sum);
        }
	}

	static void Main(string[] args) {
		int t = Convert.ToInt32(Console.ReadLine());

		for (int tItr = 0; tItr < t; tItr++) {
			int n = Convert.ToInt32(Console.ReadLine());

			int[] q = Array.ConvertAll(Console.ReadLine().Split(' '), qTemp => Convert.ToInt32(qTemp))
					;
			minimumBribes(q);
		}
	}
}

