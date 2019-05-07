using System;
using System.Linq;
using System.Collections;

class HelloWorld
{
  public sealed class HTree
  {
    public sealed class Point
    {
      public Point(double x, double y)
      {
        X = x;
        Y = y;
      }
      public readonly double X;
      public readonly double Y;
    }
    public sealed class Line
    {
      public Line(Point p1, Point p2)
      {
        P1 = p1;
        P2 = p2;
      }
      public readonly Point P1;
      public readonly Point P2;
    }
    
    public HTree(double centerX, double centerY, double length, double depth)
    {
      Center = new Point(centerX, centerY);
      Length = length;
      Depth = depth;
      // precalc Line[] for efficiencies
      var half = Length / 2;
      HorizontalLine = new Line(
        new Point(Center.X - half, Center.Y), // assume left is negative
        new Point((Center.X + half), Center.Y));  // assume right is positive
      VerticalLineLeft = new Line(
        new Point(HorizontalLine.P1.X, HorizontalLine.P1.Y + half), // assume up is positive
        new Point(HorizontalLine.P1.X, HorizontalLine.P1.Y - half));  // assume down is negative
      VerticalLineRight = new Line(
        new Point(HorizontalLine.P2.X, HorizontalLine.P2.Y + half), // assume up is positive
        new Point(HorizontalLine.P2.X, HorizontalLine.P2.Y - half));  // assume down is negative
    }
    public readonly Point Center;
    public readonly double Length;
    public readonly double Depth;
    public readonly Line HorizontalLine;
    public readonly Line VerticalLineLeft;
    public readonly Line VerticalLineRight;
  }
  
  static void drawLine(double depth, HTree.Point p1, HTree.Point p2)
  {
    Console.WriteLine($"{depth}: {(p1.X == p2.X ? "H": "V")}({p1.X}, {p1.Y}), ({p2.X}, {p2.Y})");
  }
  
  static void drawHLines(HTree ht)
  {
    drawLine(ht.Depth, ht.HorizontalLine.P1, ht.HorizontalLine.P2);
    drawLine(ht.Depth, ht.VerticalLineLeft.P1, ht.VerticalLineLeft.P2);
    drawLine(ht.Depth, ht.VerticalLineRight.P1, ht.VerticalLineRight.P2);
  }
  static void visitNode(HTree current)
  {
    if (current.Depth <= 0)
      return;
    
    drawHLines(current);
    visitNode(new HTree(current.VerticalLineLeft.P1.X, current.VerticalLineLeft.P1.Y, current.Length / 2, current.Depth - 1));
    visitNode(new HTree(current.VerticalLineLeft.P2.X, current.VerticalLineLeft.P2.Y, current.Length / 2, current.Depth - 1));
    visitNode(new HTree(current.VerticalLineRight.P1.X, current.VerticalLineRight.P1.Y, current.Length / 2, current.Depth - 1));
    visitNode(new HTree(current.VerticalLineRight.P2.X, current.VerticalLineRight.P2.Y, current.Length / 2, current.Depth - 1));
  }
  static void drawTree(double centerX, double centerY, double length, double depth)
  {
    var center = new HTree(centerX, centerY, length, depth);
    // recurse until depth reaches final
    visitNode(center);
  }
  
    static void Main()
    {
        drawTree(50, 50, 10, 3);
    }
}

