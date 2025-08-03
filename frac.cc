/* 
 * frac.cc -- ePiX rational number class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
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

#include "frac.h"

namespace ePiX {

  const int MAX_DENOM(10000);
  const double EPS(1.0/MAX_DENOM);

  unsigned int __epix_gcd (int i, unsigned int j);

  frac::frac(const int n, const unsigned int d)
    : m_num(n), m_denom(d) { }

  // express arg as a fraction with denominator no larger than MAX_DENOM
  frac::frac(double arg)
  {
    unsigned int denom(1);
    if (fabs(arg) < EPS)
      {
	m_num = 0;
	m_denom = 1;
      }

    else
      {
	int sgn(arg < 0 ? -1 : 1);
	double abs_arg(sgn*arg);

	// store best approximation
	int best_num(0);
	double running_error(0.5);

	while (denom <= 1+MAX_DENOM)
	  {
	    const double tmp(abs_arg*denom);

	    int tmp_lo((int) floor(tmp));

	    if (tmp - tmp_lo < EPS) // good approx
	      {
		best_num = tmp_lo;
		break;
	      }

	    else if (tmp - tmp_lo < running_error) // improved approx
	      {
		best_num = tmp_lo;
		running_error = tmp - best_num;
	      }

	    int tmp_hi((int) ceil(tmp));
	    if (tmp_hi - tmp < EPS)
	      {
		best_num = tmp_hi;
		break;
	      }

	  else if (tmp_hi - tmp < running_error)
	    {
	      best_num = tmp_hi;
	      running_error = best_num - tmp;
	    }

	    ++denom;
	  }

	m_num = sgn*best_num;
	m_denom = denom;
      }
  } // end of frac::frac(double arg)


  // increment operators
  frac& frac::operator += (const frac& arg)
  {
    m_num *= arg.m_denom;
    m_num += m_denom*arg.m_num;
    m_denom *= arg.m_denom;

    return *this;
  }

  frac& frac::operator -= (const frac& arg)
  {
    m_num *= arg.m_denom;
    m_num -= m_denom*arg.m_num;
    m_denom *= arg.m_denom;

    return *this;
  }

  frac& frac::operator *= (const frac& arg)
  {
    m_num *= arg.m_num;
    m_denom *= arg.m_denom;

    return *this;
  }

  frac& frac::operator /= (const frac& arg)
  {
    unsigned int arg_num(arg.m_num < 0 ? -arg.m_num : arg.m_num);

    m_num *= arg.m_denom;
    m_denom *= arg_num;

    if (arg.m_num < 0)
      m_num = -1;

    return *this;
  }

  frac& frac::reduce()
  {
    unsigned int factor(__epix_gcd(m_num, m_denom));
    m_num /= factor;
    m_denom /= factor;

    return *this;
  }

  double frac::eval() const
  {
    double temp(m_num);
    return temp /= m_denom;
  }

  int frac::num() const
  {
    return m_num;
  }

  unsigned int frac::denom() const
  {
    return m_denom;
  }

  bool frac::is_int() const
  {
    return __epix_gcd(m_num, m_denom) == m_denom;
  }


  frac operator+ (frac arg1, const frac& arg2)
  {
    return arg1 += arg2;
  }

  frac operator- (frac arg1)
  {
    return arg1 *= -1;
  }

  frac operator- (frac arg1, const frac& arg2)
  {
    return arg1 -= arg2;
  }

  frac operator* (frac arg1, const frac& arg2)
  {
    return arg1 *= arg2;
  }

  frac operator/ (frac arg1, const frac& arg2)
  {
    return arg1 /= arg2;
  }

  // (in)equality
  bool operator == (const frac& u, const frac& v)
  {
    return u.num()*v.denom() == v.num()*u.denom();
  }

  bool operator != (const frac& u, const frac& v)
  {
    return !(u == v);
  }

  // denoms are unsigned
  bool operator < (const frac& u, const frac& v)
  {
    return u.num()*v.denom() < v.num()*u.denom();
  }

  bool operator > (const frac& u, const frac& v)
  {
    return u.num()*v.denom() > v.num()*u.denom();
  }

  bool operator <= (const frac& u, const frac& v)
  {
    return u.num()*v.denom() <= v.num()*u.denom();
  }

  bool operator >= (const frac& u, const frac& v)
  {
    return u.num()*v.denom() >= v.num()*u.denom();
  }

  // N.B.: gcd(0,i) = |i|
  unsigned int __epix_gcd (int i, unsigned int j)
  {
    unsigned int new_i(i<0 ? -i : i);
    unsigned int temp;

    if (new_i==0 || j==0) // (1,0) and (0,1) coprime, others not
      return new_i + j;

    else {
      if (j < new_i) // swap them
	{
	  temp = j;
	  j = new_i;
	  new_i = temp;
	}
      // Euclidean algorithm
      while ((temp = j%new_i)) // i does not evenly divide j
	{
	  j = new_i;
	  new_i = temp;
	}
    
      return new_i;
    }
  }
} // end of namespace

