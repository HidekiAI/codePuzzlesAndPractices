// KnightL is a chess piece that moves in an L shape. We define the possible moves of KnightL(a,b) as any movement 
// from some position  to some  satisfying either of the following:
//
// * x2 = x1 +/- a and y2 = y1 +/- b, or
// * x2 = x1 +/- a and y2 = y1 +/- a
//
//Note that (a,b) and (b,a)  allow for the same exact set of movements. For example, the diagram below depicts the
//possible locations that KnightL(1,2) or Knight(2,1) can move to from its current location at the center of a 5x5 chessboard:
//
//image
//
//Observe that for each possible movement, the Knight moves 2 units in one direction (i.e., horizontal or vertical) and 1 unit 
//in the perpendicular direction.
//
//Given the value of n for an nxn chessboard, answer the following question for each (a,b) pair where 1<= a,b < n:
//
// * What is the minimum number of moves it takes for Knight(a,b) to get from position (0,0) to position
//   (n-1, n-1)? If it's not possible for the Knight to reach that destination, the answer is -1 instead.
//
//Then print the answer for each KnightL(a,b) according to the Output Format specified below.
//
//Input Format
//
//A single integer denoting n.
//
//Constraints
// * 5 <= n <= 25
//
//Output Format
//
//Print exactly n-1 lines of output in which each line i (where 1 <= i < n) contains n-1 space-separated integers
//describing the minimum number of moves KnightL(i,j) must make for each respective j (where 1<=j<n). If some
//KnightL(i,j) cannot reach position (n-1,n-1), print -1 instead.
//
//For example, if n=3, we organize the answers for all the (i,j) pairs in our output like this:
//
//  (1,1) (1,2)
//  (2,1) (2,2)
//Sample Input 0
//
//  5
//
//Sample Output 0
//
//  4 4 2 8
//  4 2 4 4
//  2 4 -1 -1
//  8 4 -1 1
//Explanation 0
//
//The diagram below depicts possible minimal paths for KnightL(1,1), KnightL(1,2), and KnightL(1,3):
//
//image
//
//One minimal path for KnightL(1,4) is:
//
//  (0,0) -> (1,4) -> (2,0) -> (3,4) -> (4,0) => (0,1) -> (4,2) -> (0,3) -> (4,4)
//
//We then print 4 4 2 8 as our first line of output because KnightL(1,1) took 4 moves, KnightL(1,2) took 4 moves,
//KnightL(1,3) took 2 moves, and KnightL(1,4) took 8 moves.
//
//In some of the later rows of output, it's impossible for KnightL(i,j) to reach position (4,4). For example,
//KnightL(3,3) can only move back and forth between (0,0) and (3,3) so it will never reach (4,4).

// -----------------------------------
// For this, the hint is shortest paths, in which each cell is a node, and it's neighbors being its child.
// For example, on a 2x2 grid:
//           (0, 0)                    0,0 1,0
//          /      \                   1,0 1,1
//      D:(1,0)    R:(0,1)
//     /   \        /   \
//   D:-    R:(1,1):D   R:-
// Note that (1,0) has no D-child, but has R-child, while (0,1) has D-child and no R-child.
// The children's distance/weight is assumed to be 1 if exist, 0 otherwise, so no need to explicitly define weights
// We still need separate Edge list to understand the path?
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

    class CellNode
    {
        public CellNode(int x, int y, CellNode dChild, CellNode rChild)
        {
            X = x;
            Y - y;
            DChild = dChild;
            RChild = rChild;
        }
        public int X;
        public int Y;
        public CellNode DChild;
        public CellNode RChild;
    }
    class Edge
    {
        public Edge(CellNode e0, CellNode e1)
        {
            E0 = e0;
            E1 = e1;
        }
        public CellNode E0;
        public CellNode E1;
    }

    class Tree
    {
        CellNode Root;
        LinkedList<CellNode> DChild;
        LinkedList<CellNode> RChild;
        Edge[] Edges;
    }

    // Complete the knightlOnAChessboard function below.
    static int[][] knightlOnAChessboard(int n) {


    }

    static void Main(string[] args) {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int n = Convert.ToInt32(Console.ReadLine());

        int[][] result = knightlOnAChessboard(n);

        textWriter.WriteLine(String.Join("\n", result.Select(x => String.Join(" ", x))));

        textWriter.Flush();
        textWriter.Close();
    }
}
