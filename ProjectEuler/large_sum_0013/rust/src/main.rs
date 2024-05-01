// Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.
// (see data.txt for the numbers)

// Whiteboard:
// - I am assuming that each line represents a "50-digit"s number (huge number)
// - There are 100 lines
// - sum(100_lines) is what I think it means
// Most likely, the trick here is that summation of the numbers is too large to fit in a single number
// i.e. max 64-bit is 2^64-1 = 18446744073709551615, which is less than 50-digits
// The most primitive way to solve this is to do it the way we've learned how to add numbers
// in elementary school, which is to add each digit from right to left and carry over the overflow
// to the next digit. Fortunately, each lines are fixed length of 50-digits, so the assumption is
// is that all numbers aligns to from the right without needing to shift the numbers.
// 1. Read the file line by line as a string; just for optmizations, we'll preallocate 50 chars, and
//    if the line does not have 50 chars, we'll return an error (in future, we'll pad left with 0s)
// 2. From right-most char-index, we'll add the column values and carry over the overflow to the next
//    column. We'll keep the result in a vector of 50 elements.
// Data-structure-wise, it will be:
// * an array of 50 elements per line (100 lines)
// * an array of u64 (i.e. Vec<u64>) to keep the sum of the columns, where Vec[0] is the right-most
//   digit of the sum
//
// This it turns out, is a simple problem but a very slow processing task dealing with 50digits x 100lines
// Once the solution has been found, some optimization strategies will have to be employed to make it
// more efficient.  Data-structure wise, vectors are quite efficient, and at the same time, because
// we preallocated the vector, there should be no reallocation overhead.
// Memory and CPU wise, it's not too bad, hence it's the issue of iterating through 50 digits x 100 lines
// One approach I initially thought of was to pre-rotate the columns to become rows, so that you'd deal
// with single vector per summation, but it most likely is the cost of converting a char (32-bit UTF-8 unicode)
// to a single digit based on not an index but rather, String::chars()::nth() method, which is O(n) operation.
// So perhaps the initial optimization should be based on first converting Vec<char> ('0'..'9') to Vec<u8> (0..9)
// in which it'll also benefit from immediate indexing (i.e. O(1) operation) the column (i.e. vec[column]).
// After evaluations and debugging (println debugging), it was found that there was a bug in which the final
// carry-over was not being processed correctly.  It was just passing the same carry-over over and over
// without shifting, so it never reached 0 to exit the while(carry>0) loop.  In the end, the optimizations
// were not necessary, but it was a good exercise to go through the process of optimization.
// Just for curiosity, optimized took about 523.301µs, and non-optimized took about 5.371384ms on my laptop running Win11.
// Though the differences is significant (1/2 ms vs 5 ms) which (percentage-wise) is about 1000% (10x) faster,
// hence I'd like to see how well C++ version does in comparison optimized.
//
// Postmortem:
// The final answer I provided to the site was 2244068323, which gave me my very first red-X!
// Interestingly, CoPilot calculated the answer to be 5537376230, which just for curiousity, I submitted
// and it was correct!  Now I am in the process of debugging to find out why my answer was wrong...
// Bug found: Original assumptions was that if the column sum was 65535, then digit is 5 and carry-over is 65530
//            but that cannot just be added to next column, but rather, it has to be shifted to the right
//            for each digit.
// With final answer now corrected, performance comparison is as follows:
// Rust unoptimized: 5.371384ms
// Rust optimized: 197.107µs
// C++ unoptimized: 531µs (not going to even bother with optimized version, hands down, C++ wins even via string based)

use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

const NUM_DIGITS: usize = 50;
const NUM_LINES: usize = 100;
const DATA_FILE: &str = "../data.txt"; // Expected answer for this data should be 5537376230 (first 10 digits)
                                       // Processing carry-over took: 1.729µs (Total so far: 171.576µs)
                                       // Final sum (52 digits): 5537376230390876637302048746832985971773659831892672

fn read_data(filename: &str) -> io::Result<Vec<String>> {
    let mut data = Vec::with_capacity(NUM_LINES);
    let file = File::open(filename)?;

    let reader = io::BufReader::new(file);
    for line in reader.lines() {
        let line = line?;
        if line.len() != NUM_DIGITS {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "Invalid number of digits",
            ));
        }
        data.push(line);
    }
    Ok(data)
}

