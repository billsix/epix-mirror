/* 
 *  triples.cc -- ePiX::P class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: March 06, 2006
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
#include "triples.h"

namespace ePiX {

  // cross product
  P& P::operator *= (const P& v)
	{
	  P temp = *this;
	  
	  X1 = (temp.X2 * v.X3 - temp.X3 * v.X2);
	  X2 = (temp.X3 * v.X1 - temp.X1 * v.X3);
	  X3 = (temp.X1 * v.X2 - temp.X2 * v.X1);

	  return *this;
	}

  // vector space operations
  P operator- (const P& u) 
  { 
    P temp = u;
    return temp *= -1;
  }

  P operator+ (const P& u, const P& v) 
  { 
    P temp = u;
    return temp += v;
  }

  P operator- (const P& u, const P& v) 
  { 
    P temp = u;
    return temp -= v;
  }

  // scalar multiplication
  P operator* (const double c, const P& v) 
  { 
    P temp = v;
    return temp *= c;
  }

  // cross product
  P operator* (const P& u, const P& v) 
  { 
    P temp = u;
    return temp *= v;
  }

  P J(const P& arg) 
  { 
    P temp = -arg;
    return temp *= E_3; 
  }

  // dot product
  double operator | (const P& u, const P& v) 
    { 
      return u.x1()*v.x1() + u.x2()*v.x2() + u.x3()*v.x3(); 
    }

  // componentwise product (a,b,c)&(x,y,z)=(ax,by,cz)
  P operator& (const P& u, const P& v) 
  { 
    P temp = u;
    return temp &= v;
  }

  // orthogonalization: u%v is component of u perp to v
  P& P::operator%= (const P& v)
    { 
      double c = (*this|v)/(v|v);
      X1 -= c*v.X1;
      X2 -= c*v.X2;
      X3 -= c*v.X3;

      return *this;
    }

  P operator% (const P& u, const P& v) 
  { 
    P temp = u;
    return temp %= v;
  }

  // (in)equality
  bool operator == (const P& u, const P& v)
    { 
      return (norm(u-v) < EPIX_EPSILON); 
    }

  bool operator != (const P& u, const P& v)
    { 
      return !(u==v); 
    }

} /* end of namespace */

