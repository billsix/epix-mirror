/*
 * Complex.h -- ePiX::Complex number class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.18
 * Last Change: September 17, 2017
 *
 *
 * Copyright (C) 2007, 2008, 2017
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
 * ePiX is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ePiX is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ePiX; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef EPIX_COMPLEX
#define EPIX_COMPLEX

namespace ePiX {

class Complex {
 public:
  Complex(double real = 0, double imag = 0);

  Complex& operator+=(const Complex& arg);
  Complex& operator-=(const Complex& arg);
  Complex& operator*=(const Complex& arg);
  Complex& operator/=(const Complex& arg);
  Complex& conj();

  const bool operator==(const Complex& arg) const;
  const bool operator!=(const Complex& arg) const;

  double re() const;
  double im() const;
  double Arg(int branch = 0) const;  // Arg(0, 0) = 0;
  double norm() const;

 private:
  double m_real;
  double m_imag;
};  // end of class Complex

const Complex operator+(Complex arg1, const Complex& arg2);
const Complex operator-(Complex arg1, const Complex& arg2);
const Complex operator-(Complex arg);

const Complex operator*(Complex arg1, const Complex& arg2);
const Complex operator/(Complex arg1, const Complex& arg2);

double Arg(const Complex& arg, int branch = 0);
double norm(const Complex& arg);

Complex expC(const Complex&);
Complex logC(const Complex&, int branch = 0);

Complex sqrtC(const Complex&, int branch = 0);
Complex rootC(const Complex&, int order, int branch = 0);
Complex powC(const Complex&, int);

Complex SinC(const Complex&);
Complex CosC(const Complex&);

Complex TanC(const Complex&);
Complex CotC(const Complex&);

Complex SecC(const Complex&);
Complex CscC(const Complex&);

Complex SinhC(const Complex&);
Complex CoshC(const Complex&);

Complex TanhC(const Complex&);
Complex CothC(const Complex&);

Complex SechC(const Complex&);
Complex CschC(const Complex&);
}  // namespace ePiX

#endif /* EPIX_COMPLEX */
