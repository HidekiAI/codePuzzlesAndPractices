#include <bits/stdc++.h>

using namespace std;


/*
 * Complete the class below.
 */
class Stack 
{
public:
    Stack(){current_stack_index = 0; current_stack_tail = 0;}
    ~Stack(){};
    void Push(int n)
    {
       const auto kMax_stack_count = 256;
       const auto kMax_stack_depth = 4 * 1024;
       if(current_stack_tail + 1 > kMax_stack_depth)
       {
           // use next stack
           ++current_stack_index;
           current_stack_tail = 0;
           if (current_stack_index > kMax_stack_count)
           {
               throw runtime_error("Out of preserved memory, increase stack size!");
           }
       }
       else {
           ++current_stack_tail;
       }
       // now, finally push it
       stack[current_stack_index][current_stack_tail] = n;
    }
    int Pop() // will never get called more than Push
    {
        auto ret =  stack[current_stack_index][current_stack_tail];
        if ((current_stack_tail - 1) < 0)
        {
            current_stack_tail = 0;
            if((current_stack_index - 1) < 0)
            {
                throw runtime_error("Programmer error or data error?  there is more pops then pushes!");    
            }
            else {
            --current_stack_index;        
                
            }
        }
        else {
           --current_stack_tail ; 
        }
        return ret;
    }

private:
    // not allowed to use STL containers, will use simple array
    // Description says: "This container may be used to store anywhere from zero to million values"
    // so we need to make sure we can handle up to 1M (1024x1024)
    // 256 X 4K = 1M
    // we'll start off with 256 sub-stacks of 4K each and if I get memory issue, will increase?
    int stack[256][1024*4];  // 4K is most memory efficient on avoiding swap cache-miss (I may be wrong, but that's what it was like 10-15 years ago...)
    int current_stack_index = 0;
    int current_stack_tail = 0;
};

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int numValues = 0;
    cin >> numValues;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Stack stack;
    for(int i = 0; i < numValues; i++) {
        int value;
        cin >> value;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        stack.Push(value);
    }

    for(int i = 0; i < numValues; i++) {
        fout << stack.Pop() << endl;
    }  

    fout.close();
    return 0;
}
