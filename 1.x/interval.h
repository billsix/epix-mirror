/* 
 * interval.h -- ePiX::interval class and operations
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
 * 
 * This file provides the interval class:
 *
 *     - Minkowski sum, intersection
 */

#ifndef EPIX_INTERVAL
#define EPIX_INTERVAL

namespace ePiX {

  class interval {
  public:
    interval(std::string); // allow implicit conversion, must mangle argument
    interval(const double, const double); // closed interval

    static interval emptyset;

    double min() const;
    double max() const;
    double avg() const;

    bool contains(const double) const;

    // Minkowski sum
    interval& operator +=(const interval&);

    // intersection
    interval& operator *=(const interval&);

    bool operator== (const interval&) const;
    bool operator!= (const interval&) const;
    bool is_empty() const;

  private:
    double m_rmin, m_rmax;

    bool m_closed_l, m_closed_r;

    static interval literal(double a, double b); // can force b<a
  }; // end of interval class

  interval operator+ (interval I1, const interval& I2);
  interval operator* (interval I1, const interval& I2);

} // end of namespace

#endif /* EPIX_INTERVAL */
