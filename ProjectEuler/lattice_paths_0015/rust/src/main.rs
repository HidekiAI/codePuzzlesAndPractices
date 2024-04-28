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

use core::fmt::{self, Formatter};
use std::collections::HashMap;

const GRID_SIZE: usize = 20;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq, Hash)]
struct NodeKey {
    row: usize,
    col: usize,
}

#[derive(Clone, Default)]
struct Node {
    // Note that a Node should not have to care about its parent
    left_child: Option<Box<Node>>,
    right_child: Option<Box<Node>>,
    node_key: NodeKey,
}
impl fmt::Debug for Node {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        write!(
            f,
            "{{ (cx:{}, ry:{}): L:{}, R:{} }}", // NOTE: unfortunately, I can't have carriage returns "\n" is ignored
            self.node_key.col,
            self.node_key.row,
            (match &self.left_child {
                Some(l) => format!("Some(cx:{}, ry:{})", l.node_key.col, l.node_key.row),
                _ => "None".to_string(),
            }),
            (match &self.right_child {
                Some(r) => format!("Some(cx:{}, ry:{})", r.node_key.col, r.node_key.row),
                _ => "None".to_string(),
            })
        )
    }
}
impl Node {
    fn new(left: Option<Box<Node>>, right: Option<Box<Node>>, row: usize, col: usize) -> Node {
        Node {
            left_child: left,
            right_child: right,
            node_key: NodeKey { row, col },
        }
    }

    // returns 0 if neither children can reach goal, otherwise 1 if either or 2 if both children can reach goal
    fn can_children_reach_goal(node: Node) -> u32 {
        if node.left_child.is_none() && node.right_child.is_none() {
            //println!("*");
            return 1;
        }

        // RIGHT: right child
        let reached_goal_right = if let Some(right_child) = node.right_child.clone() {
            //print!("r");
            Self::can_children_reach_goal(*right_child)
        } else {
            0
        };

        // Down: left child
        let reached_goal_left = if let Some(left_child) = node.left_child.clone() {
            //print!("d");
            Self::can_children_reach_goal(*left_child)
        } else {
            0
        };

        reached_goal_left + reached_goal_right
    }
}

#[derive(Debug, Default)]
struct Nodes {
    root: Option<Node>,
    nodes: HashMap<NodeKey, Node>,
    grid_size: usize,
}

impl Nodes {
    fn new() -> Nodes {
        Nodes {
            root: None,
            nodes: HashMap::new(),
            grid_size: 0,
        }
    }

    fn init(&mut self, dimension: usize) {
        println!(
            "Initializing Nodes with dimension: {} x {} graph",
            dimension, dimension
        );
        self.grid_size = dimension;
        let mut nodes_mapped: HashMap<NodeKey, Node> = HashMap::new();
        Self::make_nodes(0, 0, &dimension, &mut nodes_mapped);

        // truthfully, we only care about the values but by keeping track of key via (row,col)
        // we can even lookup even quicker if we need to find a specific node
        self.root = nodes_mapped
            .clone()
            .get(&NodeKey { row: 0, col: 0 })
            .cloned();
        self.nodes = nodes_mapped;

        //for node in self.nodes.clone() {
        //    println!("{:?}", node);
        //}
    }

