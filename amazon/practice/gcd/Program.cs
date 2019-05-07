using System;
using System.Linq;

namespace gcd
{
    public int generalizedGCD(int num, int[] arr)
    {
        if (num <= 0)
            return 0;

        var found = arr.OrderByDescending(x => x)
            .Where(x => (arr.Where(y => (y % x) == 0).Count() == num))
            .OrderByDescending(x => x);
        if (found.Count() == 0)
            return 1;
        return found.First();
    }
}
