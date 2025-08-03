/* 
 * pairs.h -- ePiX pair:: class and mathematical operators
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.10
 * Last Change: August 04, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef EPIX_PAIRS
#define EPIX_PAIRS

namespace ePiX {

  class P;

  class pair {
  public:
    pair(double arg1=0, double arg2=0)
      : m_x1(arg1), m_x2(arg2) { }

    pair(const P& arg); // take first two coordinates

    double x1() const { return m_x1; }
    double x2() const { return m_x2; }

    // unary increment operators
    pair& operator += (const pair& arg);
    pair& operator -= (const pair& arg);
    pair& operator *= (double c);

    // complex multiplication and division
    pair& operator *= (const pair& arg);
    pair& operator /= (const pair& arg);
    bool operator== (const pair& u) const;
    bool operator!= (const pair& u) const;

  private:
    double m_x1;
    double m_x2;

  }; // end of class pair
  
  pair operator- (pair u);
  pair operator+ (pair u, const pair& v);
  pair operator- (pair u, const pair& v);
  pair operator* (double c, pair u);

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
