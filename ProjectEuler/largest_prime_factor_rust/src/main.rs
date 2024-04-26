// The prime factors of 13195 are 5, 7, 13, and 29
// What is the largest prime factor of number 600851475143?
const NUM: ui64 = 600851475143;

fn is_prime(n: ui64) -> bool {
    if n == 2 {
        return true;
    }
    if n < 2 || n % 2 == 0 {
        return false;
    }
    // If a number n has a divisor greater than its square root, it must also have a corresponding 
    // divisor smaller than its square root. Therefore, we only need to check up to the square root.
    let mut i = 3;  // first odd number
    while i * i <= n {
        if n % i == 0 {
            return false;   // opt out early
        }
        i += 2; // skip all even numbers
    }
    return true;
}

fn main() {

}
