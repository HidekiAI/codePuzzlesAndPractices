#include <bits/stdc++.h>
using namespace std;
/* A binary tree node has data, pointer to left child
   and a pointer to right child */
struct Node
{
    int data;
    struct Node* left;
    struct Node* right;
};
/* Helper function that allocates a new node with the
   given data and NULL left and right pointers. */
struct Node* newNode(int data)
{
  struct Node* node = new Node;
  node->data = data;
  node->left = NULL;
  node->right = NULL;
  return(node);
}
/* Computes the number of nodes in a tree. */
int height(struct Node* node);
void inorder(Node *root)
{
    if (root == NULL)
       return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);    
}
/* Driver program to test size function*/
int main()
{
  int t;
  scanf("%d", &t);
  while (t--)
  {
     map<int, Node*> m;
     int n;
     scanf("%d",&n);
     struct Node *root = NULL;
     struct Node *child;
     while (n--)
     {
        Node *parent;
        char lr;
        int n1, n2;
        scanf("%d %d %c", &n1, &n2, &lr);
        if (m.find(n1) == m.end())
        {
           parent = newNode(n1);
           m[n1] = parent;
           if (root == NULL)
             root = parent;
        }
        else
           parent = m[n1];
        child = newNode(n2);
        if (lr == 'L')
          parent->left = child;
        else
          parent->right = child;
        m[n2]  = child;
     }
     cout << height(root) << endl;
  }
  return 0;
}

/*Please note that it's Function problem i.e.
you need to write your solution in the form of Function(s) only.
Driver Code to call/invoke your function is mentioned above.*/

/* Tree node structure  used in the program
 struct Node
 {
     int data;
     Node* left, *right;
}; */
int walk(Node* node, int depth);
/* Computes the height of binary tree with given root.  */
int height(Node* node)
{
    // assume node passed is the root - see if it has any edges
    auto h = node->left != NULL || node->right != NULL ? 1 : 0;
    if (h > 0)
    {
        return walk(node, 1);
    }
    return h;
}
int walk(Node* node, int depth)
{
    //cout << std::endl << "D:" << depth << " L:" << (node->left ? "1" : "0") << " R:" << (node->right ? "1" : "0") << std::endl;
    if (node->left)
    {
        return walk(node->left, depth + 1);
    }
    if (node->right)
    {
        return walk(node->right, depth + 1);
    }
    return depth + (node->left != NULL || node->right != NULL ? 1 : 0);
}