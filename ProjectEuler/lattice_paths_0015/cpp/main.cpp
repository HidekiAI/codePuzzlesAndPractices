
// Starting in the top left corner of a 2×2 grid, and only being able to move
// to the right and down, there are exactly 6 routes to the bottom right corner.
//  == ==      == __      == __
// |  |  !    |  !  |    |  !  |
//  -- --      -- ==      -- --
// |  |  !    |  |  !    |  !  |
//  -- --*     -- --*     -- ==>
// r,r,d,d    r,d,r,d    r,d,d,r
//
//  __ __      __ __      __ __
// !  |  |    !  |  |    !  |  |
//  == ==      == --      -- --
// |  |  !    |  !  |    !  |  |
//  -- --*     -- ==>     == ==>
// d,r,r,d     d,r,d,r    d,d,r,r
// How many such routes are there through a 20×20 grid?

// Whiteboard:
// On a NxN (square) grid, starting position is always at (0, 0)
// the question of "where is origin" is not needed because the rule is
// to only be able to go right or DOWN.  Meaning, positive X is RIGHT,
// and positive Y is DOWN.
// Next, rather than thinking of it as a grid, I'd like to think of each
// "point" as a node in a graph, and the origin (0,0) is the root node.
//                              (0,0)
//                             /     \
//                          (0,1)   (1,0)
//                          /    \ /    \
//                       (0,2)  (1,1)   (2,0)
// Note that (0,1) and (1,0) share the same child-node (1,1).  Each node
// can have range of 0, 1, or 2 children, as well as 0, 1, or 2 parents.
// Root node has no parent, and final goal node has no children.
// Though it is not relevant on how many parents a node has because the
// rule disallows going "up" (or "left") to backtrace where it has already
// visited.
// The method visit_children() will process each node, and recursively
// call itself to visit its children.  If a node has no children, it
// will return 1, otherwise it will return 0.  The sum of all children
// will be the total number of paths.

#include <iostream>      // std::cout
#include <vector>        // std::vector
#include <memory>        // std::shared_ptr
#include <unordered_map> // std::unordered_map

const long GRID_SIZE = 20;

class Nodes
{
    class NodeKey
    {
    public:
        unsigned int col;
        unsigned int row;

    public:
        bool operator==(const NodeKey &other) const
        {
            return (col == other.col) && (row == other.row);
        }

    };

    class Node
    {
    public:
        NodeKey key;
        // using shared pointer for children since multiple parents (ok, at most, 2) can be parent
        std::shared_ptr<Node> left_children;
        std::shared_ptr<Node> right_children;

    public:
        Node()
        {
            key.col = 0;
            key.row = 0;
        }
    };

public:
    Nodes()
    {
        root = std::make_unique<Node>();
        root->key.col = 0;
        root->key.row = 0;
        nodes[root->key] = root;
    }

    ~Nodes()
    {
        nodes.clear();
    }

    void init(ulong dimension)
    {
        for (unsigned int col = 0; col < dimension; ++col)
        {
            for (unsigned int row = 0; row < dimension; ++row)
            {
                NodeKey key;
                key.col = col;
                key.row = row;

                auto node = std::make_shared<Node>();
                node->key = key;
                nodes[key] = node;

                if (col > 0)
                {
                    NodeKey left_key;
                    left_key.col = col - 1;
                    left_key.row = row;
                    node->left_children = nodes[left_key];
                }

                if (row > 0)
                {
                    NodeKey right_key;
                    right_key.col = col;
                    right_key.row = row - 1;
                    node->right_children = nodes[right_key];
                }
            }
        }
    }

    long visit_children()
    {
        return visit_children(root);
    }

    // DFS 
    void visit_children(std::shared_ptr<Node> node)
    {
        if (node->left_children == nullptr && node->right_children == nullptr)
        {
            return 1;
        }

        long total_paths = 0;
        if (node->left_children != nullptr)
        {
            total_paths += visit_children(node->left_children);
        }

        if (node->right_children != nullptr)
        {
            total_paths += visit_children(node->right_children);
        }

        return total_paths;
    }

private:
    std::unordered_map<NodeKey, std::shared_ptr<Node>> nodes;
    std::unique_ptr<Node> root; // 
}; // class Nodes

void main()
{
    Nodes nodes;
    nodes.init(GRID_SIZE);
    std::cout << "Total paths: " << nodes.visit_children() << std::endl;
}