// pre-transform the data from Vec<String> to Vec<Vec<u8>> for faster processing
// and to avoid converting char to digit on the fly as well as
// to avoid using String::chars()::nth() method, which is O(n) operation
// One care that has to be made is that we do not flip the order of the digits
// so that when str_digits[0] is read and converted, it gets stored in u8_digits[0]
// honestly, like C++/C# it would be nice if we can transform Vec<Vec<_>> to
// [[u8; NUM_DIGITS]; NUM_LINES] so that it will present to the users that it's
// explicitly read-only, but the pain in the butt is ownerships, etc
// similar to Vec<&str> (or [&str; NUM_LINES]) is very difficult to manage when
// attempt to store it in a struct...  Best we can do, just like how it is
// passed here, is to pass it as a reference as a contract to assure the caller
// that it will not be modified (read-only/immutable)...
fn transform_data(data: &Vec<String>) -> Vec<Vec<u8>> {
    if data.len() != NUM_LINES {
        panic!("Invalid number of lines");
    }
    if data.iter().any(|line| line.len() != NUM_DIGITS) {
        panic!("Invalid number of digits");
    }

    let empty_line = vec![0; NUM_DIGITS]; // preallocate the vector
    let mut u8_data = vec![empty_line; NUM_LINES]; // should we panic if data.length() > NUM_LINES?
    for (row_index, line) in data.iter().enumerate() {
        let mut u8_line = vec![0; NUM_DIGITS]; // preallocate the vector
        for (col_index, ch) in line.chars().enumerate() {
            let digit = ch.to_digit(10).unwrap() as u8;
            u8_line[col_index] = digit; // rather than second-guessing whether push(digit) or insert(0,digit), but just using same index is least buggy
        }
        //println!("{:?}", u8_line.clone());
        // push() will append to the end of the vector, so this is acceptable as well, but we'll use upsert() for consistency
        //u8_data.push(u8_line);
        u8_data[row_index] = u8_line;
    }
    u8_data // transfer the ownership to the caller
}

// just sum the specified column
// note that column 50 is right most, and column 0 is left most
// this function will NOT try to guess whether you meant column 0 really was right-most
// the calling method should do something like:
//      for col in (0..NUM_DIGITS).rev() {  // reverse it, so we start from right-most
fn sum_column(data: &Vec<String>, col: usize, carry: u64) -> u64 {
    let mut sum = carry;
    for line in data {
        let digit = line.chars().nth(col).unwrap().to_digit(10).unwrap() as u64;
        sum += digit;
    }
    sum
}

// optimized version of sum_column which uses pre-transformed data
fn sum_column_v2(data: &Vec<Vec<u8>>, col: usize, carry: u64) -> u64 {
    let mut sum = carry; // even if previous carry_over was more than 1 dibit, it'll be handled in split_digit_carryover
    for (row_index, line) in data.iter().enumerate() {
        let digit = line[col] as u64;
        sum += digit;
        //println!("sum: {} - ({},{}):{}", sum, row_index, col, digit);
    }
    sum
}

// split the carry-over and digit of the column value
// i.e. the column sum was 65535, then digit is 5 and carry-over is [6,5,5,3]
//fn split_digit_carryover(sum: u64) -> (u8 /*digit*/, Vec<u8> /*carry_per_digit*/) {
fn split_digit_carryover(sum: u64) -> (u8 /*digit*/, u64 /*carry_per_digit*/) {
    let digit = sum % 10; // i.e. 65535 % 10 = 5
    let carryover = (sum / 10); // i.e. 65535 / 10 = 6553

    // carryovers are used to update sum of NEXT calculations, so multiply by 10 to shift digit over to the left
    (digit as u8, carryover)
}

fn dump_row(row: Vec<u8>) {
    for digit in row {
        print!("{}", digit);
    }
    println!();
}

