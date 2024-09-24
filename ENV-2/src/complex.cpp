#include "complex.h"
#include <cmath>

 Complex::Complex(double r, double i) : real(r), imag(i) {}

 Complex Complex::operator+(const Complex& other) const {
         return Complex(real + other.real, imag + other.imag);
 }

 Complex Complex::operator-(const Complex& other) const {
         return Complex(real - other.real, imag - other.imag);
 }

 Complex Complex::operator*(double scalar) const {
         return Complex(real * scalar, imag * scalar);
 }

 double Complex::abs() const {
         return std::sqrt(real * real + imag * imag);
 }

void Complex::print() const { 
        std::cout << "(" << real << "," << imag << "i)" << std::endl;
}

