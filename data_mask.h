/* 
 * data_mask.h -- ePiX::data_mask, for selective row removal from data_file
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.6
 * Last Change: July 3, 2007
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
#ifndef EPIX_DATA_MASK
#define EPIX_DATA_MASK

#include <string>

#include "interval.h"

namespace ePiX {

  class data_mask {
  public:
    data_mask(std::string);
    data_mask(std::string, double f(double));

    data_mask(const interval& range);
    data_mask(const interval& range, double f(double));

    data_mask(double range_min, double range_max);
    data_mask(double range_min, double range_max,
	      double f(double));

    data_mask& reverse(); // negate mask criterion
    bool masks(double) const; // true if we mask arg 

  private:
    interval m_range;
    double (*m_filter)(double); // filter criterion
    bool m_reverse;

  }; // end of data_mask
} // end of namespace

#endif /* EPIX_DATA_MASK */
