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

use core::{
    fmt::{self, Formatter},
    panic,
};
use std::{collections::HashMap, mem};

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
    fn new(left: Option<Box<Node>>, right: Option<Box<Node>>, xy_key: NodeKey) -> Node {
        Node {
            left_child: left,
            right_child: right,
            node_key: xy_key,
        }
    }
}

#[derive(Debug, Default)]
struct Nodes {
    nodes: HashMap<NodeKey, Node>,
    grid_size: usize,
}

impl Nodes {
    fn new() -> Nodes {
        Nodes {
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
        self.nodes = HashMap::new();
        //Self::make_nodes_rec(NodeKey { row: 0, col: 0 }, &dimension, &mut self.nodes);
        self.make_nodes_iter();

        if cfg!(debug_assertions) {
            // truthfully, we only care about the values but by keeping track of key via (row,col)
            // we can even lookup even quicker if we need to find a specific node
            let found_root = self
                .nodes
                .clone()
                .get(&NodeKey { row: 0, col: 0 })
                .cloned()
                .expect("Unable to locate root node!");
            self.get_root(); // should panic if root not found...
        }

        if cfg!(debug_assertions) {
            //for node in self.nodes.clone() {
            //    println!("{:?}", node);
            //}
        }
    }


    fn make_nodes_rec(
        xy: NodeKey,
        dimension: &usize, // Square (NxN), i.e. 2x2 is 2, and range is inclusive (i.e. 0..=2)
        nodes: &mut HashMap<NodeKey, Node>,
    ) -> Option<Box<Node>> {
        if cfg!(debug_assertions) {
            //println!("Making Node: (ry:{}, cx:{})", xy.row, xy.col);

            //println!("Making Node: (ry:{}, cx:{})", xy.row, xy.col);
            //// memory checking whenever col==0, col==dim/2 or col==dimension just to see where we run out of memory
            //if xy.col == 0 || xy.col == *dimension / 2 || xy.col == *dimension {
            //    // On Linux if you see a message "Killed", it's because it ran out of memory
            //    // if you have "oom-killer" enabled (systemd) - I LOVE this feature!
            //    // do a 'sudo dmesg' to verify that it was killed for that reason (should say "Out of memory")...
            //    // I'm still at wonder of how it (intelligently) knows which process is the correct process to kill...
            //    // The following debug is for Windows...
            //    let mem = sys_info::mem_info().unwrap();
            //    println!(
            //        "Memory check: {:?}, Free: {} bytes ({:.5} MB | {:.5} GB)",
            //        xy,
            //        mem.free,
            //        mem.free as f32 / 1024.0,
            //        mem.free as f32 / 1024.0 / 1024.0
            //    );
            //}
        }
        if dimension < &2 {
            panic!("Invalid dimension: {}", dimension);
        }

        if xy.row > *dimension || xy.col > *dimension {
            panic!("Invalid row or col: {:?} - where max={}", xy, dimension);
        }
        // for optimization, we can opt-out early if we've already visited the node
        let found = nodes.get(&xy).clone();
        match found {
            Some(n) => {
                if n.left_child.is_some() && n.right_child.is_some() {
                    // if it has both children, we can just return the node
                    return Some(Box::new(n.clone()));
                } else if xy.col == *dimension && xy.row == *dimension {
                    // it's  the final-goal bottom-right node
                    return Some(Box::new(n.clone()));
                } else if xy.col == 0 && n.right_child.is_some() {
                    // left-most column has no left child, but should have right child
                    return Some(Box::new(n.clone()));
                } else if xy.row == 0 && n.left_child.is_some() {
                    // top-most row has no top (right) child, but should have bottom (left) child
                    return Some(Box::new(n.clone()));
                }
            }
            None => {}
        }
        //// Opt out early, if it is bottom-right (final goal) node, just return as NO Children:
        //if row == *dimension && col == *dimension {
        //    // NOTE: HashMap will UPSERT if key already exists, so we can just insert without checking if key already exists
        //    let new_node = Node::new(None, None, row, col);
        //    nodes.insert(NodeKey { row: row, col: col }, new_node.clone()); // alternatively, if we made this as HashMap with key=(col,row), we can avoid this check because it'll just upsert...
        //    return Some(Box::new(new_node));
        //}

        // note that we should be able to opt-out early if we've already visited the node
        // in which both children are already created.  Unsure if this is really optimzation
        // so for now, it is not considered

        // Firstly, for a 2x2 grid, there are 9 nodes; which basically means that it is ({0..=2} , {0..=2}) (3x3)
        let max_vert_count = dimension + 1;

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
        // * both col and row are 0-based, and assumed to ALWAYS be positive (usize)
        // * as long as col and row are less than max_vert_count, it has at least 1 child
        // * if col is 0, it has no left child - i.e. (2, 0) is bottom-left corner
        // * if row is 0, it has no right child - i.e. (0, 2) is top-right corner
        // * if col == dimension (inclusive), it has no right child - i.e. (1, 2) is middle-right edge
        // * if row == dimension (inclusive), it has no left child - i.e. (2, 1) is bottom-middle edge
        // * if col > 0 && row > 0 && col < dimension && row < dimension, it has 2 children - i.e. (1, 1) is middle node
        let (possible_left_child_key, possible_right_child_key): (
            Option<NodeKey>,
            Option<NodeKey>,
        ) = match xy {
            // origin
            NodeKey { row: 0, col: 0 } => {
                // must be parent node, it has both left and right child
                let left_child = Some(NodeKey {
                    row: xy.row + 1,
                    col: xy.col,
                });
                let right_child = Some(NodeKey {
                    row: xy.row,
                    col: xy.col + 1,
                });
                (left_child, right_child)
            }
            // bottom-right corner, disallow both left and right branches
            NodeKey { row: r, col: c } if r == *dimension && c == *dimension => {
                // must be the bottom-right corner (goal), it has no children
                (None, None)
            }
            // bottom row, disallow left branches
            NodeKey { row: r, col: _ } if r == *dimension => {
                // must be the bottom row, it has no left child
                let left_child = None;
                let right_child = Some(NodeKey {
                    row: xy.row,
                    col: xy.col + 1,
                });
                (left_child, right_child)
            }
            // right column, disallow right branches
            NodeKey { row: _, col: c } if c == *dimension => {
                // must be the right column, it has no right child
                let left_child = Some(NodeKey {
                    row: xy.row + 1,
                    col: xy.col,
                });
                let right_child = None;
                (left_child, right_child)
            }
            _ => {
                // NOTE: Top row and left columns are treated SAME like normal nodes
                // all other nodes have both left and right child
                let left_child = Some(NodeKey {
                    row: xy.row + 1,
                    col: xy.col,
                });
                let right_child = Some(NodeKey {
                    row: xy.row,
                    col: xy.col + 1,
                });
                (left_child, right_child)
            }
        }; // match

        // striving for tail-recursion, so that we can turn this to iteration IF this is costing too much stack
        let possible_left_child = match possible_left_child_key {
            Some(key) => Self::make_nodes_rec(key, dimension, nodes),
            None => None,
        };
        let possible_right_child = match possible_right_child_key {
            Some(key) => Self::make_nodes_rec(key, dimension, nodes),
            None => None,
        };

        let new_node = Node::new(possible_left_child, possible_right_child, xy);
        nodes.insert(xy, new_node.clone()); // alternatively, if we made this as HashMap with key=(col,row), we can avoid this check because it'll just upsert...
        Some(Box::new(new_node))
    }

    // rather than recursively, this method will implement via iterations (DFS)
    // so that we do not run out of stack space...  Unlike the recursive function,it only takes 17.6 mSec for 20x20
    fn make_nodes_iter(&mut self) {
        let start_timer = std::time::Instant::now();
        println!(
            "Initializing Nodes with dimension: {} x {} graph",
            self.grid_size, self.grid_size
        );

        // when converting from recursive to iterative, what usually gets passed into the function parameters
        // are what goes into the stack instead.  We only need to keep track of NodeKey (xy) in this case,
        // because dimension is constant, and the hashmap is static (note that it is NOT thread-safe so
        // avoid trying to break the while() loop into multiple threads)
        let mut node_to_visit_stack: Vec<(
            NodeKey,
            Option<NodeKey>, /*possible parent's left*/
            Option<NodeKey>, /*possible parent's right*/
        )> = Vec::new();
        node_to_visit_stack.push((NodeKey { row: 0, col: 0 }, None, None)); // initial node is always (0,0), commonly passed by the caller if in recursive function

        // Note that unlike recursive function, we cannot "wait" for the return value of the children, hence
        // if the node is already created, we'll need to check if it belonged to a parent node, and if it has
        // one, go and update the parent node's left or right child.
        while !node_to_visit_stack.is_empty() {
            let (xy, possible_parent_left, possible_parent_right) =
                node_to_visit_stack.pop().unwrap();

            // for optimization, we can opt-out early if we've already visited the node
            // NOTE: Just by adding the check below, it can increase from 700µs to 110µs on a 3x3 (16 nodes)
            let found = self.get(&xy).clone();
            match found {
                Some(n) => {
                    if n.left_child.is_some() && n.right_child.is_some() {
                        // if it has both children, we can just return the node
                        continue;
                    } else if xy.col == self.grid_size && xy.row == self.grid_size {
                        // it's  the final-goal bottom-right node
                        continue;
                    } else if xy.col == 0 && n.right_child.is_some() {
                        // left-most column has no left child, but should have right child
                        continue;
                    } else if xy.row == 0 && n.left_child.is_some() {
                        // top-most row has no top (right) child, but should have bottom (left) child
                        continue;
                    }
                }
                None => {}
            }

            // setup the children nodes that needs visisting (to be pushed to stack)
            let (possible_left_child_key, possible_right_child_key): (
                Option<NodeKey>,
                Option<NodeKey>,
            ) = match xy {
                // origin
                NodeKey { row: 0, col: 0 } => {
                    // must be parent node, it has both left and right child
                    let left_child = Some(NodeKey {
                        row: xy.row + 1,
                        col: xy.col,
                    });
                    let right_child = Some(NodeKey {
                        row: xy.row,
                        col: xy.col + 1,
                    });
                    (left_child, right_child)
                }
                // bottom-right corner, disallow both left and right branches
                NodeKey { row: r, col: c } if r == self.grid_size && c == self.grid_size => {
                    // must be the bottom-right corner (goal), it has no children
                    (None, None)
                }
                // bottom row, disallow left branches
                NodeKey { row: r, col: _ } if r == self.grid_size => {
                    // must be the bottom row, it has no left child
                    let left_child = None;
                    let right_child = Some(NodeKey {
                        row: xy.row,
                        col: xy.col + 1,
                    });
                    (left_child, right_child)
                }
                // right column, disallow right branches
                NodeKey { row: _, col: c } if c == self.grid_size => {
                    // must be the right column, it has no right child
                    let left_child = Some(NodeKey {
                        row: xy.row + 1,
                        col: xy.col,
                    });
                    let right_child = None;
                    (left_child, right_child)
                }
                _ => {
                    // NOTE: Top row and left columns are treated SAME like normal nodes
                    // all other nodes have both left and right child
                    let left_child = Some(NodeKey {
                        row: xy.row + 1,
                        col: xy.col,
                    });
                    let right_child = Some(NodeKey {
                        row: xy.row,
                        col: xy.col + 1,
                    });
                    (left_child, right_child)
                }
            }; // match

            // prepare for next iteration (if any) by pushing the children nodes to the stack
            // and also create the node and insert into the HashMap
            let possible_left_child_placeholder =
                if let Some(left_child_key) = possible_left_child_key {
                    // mark this node as parent which has a left child
                    node_to_visit_stack.push((left_child_key, Some(xy), None));
                    // place holder for left child (no children yet)
                    Some(Box::new(Node::new(None, None, left_child_key)))
                } else {
                    None
                };
            let possible_right_child_placeholder =
                if let Some(right_child_key) = possible_right_child_key {
                    // mark this node as a parent which has a right child
                    node_to_visit_stack.push((right_child_key, None, Some(xy)));
                    // place holder for right child (no children yet)
                    Some(Box::new(Node::new(None, None, right_child_key)))
                } else {
                    None
                };
            // now that we've got children nodes, we can create the node and insert into the HashMap
            let new_node = Node::new(
                possible_left_child_placeholder,
                possible_right_child_placeholder,
                xy,
            );
            self.nodes.insert(xy, new_node.clone()); // add it before we test its parents

            // Now that we've created a Node place holder, check if this Node has to update its parent's left or right child
            // if parent's exists, make sure to update the parent's left or right child IF it's not yet set
            if let Some(parent_left_key) = possible_parent_left {
                // There's SOMETHING WRONG if a child is getting processed and has NO parent(s)...
                let parent = self
                    .get_mut(&parent_left_key)
                    .expect("Parent for left child {parent_left_key:?} not found");
                match parent.left_child {
                    Some(ref l) => {
                        // NOTE: if it does already have a left child, you have to panic if it's not the same as the current node!
                        if l.node_key != xy {
                            panic!("Parent's left child is not the same as the current node");
                        }
                    }
                    None => {
                        // update parent's left child if it's not yet set
                        parent.left_child = Some(Box::new(new_node.clone()));
                    }
                }
            }
            if let Some(parent_right_key) = possible_parent_right {
                // There's SOMETHING WRONG if a child is getting processed and has NO parent(s)...
                let parent = self
                    .get_mut(&parent_right_key)
                    .expect("Parent for right child {parent_right_key:?} not found");
                match parent.right_child {
                    Some(ref r) => {
                        // NOTE: if it does already have a right child, you have to panic if it's not the same as the current node!
                        if r.node_key != xy {
                            panic!("Parent's right child is not the same as the current node");
                        }
                    }
                    None => {
                        // update parent's right child if it's not yet set
                        parent.right_child = Some(Box::new(new_node.clone()));
                    }
                }
            }

            // dump this node's parents and self in debug mode
            if cfg!(debug_assertions) {
                //let this_node = self
                //    .get(&xy)
                //    .expect("SOMETHING WRONG!!!  Why did it not find this node {xy:?}?!?!?");
                //println!("UPSERT Node: {:?}", this_node);

                //// first left parent
                //match possible_parent_left {
                //    Some(nodekey) => {
                //        let parent = self
                //            .get(&nodekey)
                //            .expect("Missing parent {nodekey:?} in node map!");

                //        println!("\tLeft parent: {:?}", parent)
                //    }
                //    None => println!("\tLeft parent: None"),
                //};

                //// next right parent
                //match possible_parent_right {
                //    Some(nodekey) => {
                //        let parent = self
                //            .get(&nodekey)
                //            .expect("Missing parent {nodekey:?} in node map!");

                //        println!("\tRight parent: {:?}", parent)
                //    }
                //    None => println!("\tRight parent: None"),
                //};
            }
        } // while stack is not empty
        println!(
            "Initialization took: {:?} - for {} nodes",
            start_timer.elapsed(),
            self.nodes.len()
        );
    } // make_nodes_iter

    fn get(&self, key: &NodeKey) -> Option<&Node> {
        self.nodes.get(&key)
    }

    fn get_mut(&mut self, key: &NodeKey) -> Option<&mut Node> {
        self.nodes.get_mut(&key)
    }
    fn get_root(&self) -> &Node {
        match self.grid_size {
            0 => panic!("Grid size is not initialized"),
            _ => match self.get(&NodeKey { row: 0, col: 0 }) {
                Some(root) => root,
                None => panic!("Root node not found"),
            },
        }
    }

    // returns 0 if neither children can reach goal, otherwise 1 if either or 2 if both children can reach goal
    // DFS based recursive function to visit each node and its children until it reaches the goal,
    // and sum up which children can reach the goal (both children means 2)
    fn can_children_reach_goal_rec(&self, nodekey: &NodeKey) -> u64 {
        let node = self.get(nodekey).unwrap();
        if node.left_child.is_none() && node.right_child.is_none() {
            //println!("*");
            return 1;
        }

        // RIGHT: right child
        let reached_goal_right = if let Some(right_child) = node.right_child.clone() {
            //print!("r");
            self.can_children_reach_goal_rec(&right_child.node_key)
        } else {
            0
        };

        // Down: left child
        let reached_goal_left = if let Some(left_child) = node.left_child.clone() {
            //print!("d");
            self.can_children_reach_goal_rec(&left_child.node_key)
        } else {
            0
        };

        reached_goal_left + reached_goal_right
    }
    // Traverse tree in DFS and return number of routes/paths
    fn visit_children(&self) -> u64 {
        // there's only ONE start and ONE end, so we can just call the root node
        // and recursively visit its children
        let current_node = self.get_root();
        self.can_children_reach_goal_rec(&current_node.clone().node_key)
    }

    // DFS: O(V + E)
    fn debug_dump_paths_rec(&self, node_key: &NodeKey, path: String, paths: &mut Vec<String>) {
        let node = self.get(node_key).unwrap();

        let mut path = path.clone();
        path.push_str(&format!(
            "(ry:{},cx:{}) ",
            node.node_key.row, node.node_key.col,
        ));
        if node.left_child.is_none() && node.right_child.is_none() {
            paths.push(path);
            return;
        }

        if let Some(left_child) = &node.left_child {
            self.debug_dump_paths_rec(&left_child.node_key, path.clone(), paths);
        }
        if let Some(right_child) = &node.right_child {
            self.debug_dump_paths_rec(&right_child.node_key, path.clone(), paths);
        }
    }
    fn debug_dump_paths(&self) {
        let current_node = self.get_root();
        let mut paths: Vec<String> = Vec::new();
        self.debug_dump_paths_rec(&current_node.clone().node_key, "".to_string(), &mut paths);
        for path in paths {
            println!("{}", path);
        }
    }
} // Nodes

fn main() {
    let app_timer = std::time::Instant::now();
    let start_timer = std::time::Instant::now();
    let mut nodes = Nodes::new();
    nodes.init(GRID_SIZE);
    println!("Initialization took: {:?}", start_timer.elapsed());
    println!("Nodes: count={}", nodes.nodes.len());

    let start_timer = std::time::Instant::now();
    let total_paths = nodes.visit_children();
    println!(
        "Processing took: {:?} (Total: {:?})",
        start_timer.elapsed(),
        app_timer.elapsed()
    );
    println!("Total paths: {}", total_paths);
    assert!(total_paths == 137846528820);
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

    // init of 15x15 (and bigger) is taking forever, test here for sizes bigger than 2x2
    #[test]
    fn test_make_new_nodes_10x10() {
        let start_timer = std::time::Instant::now();
        // - 10x10 takes ~4 seconds on recursive
        // - 11x11 takes about ~17s on recursive
        let grid_size = 10;
        let mut nodes = Nodes::new();
        nodes.init(grid_size);
        assert_eq!(nodes.nodes.len(), (grid_size + 1) * (grid_size + 1));
        println!("Initialization took: {:?}", start_timer.elapsed());
    }

    #[test]
    fn test_visit_children_2x2() {
        let grid_size = 2;
        let mut nodes = Nodes::new();
        nodes.init(grid_size);
        let total_paths = nodes.visit_children();

        // dump each routes
        nodes.debug_dump_paths();

        assert_eq!(total_paths, 6);
    }

    #[test]
    fn test_visit_children_3x3() {
        let grid_size = 3;
        let mut nodes = Nodes::new();
        nodes.init(grid_size);
        let total_paths = nodes.visit_children();

        // dump each routes
        nodes.debug_dump_paths();

        assert_eq!(total_paths, 20);
    }
}
