// A palindromic number reads the same both ways.
// The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.$
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

const MAX_DIGITS: u32 = 3;
const MAX_NUM: u64 = 999;   // for 3 digits, this is the max number

fn is_palindrome(num: u64) -> bool {
    let num_str = num.to_string();
    let num_rev: String = num_str.chars().rev().collect();
    num_str == num_rev
}

fn is_N_digit(num: u64, digits: u32) -> bool {
    let num_str = num.to_string();
    let mut num_set: std::collections::HashSet<char> = std::collections::HashSet::new();
    for c in num_str.chars() {
        num_set.insert(c);
    }
    num_set.len() == digits
}

fn main() {
    let mut max_palindrome: u64 = 0;
    let mut x: u64 = MAX_NUM;
    let mut y: u64 = MAX_NUM;
    while x > 0 {
        while y > 0 {
            let z = x * y;
            // opt out early on FIRST encounter of palindromic number, for it SHOULD be the largest since we're iterating in decending order
            if is_palindrome(z) && is_N_digit(x, MAX_DIGITS) && is_N_digit(y, MAX_DIGITS) {
                max_palindrome = z;
                println!("Largest palindrome: {} = {} x {}", max_palindrome, x, y);
                return;
            }
            y -= 1;
        }
        x -= 1;
        y = MAX_NUM;
    }
}