    // recursively build a tree of nodes
    fn make_nodes(
        row: usize,        // 0-based
        col: usize,        // 0-based
        dimension: &usize, // Square (NxN), i.e. 2x2 is 2, and range is inclusive (i.e. 0..=2)
        nodes: &mut HashMap<NodeKey, Node>,
    ) -> Option<Box<Node>> {
        if dimension < &2 {
            panic!("Invalid dimension: {}", dimension);
        }

        if row > *dimension || col > *dimension {
            panic!(
                "Invalid row or col: ({}, {}) - where max={}",
                row, col, dimension
            );
        }

        // Firstly, for a 2x2 grid, there are 9 nodes; which basically means that it is ({0..=2} , {0..=2}) (3x3)
        let max_vert_count = dimension + 1;

        // * both col and row are 0-based, and assumed to ALWAYS be positive (usize)
        // * as long as col and row are less than max_vert_count, it has at least 1 child
        // * if col is 0, it has no left child - i.e. (2, 0) is bottom-left corner
        // * if row is 0, it has no right child - i.e. (0, 2) is top-right corner
        // * if col == dimension (inclusive), it has no right child - i.e. (1, 2) is middle-right edge
        // * if row == dimension (inclusive), it has no left child - i.e. (2, 1) is bottom-middle edge
        // * if col > 0 && row > 0 && col < dimension && row < dimension, it has 2 children - i.e. (1, 1) is middle node
        let mut left_child = None;
        let mut right_child = None;

        // condition of when to stop:
        // * if we've visited all the nodes at least once, we're done
        // on a 2x2 grid, it's actually 3x3 because it's 0..=2.  In which
        // there are 9 vertices, and 12 edges.  Ideally, if we can just track
        // count of edges, we can just stop when we've visited all the edges.
        // but I'm too lazy to figure out the formula required to calculate
        // number of edges on a NxN grid, I'll just count number of created
        // vertices.  This method is not efficient, but it's a brute-force
        // because the same vert can be visited more than once.
        if nodes.len() == (max_vert_count * max_vert_count) {
            // doesn't mean we're done, since 2 parents will have a child node of the goal-node...
            // but we'll just opt out
            println!("################# All nodes have been created");
            return None;
        }
        match (row, col) {
            // origin
            (0, 0) => {
                // must be parent node, it has both left and right child
                left_child = Self::make_nodes(row + 1, col, &dimension, nodes);
                right_child = Self::make_nodes(row, col + 1, &dimension, nodes);
                ()
            }
            // bottom-right corner, disallow both left and right branches
            (r, c) if r == *dimension && c == *dimension => {
                // must be the bottom-right corner (goal), it has no children
                left_child = None;
                right_child = None;
                ()
            }
            // bottom row, disallow left branches
            (r, _) if r == *dimension => {
                // must be the bottom row, it has no left child
                left_child = None;
                right_child = Self::make_nodes(row, col + 1, &dimension, nodes);
                ()
            }
            // right column, disallow right branches
            (_, c) if c == *dimension => {
                // must be the right column, it has no right child
                left_child = Self::make_nodes(row + 1, col, &dimension, nodes);
                right_child = None;
                ()
            }
            _ => {
                // NOTE: Top row and left columns are treated SAME like normal nodes
                // all other nodes have both left and right child
                left_child = Self::make_nodes(row + 1, col, &dimension, nodes);
                right_child = Self::make_nodes(row, col + 1, &dimension, nodes);
                ()
            }
        } // match

        let new_node = Node::new(left_child, right_child, row, col);
        nodes.insert(NodeKey { row: row, col: col }, new_node.clone()); // alternatively, if we made this as HashMap with key=(col,row), we can avoid this check because it'll just upsert...
                                                                        //println!(
                                                                        //    "Added Node: {:?} (dim={}) - count={}",
                                                                        //    new_node,
                                                                        //    dimension,
                                                                        //    nodes.len()
                                                                        //);
        Some(Box::new(new_node))
    }

    fn get(&self, key: NodeKey) -> Option<&Node> {
        self.nodes.get(&key)
    }

    fn get_root(&self) -> Option<&Node> {
        match self.grid_size {
            0 => panic!("Grid size is not initialized"),
            _ => match self.get(NodeKey { row: 0, col: 0 }) {
                Some(root) => Some(root),
                None => panic!("Root node not found"),
            },
        }
    }

    fn visit_children(&self) -> u32 {
        // there's only ONE start and ONE end, so we can just call the root node
        // and recursively visit its children
        let current_node = self.get_root().unwrap();
        Node::can_children_reach_goal(current_node.clone())
    }
} // Nodes

fn main() {
    let mut nodes = Nodes::new();
    nodes.init(GRID_SIZE);
    let total_paths = nodes.visit_children();
    println!("Total paths: {}", total_paths);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_make_new_nodes() {
        // O  R1 R2
        // L1 M1 M2
        // L2 M3 G
        // O: (0, 0)
        // R1: (0, 1)
        // R2: (0, 2)
        // L1: (1, 0)
        // M1: (1, 1)
        // M2: (1, 2)
        // L2: (2, 0)
        // M3: (2, 1)
        // G: (2, 2)
        // total of 9 nodes
        let grid_size = 2;
        let mut nodes = Nodes::new();
        nodes.init(grid_size);
        assert_eq!(nodes.nodes.len(), (grid_size + 1) * (grid_size + 1));
    }

    #[test]
    fn test_visit_children() {
        let grid_size = 2;
        let mut nodes = Nodes::new();
        nodes.init(grid_size);
        let total_paths = nodes.visit_children();
        assert_eq!(total_paths, 6);
    }
}
