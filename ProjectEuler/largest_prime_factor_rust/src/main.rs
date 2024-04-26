// The prime factors of 13195 are 5, 7, 13, and 29
// What is the largest prime factor of number 600851475143?
const NUM: u64 = 600851475143;

// Prime number: a number greater than 1 that has no positive divisors other than 1 and itself.
// i.e. 5, 7, 13, and 29 are prime numbers
fn is_prime(n: u64) -> bool {
    // 2 is a known prime number
    if n == 2 {
        return true;
    }
    // if divisible by 2, it's not a prime number, also 0 and 1 are not prime numbers
    if n < 2 || n % 2 == 0 {
        return false;
    }

    // if n = 13, the largest divisor is 13, which is less than sqrt(13) = 3.6  
    // if n = 25, the largest divisor is 5, which is less than sqrt(25) = 5
    // if n = 29, the largest divisor is 29, which is less than sqrt(29) = 5.4
    // hence if n is not a prime number, it must have a divisor less than its square root
    // so for 25 = 5 * 5, the largest divisor is 5, which is less than sqrt(25) = 5
    // for 29 = 29 * 1, the largest divisor is 29, which is less than sqrt(29) = 5.4
    // If a number n has a divisor greater than its square root, it must also have a corresponding 
    // divisor smaller than its square root. Therefore, we only need to check up to the square root.
    let mut i = 3;  // we've already tested for 0..2, so start from 3
    while i * i <= n {
        if n % i == 0 {
            return false;   // opt out early
        }
        i += 1; // can I do +2 here?
    }
    // passed all the tests between 2 and sqrt(n), this number is prime
    return true;
}


fn main() {
    // iterate from 2 to the square root of NUM and sum it
    let mut i = 2;
    let mut largest_prime = 0;
    for i in 2..(NUM as f64).sqrt() as u64 {
        if NUM % i == 0 && is_prime(i) {
            largest_prime = i;
        }
    }
    println!("The largest prime factor of {} is {}", NUM, largest_prime);
}
