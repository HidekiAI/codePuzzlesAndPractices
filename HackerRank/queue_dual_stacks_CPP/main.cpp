#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

// NOTE: In this quiz, we DO NOT use any of the STL container
// classes that can easily understand how to pop from the head
// nor push to the tail...  The whole purpose of this excercise
// is to make all that happen with dual stacks where
// you push to the in-stack and pop from the out-stack.
template <typename TElement> class DualStacksQueue {
private:
  class Stack {
  private:
    // it's too tedious to do new/delete static array elements, so we'll use
    // vector instead for memory management purpose only but not to
    // do anything special with vector
    vector<TElement> elements; // best we will do is push_back and pop_back

  public:
    // push to the tail
    void Push(TElement element) { elements.push_back(element); }
    // pop from the tail
    TElement Pop() {
      TElement element = elements.back();
      elements.pop_back();
      return element;
    }
    bool IsEmpty() { return elements.empty(); }
    // a method in which we get the head value without popping and is ONLY
    // useful for in_stack, not for out_stack since we're inspecting the HEAD
    // (very bottom) of the stack
    TElement Peek_bottom() { return *elements.begin(); }
  };

public:
  // 2: Dequeue the element at the front of the queue.
  // only pops the tail of the stack.  The problem here
  // is that if we do not restore in_stack back to its original
  // order, then we'll have a problem on the next dequeue
  // if the out_stack is not empty because it'll now push
  // to out_stack in wrong order (out_stack's tail is assumed
  // to be bottom/head of in_stack)
  // so we'll pop only one from out_stack, then restore in_stack
  // in the original order
  TElement Dequeue() {
    // fill out_stack with in_stack
    while (!in_stack.IsEmpty()) {
      out_stack.Push(in_stack.Pop());
    }
    // pop the tail of out_stack and save it to return
    auto element = out_stack.Pop();
    // now restore in_stack back to original order
    while (!out_stack.IsEmpty()) {
      in_stack.Push(out_stack.Pop());
    }
    return element;
  }
  // 1 x: Enqueue element  into the end of the queue.
  void Enqueue(TElement element) {
    in_stack.Push(element);
  }
  // 3: Print the element at the FRONT of the queue. (just the head) but do NOT
  // pop
  void Print() {
    // get the element (without popping) from the in_stack and push to the
    // out_stack but do NOT pop assume that the in_stack is never empty when
    // this is called we also do not need to deal with out_stack since we're not
    // poppping anything
    cout << in_stack.Peek_bottom() << endl;
  }

private:
  // basically, dual-stack has the effect of reversing the order of the stack
  // stacks have the charateristic of LIFO (last-in-first-out) but with
  // dual-stack we can achieve FIFO (first-in-first-out) by pushing to one stack
  // and popping from another stack
  // for example: Push 1, Push 2, Push 3
  // would make stack look like (from tail to head because we'll pop from tail):
  // 3, 2, 1 But we want to pop from head, so we'll push to another stack and
  // pop from that stack.  So we'll push 3, 2, 1 to another stack and pop from
  // that stack, we'll get 1, 2, 3
  // to achieve this, whenever we dequeue, we'll first pop from in_stack all the
  // way up until it is empty and push to out_stack.  Then we'll pop (just one)
  // from out_stack.  We then need to preserve the order of in_stack so we'll
  // then again pop from out_stack and push to in_stack.  This way, we'll
  // preserve the order
  Stack in_stack;
  Stack out_stack; // should always be empty when we dequeue (once head is
                   // popped, we push back in order to in_stack)
};

const int kEnqueue = 1;
const int kDequeue = 2;
const int kPrint = 3;

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT */
  auto queue = DualStacksQueue<int>();
  auto queue_count = 0;
  cin >> queue_count;

  for (auto q = 0; q < queue_count; ++q) {
    auto command = 0;
    cin >> command;
    switch (command) {
    case kEnqueue: {
      auto element = 0;
      cin >> element;
      queue.Enqueue(element);
      break;
    }
    case kDequeue: {
      queue.Dequeue();
      break;
    }
    case kPrint: {
      queue.Print();
      break;
    }
    default:
      break;
    }
  } // for

  return 0;
}