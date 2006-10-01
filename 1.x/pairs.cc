/* 
 * pairs.cc -- ePiX pair:: class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: May 17, 2006
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

#include "globals.h"
#include "functions.h"
#include "triples.h"
#include "pairs.h"

namespace ePiX {
  pair::pair(const P& arg)
  {
    X1 = arg.x1();
    X2 = arg.x2();
  }

  // unary increment operators
  pair& pair::operator += (const pair& arg)
  {
    X1 += arg.X1;
    X2 += arg.X2;

    return *this;
  }

  pair& pair::operator -= (const pair& arg)
  {
    X1 -= arg.X1;
    X2 -= arg.X2;
  
    return *this;
  }

  pair& pair::operator *= (const double c)
  {
    X1 *= c;
    X2 *= c;
  
    return *this;
  }

  // complex multiplication and division
  pair& pair::operator *= (const pair& arg)
  {
    double temp = X1;
    X1 = temp * arg.X1 - X2 * arg.X2;
    X2 = temp * arg.X2 + X2 * arg.X1;
  
    return *this;
  }

  pair& pair::operator /= (const pair& arg)
  {
    double denom = (arg.X1)*(arg.X1)+(arg.X2)*(arg.X2);

    double temp = X1;
    X1 = (temp * arg.X1 + X2 * arg.X2)/denom;
    X2 = (X2 * arg.X1 - temp * arg.X2)/denom;
  
    return *this;
  }

  bool pair::operator== (const pair& u) const
  {
    return ( fabs(X1-u.X1) < EPIX_EPSILON && 
	     fabs(X2-u.X2) < EPIX_EPSILON );
  }

  bool pair::operator!= (const pair& u) const
  {
    return !((*this) == u);
  }

  // non-member functions
  pair operator- (pair u) { return u *= -1; }
  pair operator+ (pair u, const pair& v) { return u += v; }
  pair operator- (pair u, const pair& v) { return u -= v; }
  pair operator* (const double c, pair u) { return u *= c; }

  // complex arithmetic
  pair J(pair p) { return p *= pair(0,1); }
  pair operator* (pair u, const pair& v) { return u *= v; }
  pair operator/ (pair u, const pair& v) { return u /= v; }

  // dot product
  double operator| (const pair& u, const pair& v)
  {
    return u.x1()*v.x1() + u.x2()*v.x2();
  }
  double norm (const pair& u) { return sqrt(u|u); }

  // componentwise product (a,b)&(x,y)=(ax,by)
  pair operator& (const pair& u, const pair& v)
  {
    return pair(u.x1()*v.x1(), u.x2()*v.x2());
  }

} // end of namespace
