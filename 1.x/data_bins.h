/* 
 * data_bins.h -- ePiX::data_bins class for histogram data
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.25
 * Last Change: May 20, 2007
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef EPIX_DATA_BINS
#define EPIX_DATA_BINS

#include <vector>
#include <list>
#include "globals.h"
#include "functions.h"

namespace ePiX {

  class d_bin;

  class data_bins {
  public:
    data_bins(const double lo, const double hi, unsigned int n=1);

    data_bins(const data_bins&);
    data_bins& operator= (const data_bins&);
    ~data_bins();

    data_bins&  cut(const double); // add a cut
    data_bins& read(const std::vector<double>&);

    unsigned int pop() const; // current population

    void histogram(const double scale=1) const; // draw rectangles
    void plot(const double scale=1) const;  // smooth interpolation

  private:
    double m_lo_val;
    double m_hi_val;

    unsigned int m_lo_ct; // low count
    unsigned int m_hi_ct; // high count
    unsigned int m_pop; // current population

    bool m_cuts_locked; // true once we read data

    std::list<double> m_cuts;
    std::vector<d_bin*> m_bins;

    // bookkeeping
    void initialize();         // convert cuts to bins and lock
    void insert(const double); // add data point
  }; // end of class data_bins

} // end of namespace

#endif /* EPIX_DATA_BINS */
