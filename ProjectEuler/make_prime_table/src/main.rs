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

// Prime number: a number greater than 1 that has no positive divisors other than 1 and itself.
// i.e. 5, 7, 13, and 29 are prime numbers
// Some rules:
// * 1 is not a prime.
// * All primes except 2 are odd.
// * All primes greater than 3 can be written in the form 6k+/-1.
// * Any number n can have only one primefactor greater than n .
// * The consequence for primality testing of a number n is: if we cannot find a number f less than 
//   or equal n that divides n then n is prime: the only primefactor of n is n itself
fn is_prime(n: usize) -> bool {
    // 2 is a known prime number
    if n == 2 {
        return true;
    }
    // all primes except for 2 are odd
    // 1 is not a prime
    // if divisible by 2, it's not a prime number, also 0 and 1 are not prime numbers
    if n < 2 || n % 2 == 0 {
        return false;
    }

    if n > 3 {
        // all primes greater than 3 can be written in the form 6k+/-1
        if n % 6 != 1 && n % 6 != 5 {
            return false;
        }
    }

    // n < 9: 2, 3, 5, 7 (4, 6, and 8 tested above)
    if n >= 9 && n % 3 == 0 {
        return false;
    }

    let sqrt_n = ((n as f64).sqrt()).floor() as usize;
    let mut f = 5;
    while f <= sqrt_n {
        if n % f == 0 {
            return false;
        }
        if n % (f + 2) == 0 {
            return false;
        }
        f += 6;
    }

    // if n = 13, the largest divisor is 13, which is less than sqrt(13) = 3.6
    // if n = 25, the largest divisor is 5, which is less than sqrt(25) = 5
    // if n = 29, the largest divisor is 29, which is less than sqrt(29) = 5.4
    // hence if n is not a prime number, it must have a divisor less than its square root
    // so for 25 = 5 * 5, the largest divisor is 5, which is less than sqrt(25) = 5
    // for 29 = 29 * 1, the largest divisor is 29, which is less than sqrt(29) = 5.4
    // If a number n has a divisor greater than its square root, it must also have a corresponding
    // divisor smaller than its square root. Therefore, we only need to check up to the square root.
    let mut i = 3; // we've already tested for 0..2, so start from 3
    while i * i <= n {
        if n % i == 0 {
            return false; // opt out early
        }
        i += 1; // can I do +2 here?
    }
    // passed all the tests between 2 and sqrt(n), this number is prime
    return true;
}

// NOTE: On a 64-bit machine, usize is 64-bit, while on a 32-bit host, usize is 32-bit.
//       (usize is a size of a pointer)
fn make_prime_table(max_value: usize, index_lookup_filename: &str, known_primes_filename: &str) {
    // create an output stream, output will be no-space separated
    // and each line will be up to 64 numbers, either 0 (if not)
    // prime, and 1 if it is.
    // it will be from left-to-right order, in which left-most
    // (i.e. row=0, column=0) is 0th prime number, and so on.
    // NOTE: 0 and 1 are NOT prime numbers, so they will be
    // marked as 0.
    let mut file_lookup = std::fs::File::create(index_lookup_filename).unwrap();
    let mut file_known_primes = std::fs::File::create(known_primes_filename).unwrap();  // each line is just a prime number

    // Note that if the last row does not have 64 numbers, then
    // it will continue to iterate until the last number.
    let max_iter = (max_value / 64) + 1; // number of rows
    println!("max_value={}, max_iter={} rows", max_value, max_iter);
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
            // and write the prime number to the known prime file
            file_known_primes.write_all(format!("{}\n", n).as_bytes()).unwrap();
        }
        // write the row to the file (no carriage return!)
        file_lookup.write_all(prime_row64.as_slice()).unwrap();
    }
}

fn main() {
    let start_timer = std::time::Instant::now();
    make_prime_table(2 << 15, "prime_table_65535.bin", "known_primes_65535.txt");
    println!("Elapsed time: {:?}", start_timer.elapsed());

    let start_timer = std::time::Instant::now();
    make_prime_table(2 << 26, "prime_table_4M.bin", "known_primes_4M.txt");
    println!("Elapsed time: {:?}", start_timer.elapsed());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_prime() {
        assert_eq!(is_prime(0), false); // 0 is not a prime
        assert_eq!(is_prime(1), false); // 1 is NOT a prime
        assert_eq!(is_prime(2), true);  // all prime except for 2 are odd
        assert_eq!(is_prime(3), true);
        assert_eq!(is_prime(4), false);
        assert_eq!(is_prime(5), true);
        assert_eq!(is_prime(6), false);
        assert_eq!(is_prime(7), true);
        assert_eq!(is_prime(22), false);
        assert_eq!(is_prime(32), false);
        assert_eq!(is_prime(38), false);
        assert_eq!(is_prime(42), false);
        assert_eq!(is_prime(43), true);
    }
}
