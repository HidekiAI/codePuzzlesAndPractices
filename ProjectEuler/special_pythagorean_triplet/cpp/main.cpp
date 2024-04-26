
// A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
//      a^2 + b^2 = c^2
// For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.
// There exists exactly one Pythagorean triplet for which a + b + c = 1000.
// Find the product abc.

// Whiteboard:
// a^2 + b^2 = c^2
// and a < b < c
// a = 3, b = 4, then:
// 3^2 + 4^2 = 9 + 16 = 25
// c = sqrt(25) = 5
// a(3) < b(4) < c(5) - passes the a < b < c test
// N = a + b + c
// N = 3 + 4 + 5 = 12
// Knowing N = 12, can we find a, b, c?
// We know that a, b, and c summed together, must be 12
// so combinations we'd have are:
// a = 0, b = 1, c = 11 // b cannot be 0 due to the rule a < b < c
//      0^2 + 1^2 != 11^2   // fails the test
// a = 0, b = 2, c = 10
//      0^2 + 2^2 != 10^2   // fails the test
// ...
// a = 3, b = 4, c = 5
//      3^2 + 4^2 = 5^2  // passes the test
// a = 3, b = 5, c = 4 // b cannot be greater than c, so stop here (did we need to even check this once we've passed the test above?)
// I will need the following methods:
// * A method in which, given 'a and 'b, will return Some('c) in which 'a < 'b < 'c, else None
// * tests 'a + 'b + 'c = 'N
// For iterations, I will start with a = 0, b = 1, c = N - b in which a is the outer most loop,
// b is the inner loop, and c is calculated as N - b

#include <iostream> // std::cout
#include <cmath>    // std::sqrt and std::pow
#include <cassert>  // assert

const long N = 1000; // N=1000 -> product of a, b, and c is 31875000 where a=200, b=375, c=425

// Returns the product of a, b, and c in which a^2 + b^2 = c^2
// in which we test to verify that a != b && a < b < c
// Returns -1 if input is invalid
long find_c(long a, long b)
{
    // if a == 0, c^2 == b^2, which will cause a<b<c to fail!
    // we won't check whether a < b or not, but we need to test if they aren't equal
    if ((a == 0 || b == 0) || (a == b))
    {
        return -1;
    }
    auto c_squared = std::pow(a, 2) + std::pow(b, 2);
    long c_long = (long)std::sqrt(c_squared); // C-style dynamic_cast<>
    if ((c_long <= a || c_long <= b) || (c_squared != c_long * c_long))
    {
        return -1;
    }
    return c_long;
}

int main()
{
    for (long a = 1; a < N; a++)
    {
        // make sure that b > a
        for (long b = a + 1; b < N; b++)
        {
            auto c = find_c(a, b);
            // assert(c != -1, "a and b cannot be equal");
            if (c == -1)
            {
                continue; // why can't we get c^2?
            }
            if (a + b + c == N)
            {
                std::cout << "N=" << N << ", a: " << a << ", b: " << b << ", c: " << c << std::endl;
                std::cout << "Product of a, b, and c: " << a * b * c << std::endl;
                return 0;
            }
        }
    }
    return 0;
}