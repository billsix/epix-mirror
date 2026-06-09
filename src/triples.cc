/* 
 * triples.cc -- ePiX::P class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.17
 * Last Change: July 20, 2017
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2017
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

#include <cmath>

#include "constants.h"
#include "errors.h"
#include "Complex.h"
#include "triples.h"

namespace ePiX {

  P::P(double arg1, double arg2, double arg3)
    : m_x1(arg1), m_x2(arg2), m_x3(arg3) { }

  P::P(const Complex& arg)
    : m_x1(arg.re()), m_x2(arg.im()), m_x3(0) { }

  double P::x1() const
  {
    return m_x1;
  }

  double P::x2() const
  {
    return m_x2;
  }

  double P::x3() const
  {
    return m_x3;
  }

  bool P::is_valid() const
  {
    using std::isnan;
    using std::isinf;

    return
         !isinf(m_x3) && !isnan(m_x3)
      && !isinf(m_x2) && !isnan(m_x2)
      && !isinf(m_x1) && !isnan(m_x1);
  }

  // increment operators
  P& P::operator += (const P& arg)
  {
    m_x1 += arg.m_x1;
    m_x2 += arg.m_x2;
    m_x3 += arg.m_x3;

    return *this;
  }

  P& P::operator -= (const P& arg)
  {
    m_x1 -= arg.m_x1;
    m_x2 -= arg.m_x2;
    m_x3 -= arg.m_x3;  

    return *this;
  }

  // scalar multipication
  P& P::operator *= (double c)
  {
    m_x1 *= c;
    m_x2 *= c;
    m_x3 *= c;

    return *this;
  }

  // cross product
  P& P::operator *= (const P& v)
  {
    P temp(*this);
	  
    m_x1 = (temp.m_x2 * v.m_x3 - temp.m_x3 * v.m_x2);
    m_x2 = (temp.m_x3 * v.m_x1 - temp.m_x1 * v.m_x3);
    m_x3 = (temp.m_x1 * v.m_x2 - temp.m_x2 * v.m_x1);

    return *this;
  }

  // componentwise product
  P& P::operator &= (const P& v)
  {
    m_x1 *= v.m_x1;
    m_x2 *= v.m_x2;
    m_x3 *= v.m_x3;

    return *this;
  }

  // orthogonalization: u %= v is the vector of the form u-k*v perp to v
  P& P::operator%= (const P& v)
  { 
    double denom(v.m_x1*v.m_x1 + v.m_x2*v.m_x2 + v.m_x3*v.m_x3);
    if (denom < EPIX_EPSILON)
      {
	epix_warning("Orthogonalizing by zero vector, no action");
	return *this;
      }

    // else c=(u|v)/(v|v)
    double c((m_x1*v.m_x1 + m_x2*v.m_x2 + m_x3*v.m_x3)/denom);

    m_x1 -= c*v.m_x1;
    m_x2 -= c*v.m_x2;
    m_x3 -= c*v.m_x3;

    return *this;
  }
  // end of class functions

  // vector space operations
  P operator- (P u) 
  { 
    return u *= -1;
  }

  P operator+ (P u, const P& v) 
  { 
    return u += v;
  }

  P operator- (P u, const P& v) 
  { 
    return u -= v;
  }

  // scalar multiplication
  P operator* (double c, P v) 
  { 
    return v *= c;
  }

  // cross product
  P operator* (P u, const P& v) 
  { 
    return u *= v;
  }

  P J(P arg) 
  {
    // E_3 * arg, but E_3 is const
    return arg *= -E_3; 
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
  P operator& (P u, const P& v) 
  { 
    return u &= v;
  }

  P operator% (P u, const P& v) 
  { 
    return u %= v;
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
