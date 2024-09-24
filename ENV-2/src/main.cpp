#include <vector>
#include "complex.h"
#include "sort.h"

int main() {
	// Create a vector of complex nums
	std::vector<Complex> numbers = {
		Complex(1, 2), Complex(3, 4), Complex(2, 2), Complex(0, 5)
	};
	
        // Print numbers before sorting
	std::cout << "Before sorting:" << std::endl;
	for (const auto& num : numbers) {
		num.print();
	}
	
	// Sort complex numbers
	sortComplex(numbers);

	// Print numbers after sorting
	std::cout << "After sorting:" << std::endl;
	for (const auto& num : numbers) {
		num.print();
	}

	return 0;
}
