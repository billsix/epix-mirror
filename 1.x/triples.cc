/* 
 *  triples.cc -- ePiX::P class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 09, 2006
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
#include "errors.h"
#include "triples.h"

namespace ePiX {

  P::P(double arg1, double arg2, double arg3)
    : m_X1(arg1), m_X2(arg2), m_X3(arg3) { }

  double P::x1() const
  {
    return m_X1;
  }
  double P::x2() const
  {
    return m_X2;
  }
  double P::x3() const
  {
    return m_X3;
  }

  // increment operators
  P& P::operator += (const P& arg)
  {
    m_X1 += arg.m_X1;
    m_X2 += arg.m_X2;
    m_X3 += arg.m_X3;

    return *this;
  }

  P& P::operator -= (const P& arg)
  {
    m_X1 -= arg.m_X1;
    m_X2 -= arg.m_X2;
    m_X3 -= arg.m_X3;  

    return *this;
  }

  // scalar multipication
  P& P::operator *= (const double c)
  {
    m_X1 *= c;
    m_X2 *= c;
    m_X3 *= c;

    return *this;
  }

  // cross product
  P& P::operator *= (const P& v)
  {
    P temp(*this);
	  
    m_X1 = (temp.m_X2 * v.m_X3 - temp.m_X3 * v.m_X2);
    m_X2 = (temp.m_X3 * v.m_X1 - temp.m_X1 * v.m_X3);
    m_X3 = (temp.m_X1 * v.m_X2 - temp.m_X2 * v.m_X1);

    return *this;
  }

  // componentwise product
  P& P::operator &= (const P& v)
  {
    m_X1 *= v.m_X1;
    m_X2 *= v.m_X2;
    m_X3 *= v.m_X3;

    return *this;
  }

  // orthogonalization: u %= v is the vector of the form u-k*v perp to v
  P& P::operator%= (const P& v)
  { 
    double denom(v.m_X1*v.m_X1 + v.m_X2*v.m_X2 + v.m_X3*v.m_X3);
    if (denom < EPIX_EPSILON)
      {
	epix_warning("Orthogonalizing by zero vector, no action");
	return *this;
      }

    // else c=(u|v)/(v|v)
    double c((m_X1*v.m_X1 + m_X2*v.m_X2 + m_X3*v.m_X3)/denom);

    m_X1 -= c*v.m_X1;
    m_X2 -= c*v.m_X2;
    m_X3 -= c*v.m_X3;

    return *this;
  }
  // end of class functions

  // vector space operations
  P operator- (const P& u) 
  { 
    P temp(u);
    return temp *= -1;
  }

  P operator+ (const P& u, const P& v) 
  { 
    P temp(u);
    return temp += v;
  }

  P operator- (const P& u, const P& v) 
  { 
    P temp(u);
    return temp -= v;
  }

  // scalar multiplication
  P operator* (const double c, const P& v) 
  { 
    P temp(v);
    return temp *= c;
  }

  P midpoint(const P& u, const P& v, const double t)
  {
    return u + t*(v-u);
  }

  // cross product
  P operator* (const P& u, const P& v) 
  { 
    P temp(u);
    return temp *= v;
  }

  P J(const P& arg) 
  {
    // E_3 * arg
    P temp(-arg);
    return temp *= E_3; 
  }

  // dot product
  double operator | (const P& u, const P& v) 
  { 
    return u.x1()*v.x1() + u.x2()*v.x2() + u.x3()*v.x3(); 
  }

  double norm(const P& u)
  {
    return sqrt(u|u);
  }

  // componentwise product (a,b,c)&(x,y,z)=(ax,by,cz)
  P operator& (const P& u, const P& v) 
  { 
    P temp(u);
    return temp &= v;
  }

  P operator% (const P& u, const P& v) 
  { 
    P temp(u);
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
} // end of namespace
