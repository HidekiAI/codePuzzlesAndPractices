// 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
// What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?

// White board:
// First, we look for the starting number, in which case, would be 10 for range of 1..10
// in which we'll do mod operation for each number (1..10) and make sure each number will
// be divisible by the number without any remainder.
// At this point, my thoughts are whether there are any way to start from higher number 
// rather than MAX, such as possibly MAX^2?  We'll come back to optimiations later after
// we get the answer.

#include <iostream> // for cout

const int MAX = 20;

int main() {
    int num = MAX;
    bool found = false;
    while (!found) {
        bool divisible = true;
        for (int i = 1; i <= MAX; i++) {
            if (num % i != 0) {
                divisible = false;
                break;
            }
        }
        if (divisible) {
            found = true;
        } else {
            num++;
        }
    }
    std::cout << "Smallest positive number that is evenly divisible by all of the numbers from 1 to " << MAX << " is " << num << std::endl;
    return 0;
}