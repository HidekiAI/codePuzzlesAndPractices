// In the 20×20 grid below, four numbers along a diagonal line have been marked in red.
//    08  02  22  97  38  15  00  40  00  75  04  05  07  78  52  12  50  77  91  08
//    49  49  99  40  17  81  18  57  60  87  17  40  98  43  69  48  04  56  62  00
//    81  49  31  73  55  79  14  29  93  71  40  67  53  88  30  03  49  13  36  65
//    52  70  95  23  04  60  11  42  69  24  68  56  01  32  56  71  37  02  36  91
//    22  31  16  71  51  67  63  89  41  92  36  54  22  40  40  28  66  33  13  80
//    24  47  32  60  99  03  45  02  44  75  33  53  78  36  84  20  35  17  12  50
//    32  98  81  28  64  23  67  10  26* 38  40  67  59  54  70  66  18  38  64  70
//    67  26  20  68  02  62  12  20  95  63* 94  39  63  08  40  91  66  49  94  21
//    24  55  58  05  66  73  99  26  97  17  78* 78  96  83  14  88  34  89  63  72
//    21  36  23  09  75  00  76  44  20  45  35  14* 00  61  33  97  34  31  33  95
//    78  17  53  28  22  75  31  67  15  94  03  80  04  62  16  14  09  53  56  92
//    16  39  05  42  96  35  31  47  55  58  88  24  00  17  54  24  36  29  85  57
//    86  56  00  48  35  71  89  07  05  44  44  37  44  60  21  58  51  54  17  58
//    19  80  81  68  05  94  47  69  28  73  92  13  86  52  17  77  04  89  55  40
//    04  52  08  83  97  35  99  16  07  97  57  32  16  26  26  79  33  27  98  66
//    88  36  68  87  57  62  20  72  03  46  33  67  46  55  12  32  63  93  53  69
//    04  42  16  73  38  25  39  11  24  94  72  18  08  46  29  32  40  62  76  36
//    20  69  36  41  72  30  23  88  34  62  99  69  82  67  59  85  74  04  36  16
//    20  73  35  29  78  31  90  01  74  31  49  71  48  86  81  16  23  57  05  54
//    01  70  54  71  83  51  54  69  16  92  33  48  61  43  52  01  89  19  67  48
// The product of these numbers is 26 × 63 × 78 × 14 = 1788696.
// What is the greatest product of four adjacent numbers in the same direction 
// (up, down, left, right, or diagonally) in the 20×20 grid?

// Whiteboard:
// I'll need a method/function that will calculate products of N adjacent numbers
// on up, down, left, right, and diagonally
// It turns out I have to consider diagonals in all 4 directions (up-left, up-right, down-left, down-right)
// hence I need 8 functions
// Unlike C++, in Rust I can return an Option<u64> to indicate that the (row, col) is not valid for the direction
// at the edges/corners of the grid.

const ADJACENT_COUNT: usize = 4;    // for the grid, max product for 4 adjacent numbers is 70600674
const GRID_FILENAME: &str = "../grid_20_20.txt";    // note: in Rust std::fs, the folks were nice enough to be agnostic of windows/linux paths

fn read_grid(filename: &str) -> Vec<Vec<u32>> {
    let mut grid = Vec::new();
    let file = std::fs::read_to_string(filename).expect("Failed to read file");
    for line in file.lines() {
        let row: Vec<u32> = line.split_whitespace()
            .map(|x| x.parse().expect("Failed to parse number"))
            .collect();
        grid.push(row);
    }
    grid
}

fn get_product_up(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if row < adjacent - 1 {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row - i][col] as u64;
    }
    Some(product)
}

fn get_product_down(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if row + adjacent > grid.len() {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row + i][col] as u64;
    }
    Some(product)
}

fn get_product_left(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if col < adjacent - 1 {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row][col - i] as u64;
    }
    Some(product)
}

fn get_product_right(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if col + adjacent > grid[row].len() {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row][col + i] as u64;
    }
    Some(product)
}

fn get_product_diag_up_left(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if row < adjacent - 1 || col < adjacent - 1 {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row - i][col - i] as u64;
    }
    Some(product)
}

fn get_product_diag_up_right(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if row < adjacent - 1 || col + adjacent > grid[row].len() {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row - i][col + i] as u64;
    }
    Some(product)
}

fn get_product_diag_down_left(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if row + adjacent > grid.len() || col < adjacent - 1 {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row + i][col - i] as u64;
    }
    Some(product)
}

fn get_product_diag_down_right(adjacent: usize, grid: &Vec<Vec<u32>>, row: usize, col: usize) -> Option<u64> {
    if row + adjacent > grid.len() || col + adjacent > grid[row].len() {
        return None;
    }
    let mut product = 1;
    for i in 0..adjacent {
        product *= grid[row + i][col + i] as u64;
    }
    Some(product)
}

fn main() {
    // first, read the grid
    let grid = read_grid(GRID_FILENAME);
    let mut max_product = 0;
    for row in 0..grid.len() {
        for col in 0..grid[row].len() {
            if let Some(product) = get_product_up(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_down(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_left(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_right(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_diag_up_left(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_diag_up_right(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_diag_down_left(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
            if let Some(product) = get_product_diag_down_right(ADJACENT_COUNT, &grid, row, col) {
                max_product = max_product.max(product);
            }
        } // for col
    }   // for row
    println!("Max product: {}", max_product);
}

#[cfg(test)]
mod tests {

}