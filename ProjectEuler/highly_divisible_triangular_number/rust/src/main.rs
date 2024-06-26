// The sequence of triangle numbers is generated by adding the natural numbers.  So the 7th triangle number would be
// 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28. The first ten terms would be:
//      1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...
// Let us list the factors of the first seven triangle numbers:
//        1: 1                   1 -> 1/1
//        3: 1,3                3 -> 3/1
//        6: 1,2,3,6            6 -> 6/1, 6/2, 6/3 (6 % 3 == 0, so it's a factor, 6 % 4 != 0, so it's not a factor)
//       10: 1,2,5,10           10 -> 10/1, 10/2, 10/5, 10/10
//       15: 1,3,5,15           15 -> 15/1, 15/3, 15/5, 15/15
//       21: 1,3,7,21
//       28: 1,2,4,7,14,28
// We can see that 28 is the first triangle number to have over five divisors.
// What is the value of the first triangle number to have over five hundred divisors?

// Whiteboard:
// * I need a function that will generate the nth triangle number (sum of 1..n)
// * I need a function that will generate the factors of a number as vector (so I can count the length)

// Wikipedia: https://en.wikipedia.org/wiki/Triangle_number says
// Tn = Sum(1..n) = (n^2 + n) / 2 = n(n+1)/2

// Postmortem:
// * there must be some mathematical formula to calculate the number of factors of a number
//   because it's not practical to iterate through all numbers to find factors over and
//   over again.  It cost about 50 mSec on lower than n=100's but as the iteration goes up
//   goes up, it gains about extrap 10mSec per 100 iterations.
//   In the end, I am at about 11,000 iterations (Triangle = 17907120, divisors = 480)
//   and stalling there for several minutes.
// * Once I submit the answer, the site gives solutions as well as writeup on how it was solved,
//   in which, I want to integrate and see how much better it performs.
// * Overall, this problem took over 2hrs to solve (it is at 2:25:10 mark) and still waiting
//   for the final answer.
//use std::cell::OnceCell;
//use lazy_static::lazy_static;

const DIVISOR_COUNT: usize = 500; // Triangle number 76576500 has 576 divisors.  Total time: 2:49:34

// embed binary file "../make_prime_table/prime_table_64k.bin" so that we can do quick prime number check
// NOTE: 76576500 will fit if prime number lookup is up to 2^27 (134,217,728)
const PRIME_TABLE_64K: &'static [u8] =
    include_bytes!("../../../make_prime_table/prime_table_65535.bin");

// Q: Can I read in TEXT file as a static array using include_str!()?
const KNOWN_PRIMES_RAW: &'static str =
    include_str!("../../../make_prime_table/known_primes_65535.txt");
