#include <vector>
#include "complex.h"
#include "sort.h"

int main() {
	std::vector<Complex> numbers = {
		Complex(1, 2), Complex(3, 4), Complex(2, 2), Complex(0, 5)
	};

	std::cout << "Before sorting:" << std::endl;
	for (const auto& num : numbers) {
		num.print();
	}
	
	sortComplex(numbers);

	std::cout << "After sorting:" << std::endl;
	for (const auto& num : numbers) {
		num.print();
	}

	return 0;
}
