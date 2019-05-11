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
// For this, the hint began with shortest paths (height of BFS) but at the same time, it wants to know whether
// the path exists (DFS).  Hence for Brute Force, we'll do both:
//  1. First determine if path exists with DFS
//  2. Next rebuild the graph as a tree (just track parent) via DFS to determine the height to determine
//     one (of the possible few, other heights may be taller) of the paths that resulted in smallest height
// In part two, once Brute Force has been discussed, we'll explore how we can work with either DFS or BFS
// (or maybe even hybrid?) that can determine whether the paths exists AND determine the shortest paths.
//
// This is a directed-graph problem, for example, on a 2x2 grid:
//           (0, 0)                    0,0 1,0
//          /      \                   1,0 1,1
//      D:(1,0)    R:(0,1)
//         \        /
//          R:(1,1):D
// Note that (1,0) has no D-child, but has R-child, while (0,1) has D-child and no R-child.
// The children's distance/weight is assumed to be 1 if exist, 0 otherwise, so no need to explicitly define weights
// We still need separate Edge list to understand the path?  Yes
// Also, once the algorithm has been decided (in this case, DFS), the complications of linked-list
// can be eliminated by streaming the data in the order of DFS in an array of single dimension.
// In the example above, with DFS, the stream should be rebuilt as an array of:
//      (0,0), (1,0), (1,1), (0,1)
// For this excercise, there are no stream, just a value N which describes that the data structures will be
// an NxN array.
// With that in mind, given the dimension N, the array would be defined as:
//            A
//          F   B
//        K   G   C
//      P   L   H   D
//    U   Q   M   I   E
//      V   R   N   J
//        W   S   O
//          X   T
//            Y
// Following example is based on i=1, j=1 for a 5x5 grid.
//
// Edge list (source-target):
//    AF AB FK FG BG BC KP KL GL GH CH CD PU PQ LQ LM HM HI DI DE UV QV QR MR MN IN IJ EJ VW RW RS NS NO JO WX SX ST OT XY TY
//
// For BFS:
//    A F B K G C P L H D U Q M I E V R N J W S O X T Y
//    ^                                               ^
// Constructing this requires a mechanism in which if "seen", you skip it
//
// For DFS, a matrix can be constructed to represent adjacent list:
//       A B C D E F G H I J K L M N O P Q R S T U V W X Y
//     A   1 . . . 1 . . . . . . . . . . . . . . . . . . .
//     B 0   1 . . . 1 . . . . . . . . . . . . . . . . . .
//     C . 0   1 . . . 1 . . . . . . . . . . . . . . . . .
//     D . . 0   1 . . . 1 . . . . . . . . . . . . . . . .
//     E . . . 0   . . . . 1 . . . . . . . . . . . . . . .
//     F 0 . . . .   1 . . . 1 . . . . . . . . . . . . . .
//     G . 0 . . . 0   1 . . . 1 . . . . . . . . . . . . .
//     H . . 0 . . . 0   1 . . . 1 . . . . . . . . . . . .
//     I . . . 0 . . . 0   1 . . . 1 . . . . . . . . . . .
//     J . . . . 0 . . . 0   . . . . 1 . . . . . . . . . .
//     K . . . . . 0 . . . .   1 . . . 1 . . . . . . . . .
//     L . . . . . . 0 . . . 0   1 . . . 1 . . . . . . . .
//     M . . . . . . . 0 . . . 0   1 . . . 1 . . . . . . .
//     N . . . . . . . . 0 . . . 0   1 . . . 1 . . . . . .
//     O . . . . . . . . . 0 . . . 0   . . . . 1 . . . . .
//     P . . . . . . . . . . 0 . . . .   1 . . . 1 . . . .
//     Q . . . . . . . . . . . 0 . . . 0   1 . . . 1 . . .
//     R . . . . . . . . . . . . 0 . . . 0   1 . . . 1 . .
//     S . . . . . . . . . . . . . 0 . . . 0   1 . . . 1 .
//     T . . . . . . . . . . . . . . 0 . . . 0   . . . . 1
//     U . . . . . . . . . . . . . . . 0 . . . .   1 . . .
//     V . . . . . . . . . . . . . . . . 0 . . . 0   1 . .
//     W . . . . . . . . . . . . . . . . . 0 . . . 0   1 .
//     X . . . . . . . . . . . . . . . . . . 0 . . . 0   1
//     Y . . . . . . . . . . . . . . . . . . . 0 . . . 0
// The adjacency matrix representation above indicates that the upper partition
// is the direction towards the child and bottom partition is the relationship
// to its parents.  For example, H->I==1 to walk down and I->H==0 to backtrack
// Adjacency List Representations:
// There are other ways to represent this, such as via linked list of
//      A -> F -> B
//      B -> G -> C
//      ...
//      G -> L -> H
//      ...
//      T -> Y
//      U -> V
//      V -> W
//      W -> X
//      X -> Y
//      Y -> null
//
// DFS Preorder: (i.e. 1 2 4 5 3)
//    A F K P U V W X Y Q R S T L M N O G H I J B C D E
//    ^               ^
// DFS post-ordering (i.e. 4 5 2 3 1)
//    Y X T W S O V R N J U Q M I E P L H D K G C F B A
//    ^                                               ^
// DFS Inorder: (i.e. 4 2 5 1 3)
//    U P V Q W R X S Y T K L N N O F G H I J A B C D E
//                    ^                       ^
// DFS is the ideal pick/choice for the purpose of DFS is to inspect all its
// children to determine if the path exists!
// For example, for a 5x5 with i=1,j=1, if I am at G, looking at the visual
// representations, we should be following:
//      G -> M -> S -> Y
// Or if I am on node Q, the shortest paths are either:
//      Q -> W -> X -> Y
//      Q -> R -> S -> Y
// Other pa
//
//
//
//
// 
//
// When i=1, j=2, the edge list will be:
//      AL AH ... YR YN
// and so on...

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
