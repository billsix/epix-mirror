/* 
 * hatching.h -- ePiX hatched region data structures
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.13
 * Last Change: August 29, 2007
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
#ifndef EPIX_HATCHING
#define EPIX_HATCHING

#include <list>
#include <vector>

#include "pairs.h"
#include "edge_data.h"

namespace ePiX {

  // edge, endpoint heights, orientation (+/-1 = up/down)
  class bd_edge {
  public:
    bd_edge(const edge2d&, double, double, int);

    pair tail() const;
    pair head() const;
    pair dir() const;

    double lo() const;
    double hi() const;

    int orient() const;

  private:
    edge2d m_edge;

    double m_lo;
    double m_hi;

    int m_orient;
  };

  // location where scan line crosses bd_edge
  class cut {
  public:
    cut(pair loc, int sign);

    pair loc() const;
    int sign() const;

  private:
    pair m_loc; // location
    int m_sign; // orientation
  };

  // ax + by + c = 0
  class scan_line {
  public:
    scan_line(const pair& t, const pair& h);

    void add_cut(const cut&);

    // store what we find in value
    void draw(std::vector<edge2d>& value) const;

  private:
    pair m_perp; // (a, b)
    double m_ht; // c

    std::list<cut> m_cuts;
  }; // end of class scan_line


  // ctor converts a list of edge2d into a (large) vector of scan lines
  class hatch_data {
  public:
    // components of hatch direction, scanline separation, polygon to fill
    hatch_data(double, double,
	       double sep, const std::list<edge2d>&);

    // angle, scanline separation, polygon to fill
    hatch_data(double th, double sep, const std::list<edge2d>&);

    std::vector<edge2d> data() const;

  private:
    std::list<bd_edge> m_edges;
    std::vector<scan_line> m_scans;

    // convert polygon to scan lines parallel to dir, common to both ctors
    void hatch_init(const pair& dir,
		    double sep,
		    std::list<edge2d> polygon);
  }; // end of class hatch_data

} // end of namespace

#endif /* EPIX_HATCHING */
