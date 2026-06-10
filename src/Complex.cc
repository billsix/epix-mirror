/*
 * Complex.cc -- ePiX::Complex number class implementation
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.18
 * Last Change: March 21, 2020
 *
 *
 * Copyright (C) 2007, 2008, 2017, 2020
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

#include <cmath>
#include "constants.h"
#include "functions.h"
#include "angle_units.h"
#include "state.h"

#include "Complex.h"

namespace ePiX {

Complex::Complex(double real, double imag) : m_real(real), m_imag(imag) {}

Complex& Complex::operator+=(const Complex& arg) {
  m_real += arg.m_real;
  m_imag += arg.m_imag;
  return *this;
}

Complex& Complex::operator-=(const Complex& arg) {
  m_real -= arg.m_real;
  m_imag -= arg.m_imag;
  return *this;
}

Complex& Complex::operator*=(const Complex& arg) {
  double tmp_re(m_real * arg.m_real - m_imag * arg.m_imag);
  double tmp_im(m_real * arg.m_imag + m_imag * arg.m_real);

  m_real = tmp_re;
  m_imag = tmp_im;

  return *this;
}

Complex& Complex::operator/=(const Complex& arg) {
  double tmp_re(m_real * arg.m_real + m_imag * arg.m_imag);
  double tmp_im(-m_real * arg.m_imag + m_imag * arg.m_real);

  double tmp_denom(pow(hypot(arg.m_real, arg.m_imag), 2));

  m_real = tmp_re / tmp_denom;
  m_imag = tmp_im / tmp_denom;

  return *this;
}

Complex& Complex::conj() {
  m_imag *= -1;
  return *this;
}

const bool Complex::operator==(const Complex& arg) const {
  return m_real == arg.m_real && m_imag == arg.m_imag;
}

const bool Complex::operator!=(const Complex& arg) const {
  return *this != arg;
}

double Complex::re() const { return m_real; }

double Complex::im() const { return m_imag; }

double Complex::Arg(int branch) const {
  if (norm() > EPIX_EPSILON)
    return atan2(m_imag, m_real) / the_angle_style().to_radians(1) +
           branch * full_turn();

  else
    return 0;  // arg(0, 0) = 0
}

double Complex::norm() const { return hypot(m_real, m_imag); }

//// non-member functions ////
const Complex operator+(Complex arg1, const Complex& arg2) {
  return arg1 += arg2;
}

const Complex operator-(Complex arg1, const Complex& arg2) {
  return arg1 -= arg2;
}

const Complex operator-(Complex arg) { return arg *= -1; }

const Complex operator*(Complex arg1, const Complex& arg2) {
  return arg1 *= arg2;
}

const Complex operator/(Complex arg1, const Complex& arg2) {
  return arg1 /= arg2;
}

double Arg(const Complex& arg, int branch) { return arg.Arg(branch); }

double norm(const Complex& arg) { return arg.norm(); }

Complex expC(const Complex& arg) {
  double rad(exp(arg.re()));
  return Complex(rad * Cos(arg.im()), rad * Sin(arg.im()));
}

Complex logC(const Complex& arg, int branch) {
  return Complex(log(arg.norm()), arg.Arg(branch));
}

Complex sqrtC(const Complex& arg, int branch) {
  double rad(sqrt(arg.norm())), th(0.5 * Arg(arg));
  return Complex(rad * Cos(th), rad * Sin(th));
}

Complex rootC(const Complex& arg, int order, int branch) {
  bool neg(order < 0);

  double pwr(neg ? -1.0 / order : 1.0 / order);  // nan if order == 0

  double rad(pow(arg.norm(), pwr)), th(arg.Arg(branch) * pwr);

  if (neg) {
    rad = 1.0 / rad;
    th = -th;
  }
  return Complex(rad * Cos(th), rad * Sin(th));
}

Complex powC(const Complex& arg, int n) {
  bool neg(n < 0);
  if (neg) n = -n;

  Complex tmp(1, 0);
  while (n > 0) {
    tmp *= arg;
    --n;
  }

  return (neg ? Complex(1, 0) / tmp : tmp);
}

Complex SinC(const Complex& arg) {
  return Complex(Sin(arg.re()) * cosh(arg.im()),
                 Cos(arg.re()) * sinh(arg.im()));
}

Complex CosC(const Complex& arg) {
  return Complex(Cos(arg.re()) * cosh(arg.im()),
                 -Sin(arg.re()) * sinh(arg.im()));
}

Complex TanC(const Complex& arg) { return SinC(arg) / CosC(arg); }

Complex CotC(const Complex& arg) { return CosC(arg) / SinC(arg); }

Complex SecC(const Complex& arg) { return Complex(1) / CosC(arg); }

Complex CscC(const Complex& arg) { return Complex(1) / SinC(arg); }

Complex SinhC(const Complex& arg) {
  return Complex(-Cos(arg.im()) * sinh(arg.re()),
                 Sin(arg.im()) * cosh(arg.re()));
}

Complex CoshC(const Complex& arg) {
  return Complex(Cos(arg.im()) * cosh(arg.re()),
                 Sin(arg.im()) * sinh(arg.re()));
}

Complex TanhC(const Complex& arg) { return SinhC(arg) / CoshC(arg); }

Complex CothC(const Complex& arg) { return CoshC(arg) / SinhC(arg); }

Complex SechC(const Complex& arg) { return Complex(1) / CoshC(arg); }

Complex CschC(const Complex& arg) { return Complex(1) / SinhC(arg); }
}  // namespace ePiX
