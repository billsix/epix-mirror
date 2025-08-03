/* 
 * hatching.cc -- ePiX's internal data for filled polygons
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
#include <list>
#include <vector>
#include <cmath>

#include "errors.h"

#include "constants.h"
#include "functions.h"
#include "pairs.h"

#include "edge_data.h"

#include "state.h"
#include "hatching.h"

namespace ePiX {

  bd_edge::bd_edge(const edge2d& edge, double ht1, double ht2, int orient)
    : m_edge(edge), m_lo(min(ht1, ht2)), m_hi(max(ht1, ht2)),
      m_orient(orient) { }

  pair bd_edge::tail() const
  {
    return m_edge.tail();
  }

  pair bd_edge::head() const
  {
    return m_edge.head();
  }

  pair bd_edge::dir() const
  {
    return m_edge.head() - m_edge.tail();
  }

  double bd_edge::lo() const
  {
    return m_lo;
  }

  double bd_edge::hi() const
  {
    return m_hi;
  }

  int bd_edge::orient() const
  {
    return m_orient;
  }


  cut::cut(pair loc, int sign)
    : m_loc(loc), m_sign(sign) { }

  pair cut::loc() const
  {
    return m_loc;
  }

  int cut::sign() const
  {
    return m_sign;
  }


  // (h-t) assumed unit
  scan_line::scan_line(const pair& t, const pair& h)
    : m_perp(J(t - h)), m_ht(h.x1()*t.x2() - h.x2()*t.x1()) { }


  // keep m_cuts sorted by x
  void scan_line::add_cut(const cut& crossing)
  {
    if (EPIX_EPSILON < fabs((m_perp|crossing.loc()) + m_ht))
      return;

    const double x(J(m_perp)|crossing.loc());

    std::list<cut>::iterator p(m_cuts.begin());

    while (p != m_cuts.end() && (J(m_perp)|(*p).loc()) < x)
      ++p;

    m_cuts.insert(p, crossing);
  }


  // add scans to value
  void scan_line::draw(std::vector<edge2d>& value) const
  {
    std::list<cut>::const_iterator p(m_cuts.begin());

    if (p == m_cuts.end()) // redundant guard
      return;

    int winding((*p).sign());

    pair start((*p).loc());
    pair end(start);

    while (p != m_cuts.end())
      {
	// non-zero winding rule
	while (winding != 0 && ++p != m_cuts.end())
	  {
	    end = (*p).loc();
	    winding += (*p).sign();
	  }

	if (start != end)
	  value.push_back(edge2d(start, end, true));

	// seek to next start
	while (winding == 0 && ++p != m_cuts.end())
	  {
	    start = (*p).loc();
	    winding += (*p).sign();
	  }
      }
  } // end of scan_line::draw


  // private function; assumes horiz is unit
  void hatch_data::hatch_init(const pair& horiz,
			      double sep,
			      std::list<edge2d> polygon)
  {
    const pair vert(J(horiz));

    // initialize loop with first edge
    std::list<edge2d>::iterator p(polygon.begin());

    // first pass: Get min, max vertex heights; remove horizontal edges
    double min_ht((*p).tail()|vert);
    double max_ht(min_ht);

    for (p = polygon.begin(); p != polygon.end(); ) // increment at bottom
      {
	double tmp_tl((*p).tail()|vert);
	double tmp_hd((*p).head()|vert);

	if (fabs(tmp_hd - tmp_tl) < EPIX_EPSILON) // horizontal edge
	  p = polygon.erase(p);

	else
	  {
	    // running extrema from head height
	    min_ht = min(min_ht, tmp_hd);
	    max_ht = max(max_ht, tmp_hd);

	    int up(tmp_tl < tmp_hd ? 1 : -1); // get edge orientation
	    m_edges.push_back(bd_edge(*p, tmp_tl, tmp_hd, up));
	    ++p;
	  }
      }

    // compute subdivision heights, initialize vector of scan_lines
    unsigned int num_cuts((unsigned int) ceil((max_ht - min_ht)/sep));
    const double true_sep((max_ht - min_ht)/num_cuts);

    // initialize hatch lines
    for (unsigned int i=0; i <= num_cuts; ++i)
      {
	double ht(min_ht + i*true_sep);
	m_scans.push_back(scan_line(ht*vert, horiz + ht*vert));
      }

    // Second pass: add scan line cuts
    for (std::list<bd_edge>::iterator ep=m_edges.begin();
	 ep != m_edges.end(); ++ep)
      {
	// indices of scan lines we cross
	unsigned int i_min((unsigned int)  ceil(((*ep).lo()-min_ht)/true_sep));
	unsigned int i_max((unsigned int) floor(((*ep).hi()-min_ht)/true_sep));

	// compute parameters once per edge
	const pair tl((*ep).tail());
	const pair dir((*ep).dir());

	// relative heights of endpoints
	const double ht_hd((*ep).head()|vert);
	const double ht_tl(tl|vert);

	const int up((*ep).orient());

	for (unsigned int i = i_min; i <= i_max; ++i)
	  {
	    const double t((min_ht + i*true_sep - ht_tl)/(ht_hd - ht_tl));
	    m_scans.at(i).add_cut(cut(tl + t*dir, up));
	  }
      }
  } // end of hatch_init


  // user-visible constructors
  hatch_data::hatch_data(double dir_x, double dir_y,
			 double sep, const std::list<edge2d>& polygon)
  {
    pair dir(dir_x, dir_y);
    dir *= 1.0/norm(dir);

    hatch_init(dir, sep, polygon);
  }

  hatch_data::hatch_data(double th, double sep,
			 const std::list<edge2d>& polygon)
  {
    const double theta(th*PI_180); // th in radians

    // orthonormal basis; horiz along cuts, vert perp to cuts
    hatch_init(pair(std::cos(theta), std::sin(theta)), sep, polygon);
  }


  std::vector<edge2d> hatch_data::data() const
  {
    std::vector<edge2d> value;

    for (unsigned int i=0; i < m_scans.size(); ++i)
      m_scans.at(i).draw(value);

    return value;
  }
} // end of namespace
