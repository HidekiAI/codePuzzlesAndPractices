// By starting at the top of the triangle below and moving to adjacent numbers
// on the row below, the maximum total from top to bottom is 23.
//      3           3
//     7 4          7
//    2 4 6         4
//   8 5 9 3        9
// That is, 3 + 7 + 4 + 9 = 23.
// Find the maximum total from top to bottom of the triangle below:
//
//                      75
//                     95 64
//                    17 47 82
//                   18 35 87 10
//                  20 04 82 47 65
//                 19 01 23 75 03 34
//                88 02 77 73 07 63 67
//               99 65 04 28 06 16 70 92
//              41 41 26 56 83 40 80 70 33
//             41 48 72 33 47 32 37 16 94 29
//            53 71 44 65 25 43 91 52 97 51 14
//           70 11 33 28 77 73 17 78 39 68 17 57
//          91 71 52 38 17 14 91 43 58 50 27 29 48
//         63 66 04 68 89 53 67 30 73 16 69 87 40 31
//        04 62 98 27 23 09 70 98 73 93 38 53 60 04 23
//
// NOTE: As there are only 16384 routes, it is possible to solve this problem by
// trying every route. However, Problem 67, is the same challenge with a
// triangle containing one-hundred rows; it cannot be solved by brute force, and
// requires a clever method! ;o)

// Whiteboard:
// Firstly, my impression was breadth-first search, until I realized that I
// cannot just pick largest number of each row.  I have to chain its larger
// child. Initial pattern/thought WAS (not anymore) is just look for the largest
// number WITHOUT considerations of what is the children of that number.
// I'd most likely have to visit each children all the way to the bottom, and
// find the paths to the largest number.  One thought (though it won't work if I
// think about it) is while recursing (or if iterating, while iterating), if the
// sum of path on the right or left child is smaller than the currently known
// largest path (that is passed down as recursion function (of if iterating, in
// the stack as a tuple)) then immediately opt out of that branch.  But the
// problem is that the untaken path may end up having a total sum that is larger
// than the currently known largest path.  So we have NO CHOICE but to visit all
// children of each node until we reach the bottom.

fn make_triangle() -> Vec<Vec<u32>> {
    vec![
        vec![75],
        vec![95, 64],
        vec![17, 47, 82],
        vec![18, 35, 87, 10],
        vec![20, 4, 82, 47, 65],
        vec![19, 1, 23, 75, 3, 34],
        vec![88, 2, 77, 73, 7, 63, 67],
        vec![99, 65, 4, 28, 6, 16, 70, 92],
        vec![41, 41, 26, 56, 83, 40, 80, 70, 33],
        vec![41, 48, 72, 33, 47, 32, 37, 16, 94, 29],
        vec![53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14],
        vec![70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57],
        vec![91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48],
        vec![63, 66, 4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31],
        vec![04, 62, 98, 27, 23, 9, 70, 98, 73, 93, 38, 53, 60, 4, 23],
    ]
}

// Unlike the C++ version, we won't create a tree structure.  We'll
// just test for each vertex based on coordinates and determine its
// left and right children dynamically.
fn max_path_sum(
    triangle: &Vec<Vec<u32>>,
    position: &(usize /*row*/, usize /*col*/),
    current_sum: &u32,
) -> u32 /*new_sum*/ {
    let max = |a: u32, b: u32| if a > b { a } else { b };
    let vert_value = triangle[position.0][position.1];

    // if we're at the bottom row, return the current sum since it has no children
    if position.0 == triangle.len() - 1 {
        // by here, current_sum is the sum of the path from the top down to THIS bottom row vertex
        return current_sum + vert_value;
    }

    // special case, if at (0, 0), we don't check the edges and just assume it's got 2 children
    if position == &(0, 0) {
        let new_left_child_sum = max_path_sum(triangle, &(1, 0), &(current_sum + vert_value));
        let new_right_child_sum = max_path_sum(triangle, &(1, 1), &(current_sum + vert_value));
        // return the larger of the two children
        return max(new_left_child_sum, new_right_child_sum);
    }

    // left most edge (when col==0), only has 1 (right) child
    if position.1 == 0 {
        return max_path_sum(
            triangle,
            &(position.0 + 1, position.1), // right child
            &(current_sum + vert_value),
        );
    }
    // right most edge (when col==row), only has 1 (left) child
    if position.1 == triangle[position.0].len() - 1 {
        return max_path_sum(
            triangle,
            &(position.0 + 1, position.1 - 1), // left child
            &(current_sum + vert_value),
        );
    }

    // we have 2 children
    let left_child = max_path_sum(
        triangle,
        &(position.0 + 1, position.1), // left child
        &(current_sum + vert_value),
    );
    let right_child = max_path_sum(
        triangle,
        &(position.0 + 1, position.1 + 1), // right child
        &(current_sum + vert_value),
    );
    // return the larger of the two children plus the current sum
    return max(left_child, right_child);
}

fn main() {
    let mut max_sum = 0;
    let sum = max_path_sum(&make_triangle(), &(0, 0), &0);
    println!("The maximum path sum is: {}", max_sum);
    assert_eq!(sum, 1074);
}
