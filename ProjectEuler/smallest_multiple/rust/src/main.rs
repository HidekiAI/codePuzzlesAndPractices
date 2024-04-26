// 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
// What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?

// White board:
// First, we look for the starting number, in which case, would be 10 for range of 1..10
// in which we'll do mod operation for each number (1..10) and make sure each number will
// be divisible by the number without any remainder.
// At this point, my thoughts are whether there are any way to start from higher number 
// rather than MAX, such as possibly MAX^2?  We'll come back to optimiations later after
// we get the answer.

const MAX_NUM: u64 = 20;

fn main() {
    // outer loop: start from MAX_NUM and increment by MAX_NUM (i.e. 10, 20, 30, etc)
    for i in (MAX_NUM..).step_by(MAX_NUM as usize) {
        // inner loop: from 1..MAX_NUM, verify that i is divisible by each number
        let mut is_divisible = true;
        for j in 1..MAX_NUM {
            if i % j != 0 {
                is_divisible = false;
                break;
            }
        }
        // if i is divisible by all numbers, print the result and break the loop
        if is_divisible {
            println!("The smallest positive number that is evenly divisible by all of the numbers from 1 to {} is {}", MAX_NUM, i);
            break;
        }
    }
}
