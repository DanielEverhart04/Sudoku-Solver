#include <iostream>
#include <array>

class Sudoku {
    public:
    Sudoku() {
        std::array<int, 81> initialFixedContents = {
            8, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 3, 6, 0, 0, 0, 0, 0,
            0, 7, 0, 0, 9, 0, 2, 0, 0,
            0, 5, 0, 0, 0, 7, 0, 0, 0,
            0, 0, 0, 0, 4, 5, 7, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 3, 0,
            0, 0, 1, 0, 0, 0, 0, 6, 8,
            0, 0, 8, 5, 0, 0, 0, 1, 0,
            0, 9, 0, 0, 0, 0, 4, 0, 0
        };

        std::copy(initialFixedContents.begin(), initialFixedContents.end(), fixedContents.begin());

        variableContents.fill(0);
    }

    bool solve() {
        const size_t index = this->getNextEmpty();

        if (index == static_cast<size_t>(-1)) {
            return true;
        }

        for (int i = 1; i < 10; i++) {
            if (this->isValid(index,i)) {
                variableContents[index] = i;
                //this->print();
                if (solve()) {
                    return true;
                }
            }
        }
        variableContents[index] = 0;
        return false;
    }

    void print() const {
        for (int i = 0; i < 81; ++i) {
            int num = variableContents[i] + fixedContents[i];

            // Calculate the row and column from the 1D index
            int row = i / 9;
            int col = i % 9;

            // Print a horizontal separator after every 3rd row, except the first one
            if (row > 0 && row % 3 == 0 && col == 0) {
                std::cout << "-----------------------" << std::endl;
            }

            // Print a vertical separator after every 3rd column, except the last one
            if (col > 0 && col % 3 == 0) {
                std::cout << "| ";
            }

            // Print the number, replacing 0 with a placeholder for empty cells
            if (num == 0) {
                std::cout << ". ";
            } else {
                std::cout << num << " ";
            }

            // Print a newline at the end of each row
            if (col == 8) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }

    [[nodiscard]] std::array<int,9> getRow(const size_t row) const {
        std::array<int,9> returnRow{};

        for (int i = 0; i < 9; ++i) {
            returnRow[i] = fixedContents[9*row+i] + variableContents[9*row+i];
        }

        return returnRow;
    }

    [[nodiscard]] std::array<int,9> getCol(const size_t col) const {
        std::array<int,9> returnCol{};

        for (int i = 0; i < 9; ++i) {
            returnCol[i] = fixedContents[col + 9*i] + variableContents[col + 9*i];
        }

        return returnCol;
    }

    [[nodiscard]] std::array<int, 9> getBox(size_t row, size_t col) const {
        std::array<int, 9> returnBox{};

        // Calculate the starting row and column of the 3x3 box
        const size_t boxStartRow = row - (row % 3);
        const size_t boxStartCol = col - (col % 3);

        size_t index = 0;
        // Iterate through the 3x3 box and populate the return array
        for (size_t r = 0; r < 3; ++r) {
            for (size_t c = 0; c < 3; ++c) {
                size_t gridRow = boxStartRow + r;
                size_t gridCol = boxStartCol + c;
                size_t oneD_index = gridRow * 9 + gridCol;
                returnBox[index] = fixedContents[oneD_index] + variableContents[oneD_index];
                index++;
            }
        }
        return returnBox;
    }

    [[nodiscard]] size_t getNextEmpty() const {
        for (size_t i = 0; i < 81; ++i) {
            if (const int num = variableContents[i] + fixedContents[i]; num == 0) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }

    [[nodiscard]] bool isValid(size_t index, int num) const {
        // Calculate the row and column from the 1D index
        size_t row = index / 9;
        size_t col = index % 9;

        // Check the row
        for (size_t c = 0; c < 9; ++c) {
            if (fixedContents[row * 9 + c] + variableContents[row * 9 + c] == num) {
                return false;
            }
        }

        // Check the column
        for (size_t r = 0; r < 9; ++r) {
            if (fixedContents[r * 9 + col] + variableContents[r * 9 + col] == num) {
                return false;
            }
        }

        // Check the 3x3 box
        size_t boxStartRow = row - (row % 3);
        size_t boxStartCol = col - (col % 3);
        for (size_t r = 0; r < 3; ++r) {
            for (size_t c = 0; c < 3; ++c) {
                size_t gridRow = boxStartRow + r;
                size_t gridCol = boxStartCol + c;
                size_t oneD_index = gridRow * 9 + gridCol;
                if (fixedContents[oneD_index] + variableContents[oneD_index] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    private:
    std::array<int, 81> fixedContents{};
    std::array<int, 81> variableContents{};

};

int main() {
    Sudoku sudoku;
    sudoku.print();
    sudoku.solve();
    sudoku.print();
    return 0;
}
