// A palindromic number reads the same both ways.
// The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.
// Find the largest palindrome made from the product of two 3-digit numbers.

// White board:
// First, as defined, palindrome is a "word" that are read from left to right and right to left the same.
// But dealing with numbers, it is hard to reverse a number and compare it with the original number.
// So ideally, by converting it to string, we can then create a function to check if the number (i.e. 64-bits)
// converted to a string is a palindrome.
// Next, we want to limit the number to "3 digits" (i.e. 123, 9879, 468, but not 1234, 99, 999, 988796, or 466)
// so we also need a method to test if the number has just 3-digit number.  Again, it seems
// more trivial to just convert to ASCII/char and count unique digits.
// Finally, we need to find the largest palindrome from the product of two 3-digit numbers.
// i.e. for 2 digits, it is 91 * 99 = 9009.  Here, the assumptions is that X * Y = Z,
// where X and Y are 3-digit numbers and Z is the largest palindrome.
// AND that both X and Y just has to be 3 digits, but not necessarily a palindrome.
// In which case, largest 3 digits number is 999 (as largest 2 digits is 99).
// This can then reduce down to an assumptions of 999 x X = Z, in which we
// can iterate from X=999..1 and check for first encounter of palindromic number
// and opt out.  If X=999..1 (inner loop) did not find panlindromic number, then
// we'd decrement the outer loop and repeat the process until we find the largest
// palindrome.
// Note that I am unsure whether the palindrome NEEDS to also only contain just
// 2 digits (i.e. 9009 verus 98789, both are palindromic numbers, but the former
// is the product of two 2-digit numbers, while the latter is the product of two 3-digit numbers).
// So for now, I'd assume as long as it's palindromic, it's good.
// Wrong assmptions: I think the assumptions are that the palindrome is has 3 unique digits, but
// the equations (X * Y) can be of any number of digits (i.e. 8 * 99 (1 digit * 2 digits)) as
// long as the product produces unique 3 digits.

#include <iostream> // cout
#include <string>   // to_string

const unsigned int MAX_DIGITS = 3;
const long long MAX_NUM = 999; // for 3 digits, it's 999

bool is_palindrome(const long long num)
{
    std::string str = std::to_string(num);
    for (size_t i = 0; i < str.size() / 2; ++i)
    {
        if (str[i] != str[str.size() - 1 - i])
        {
            return false;
        }
    }
    return true;
}

bool is_N_digits(const long long num, const unsigned int digits)
{
    std::string str = std::to_string(num);
    std::string unique_digits;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (unique_digits.find(str[i]) == std::string::npos)
        {
            unique_digits.push_back(str[i]);
        }
    }
    return unique_digits.size() == digits;
}

int main()
{
    long long largest_palindrome = 0;
    for (long long i = MAX_NUM; i > 0; --i)
    {
        for (long long j = MAX_NUM; j > 0; --j)
        {
            long long product = i * j;
            if (product < largest_palindrome)
            {
                break;  // should it continue instead of break?
            }
            // found product to be bigger than current largest, so record it
            if (is_palindrome(product) && is_N_digits(product, MAX_DIGITS))
            {
                largest_palindrome = product;
                std::cout << "Current match: " << largest_palindrome << " = " << i << " * " << j << std::endl;
            }
        }
    }
    std::cout << "Largest palindrome made from the product of two 3-digit numbers: " << largest_palindrome << std::endl;
    return 0;
}