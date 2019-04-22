#define TEST
using System;
using System.Linq;
public class GFG 
{
#if TEST        
	static string[]	inputs = new string[]
		{ 
            "2",
            "5 2",
            "12 5 787 1 23",
            "7 3",
            "1 23 12 9",    // continues to next line (tests multi-line input case on huge data)
            "30 2 50"
		};
    static int lineIndex = 0;
#endif

	static public void Main ()
    {
        var numTests = readData(1).First();
		for(var t=0; t < numTests; ++t)
        {
            var attrib = readData(2);
            var takeCount = attrib[1];
            var dataCount = attrib[0];
            var data = readData(dataCount);
            System.Diagnostics.Debug.Assert(dataCount == data.Count(), $"Mismatch on number of data to expected entry count");
            System.Console.WriteLine(string.Join(" ", doIt(takeCount, data)));
        }
	}

    private static int[] readData(int expectedCount)
    {
        var readCount = 0;
        var readData = new int[]{};
        do
        {
#if TEST
    		var inputLine = inputs[lineIndex];
            lineIndex++;
#else        
            var inputLine = Console.ReadLine();
#endif
            System.Console.WriteLine($"IN: '{inputLine}'");
            var intSplit = inputLine.Split(' ')
                .Select(x => int.Parse(x));
            readCount += intSplit.Count();
            readData.Concat(intSplit)
                .ToArray();
        } while(readCount < expectedCount);
        return readData;
    }

    private static int[] doIt(int takeCount, int[] iData) =>
        iData.OrderByDescending(x => x)
            .Take(takeCount)
            .ToArray();
}