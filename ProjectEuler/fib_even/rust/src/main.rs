// By considering the terms in the Fibonacci sequence whose values do not exceed four million, find the sum of the even-valued terms.

const MAX : u32 = 4000000;

fn fn_fib_rec(n: u32) -> u32 {
    if n == 0 {
        return 0;
    } else if n == 1 {
        return 1;
    } else {
        return fn_fib_rec(n - 1) + fn_fib_rec(n - 2);
    }
}

fn main() {
    let mut sum = 0;
    let mut i = 0;
    loop {
        let fib = fn_fib_rec(i);
        if fib > MAX {
            break;
        }
        if fib % 2 == 0 {
            sum += fib;
        }
        i += 1;
    }
    println!("Sum of all the even-valued terms in the Fibonacci sequence whose values do not exceed four million is: {}", sum);
}