fn large_sum(data_transformed: &Vec<Vec<u8>>) -> Vec<u8> {
    let app_start_timer = std::time::Instant::now();
    let start_timer = std::time::Instant::now();

    println!(
        "Data transformation took: {:?} (Total so far: {:?})",
        start_timer.elapsed(),
        app_start_timer.elapsed()
    );

    let start_timer = std::time::Instant::now();
    // because we're preallocating the vector, we'll start from the right-most digit
    let mut sum = Vec::with_capacity(NUM_DIGITS);
    let mut carry = 0u64;
    // EXCLUDES NUM_DIGITS (i.e. 0..50 means 0 to 49 - should do 0..=NUM_DIGITS to include 50)
    for col in (0..NUM_DIGITS).rev() {
        let col_sum = sum_column_v2(&data_transformed, col, carry);
        //let col_sum = sum_column(&data, col, carry);
        let (digit, new_carry) = split_digit_carryover(col_sum);
        sum.insert(0, digit); // insert to the left-most of the vector (i.e. push to head and shifts tails to right - [1,2,3] -> insert 4 -> [4,1,2,3])
        carry = new_carry;

        //print!( "col {:0>3}: col_sum: {:>5}, digit={} (carry: {:>5}) - sum:", col, col_sum, digit, carry);
        //dump_row(sum.clone());
    }
    println!(
        "\nProcessing columns took: {:?} (Total so far: {:?}) - carry = {}",
        start_timer.elapsed(),
        app_start_timer.elapsed(),
        carry
    );

    // add the carry-over to the sum
    let start_timer = std::time::Instant::now();
    while carry > 0 {
        let (digit, new_carry) = split_digit_carryover(carry);
        //print!("{} (carry: {}) - sum: ", digit, carry);   // for debug, this will reverse the order, but who cares...
        //dump_row(sum.clone());

        sum.insert(0, digit);

        // shift carry to the right
        carry = new_carry;
    }
    println!(
        "\nProcessing carry-over took: {:?} (Total so far: {:?})",
        start_timer.elapsed(),
        app_start_timer.elapsed()
    );
    print!("Final sum ({} digits): ", sum.len());
    dump_row(sum.clone());

    println!(
        "\n\nApplication processing took: {:?}",
        app_start_timer.elapsed()
    );
    dump_row(sum.clone());
    sum
}

