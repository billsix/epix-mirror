/* 
 *  triples.h -- ePiX::P class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc16
 * Last Change: August 24, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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

  class P 
    {
    private:
      double X1, X2, X3;

    public:
      P(double arg1=0, double arg2=0, double arg3=0)
	: X1(arg1), X2(arg2), X3(arg3) {}

      double x1(void) const { return X1; }
      double x2(void) const { return X2; }
      double x3(void) const { return X3; }

     // increment operators
      P& operator += (const P arg)
	{
	  X1 += arg.X1; X2 += arg.X2; X3 += arg.X3;
	  return (*this);
	}

      P& operator -= (const P arg)
	{
	  X1 -= arg.X1; X2 -= arg.X2; X3 -= arg.X3;  
	  return (*this);
	}

      P& operator *= (const double c) // scalar multipication
	{
	  X1 *= c; X2 *= c; X3 *= c;
	  return (*this);
	}

      // cross product
      P& operator *= (const P v);

      // componentwise product
      P& operator &= (const P v)
	{
	  X1 *= v.X1;
	  X2 *= v.X2;
	  X3 *= v.X3;

	  return *this;
	}

      // orthogonalization u %= v: vector of the form u-k*v, perp to v
      P& operator %= (const P v);

    }; // end of class P

  typedef P triple; // compatibility alias

  const P E_1 = P(1,0,0);
  const P E_2 = P(0,1,0);
  const P E_3 = P(0,0,1);

  // vector space operations
  P operator- (const P u);  // unary negation
  P operator+ (const P u, const P v);
  P operator- (const P u, const P v);
  // scalar multiplication
  P operator* (const double c, const P v);

  inline P midpoint(const P u, const P v, const double t=0.5)
    {
      return u + t*(v-u);
    }

  // cross product
  P operator* (const P u, const P v);
  P J(const P arg); // quarter turn about E_3-axis

  // dot product
  double operator | (const P u, const P v);
  inline double norm(const P u) { return sqrt(u|u); }

  // componentwise product (a,b,c)&(x,y,z)=(ax,by,cz)
  P operator& (const P u, const P v);
  // orthogonalization
  P operator% (const P u, const P v);

  // (in)equality
  bool operator == (const P u, const P v);
  bool operator != (const P u, const P v);

} /* end of namespace */

#endif /* EPIX_TRIPLES */
