#include "complex.h"
#include <cmath>

// Constructor: initializes a Complex number
 Complex::Complex(double r, double i) : real(r), imag(i) {}

// Operator overload: adds two Complex numbers
 Complex Complex::operator+(const Complex& other) const {
         return Complex(real + other.real, imag + other.imag);
 }

// Operator overload: takes away one Complex num from another.
 Complex Complex::operator-(const Complex& other) const {
         return Complex(real - other.real, imag - other.imag);
 }

//Operator overload: multiplies by scalar
 Complex Complex::operator*(double scalar) const {
         return Complex(real * scalar, imag * scalar);
 }

 //Returns the length of Complex num
 double Complex::abs() const {
         return std::sqrt(real * real + imag * imag);
 }

// Prints the Complex numb
void Complex::print() const { 
        std::cout << "(" << real << "," << imag << "i)" << std::endl;
}

