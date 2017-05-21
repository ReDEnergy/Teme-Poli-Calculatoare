#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <cmath>

#ifndef COMPLEX_H_
#define COMPLEX_H_

using namespace std;


/*
 * Clasa Complex
 * 	-	implementeaza operatiile de baza pentru numerele complexe
 */
class Complex {
	public:
		Complex();
		Complex(double x, double y);
		virtual ~Complex();

		double module();
		double square_module();
		void operator= (const Complex &C);

		Complex& operator+= (const Complex &C);
		Complex& operator-= (const Complex &C);
		Complex& operator*= (const Complex &C);

		const Complex& operator+ (const Complex &C) const;
		const Complex& operator- (const Complex &C) const;
		const Complex& operator* (const Complex &C) const;

		void print();
		void printComplex();

	public:
		double x;
		double y;
};

#endif /* COMPLEX_H_ */
