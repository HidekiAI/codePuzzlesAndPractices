// By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.
// What is the 10001st prime number?

// Whiteboard:
// Either a vector or counter to keep track of prime numbers up to the 10,001st prime number.
// Since the vector will just be wastefull, I will use a counter.
// I need a function that would take in a number and return true if it is a prime number.
// Though that is trivial, I'll just copy and paste the is_prime function from the largest_prime_factor snippet.
// And finally, just iterate from 2 to infinity and keep track of the number of prime numbers found
// up until the 10,001st prime number is found.

const N: i32 = 10001;

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
        i += 1;
    }
    return true;
}

fn main() {
    // Start from 2 since 1 is not a prime number.
    let mut prime_count = 0;
    let mut i = 2;
    while prime_count < N {
        if is_prime(i) {
            prime_count += 1;
            if prime_count == N {
                println!("The {}st prime number is: {}", N, i);
                break;
            }
        }
        i += 1;
    }
}
