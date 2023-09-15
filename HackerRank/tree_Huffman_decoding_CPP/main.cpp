//
//  main.cpp
//  Huffman
//
//  Created by Vatsal Chanana

#include<bits/stdc++.h>
using namespace std;


// Post-mortem:
// Wasn't sure whether I've met the 30-minutes time limit since timer broke...
// Things that I got stuck on:
// * when returning back to root node, I was recursing with si+1 instead of si
// * when testing for end of string, I was using si >= s.size() instead of si > s.size()
// The other mock-test, I wasn't able to use functions and lambdas (it was explicitly)
// defined to not allow c++11 and up, so at first, I was trying to figure out whether
// this was also the case (which this one allowed lambdas/functins)
// Overall, the time spent more was on trying to follow the example more than coding.

typedef struct node {
    
  	int freq;
    char data;
    node * left;
    node * right;
    
} node;

struct deref:public binary_function<node*, node*, bool> {
  	bool operator()(const node * a, const node * b)const {
        return a->freq > b->freq;
    }
};

typedef priority_queue<node *,vector<node*>, deref> spq;

node * huffman_hidden(string s) {

    spq pq;
    vector<int>count(256, 0);
  
    for(int i = 0; i < s.length(); i++) {
        count[s[i]]++;
    }
    
    for(int i = 0; i < 256; i++) {
        
        node * n_node = new node;
        n_node->left = NULL;
        n_node->right = NULL;
        n_node->data = (char)i;
        n_node->freq = count[i];
      
        if( count[i] != 0 )
        	pq.push(n_node);
      
    }
    
    while( pq.size() != 1 ) {
      
        node * left = pq.top();
        pq.pop();
        node * right = pq.top();
        pq.pop();
        node * comb = new node;
        comb->freq = left->freq + right->freq;
        comb->data = '\0';
        comb->left = left;
        comb->right = right;
        pq.push(comb);
      
    }
    
    return pq.top();
    
}

void print_codes_hidden(node * root, string code, map<char, string>&mp) {
    
  	if(root == NULL)
        return;
    if(root->data != '\0') {
        mp[root->data] = code;
    }
  
    print_codes_hidden( root->left, code+'0', mp );
    print_codes_hidden( root->right, code+'1', mp );
    
}


/* 
The structure of the node is

typedef struct node
{
    int freq;
    char data;
    node * left;
    node * right;
    
}node;

*/

void decode_huff(node * root, string s) {
    // s="1001011"
    // root -> (null,5) +-> (A,3)
    //                  +-> (null,2) +-> (C,1)
    //                               +-> (B,1) 
    // 1. S[0]='1', move to right, found (A,3), return to root
    // 2. S[1]='0', to left (null,2)
    // 3. S[2]='0', to left, found (B,1), return to root
    // 4. S[3]='1', to right (A,3), return to root
    // 5. S[4]='0', to left
    // 6. S[5]='1' to right, found (C,1), to root
    // 7. S[6]='1', to right (A,3)
    // Result: "ABACA" 3-A's, 1-B, 1-C
    // function: Recursively visit each node based on current
    // index to to the string s[] and current node until we
    // encounter a node that has either (or both) left and right
    // child as nullptr, then we know we have reached a leaf node
    // we'll print the data of the leaf node and return to the
    // so that we do not have to persist the string/result
    function<void(node*, int)> visit = [&](node *node, int si) {
        if(node == nullptr) {
            return;
        }
        if (si > s.size()) {
            // if we've reached the end of the string, we're done!
            return;
        }
        if(node->left == nullptr || node->right == nullptr) {
            // we've reached the leaf node, just print it, and return
            // to root node
            std::cout << node->data;
            visit(root, si);  // return to root
        }
        auto code = s[si];
        if(code == '0') {
            // visit left
            visit(node->left, si+1);
        }
        else {
            // visit right
            visit(node->right, si+1);
        }
    };

    visit(root, 0);
}

int main() {
    
  	string s;
    std::cin >> s;
  
    node * tree = huffman_hidden(s);
    string code = "";
  
    map<char, string> mp;
    print_codes_hidden(tree, code, mp);
    
    string coded;
  
    for(int i = 0; i < s.length(); i++) {
        coded += mp[s[i]];
    }
    
    decode_huff(tree, coded);
  
    return 0;
}