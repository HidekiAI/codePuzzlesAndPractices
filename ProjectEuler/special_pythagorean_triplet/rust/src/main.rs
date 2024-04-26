// A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
//      a^2 + b^2 = c^2
// For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.
// There exists exactly one Pythagorean triplet for which a + b + c = 1000.
// Find the product abc.

// Whiteboard:
// a^2 + b^2 = c^2
// and a < b < c
// a = 3, b = 4, then:
// 3^2 + 4^2 = 9 + 16 = 25
// c = sqrt(25) = 5
// a(3) < b(4) < c(5) - passes the a < b < c test
// N = a + b + c
// N = 3 + 4 + 5 = 12
// Knowing N = 12, can we find a, b, c?
// We know that a, b, and c summed together, must be 12
// so combinations we'd have are:
// a = 0, b = 1, c = 11 // b cannot be 0 due to the rule a < b < c
//      0^2 + 1^2 != 11^2   // fails the test
// a = 0, b = 2, c = 10
//      0^2 + 2^2 != 10^2   // fails the test
// ...
// a = 3, b = 4, c = 5
//      3^2 + 4^2 = 5^2  // passes the test
// a = 3, b = 5, c = 4 // b cannot be greater than c, so stop here (did we need to even check this once we've passed the test above?)
// I will need the following methods:
// * A method in which, given 'a and 'b, will return Some('c) in which 'a < 'b < 'c, else None
// * tests 'a + 'b + 'c = 'N
// For iterations, I will start with a = 0, b = 1, c = N - b in which a is the outer most loop,
// b is the inner loop, and c is calculated as N - b

const N: u64 = 1000;    // N = 1000 -> product of a*b*c = 31875000 where a = 200, b = 375, c = 425  

// given 'a and 'b, will return Some('c) in which 'a < 'b < 'c, else None
// TODO: In future, should make it generic rather than futzing with u32 vs u64...
fn find_c(a: u64, b: u64) -> Option<u64> {
    if a == b {
        // should panic?
        return None;
    }
    let mut left = a;
    let mut right = b;
    if a > b {
        left = b;
        right = a;
    }

    // c^2 = a^2 + b^2
    let c_squared = left.pow(2) + right.pow(2); // we'll use N.pow(2) here instead of (N * N) since we're including math to calculate sqrt()
    let c = (c_squared as f64).sqrt() as u64;
    // verify that a < b < c
    if left < right && right < c && c.pow(2) == c_squared {
        Some(c)
    } else {
        None
    }
}

// tests N = a + b + c
fn test_n(a: u64, b: u64, c: u64, n: u64) -> bool {
    if a < b && b < c {
        return a + b + c == n;
    }
    false
}

fn do_it(n: u64) -> Option<(u64, u64, u64)> {
    for a in 0..n { // probably should start from 1 since a^2 + b^2 == b^2 if a==0
        // note: b > a, so we start at a+1
        for b in a + 1..n {
            if let Some(c) = find_c(a, b) {
                if test_n(a, b, c, n) {
                    return Some((a, b, c));
                }
            }
        }
    }
    None
}

fn main() {
    let result = do_it(N);
    println!("N={N}, {:?}", result);
    println!("Product of a*b*c = {}", result.unwrap().0 * result.unwrap().1 * result.unwrap().2);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_find_c() {
        assert_eq!(find_c(3, 4), Some(5));
        assert_eq!(find_c(3, 5), None);
        assert_eq!(find_c(3, 3), None);
    }

    #[test]
    fn test_test_n() {
        assert_eq!(test_n(3, 4, 5, 12), true);
        assert_eq!(test_n(3, 5, 4, 12), false); // b > c
        assert_eq!(test_n(3, 4, 5, 42), false); // a + b + c != N
        assert_eq!(test_n(3, 5, 5, 13), false); // b == c
        assert_eq!(test_n(3, 5, 4, 12), false); // b > c
    }

    #[test]
    fn test_do_it() {
        assert_eq!(do_it(12), Some((3, 4, 5)));
    }
}