fn get_known_primes() -> Vec<&'static str> {
    let lines: Vec<_> = KNOWN_PRIMES_RAW.lines().collect::<Vec<_>>();
    //for i in 0..10 { println!("{}: {}", i, lines[i]); }
    lines
}
//static KNOWN_PRIMES: Lazy<[&str; 65535]> = Lazy::new(|| {
//    KNOWN_PRIMES_RAW
//        .lines()
//        .collect::<Vec<_>>()
//        .try_into() // convert Vec<String> to [&str; 65535]
//        .unwrap()
//});
//static KNOWN_PRIMES: OnceCell<[&'static str; 65535]> = OnceCell::new();
//fn initialize_known_primes() {
//    KNOWN_PRIMES.get_or_init(|| {
//        KNOWN_PRIMES_RAW
//            .lines()
//            .collect::<Vec<_>>()
//            .try_into()
//            .unwrap()
//    });
//}
//lazy_static! {
//    static ref KNOWN_PRIMES: [&'static str; 65535] = {
//        let lines: Vec<&str> = KNOWN_PRIMES_RAW.lines().collect();
//        lines.try_into().unwrap()
//    };
//}

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

    // if n < 64K, we can just use the lookup table
    if n < 65536 {
        return PRIME_TABLE_64K[n] == 1;
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

fn triangle_number(n: usize) -> usize {
    n * (n + 1) / 2
}

// N = P1^a * P2^b * P3^c * ... * Pn^z
// where p1, p2, p3, ..., pn are prime numbers
// For example, 28 = 2^2 * 7^1
// D(N) = (a+1) * (b+1) * (c+1) * ... * (z+1)
// D(28) = (2+1) * (1+1) = 3 * 2 = 6
fn dn() -> usize {
    let KNOWN_PRIMES = get_known_primes();
    let mut n = 3;
    let mut Dn = 2;
    let mut cnt = 0;
    let mut n1 = 0;
    let mut Dn1 = 0;
    let mut exponent = 0;
    let mut current_prime_table_index: usize = 0;

    while cnt <= DIVISOR_COUNT {
        n += 1;
        n1 = n;
        if n1 % 2 == 0 {
            n1 /= 2;
        }
        Dn1 = 1;

        for i in 0..KNOWN_PRIMES.len() {
            current_prime_table_index = i;
            // read str and convert it to uint32
            let prime = KNOWN_PRIMES[current_prime_table_index]
                .parse::<usize>()
                .unwrap();
            if prime * prime > n1 {
                Dn1 *= 2;
                break;
            }
        }
        // When the prime divisor would be greater than the residual n1,
        // that residual n1 is the last prime factor with an exponent=1
        // No necessity to identify it.
        exponent = 1;
        let prime = KNOWN_PRIMES[current_prime_table_index]
            .parse::<usize>()
            .unwrap();
        while n1 % prime == 0 {
            exponent += 1;
            n1 /= prime;
        }
        if exponent > 1 {
            Dn1 *= exponent;
        }
        if n1 == 1 {
            break;
        }
        cnt = Dn * Dn1;
        Dn = Dn1;
    }

    n * (n - 1) / 2
}

fn factors(n: usize) -> Vec<usize> {
    let mut factors = Vec::new();
    let mut factor_count = 0;
    for i in 1..=n {
        if n % i == 0 {
            // NOTE: For optimization, rather than pushing to Vec, I could just increment a counter and return final count...
            factors.push(i);
        }
    }
    factors
}

fn get_factor_count(n: usize) -> usize {
    let mut factor_count = 0;
    for i in 1..=n {
        if n % i == 0 {
            factor_count += 1;
        }
    }
    factor_count
}

fn main() {
    // let mut n = 12300;  // NOTE: I already know that at 11900, Triangle=1707120 with 480 factor_count
    let mut n = 0; //
    let mut largest_so_far = 0;
    let mut largest_so_far_triangle = 0;
    let mut avg_time_msec = 0.0;
    loop {
        let start_timer = std::time::Instant::now();
        let triangle = triangle_number(n);
        //let factor_count = get_factor_count(triangle);
        let factor_count = get_factor_count(triangle);
        if factor_count > largest_so_far {
            largest_so_far = factor_count;
            largest_so_far_triangle = triangle;
            println!(
                "\n{n} Triangle number {} has {} factors",
                triangle, factor_count
            );
        }
        if factor_count > DIVISOR_COUNT {
            println!(
                "\n{n} The first triangle number to have over {} divisors is {}",
                DIVISOR_COUNT, triangle
            );
            break;
        }
        n += 1;
        if n % 100 == 0 {
            println!("Avg {avg_time_msec} mSec: Checking triangle number [{}..{}], max so far {largest_so_far} for Triangle={largest_so_far_triangle}", n, n + 100);
        }
        avg_time_msec = (avg_time_msec + start_timer.elapsed().as_millis() as f64) / 2.0;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_triangle_number() {
        assert_eq!(triangle_number(1), 1);
        assert_eq!(triangle_number(2), 3);
        assert_eq!(triangle_number(3), 6);
        assert_eq!(triangle_number(4), 10);
        assert_eq!(triangle_number(5), 15);
        assert_eq!(triangle_number(6), 21);
        assert_eq!(triangle_number(7), 28);
    }

    #[test]
    fn test_factors() {
        assert_eq!(factors(1), vec![1]);
        assert_eq!(get_factor_count(1), 1);

        assert_eq!(factors(3), vec![1, 3]);
        assert_eq!(get_factor_count(3), 2);

        assert_eq!(factors(6), vec![1, 2, 3, 6]);
        assert_eq!(get_factor_count(6), 4);

        assert_eq!(factors(10), vec![1, 2, 5, 10]);
        assert_eq!(get_factor_count(10), 4);

        assert_eq!(factors(15), vec![1, 3, 5, 15]);
        assert_eq!(get_factor_count(15), 4);

        assert_eq!(factors(21), vec![1, 3, 7, 21]);
        assert_eq!(get_factor_count(21), 4);

        assert_eq!(factors(28), vec![1, 2, 4, 7, 14, 28]);
        assert_eq!(get_factor_count(28), 6);
    }

    #[test]
    fn test_known_prime_table() {
        // check to see if some known prime number is
        // found in the prime_table_64k.bin and known_primes_64k.txt
        let my_prime = 29;
        assert_eq!(is_prime(my_prime), true);
        println!("{} is a prime number", my_prime);

        // check if it's in the prime_table_64k.bin
        assert_eq!(PRIME_TABLE_64K[my_prime], 1);
        println!("{} is in the prime_table_64k.bin", my_prime);

        let lines: Vec<_> = KNOWN_PRIMES_RAW.lines().collect::<Vec<_>>();
        for i in 0..10 {
            println!("{}: {}", i, lines[i]);
        }
        // check if it's in the known_primes_64k.txt
        let KNOWN_PRIMES = get_known_primes();
        let found = KNOWN_PRIMES.iter().any(|&x| x == my_prime.to_string());
        assert_eq!(found, true);
        println!("{} is in the known_primes_64k.txt", my_prime);
    }
}
