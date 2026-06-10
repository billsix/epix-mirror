/*
 * data_bins.cc -- ePiX::data_bins class for histogram data
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
#include <string>
#include <sstream>
#include <list>
#include <vector>

#include "errors.h"

#include "constants.h"
#include "functions.h"
#include "triples.h"

#include "path.h"
#include "spline.h"

#include "curves.h"

#include "data_bins.h"

namespace ePiX {

// print summary statistics
static void __epix_out_of_range(const std::string& fcn, unsigned int lo,
                                unsigned int hi, unsigned int hits) {
  if (0 == lo + hi && 0 == hits) return;

  // else
  if (0 < lo + hi) {
    std::stringstream buf;
    buf << fcn << " has " << lo + hi << " point(s) out of range: ";

    bool need_comma(false);

    if (0 < lo) {
      buf << lo << " below";
      need_comma = true;
    }

    if (0 < hi) {
      if (need_comma) buf << ", ";

      buf << hi << " above";
    }
    epix_warning(buf.str());
  }

  if (0 < hits) {
    std::stringstream buf;
    buf << fcn << " has " << hits / 2 << " point(s) on cuts";
    epix_warning(buf.str());
  }
}

//// histogram data bin ////
class d_bin {
 public:
  d_bin(double low, double high) : m_lo(low), m_hi(high), m_dbl_count(0) {}

  double add_datum(double x) {
    if (x < m_lo || m_hi < x)
      return 0.0;

    else if (m_lo < x && x < m_hi) {
      m_dbl_count += 2;
      return 1.0;
    }

    else  // x == m_lo || x == m_hi
    {
      ++m_dbl_count;
      return 0.5;
    }
  }

  // draw rectangle; height = scale*population
  void draw_ht(double scale) const {
    rect(P(m_lo, 0), P(m_hi, 0.5 * scale * m_dbl_count));
  }

  // area = scale*population
  void draw_area(double scale) const {
    if (EPIX_EPSILON < m_hi - m_lo)
      rect(P(m_lo, 0), P(m_hi, 0.5 * scale * m_dbl_count / (m_hi - m_lo)));
  }

  double lo() const { return m_lo; }

  double hi() const { return m_hi; }

  double ht(double scale) const { return 0.5 * scale * m_dbl_count; }

  P loc(double scale) const {
    return P(0.5 * (m_lo + m_hi), 0.5 * scale * m_dbl_count);
  }

  d_bin* clone() const { return new d_bin(*this); }

 private:
  double m_lo, m_hi;  // end values

  // twice the number of elements; endpoint hits count as 1
  unsigned int m_dbl_count;
};  // end of class d_bin

//// data_bins functions ////
data_bins::data_bins(double lo, double hi, unsigned int n)
    : m_lo_val(min(lo, hi)),
      m_hi_val(max(lo, hi)),
      m_lo_ct(0),
      m_hi_ct(0),
      m_pop(0),
      m_cut_hits(0),
      m_cuts_locked(false) {
  // divide [lo, hi] into n pieces (n>0), or into unit pieces (n=0)
  unsigned int ct(n > 0 ? n : (unsigned int)(m_hi_val - m_lo_val));
  const double dx((m_hi_val - m_lo_val) / ct);

  for (unsigned int i = 0; i <= ct; ++i) m_cuts.push_back(m_lo_val + i * dx);
}

data_bins::data_bins(const data_bins& db)
    : m_lo_val(db.m_lo_val),
      m_hi_val(db.m_hi_val),
      m_lo_ct(db.m_lo_ct),
      m_hi_ct(db.m_hi_ct),
      m_pop(db.m_pop),
      m_cut_hits(db.m_cut_hits),
      m_cuts_locked(db.m_cuts_locked),
      m_cuts(db.m_cuts) {
  for (auto m_bin : db.m_bins) m_bins.push_back(m_bin->clone());
}

data_bins& data_bins::operator=(const data_bins& db) {
  if (this != &db) {
    std::list<d_bin*> tmp;
    for (auto m_bin : db.m_bins) tmp.push_back(m_bin->clone());

    // assign data
    m_lo_val = db.m_lo_val;
    m_hi_val = db.m_hi_val;

    m_lo_ct = db.m_lo_ct;
    m_hi_ct = db.m_hi_ct;
    m_pop = db.m_pop;
    m_cut_hits = db.m_cut_hits;
    m_cuts_locked = db.m_cuts_locked;

    // deallocate memory
    for (auto& m_bin : m_bins) delete m_bin;

    m_cuts = db.m_cuts;
    swap(m_bins, tmp);
  }

  return *this;
}

data_bins::~data_bins() {
  for (auto& m_bin : m_bins) delete m_bin;
}

data_bins& data_bins::cut(double x) {
  if (m_cuts_locked)
    epix_warning("cut() called on locked data_bins, no action");

  else if (x < m_lo_val || m_hi_val < x)
    epix_warning("cut() out of range, no action");

  else
    m_cuts.push_back(x);

  return *this;
}

data_bins& data_bins::read(const std::vector<double>& data) {
  if (0 < data.size()) {
    if (!m_cuts_locked) initialize();

    for (double p : data) insert(p);
  }
  return *this;
}

unsigned int data_bins::pop() const { return m_pop; }

// rectangles
void data_bins::histogram(double scale) const {
  if (m_pop == 0) return;

  // else
  __epix_out_of_range("histogram", m_lo_ct, m_hi_ct, m_cut_hits);

  // const double denom((m_hi_val - m_lo_val)/m_pop);
  const double adj(scale / m_pop);

  for (auto m_bin : m_bins) m_bin->draw_area(adj);
}

void data_bins::bar_chart(double scale) const {
  if (m_pop == 0) return;

  // else
  __epix_out_of_range("bar_chart", m_lo_ct, m_hi_ct, m_cut_hits);

  const double adj(scale / m_pop);  // scale=1 -> ht=frac of pop in bin

  for (auto m_bin : m_bins) m_bin->draw_ht(adj);
}

// piecewise-cubic interpolation of bar_chart
void data_bins::plot(double scale) const {
  if (m_pop == 0) return;

  // else
  __epix_out_of_range("data_bins::plot", m_lo_ct, m_hi_ct, m_cut_hits);

  // get rectangle corners
  std::vector<P> vertices;

  auto bn(m_bins.begin());
  vertices.push_back(P((*bn)->lo(), 0));

  for (bn = m_bins.begin(); bn != m_bins.end(); ++bn) {
    vertices.push_back(P((*bn)->lo(), (*bn)->ht(scale / m_pop)));
    vertices.push_back(P((*bn)->hi(), (*bn)->ht(scale / m_pop)));
  }

  --bn;  // last bin
  vertices.push_back(P((*bn)->hi(), 0));

  const unsigned int N(vertices.size());

  // draw piecewise cubic interpolation
  line(vertices.at(0), 0.5 * (vertices.at(0) + vertices.at(1)));

  // Magic number 20 to reduce file size
  for (unsigned int i = 1; i < N - 1; ++i)
    spline(0.5 * (vertices.at(i - 1) + vertices.at(i)), vertices.at(i),
           vertices.at(i), 0.5 * (vertices.at(i) + vertices.at(i + 1)), 20);

  line(0.5 * (vertices.at(N - 2) + vertices.at(N - 1)), vertices.at(N - 1));
}  // end of plot(scale)

// private bookkeeping functions
void data_bins::initialize() {
  if (!m_cuts_locked)  // defensive redundancy
  {
    // sort list of cuts, remove duplicates, and lock
    m_cuts.sort();
    m_cuts.unique();

    m_cuts_locked = true;

    // allocate d_bins
    for (auto curr = m_cuts.begin(); curr != m_cuts.end(); ++curr) {
      auto next(curr);
      ++next;

      if (next != m_cuts.end() && *curr != *next)
        m_bins.push_back(new d_bin(*curr, *next));

      else if (*curr != m_hi_val)
        m_bins.push_back(new d_bin(*curr, m_hi_val));
    }
  }
}

// Ideally no data lies on a cut, but we'll cope as well as possible
// otherwise. Data on an end cut is "out of range". Data on an interior
// cut contributes 1/2 to the population of each adjacent bin.
void data_bins::insert(double x) {
  ++m_pop;

  auto p(m_bins.begin());

  // check end bins first
  if (x <= m_lo_val) {
    ++m_lo_ct;
    if (fabs(x - m_lo_val) < EPIX_EPSILON) {
      (*p)->add_datum(x);
      m_cut_hits += 2;
    }
    return;
  }

  if (m_hi_val <= x) {
    ++m_hi_ct;
    if (fabs(x - m_hi_val) < EPIX_EPSILON) {
      p = m_bins.end();
      --p;
      (*p)->add_datum(x);
      m_cut_hits += 2;
    }
    return;
  }

  // else
  double success(0.0);
  p = m_bins.begin();

  // try "normal" bins serially
  while (success < 0.75 && p != m_bins.end())  // success is a half-integer
  {
    double ct((*p++)->add_datum(x));  // 1 for yes, 0.5 for endpt, 0 for no
    success += ct;
    if (fabs(ct - 0.5) < EPIX_EPSILON) ++m_cut_hits;
  }

  if (success < 0.75)  // still not added!
    epix_warning("data_bin::insert() internal error");
}
}  // namespace ePiX
