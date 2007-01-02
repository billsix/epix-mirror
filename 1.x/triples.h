/* 
 *  triples.h -- ePiX::P class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: Obcober 09, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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

#include "globals.h"

namespace ePiX {

  class P {
  public:
    P(double arg1=0, double arg2=0, double arg3=0);

    double x1() const;
    double x2() const;
    double x3() const;

    // increment operators
    P& operator += (const P&);
    P& operator -= (const P&);
    P& operator *= (const double); // scalar multipication

    // cross product
    P& operator *= (const P&);

    // componentwise product
    P& operator &= (const P&);

    // orthogonalization: u %= v is the vector of the form u-k*v perp to v
    P& operator %= (const P&);

  private:
    double m_X1, m_X2, m_X3;
  }; // end of class P

  typedef P triple; // compatibility alias

  // standard basis in global namespace
  const P E_1(1,0,0);
  const P E_2(0,1,0);
  const P E_3(0,0,1);

  // vector space operations
  P operator- (const P& u);  // unary negation
  P operator+ (const P& u, const P& v);
  P operator- (const P& u, const P& v);
  // scalar multiplication
  P operator* (const double c, const P& v);

  P midpoint(const P& u, const P& v, const double t=0.5);

  // cross product
  P operator* (const P&, const P&);
  P J(const P&); // quarter turn about E_3-axis

  // dot product
  double operator | (const P&, const P&);
  double norm(const P&);

  // componentwise product (a,b,c)&(x,y,z)=(ax,by,cz)
  P operator& (const P&, const P&);
  // orthogonalization
  P operator% (const P&, const P&);

  // (in)equality
  bool operator == (const P&, const P&);
  bool operator != (const P&, const P&);

} // end of namespace

#endif /* EPIX_TRIPLES */
