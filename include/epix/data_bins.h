/*
 * data_bins.h -- ePiX::data_bins class for histogram data
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.19
 * Last Change: September 17, 2007
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

/*
 * This file provides the "data_bins" class.
 *
 * An object of type data_bins models a closed interval [lo, hi]
 * divided into subintervals (not necessarily of equal length). Each
 * subinterval is a "d_bin", whose endpoints are "cuts". A data_bins
 * can read and sort numerical data, count how many numbers lie in
 * each bin, and print the result as a bar chart (rectangle height
 * proportional to population), histogram (area proportional to
 * population), or as a spline interpolation.
 *
 * Ideally no data lies outside (lo, hi) or on a cut. Data on an end
 * cut is "out of range", data on an interior cut contributes 1/2 to
 * the population of each adjacent bin. A summary statistic warning is
 * printed if any points are out of range or lie on a cut.
 */

#ifndef EPIX_DATA_BINS
#define EPIX_DATA_BINS

#include <vector>
#include <list>

namespace ePiX {

class d_bin;

class data_bins {
 public:
  data_bins(double lo, double hi, unsigned int n = 1);

  data_bins(const data_bins&);
  data_bins& operator=(const data_bins&);
  ~data_bins();

  data_bins& cut(double);  // add a cut
  data_bins& read(const std::vector<double>&);

  unsigned int pop() const;  // current population

  // draw rectangles
  void histogram(double scale = 1) const;  // area prop to pop
  void bar_chart(double scale = 1) const;  // height prop to pop
  void plot(double scale = 1) const;       // smooth interpolation

 private:
  double m_lo_val;
  double m_hi_val;

  unsigned int m_lo_ct;  // population smaller than m_lo_val
  unsigned int m_hi_ct;  // population larger than m_hi_val
  unsigned int m_pop;    // current total population

  unsigned int m_cut_hits;  // number of data on a cut
  bool m_cuts_locked;       // true once we read data

  std::list<double> m_cuts;
  std::list<d_bin*> m_bins;

  // bookkeeping
  void initialize();    // convert cuts to bins and lock
  void insert(double);  // add data point
};  // end of class data_bins

}  // namespace ePiX

#endif /* EPIX_DATA_BINS */
