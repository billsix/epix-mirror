/* 
 * pairs.cc -- ePiX pair:: class and mathematical operators
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.10
 * Last Change: August 04, 2007
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
#include <cmath>

#include "constants.h"
#include "triples.h"
#include "pairs.h"

namespace ePiX {
  pair::pair(const P& arg)
  {
    m_x1 = arg.x1();
    m_x2 = arg.x2();
  }

  // unary increment operators
  pair& pair::operator += (const pair& arg)
  {
    m_x1 += arg.m_x1;
    m_x2 += arg.m_x2;

    return *this;
  }

  pair& pair::operator -= (const pair& arg)
  {
    m_x1 -= arg.m_x1;
    m_x2 -= arg.m_x2;
  
    return *this;
  }

  pair& pair::operator *= (double c)
  {
    m_x1 *= c;
    m_x2 *= c;
  
    return *this;
  }

  // complex multiplication and division
  pair& pair::operator *= (const pair& arg)
  {
    double temp(m_x1);
    m_x1 = temp * arg.m_x1 - m_x2 * arg.m_x2;
    m_x2 = temp * arg.m_x2 + m_x2 * arg.m_x1;
  
    return *this;
  }

  pair& pair::operator /= (const pair& arg)
  {
    double denom = (arg.m_x1)*(arg.m_x1)+(arg.m_x2)*(arg.m_x2);

    double temp(m_x1);
    m_x1 = (temp * arg.m_x1 + m_x2 * arg.m_x2)/denom;
    m_x2 = (m_x2 * arg.m_x1 - temp * arg.m_x2)/denom;
  
    return *this;
  }

  bool pair::operator== (const pair& u) const
  {
    return (    fabs(m_x1-u.m_x1) < EPIX_EPSILON
	     && fabs(m_x2-u.m_x2) < EPIX_EPSILON );
  }

  bool pair::operator!= (const pair& u) const
  {
    return !((*this) == u);
  }

  // non-member functions
  pair operator- (pair u)
  {
    return u *= -1;
  }

  pair operator+ (pair u, const pair& v)
  {
    return u += v;
  }

  pair operator- (pair u, const pair& v)
  {
    return u -= v;
  }

  pair operator* (double c, pair u)
  {
    return u *= c;
  }

  // complex arithmetic
  pair J(pair p)
  {
    return p *= pair(0,1);
  }

  pair operator* (pair u, const pair& v)
  {
    return u *= v;
  }

  pair operator/ (pair u, const pair& v)
  {
    return u /= v;
  }

  // dot product
  double operator| (const pair& u, const pair& v)
  {
    return u.x1()*v.x1() + u.x2()*v.x2();
  }

  double norm (const pair& u)
  {
    return sqrt(u|u);
  }

  // componentwise product (a,b)&(x,y)=(ax,by)
  pair operator& (const pair& u, const pair& v)
  {
    return pair(u.x1()*v.x1(), u.x2()*v.x2());
  }

} // end of namespace
