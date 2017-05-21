/*
 * Complex.cpp
 *
 *  Created on: Dec 11, 2012
 *      Author: ReD
 */

#include "Complex.h"

Complex::Complex()
	:x(0), y(0)
{}

Complex::Complex(double x, double y)
	:x(x), y(y)
{}


Complex::~Complex() {
}

double Complex::module() {
	return sqrt(x*x + y*y);
}

double Complex::square_module() {
	return x*x + y*y;
}

void Complex::operator =(const Complex& C) {
	x = C.x;
	y = C.y;
}

Complex& Complex::operator +=(const Complex& C){
	x += C.x;
	y += C.y;
	return *this;
}

Complex& Complex::operator -=(const Complex& C){
	x -= C.x;
	y -= C.y;
	return *this;
}

Complex& Complex::operator *=(const Complex& C) {
	double aux = x;
	x = x * C.x - y * C.y;
	y = aux * C.y + y * C.x;
	return *this;
}

const Complex& Complex::operator +(const Complex& C) const{
	return Complex(*this) += C;
}

const Complex& Complex::operator -(const Complex& C) const{
	return Complex(*this) -= C;
}

const Complex& Complex::operator *(const Complex& C) const{
	return Complex(*this) *= C;
}

void Complex::print() {
	printf("(%.16f, %.16f)\n", x, y);
}

void Complex::printComplex() {
	printf("(%.16f + %.16fi)\n", x, y);
}
