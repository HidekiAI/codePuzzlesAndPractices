// If the numbers 1 to 5 are written out in words: one, two, three, four, five,
// then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.
// If all the numbers from 1 to 1000 (one thousand) inclusive were written out in
// words, how many letters would be used?
// NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and forty-
// two) contains 23 letters and 115 (one hundred and fifteen) contains 20 letters.
// The use of "and" when writing out numbers is in compliance with British usage.

// whiteboard:
// A single char-array (unspaced, unhyphened) and regardless of order, as long as
// they are stuffed into the queue, so that we can count the length once all iteration
// has been done.  There will be a lookup table for each number, as well as special
// reading for 11-19, 10, 100, 1000, and so on as well as usage of "and" where
// needed.  We'll first try to generate a rule to generate a table, but if that
// begins to be too complex, we'll just hard-code the table.

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <cstdint>

const std::uint16_t MAX = 1000; // we really don't want to hand-build anthing more than 1000... let alone, 65535...

// build a dictionary of known words
static const std::vector<std::string> ones = {
    "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};
static const std::vector<std::string> teens = {
    "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};
static const std::vector<std::string> tens = {
    "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};
static const std::vector<std::string> hundreds = {
    "", "onehundred", "twohundred", "threehundred", "fourhundred", "fivehundred",
    "sixhundred", "sevenhundred", "eighthundred", "ninehundred"
};
// TEST: 342 = threehundredandfortytwo
// TEST: 115 = onehundredandfifteen
// TEST: 1000 = onethousand
// TEST: 100 = onehundred
// method to convert number to string
const std::string number_to_string(std::uint16_t n) {
    // throw if out of range
    if (n > MAX) {
        throw std::out_of_range("number_to_string() out of range");
    }

    std::string s;
    if (n >= 1000) {
        s += "onethousand";
        n %= 1000;
    }
    if (n >= 100) {
        s += hundreds[n / 100];
        if (n % 100 != 0) {
            s += "and";
        }
        n %= 100;
    }
    if (n >= 20) {
        s += tens[n / 10];
        n %= 10;
    }
    if (n >= 10) {
        s += teens[n - 10];
        n = 0;
    }
    if (n > 0) {
        s += ones[n];
    }
    return s;
}

int main() {
    std::string numbers_as_chars;
    for (std::uint16_t i = 1; i <= MAX; i++) {
        // just append to tail of the string (no need to worry about order)
        numbers_as_chars += number_to_string(i);
    }
    std::cout << "sum=" << numbers_as_chars.length() << std::endl;
    return 0;
}