/* 
 * interval.cc -- ePiX::interval class functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.25
 * Last Change: May 20, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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
 *
 */
#include "globals.h"
#include "functions.h"
#include "errors.h"
#include "interval.h"

namespace ePiX {

  // expect arg of the form (a,b), [a,b), (a,b], or [a,b]
  interval::interval(std::string arg)
    : m_closed_l(true), m_closed_r(true)
  {
    std::string::size_type i(arg.find_first_of("[("));

    if (i == std::string::npos) // return "empty interval"
      {
	m_rmin=1;
	m_rmax=0;
	return;
      }

    // else
    arg.erase(0, i);

    if (arg.at(0) == '(')
      m_closed_l = false;

    // expect numerical argument
    i = arg.find_first_of("-0123456789.");
    if (i == std::string::npos)
      {
	m_rmin=1;
	m_rmax=0;
	return;
      }

    arg.erase(0, i);

    // get left endpoint
    const char* tmp1(arg.c_str());
    char* new_arg;
    m_rmin = strtod(tmp1, &new_arg);
    arg=new_arg;

    i = arg.find_first_of(",");
    if (i == std::string::npos)
      {
	m_rmax=0;
	m_rmin=1;
	return;
      }

    arg.erase(0, i);

    // expect another numerical argument
    i = arg.find_first_of("-0123456789.");
    if (i == std::string::npos)
      {
	m_rmax=0;
	m_rmin=1;
	return;
      }

    arg.erase(0, i);

    const char* tmp2(arg.c_str());
    m_rmax = strtod(tmp2, &new_arg);
    arg=new_arg;

    i = arg.find_first_of(")]");

    if (i == std::string::npos)
      {
	m_rmax=0;
	m_rmin=1;
	return;
      }

    if (arg.at(0) == ')')
      m_closed_r = false;
  } // end of interval(std::string)


  interval::interval(const double a, const double b)
    : m_rmin(ePiX::min(a, b)), m_rmax(ePiX::max(a, b)),
      m_closed_l(true), m_closed_r(true) { }

  interval interval::literal(const double a, const double b)
  {
    interval value(a, a);
    value.m_rmax = b;
    return value;
  }

  interval interval::emptyset(literal(1,0));

  double interval::min() const
  {
    return m_rmin;
  }

  double interval::max() const
  {
    return m_rmax;
  }

  double interval::avg() const
  {
    return 0.5*(m_rmin + m_rmax);
  }

  bool interval::contains(const double x) const
  {
    // check bounds at each endpoint, shave off open ends
    return ( (m_closed_l ? (m_rmin <= x) : (m_rmin + EPIX_EPSILON < x)) &&
	     (m_closed_r ? (x <= m_rmax) : (x < m_rmax - EPIX_EPSILON)) );
  }

  // Minkowski sum
  interval& interval::operator +=(const interval& I)
  {
    if (is_empty())
      ;

    else if (I.is_empty())
      *this = interval::emptyset;

    else
      {
	m_rmin += I.m_rmin;
	m_rmax += I.m_rmax;

	// sum contains endpoint iff both arguments do
	m_closed_l &= I.m_closed_l;
	m_closed_r &= I.m_closed_r;
      }

      return *this;
    }

  // intersection
  interval& interval::operator *=(const interval& I)
  {
    if (!is_empty())
      {
	m_rmin = ePiX::max(m_rmin, I.m_rmin);
	m_rmax = ePiX::min(m_rmax, I.m_rmax);

	m_closed_l &= I.m_closed_l;
	m_closed_r &= I.m_closed_r;
      }

    return *this;
  }

  bool interval::operator== (const interval& I) const
  {
    if (is_empty())
      return (I.is_empty()); // both empty?

    else
      return ((m_rmin == I.m_rmin) && (m_rmax == I.m_rmax) &&
	      (m_closed_l == I.m_closed_l) && (m_closed_r == I.m_closed_r));
  }

  bool interval::operator!= (const interval& I) const
  {
    return !(*this == I);
  }

  bool interval::is_empty() const
  {
    if ((m_rmin  <  m_rmax) ||
	((m_rmin == m_rmax) && m_closed_l && m_closed_r) )
      return false;

    else
      return true;
  }

  // non-members
  interval operator+ (interval I1, const interval& I2)
  {
    return I1 += I2;
  }

  interval operator* (interval I1, const interval& I2)
  {
    return I1 *= I2;
  }

} // end of namespace
