/*
 * frac.h -- ePiX::rational number class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.10
 * Last Change: August 07, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 */

/*
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef EPIX_FRACTION
#define EPIX_FRACTION

namespace ePiX {

class frac {
 public:
  frac(const int n, const unsigned int d = 1);
  explicit frac(double);

  // increment operators
  frac& operator+=(const frac& arg);
  frac& operator-=(const frac& arg);
  frac& operator*=(const frac& arg);
  frac& operator/=(const frac& arg);

  frac& reduce();
  double eval() const;

  int num() const;
  unsigned int denom() const;

  bool is_int() const;

 private:
  int m_num;
  unsigned int m_denom;
};  // end of class frac

frac operator+(frac arg1, const frac& arg2);
frac operator-(frac arg1);
frac operator-(frac arg1, const frac& arg2);
frac operator*(frac arg1, const frac& arg2);
frac operator/(frac arg1, const frac& arg2);

// (in)equality
bool operator==(const frac& u, const frac& v);
bool operator!=(const frac& u, const frac& v);

bool operator<(const frac& u, const frac& v);
bool operator>(const frac& u, const frac& v);

bool operator<=(const frac& u, const frac& v);
bool operator>=(const frac& u, const frac& v);

}  // namespace ePiX

#endif /* EPIX_FRACTION */
