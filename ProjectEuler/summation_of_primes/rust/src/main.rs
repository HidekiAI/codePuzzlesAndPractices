// The sum of pimes below 10 is 2 + 3 + 5 + 7 = 17
// Find the sum of all the primes below two million

// Whiteboard:
// Stealing is_prime from problem one of the previously worked on prime-related problems


const MAX: u64 = 2_000_000;

fn is_prime(n: u64) -> bool {
    if n == 2 {
        return true;
    }
    if n < 2 || n % 2 == 0 {
        return false;
    }
    let mut i = 3;
    while i * i <= n {
        if n % i == 0 {
            return false;
        }
        i += 2;
    }
    true
}

fn get_sum_of_primes(max: u64) -> u64 {
    let mut sum = 0;
    if max < 2 {
        return sum;
    }
    // loop until we reach the max
    let mut current_prime = 2;
    loop {
        // Q: Are there ways to opt-out earlier, or do we have to iterate all the way to max?
        if current_prime >= max {
            break;
        }
        if is_prime(current_prime) {
            sum += current_prime;
        }
        current_prime += 1;
    }
    sum
}

fn main() {
    let sum = get_sum_of_primes(MAX);
    println!("The sum of all primes below {} is {}", MAX, sum);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_prime() {
        assert_eq!(is_prime(2), true);
        assert_eq!(is_prime(3), true);
        assert_eq!(is_prime(4), false);
        assert_eq!(is_prime(5), true);
        assert_eq!(is_prime(6), false);
        assert_eq!(is_prime(7), true);
        assert_eq!(is_prime(8), false);
        assert_eq!(is_prime(9), false);
        assert_eq!(is_prime(10), false);
    }

    #[test]
    fn test_get_sum_of_primes() {
        assert_eq!(get_sum_of_primes(10), 17);
    }
}
