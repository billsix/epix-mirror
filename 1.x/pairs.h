/* 
 * pairs.h -- ePiX pair:: class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc11
 * Last Change: June 26, 2004
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

#ifndef EPIX_PAIRS
#define EPIX_PAIRS

// #include <cstdlib>
// #include <cstdio>
// #include <cmath>

#include "functions.h"
#include "triples.h"

namespace ePiX {

  class pair 
    {
    private:
      double X1;
      double X2;

    public:
      pair(const double arg1=0, const double arg2=0) : X1(arg1), X2(arg2) {}

      double x1(void) const { return X1; }
      double x2(void) const { return X2; }

      pair(const P arg) 
	{ 
	  X1 = arg.x1();
	  X2 = arg.x2(); 
	}

      // unary increment operators
      pair& operator += (const pair arg)
	{
	  X1 += arg.X1;
	  X2 += arg.X2;
  
	  return (*this);
	}

      pair& operator -= (const pair arg)
	{
	  X1 -= arg.X1;
	  X2 -= arg.X2;
  
	  return (*this);
	}

      pair& operator *= (const double c)
	{
	  X1 *= c;
	  X2 *= c;
  
	  return (*this);
	}

      // complex multiplication and division
      pair& operator *= (const pair arg)
	{
	  double temp = X1;
	  X1 = temp * arg.X1 - X2 * arg.X2;
	  X2 = temp * arg.X2 + X2 * arg.X1;
  
	  return (*this);
	}
      pair& operator /= (const pair arg)
	{
	  double denom = hypot(arg.X1, arg.X2);

	  double temp = X1;
	  X1 = (temp * arg.X1 + X2 * arg.X2)/denom;
	  X2 = (X2 * arg.X1 - temp * arg.X2)/denom;
  
	  return (*this);
	}

      bool operator== (const pair u)
	{
	  return ( (X1 == u.X1) && (X2 == u.X2) );
	}

    }; // end of class pair
  
  inline pair& operator- (pair u)
    {
      return u *= -1;
    }

  inline pair operator+ (pair u, const pair v) { return u += v; }
  inline pair operator- (pair u, const pair v) { return u -= v; }

  // complex arithmetic
  inline pair& J(pair p) { return p *= pair(0,1); }
  inline pair& operator* (pair u, const pair v) { return u *= v; }
  inline pair& operator/ (pair u, const pair v) { return u /= v; }

  // dot product
  inline double operator| (const pair u, const pair v)
  {
    return u.x1()*v.x1() + u.x2()*v.x2();
  }
  inline double norm (const pair u) { return sqrt(u|u); }

  // componentwise product (a,b)&(x,y)=(ax,by)
  inline pair operator& (const pair u, const pair v)
  {
    return pair(u.x1()*v.x1(), u.x2()*v.x2());
  }

} // end of namespace

#endif /* EPIX_PAIRS */
