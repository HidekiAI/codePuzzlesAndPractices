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

//const MAX_DIGITS: usize = 3;
//const MAX_NUM: u64 = 999;   // for 3 digits, this is the max number
//const MIN_NUM: u64 = 100;   // for optimations, we'll only loop between MIN and MAX, and at the same time, because A * B = B * A, we can reduce the loop by half
const MAX_DIGITS: usize = 3;
const MAX_NUM: u64 = 999;
const MIN_NUM: u64 = 100;

// NOTE: Because unlike traditional C/C++ std::string, Rust's String is UTF-8 encoded,
// which makes it so capacity is 4bytes per char (32-bits), as compared to
// C/C++ char (8-bits) and wchar (16-bits), I am a bit concerned about how
// this reversed string will equate when in STRING format, hence we'll need to
// take extra step and convert the string BACK to u64 to compare and compare
// the NUMBER instead of string...
fn is_palindrome(num: u64) -> bool {
    let num_str = num.to_string();
    let num_rev: String = num_str.chars().rev().collect();
    if num_str == num_rev {
        return true;
    }
    // due to concerns, we'll convert the reversed string back to u64 and compare
    //println!("num: {}, num_str: {}, num_rev: {}", num, num_str, num_rev);
    let num_rev: u64 = num_rev.parse().unwrap();
    num == num_rev
}

// We'll use SET to store unique digits, and then compare the length of the set
// alternatively, we could also do HashMap where key is the digit and
// value is irrelavant, and just count how many keys are in the map
// NOTE: HashMap in Rust replaces existing key with new value (as compared to C# will throw)
fn is_N_digit(num: u64, digits: usize) -> bool {
    let num_str = num.to_string();
    let mut num_set: std::collections::HashSet<char> = std::collections::HashSet::new();
    // NOTE: String.chars() iterates through UTF-8 encoded sequence of chars, which is 4bytes (32-bits) per char
    for c in num_str.chars() {
        num_set.insert(c);
    }
    //println!("num: {}, num_str: '{}', num_set: {:?}", num, num_str, num_set);
    num_set.len() == digits
}

fn main() {
    let mut x: u64 = MAX_NUM;
    let mut y: u64 = MAX_NUM;
    let mut largest_product: u64 = 0;
    while x >= MIN_NUM {
        while y >= MIN_NUM {
            let z = x * y;
            // opt out early on FIRST encounter of palindromic number, for it SHOULD be the largest since we're iterating in decending order
            // optimization is also based on the && operator, in which we can either first test for N
            // digits or whether the product is palindrome, in which case, I believe testing first
            // for palindrome is more efficient, as it is a single operation, while N digits
            //if is_palindrome(z) && is_N_digit(x, MAX_DIGITS) && is_N_digit(y, MAX_DIGITS) {
            if z > largest_product && is_palindrome(z) && is_N_digit(z, MAX_DIGITS)  {
                largest_product = z;
                println!("Largest palindrome so far: {z} = {x} x {y}");
            }
            y -= 1;
        }
        x -= 1;
        y = MAX_NUM;    // reset
    }
    println!("Largest palindrome: {largest_product}");
}