fn main() {
    let data = read_data(DATA_FILE).expect("Failed to read data");
    let data_transformed = transform_data(&data);
    let sum = large_sum(&data_transformed);

    // print the first 10 digits
    let expected_answer = "5537376230";
    for i in 0..10 {
        print!("{}", sum[i]);
    }
    for i in 0..10 {
        assert_eq!(
            sum[i],
            expected_answer
                .chars()
                .nth(i)
                .unwrap()
                .to_digit(10)
                .unwrap() as u8
        );
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_read_data() {
        let data = read_data(DATA_FILE).expect("Failed to read data");
        assert_eq!(data.len(), NUM_LINES);
        for line in data {
            assert_eq!(line.len(), NUM_DIGITS);
        }
    }

    #[test]
    fn test_sum_column() {
        let data = vec![
            // 50 digits per line
            "12345678901234567890123456789012345678901234567890".to_string(),
            "12345678901234567890123456789012345678901234567890".to_string(),
            "12345678901234567890123456789012345678901234567890".to_string(),
        ];
        assert_eq!(data[0].len(), NUM_DIGITS);
        assert_eq!(sum_column(&data, (NUM_DIGITS - 1) - 0, 0), 0 * 3); // column 50: 0+0+0=0
        assert_eq!(sum_column(&data, (NUM_DIGITS - 1) - 1, 0), 9 * 3); // column 49: 9+9+9=27
        assert_eq!(sum_column(&data, (NUM_DIGITS - 1) - 2, 0), 8 * 3); // column 48: 8+8+8=24
        assert_eq!(sum_column(&data, (NUM_DIGITS - 1) - 9, 0), 1 * 3); // column 41: 1+1+1=3
    }

    #[test]
    fn test_split_digit_carryover() {
        assert_eq!(split_digit_carryover(0), (0, 0));
        assert_eq!(split_digit_carryover(1), (1, 0));
        assert_eq!(split_digit_carryover(9), (9, 0));
        assert_eq!(split_digit_carryover(10), (0, 1));
        assert_eq!(split_digit_carryover(11), (1, 1));
        assert_eq!(split_digit_carryover(19), (9, 1));
        assert_eq!(split_digit_carryover(20), (0, 2));
        assert_eq!(split_digit_carryover(21), (1, 2));
    }

    // test processing first (tail end) 3 digits for verification of carry-over concept
    #[test]
    fn test_few_loops() {
        let test_count = 3;
        // NOTE: data is mutable because we'll be padding the rest of the data to pass the test
        let mut data = vec![
            // 50 digits per line
            "12345678901234567890123456789012345678901234567890".to_string(),
            "12345678901234567890123456789012345678901234567890".to_string(),
            "12345678901234567890123456789012345678901234567890".to_string(),
        ];
        // Note that transform_data expects 100lines x 50digits, so we need pad the rest of the data so we will pass the test
        for _ in test_count..(NUM_LINES) {
            // all 0's
            data.push("00000000000000000000000000000000000000000000000000".to_string());
        }

        let data_transformed = transform_data(&data);
        // we'll only care about results of last few digits - NOTE: if you increase test_count, you need to adjust expected_result
        // Col49: 0 + 0 + 0 + Carry(0) =>  0; Digit=0, Carry: 0
        // Col48: 9 + 9 + 9 + Carry(0) => 27; Digit=7, Carry: 2
        // Col47: 8 + 8 + 8 + Carry(2) => 26; Digit=6, Carry: 2
        let expected_result = vec![6, 7, 0]; // right most columns are all 0's, so 0+0+0=0, 9+9+9=27 carry-over=20, 8+8+8=24+20=44 carry-over 4
        let exptected_final_carryover = 2;

        let mut sum = Vec::with_capacity(NUM_DIGITS); // preserve result for later analaysis
        let mut carry = 0u64;
        // EXCLUDES NUM_DIGITS (i.e. 0..50 means 0 to 49 - should do 0..=NUM_DIGITS to include 50)
        for col in (0..NUM_DIGITS).rev() {
            let col_sum = sum_column(&data, col, carry);
            let col_sum2 = sum_column_v2(&transform_data(&data), col, carry);
            assert_eq!(col_sum, col_sum2);
            let (digit, new_carry) = split_digit_carryover(col_sum);
            let (digit2, new_carry2) = split_digit_carryover(col_sum);
            assert_eq!(digit, digit2);
            assert_eq!(new_carry, new_carry2);

            // now update carry-over row

            // bail out after 3 digits
            if col < NUM_DIGITS - test_count {
                break; // get out, let's analyze the result
            }

            // preserve the result for later analysis
            sum.insert(0, digit); // insert to head and shift tails to right
            carry = new_carry;
        } // for

        println!("sum: {:?} vs expected: {:?}", sum, expected_result);
        println!(
            "carry: {} vs expected: {}",
            carry, exptected_final_carryover
        );
        // analyze the result
        for (index, expected) in expected_result.iter().enumerate() {
            let digit = sum[index as usize] as i32;
            assert_eq!(digit, *expected);
        }
        assert_eq!(carry, exptected_final_carryover);
    }

    // all 100 lines will have the highest digit to be 1, and lowest digit to be 9
    // which, if repeated 100 times, it will be 9x100=900 on the right-most column
    // and 1x100=100 on the left-most column (which will exceed 2 digits) making
    // the final result 52 digits long
    // so it will look like:
    //    .5....4....4....3....3....2....2....1....1....0....0
    //    .0....5....0....5....0....5....0....5....0....5....0
    //    _|____.____|____.____|____.____|____.____|____.____|
    //    ..12345678901234567890123456789012345678901234567890
    //    ..10000000000000000000000000000000000000000000000009
    //    1000000000000000000000000000000000000000000000000900
    #[test]
    fn test_large_carryover() {
        let mut data: Vec<String> = Vec::new();
        // Note that transform_data expects 100lines x 50digits, so we need pad the rest of the data so we will pass the test
        for _ in 0..(NUM_LINES) {
            // all 0's
            data.push("10000000000000000000000000000000000000000000000009".to_string());
        }
        let expected_result = "1000000000000000000000000000000000000000000000000900".to_string();
        let transposed_data = transform_data(&data);

        let sum = large_sum(&transposed_data);
        print!("     sum:  ");
        dump_row(sum.clone());
        println!(
            "expected: {:?} (len={})",
            expected_result,
            expected_result.len()
        );

        assert_eq!(sum.len(), expected_result.len());
        for i in 0..sum.len() {
            assert_eq!(
                sum[i],
                expected_result
                    .chars()
                    .nth(i)
                    .unwrap()
                    .to_digit(10)
                    .unwrap() as u8
            );
        }
    }
}
