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

// Postmortem:
// * It dawned on me that I do not need to do "left" and "up" directions, since they 
//   are the same as "right" and "down".  When I start at (0, 0) to inspect "right",
//   when I get to (3, 0), I should not have to look at left...  same as for "up".
// * Similarly, for diagonals, if I start at (0, 0) to inspect "down-right", when I 
//   get to (3, 3), I should not have to look at "up-left".

#include <iostream>  // std::cout
#include <vector>    // std::vector
#include <algorithm> // std::max
#include <string>
#include <fstream>    // for file stream reading text file
#include <numeric>    // std::accumulate
#include <functional> // std::multiplies

const int ADJACENT_COUNT = 4;                          // for gird 20x20, max product of 4 adjacent numbers is 70600674
const std::string GRID_FILENAME = "../grid_20_20.txt"; // I'm using MinGW G++, which will allow "/" as paths

std::vector<std::vector<int>> read_grid(const std::string &filename)
{
    std::vector<std::vector<int>> grid;
    // open file as stream
    auto file = std::ifstream(filename, std::ios::in);

    // read file line by line
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<int> row;
        // split line by space (assume each rows are separated by space)
        size_t pos = 0;
        while ((pos = line.find(' ')) != std::string::npos)
        {
            // using stoi() to convert to int
            row.push_back(std::stoi(line.substr(0, pos)));
            // erase the number and space
            line.erase(0, pos + 1);
        }
        // last column
        row.push_back(std::stoi(line)); // add last number
        grid.push_back(row);
    }

    return grid;
}

std::vector<std::vector<int>> read_grid()
{
    return read_grid(GRID_FILENAME);
}

// check if the row and col is valid for the grid
bool is_valid(const std::vector<std::vector<int>> &grid, int row, int col)
{
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[row].size();
}

// Empty vector means the direction is not valid for the (row, col)
std::vector<int> up(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (row - ADJACENT_COUNT + 1 < 0)
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row - i, col))
            return result; // not valid
        result.push_back(grid[row - i][col]);
    }
    return result;
}

std::vector<int> down(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (row + ADJACENT_COUNT - 1 >= grid.size())
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row + i, col))
            return result; // not valid
        result.push_back(grid[row + i][col]);
    }
    return result;
}

std::vector<int> left(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (col - ADJACENT_COUNT + 1 < 0)
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row, col - i))
            return result; // not valid
        result.push_back(grid[row][col - i]);
    }
    return result;
}

std::vector<int> right(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (col + ADJACENT_COUNT - 1 >= grid[row].size())
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row, col + i))
            return result; // not valid
        result.push_back(grid[row][col + i]);
    }
    return result;
}

std::vector<int> up_left(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (row - ADJACENT_COUNT + 1 < 0 || col - ADJACENT_COUNT + 1 < 0)
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row - i, col - i))
            return result; // not valid
        result.push_back(grid[row - i][col - i]);
    }
    return result;
}

std::vector<int> up_right(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (row - ADJACENT_COUNT + 1 < 0 || col + ADJACENT_COUNT - 1 >= grid[row].size())
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row - i, col + i))
            return result; // not valid
        result.push_back(grid[row - i][col + i]);
    }
    return result;
}

std::vector<int> down_left(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (row + ADJACENT_COUNT - 1 >= grid.size() || col - ADJACENT_COUNT + 1 < 0)
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row + i, col - i))
            return result; // not valid
        result.push_back(grid[row + i][col - i]);
    }
    return result;
}

std::vector<int> down_right(const std::vector<std::vector<int>> &grid, int row, int col)
{
    std::vector<int> result;
    if (row + ADJACENT_COUNT - 1 >= grid.size() || col + ADJACENT_COUNT - 1 >= grid[row].size())
        return result; // not valid
    for (int i = 0; i < ADJACENT_COUNT; i++)
    {
        if (!is_valid(grid, row + i, col + i))
            return result; // not valid
        result.push_back(grid[row + i][col + i]);
    }
    return result;
}

// calculate the max product of N adjacent numbers in the grid
long calculate_max_product(const std::vector<std::vector<int>> &grid)
{
    long max_product = 0;
    std::vector<int> result;
    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid[row].size(); col++)
        {
            // calculate products in all 8 directions
            result = up(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = down(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = left(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = right(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = up_left(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = up_right(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = down_left(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

            result = down_right(grid, row, col);
            if (result.size() == ADJACENT_COUNT)
            {
                auto product = std::accumulate(result.begin(), result.end(), 1, std::multiplies<int>());
                max_product = std::max(max_product, (long) product);
            }

        } // for col
    }     // for row
    return max_product;
}

int main()
{
    auto grid = read_grid();
    auto result = calculate_max_product(grid);
    std::cout << "The greatest product of four adjacent numbers in the same direction in the 20×20 grid is " << result << std::endl;
    return 0;
}