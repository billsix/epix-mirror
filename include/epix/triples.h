/*
 *  triples.h -- ePiX::P class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.17
 * Last Change: July 20, 2017
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2017
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

/*
 *   This file provides:
 *
 *   The P class (ordered triple of <double>s) and operators
 *
 *    - P(), P() (default to origin), E_1, E_2, E_3 (standard basis)
 *    - Vector space operators
 *    - Other vector operators, operands (a,b,c), (x,y,z)
 *       | dot product                      ax + by + cz
 *       norm(p) = sqrt(p|p)
 *       & componentwise product            (ax, by, cz)
 *       * cross product                    (bz-cy, cx-az, ay-bx)
 *       J rotate (x1,x2,0)-plane 1/4 turn  (-b,a,0)
 *       % orthogonalization                u%v = u - ((u|v)/(v|v))*v
 */
#ifndef EPIX_TRIPLES
#define EPIX_TRIPLES

namespace ePiX {

class Complex;
class P {
 public:
  P(double arg1 = 0, double arg2 = 0, double arg3 = 0);
  P(const Complex&);

  double x1() const;
  double x2() const;
  double x3() const;

  bool is_valid() const;  // coords not inf or nan

  // increment operators
  P& operator+=(const P&);
  P& operator-=(const P&);
  P& operator*=(double);  // scalar multipication

  // cross product
  P& operator*=(const P&);

  // componentwise product
  P& operator&=(const P&);

  // orthogonalization: u %= v is the vector of the form u-k*v perp to v
  P& operator%=(const P&);

 private:
  double m_x1, m_x2, m_x3;
};  // end of class P

// standard basis in global namespace
const P E_1(1, 0, 0);
const P E_2(0, 1, 0);
const P E_3(0, 0, 1);

// vector space operations
P operator-(P u);  // unary negation
P operator+(P u, const P& v);
P operator-(P u, const P& v);
// scalar multiplication
P operator*(double c, P v);

// cross product
P operator*(P, const P&);
P J(P);  // quarter turn about E_3-axis

// dot product
double operator|(const P&, const P&);
double norm(const P&);

// componentwise product (a,b,c)&(x,y,z)=(ax,by,cz)
P operator&(P, const P&);
// orthogonalization
P operator%(P, const P&);

// (in)equality
bool operator==(const P&, const P&);
bool operator!=(const P&, const P&);

}  // namespace ePiX

#endif /* EPIX_TRIPLES */
