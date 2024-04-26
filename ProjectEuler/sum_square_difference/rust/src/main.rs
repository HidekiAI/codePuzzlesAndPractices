// The sum of squares of the frist ten natural numbers is,
//      1^2 + 2^2 + ... + 10^2 = 385
// The square of the sum of the first ten natural numbers is,
//      (1 + 2 + ... + 10)^2 = 55^2 = 3025
// Hence the difference between the sum of the squares of the 
// first ten natural numbers and the square of the sum is 
// 3025 - 385 = 2640.
// Find the difference between the sum of the squares of the
// first one hundred natural numbers and the square of the sum.

// Whiteboard:
// define a function that takes in a number and returns the sum of the squares of the first n natural numbers
// define a function that takes in a number and returns the square of the sum of the first n natural numbers

const N: i32 = 100;

fn sum_of_squares(n: i32) -> i32 {
    let mut sum = 0;
    for i in 1..n+1 {
        sum += i * i ; // pow(2);
    }
    sum
}

fn square_of_sum(n: i32) -> i32 {
    let mut sum = 0;
    for i in 1..n+1 {
        sum += i;
    }
    sum * sum
}

fn main() {
    let sum_of_squares = sum_of_squares(N);
    let square_of_sum = square_of_sum(N);
    let difference = square_of_sum - sum_of_squares;
    println!("The difference between the sum of the squares of the first one hundred natural numbers and the square of the sum is: {}", difference);
}
