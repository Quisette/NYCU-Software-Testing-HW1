#include <iostream>

// This function accepts a const pointer to const data but violates const correctness
void process_matrix(const int* const* matrix, int rows, int cols) {
    std::cout << "Inside process_matrix function." << std::endl;
    std::cout << "Processing a " << rows << "x" << cols << " matrix." << std::endl;

    // Cast away const - this is a dangerous operation!
    int** non_const_matrix = const_cast<int**>(matrix);

    std::cout << "Attempting to modify const data (undefined behavior)..." << std::endl;

    // Attempt to write to memory that was declared const.
    // This will likely cause a crash (segmentation fault).
    non_const_matrix[0][0] = 999;

    std::cout << "Modified matrix[0][0] to: " << non_const_matrix[0][0] << std::endl;
}

// Global const data - these will be placed in read-only memory
static const int row0[] = {10, 20, 30};
static const int row1[] = {40, 50, 60};

// The main function to set up and demonstrate the fault.
int main() {
    // Create a matrix pointing to global const data.
    // This data is in a read-only memory segment.
    const int* const matrix[] = {row0, row1};
    int rows = 2;
    int cols = 3;

    std::cout << "Value before calling function: matrix[0][0] = " << matrix[0][0] << std::endl;

    // Call the function that incorrectly violates the const contract.
    // This is expected to cause a crash (segmentation fault) when it
    // attempts to write to read-only memory.
    std::cout << "\nCalling function that will attempt to modify const data..." << std::endl;
    process_matrix(matrix, rows, cols);

    // This code will likely not be reached due to the program crashing.
    std::cout << "\nFunction call complete." << std::endl;
    std::cout << "Value after calling function: matrix[0][0] = " << matrix[0][0] << std::endl;
    std::cout << "\nTest conclusion: If the program crashes with a segmentation fault, the test has successfully triggered the fault." << std::endl;

    return 0;
}
