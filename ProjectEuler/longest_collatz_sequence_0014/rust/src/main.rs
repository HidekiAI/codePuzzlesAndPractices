// The following iterative sequence is defined for the set of positive integers:
//      n → n/2 (n is even)
//      n → 3n + 1 (n is odd)
// Using the rule above and starting with 13, we generate the following sequence:
//      13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1
// It can be seen that this sequence (starting at 13 and finishing at 1) contains 10 terms.
// Although it has not been proved yet (Collatz Problem), it is thought that all starting
// numbers finish at 1.
// Which starting number, under one million, produces the longest chain?
// NOTE: Once the chain starts the terms are allowed to go above one million.

// Whiteboard:
// Firstly, immediately my thoughts are, I need to do some embarassingly-parallel work here for
// it's just insane amount of iterations (1mil) to do in a single thread.
// First instinct also is, rather than iterating from 1 to 1mil, I can start from 1mil and go down
// to 1. Though truth be told, we do NOT know what is the "longest chain", hence no matter
// which direction we go, we will have to iterate through all numbers.  It's probably not a
// good assumptions to make that just because starting-number-13 took 10 terms, 14 will take
// more than 10 terms, in fact, it could take less (i.e. 16 is 5 chains: 16 → 8 → 4 → 2 → 1).
// For parallelism, I will simply divide and conquer.  And if possible, split into 4 threads
// in which each thread will take 250k numbers.  I will then merge the results and find the
// longest chain.
// I will only keep the longest chain count (not the actual sequence) because there are no
// way I'd want to keep 1mil sequences in memory.  All we need is the current number in which
// one can determine next number based on even/odd.  So at best, each thread just tracks
// * Starting number that produced the longest chain
// * Number of chains it took to reach 1 for the above starting-number
// Note that if you have the starting number, even though it may take long time (i.e. 1mil chains)
// you can still recreate it (it's deterministic) and hence no need to keep the sequence.

const MAX: u64 = 1_000_000;
const THREADS: u64 = 4;
const CHUNK: u64 = MAX / THREADS;
// Total time: 256.167626ms
// Longest chain: 837799 (525 chains)

// This function will take a number and return the number of chains it took to reach 1
fn collatz(starting: u64) -> u64 {
    let start_time = std::time::Instant::now();
    let mut count = 1;
    let mut n = starting;
    while n != 1 {
        n = if n % 2 == 0 { n / 2 } else { 3 * n + 1 };
        count += 1;
    }
    //println!(
    //    "{} chains for {} in {:?}",
    //    count,
    //    starting,
    //    start_time.elapsed()
    //);
    count
}

// This function will take a range of numbers and return the number that produced the longest chain
fn longest_chain(start: u64, end: u64) -> (u64, u64) {
    let start_time = std::time::Instant::now();
    let mut longest = (0, 0);
    for i in start..end {
        let count = collatz(i);
        if count > longest.1 {
            longest = (i, count);
        }
    }
    println!("Thread {} took {:?}", start, start_time.elapsed());
    longest
}

fn main() {
    let start_time = std::time::Instant::now();
    let mut handles = vec![];
    for i in 0..THREADS {
        let start = i * CHUNK + 1;
        let end = (i + 1) * CHUNK;
        let handle = std::thread::spawn(move || longest_chain(start, end));
        handles.push(handle);
    }

    let mut longest = (0, 0);
    for handle in handles {
        let result = handle.join().unwrap();
        if result.1 > longest.1 {
            longest = result;
        }
    }
    println!("Total time: {:?}", start_time.elapsed());
    println!("Longest chain: {} ({} chains)", longest.0, longest.1);
}

#[cfg(test)]
mod tests {
    use super::*;

    // test for just single thread
    #[test]
    fn test_collatz() {
        assert_eq!(collatz(13), 10); // 13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1
        assert_eq!(collatz(14), 18); // 14 → 7 → 22 → 11 → 34 → 17 → 52 → 26 → 13 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1
        assert_eq!(collatz(15), 18); // 15 → 46 → 23 → 70 → 35 → 106 → 53 → 160 → 80 → 40 → 20 → 10 → 5 → 16 → 8 → 4 → 2 → 1
        assert_eq!(collatz(16), 5); // 16 → 8 → 4 → 2 → 1
    }
}
