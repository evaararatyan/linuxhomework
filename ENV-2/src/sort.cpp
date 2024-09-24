#include "sort.h"
#include <algorithm>

// Compare two complex numbers
bool compareComplex(const Complex& a, const Complex& b) {
    return a.abs() < b.abs();
}

// Sort array of complex numbs
void sortComplex(std::vector<Complex>& arr) {
    std::sort(arr.begin(), arr.end(), compareComplex);
}
