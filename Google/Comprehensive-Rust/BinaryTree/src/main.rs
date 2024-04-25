use std::ops::Sub;

/// Exercise: Binary Tree
/// A binary tree is a tree-type data structure where every node has two children (left and right).
/// We will create a tree where each node stores a value. For a given node N, all nodes in a N’s
/// left subtree contain smaller values, and all nodes in N’s right subtree will contain larger values.
///
/// Implement the following types, so that the given tests pass.
///
/// Extra Credit: implement an iterator over a binary tree that returns the values in order.

/// A node in the binary tree.
#[derive(Debug)]
#[allow(dead_code)]
struct Node<T: Ord> {
    value: T,
    left: Subtree<T>,
    right: Subtree<T>,
}

/// A possibly-empty subtree.
#[derive(Debug)]
struct Subtree<T: Ord>(Option<Box<Node<T>>>);

/// A container storing a set of values, using a binary tree.
///
/// If the same value is added multiple times, it is only stored once.
#[derive(Debug)]
pub struct BinaryTree<T: Ord> {
    root: Subtree<T>,
}

impl<T: Ord> BinaryTree<T> {
    fn new() -> Self {
        Self {
            root: Subtree::new(),
        }
    }

    fn insert(&mut self, value: T) {
        self.root.insert(value);
    }

    #[allow(dead_code)]
    fn has(&self, value: &T) -> bool {
        self.root.has(value)
    }

    #[allow(dead_code)]
    fn len(&self) -> usize {
        self.root.len()
    }
}

// Implement `new`, `insert`, `len`, and `has` for `Subtree`.
// wishful thinking of wanting to have a where clause which somehow
// explicitly state that Subtree<T>(U) is where U is Option<Box<Node<T>>>
// in which that type U is always Option<_>
impl<T: Ord> Subtree<T> {
    fn new() -> Self {
        // because SubTree is a wrapper around Option<Box<Node<T>>>, we can just default to None here
        Subtree(None)
    }

    fn insert(&mut self, value: T) {
        // insert to left if less than, right if greater than
        match self.0 {
            None => {
                // if the current node is None, then we can just insert the value here
                self.0 = Some(Box::new(Node {
                    value,
                    left: Subtree::new(),
                    right: Subtree::new(),
                }));
            }
            Some(ref mut node) => {
                if value < node.value {
                    node.left.insert(value);
                } else if value > node.value {
                    node.right.insert(value);
                }
            }
        }
    }

    fn has(&self, value: &T) -> bool {
        // walk from root while testing if value is less (left) or equal.  If we encounter greater, then
        // assume it has() == false
        match self.0 {
            None => false,
            Some(ref node) => {
                // if this node is less than the value, then we need to check the right subtree
                // because going towards left will return something less than the current node
                // On the other hand, if this node is greater than the value, then we need to check the left subtree
                // because going towards right will return something greater than the current node
                if node.value < *value {
                    node.right.has(value)
                } else if node.value > *value {
                    node.left.has(value)
                } else {
                    // we found a match
                    true
                }
            }
        }
    }

    // will assume that the length of the tree is the sum of the length of the left and right subtrees
    fn len(&self) -> usize {
        // if the current node is None, then the length is 0
        match self.0 {
            None => 0,
            Some(ref node) => 1 + node.left.len() + node.right.len(),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn len() {
        let mut tree = BinaryTree::new();
        assert_eq!(tree.len(), 0);
        tree.insert(2);
        assert_eq!(tree.len(), 1);
        tree.insert(1);
        assert_eq!(tree.len(), 2);
        tree.insert(2); // not a unique item
        assert_eq!(tree.len(), 2);
    }

    #[test]
    fn has() {
        let mut tree = BinaryTree::new();
        fn check_has(tree: &BinaryTree<i32>, exp: &[bool]) {
            let got: Vec<bool> = (0..exp.len()).map(|i| tree.has(&(i as i32))).collect();
            assert_eq!(&got, exp);
        }

        check_has(&tree, &[false, false, false, false, false]);
        tree.insert(0);
        check_has(&tree, &[true, false, false, false, false]);
        tree.insert(4);
        check_has(&tree, &[true, false, false, false, true]);
        tree.insert(4);
        check_has(&tree, &[true, false, false, false, true]);
        tree.insert(3);
        check_has(&tree, &[true, false, false, true, true]);
    }

    #[test]
    fn unbalanced() {
        let mut tree = BinaryTree::new();
        for i in 0..100 {
            tree.insert(i);
        }
        assert_eq!(tree.len(), 100);
        assert!(tree.has(&50));
    }
}

fn main() {
    let mut tree = BinaryTree::new();
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    println!("{:?}", tree);
}
