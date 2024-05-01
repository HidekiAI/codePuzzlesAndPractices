// FizzBuzz
// Given a number n, for each integer i in the range from 1 to n inclusive, print one value per line as follows:
// - If i is a multiple of both 3 and 5, print FizzBuzz.
// - If i is a multiple of 3 (but not 5), print Fizz.
// - If i is a multiple of 5 (but not 3), print Buzz.
// - If i is not a multiple of 3 or 5, print the value of i.
//
// Function Description
// Complete the function fizzBuzz in the editor below.
// fizzBuzz has the following parameter(s):
// - int n: upper limit of values to test (inclusive)
// Returns: NONE
// Prints:
// The function must print the appropriate response for each value i in the set {1, 2, ... n} each on a new line.
// Constraints
// 1 <= n <= 2 x 10^5
// Input format for custom testing
// Input from stdin will be processed as follows and passed to the function.
// The single integer n, the limit of the range to test: [1, 2, ...n].
// Sample Case 0:
// Sample Input 0:
// STDIN Function
// ----- --------
// 15 -> n = 15
// Sample output
// 1
// 2
// Fizz
// 4
// Buzz
// Fizz
// 7
// 8
// Fizz
// Buzz
// 11
// Fizz
// 13
// 14
// FizzBuzz
// Explanation 0:
// The numbers 3, 6, 9, and 12 are multiples of 3 (but not 5), so print Fizz on those lines.
// The numbers 5 and 10 are multiples of 5 (but not 3), so print Buzz on those lines.
// The number 15 is a multiple of both 3 and 5, so print FizzBuzz on that line.
// The rest of the numbers are not multiples of 3 or 5, so we print the value of those numbers on subsequent lines.

use std::io::{self, BufRead};

/*
 * Complete the 'fizzBuzz' function below.
 *
 * The function accepts INTEGER n as parameter.
 */

fn fizzBuzz(n: i32) {
    for i in 1..=n {
        // if i is a multiple of both 3 and 5, print FizzBuzz
        if i % 3 == 0 && i % 5 == 0 {
            println!("FizzBuzz");
            continue;
        }

        // if i is multiple of 3 (but not 5), print "Fizz"
        if i % 3 == 0 && i % 5 != 0 {
            println!("Fizz");
            continue;
        }

        // if i is multiple of 5 (but not 3), print "Buzz".
        if i % 3 != 0 && i % 5 == 0 {
            println!("Buzz");
            continue;
        }

        println!("{i}")
    } // for
}
fn main() {
    let stdin = io::stdin();
    let mut stdin_iterator = stdin.lock().lines();

    let n = stdin_iterator
        .next()
        .unwrap()
        .unwrap()
        .trim()
        .parse::<i32>()
        .unwrap();

    fizzBuzz(n);
}
