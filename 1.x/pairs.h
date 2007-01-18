/* 
 * pairs.h -- ePiX pair:: class and mathematical operators
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

#ifndef EPIX_PAIRS
#define EPIX_PAIRS

#include "functions.h"
#include "triples.h"

namespace ePiX {

  class pair 
    {
    public:
      pair(const double arg1=0, const double arg2=0) : X1(arg1), X2(arg2) { }
      pair(const P& arg); // take first two coordinates

      double x1() const { return X1; }
      double x2() const { return X2; }

      // unary increment operators
      pair& operator += (const pair& arg);
      pair& operator -= (const pair& arg);
      pair& operator *= (const double c);

      // complex multiplication and division
      pair& operator *= (const pair& arg);
      pair& operator /= (const pair& arg);
      bool operator== (const pair& u) const;
      bool operator!= (const pair& u) const;

    private:
      double X1;
      double X2;

    }; // end of class pair
  
  pair operator- (pair u);
  pair operator+ (pair u, const pair& v);
  pair operator- (pair u, const pair& v);
  pair operator* (const double c, pair u);

  // complex arithmetic
  pair J(pair p);
  pair operator* (pair u, const pair& v);
  pair operator/ (pair u, const pair& v);

  // dot product
  double operator| (const pair& u, const pair& v);
  double norm (const pair& u);

  // componentwise product (a,b)&(x,y)=(ax,by)
  pair operator& (const pair& u, const pair& v);

} // end of namespace

#endif /* EPIX_PAIRS */
