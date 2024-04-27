// It seems calculating prime number is a painfully slow process
// within 16-bit range (65535), we can pregenerate a prime number
// table.
// There are two ways to generate prime number table:
// 1. Sieve of Eratosthenes
// 2. Trial division
// Will create two functions and try out both methods.
// Also, the table(s) generated has two forms:
// 1. A list of prime numbers, in which case, if it is NOT in the
//    list, then it's NOT prime.  But this will require presorted
//    binary tree for fast search.
// 2. A linear list (vector) of 0 and 1, where 0 means NOT prime
//    and 1 means prime.  This is easier to generate, but larger
//    data (will optmize later into 64-bits data), but will be
//    O(1) search time since the value is the index itself.
// On the data packing approach for 64-bits data, it may even
// be possible to hard-code it as a fixed array...

use std::io::prelude::*;

fn sieve_of_eratosthenes() {}

fn trial_division() {}

fn is_prime(n: usize) -> bool {
    if n < 2 {
        return false;
    }

    for i in 2..n {
        if n % i == 0 {
            return false;
        }
    }

    true
}

fn make_prime_table(max_value: usize, filename: &str) {
    // create an output stream, output will be no-space separated
    // and each line will be up to 64 numbers, either 0 (if not)
    // prime, and 1 if it is.
    // it will be from left-to-right order, in which left-most
    // (i.e. row=0, column=0) is 0th prime number, and so on.
    // NOTE: 0 and 1 are NOT prime numbers, so they will be
    // marked as 0.
    let mut file = std::fs::File::create(filename).unwrap();

    // Note that if the last row does not have 64 numbers, then
    // it will continue to iterate until the last number.
    let max_iter = (max_value / 64) + 1; // number of rows
    for row in 0..max_iter {
        // create a vector of 0 and 1, where 0 means NOT prime
        // and 1 means prime.  Up to 64 numbers per line.
        let mut prime_row64: Vec<u8> = vec![0; 64]; // TODO: In the future, maybe change this to just plain u64
        for col in 0..64 {
            let n = row * 64 + col;
            if n > (max_iter * 64) {
                // if it is beyond the last row (note that max_value could be the vec[0], in which I rather continue to fill in the rest of the row rather than falsely leaving it 0)
                break;
            }

            // if it is not prime, then skip (because we initialized it as 0)
            if is_prime(n) == false {
                continue;
            }

            // mark/set this column's bit as 1
            prime_row64[col] = 1u8;
        }
        // write the row to the file (no carriage return!)
        file.write_all(prime_row64.as_slice()).unwrap();
    }
}

fn main() {
    make_prime_table(65535, "prime_table_65535.bin");
}
