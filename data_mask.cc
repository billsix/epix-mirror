/* 
 * data_mask.cc -- ePiX::data_mask class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.8
 * Last Change: July 22, 2007
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
#include "functions.h"
#include "interval.h"
#include "data_mask.h"

namespace ePiX {

  double identity(double x) { return x; }

  data_mask::data_mask(std::string arg)
    : m_range(arg), m_filter(identity), m_reverse(false) { }

  data_mask::data_mask(std::string arg, double f(double))
    : m_range(arg), m_filter(f), m_reverse(false) { }


  data_mask::data_mask(const interval& range)
    : m_range(range), m_filter(identity), m_reverse(false) { }

  data_mask::data_mask(const interval& range, double f(double))
    : m_range(range), m_filter(f), m_reverse(false) { }

  data_mask::data_mask(double arg1, double arg2)
    : m_range(arg1, arg2), m_filter(identity), m_reverse(false) { }

  data_mask::data_mask(double arg1, double arg2, double f(double))
    : m_range(arg1, arg2), m_filter(f), m_reverse(false) { }

  data_mask& data_mask::reverse()
  {
    m_reverse = !m_reverse;
    return *this;
  }

  bool data_mask::masks(double x) const
  {
    bool masked(!m_range.contains(m_filter(x)));

    if (m_reverse)
      masked = !masked;

    return masked;
  }
} // end of namespace
