using System;
using System.Linq;

namespace neighbors
{
    public class Solution
    {
        //METHOD SIGNATURE BEGINS, THIS METHOD IS REQUIRED
        // IN: 10000100
        //      1
        // OUT: 01001010
        // IN: 11101111
        //      2
        // OUT: 00000110
        public int[] cellCompete(int[] states, int days)
        {
            // if neighbors on both sides are either 0 or 1,
            // then cell becomes inactive (0), else active the next day (1)
            Func<int[], int, int, int> test = (s, l, r)
                => s[l] == s[r] ? 0 : 1;

            if (days <= 0)
                return states;
            do
            {
                var today = states;
                
                for(var i = 1; i < states.Length - 2; ++i)
                {
                    today[i] = test(i-1, i+1);
                }
            } while(--days);
        }
    // METHOD SIGNATURE ENDS
    }    
}
