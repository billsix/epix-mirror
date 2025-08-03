/*
 * facet.h -- Utility classes for shaded surface plotting
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.7
 * Last Change: July 08, 2007
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

#include "triples.h"
#include "frame.h"

#include "path.h"

#include "Color.h"
#include "pen_data.h"

namespace ePiX {

  class facet {
  public:
    // create quasi-planar region bounded by path segments in f(R)
    facet(P f(double, double),
	  double u0, double v0, double du, double dv,
	  const unsigned int N1, const unsigned int N2);

    facet(P f(double, double, double),
	  double u0, double v0, double w0,
	  double du, double dv, double dw,
	  const unsigned int N1, const unsigned int N2);

    // for surfaces of rotation
    facet(double f(double), double g(double),
	  double u0, double v0, double du, double dv,
	  const unsigned int N1, const unsigned int N2,
	  const frame& coords=frame());


    facet(P f(double, double),
	  double u0, double v0, double du, double dv,
	  const unsigned int N1, const unsigned int N2, const Color&);

    facet(P f(double, double, double),
	  double u0, double v0, double w0,
	  double du, double dv, double dw,
	  const unsigned int N1, const unsigned int N2, const Color&);

    // for surfaces of rotation
    facet(double f(double), double g(double),
	  double u0, double v0, double du, double dv,
	  const unsigned int N1, const unsigned int N2, const Color&,
	  const frame& coords=frame());


    facet* clone() const;

    double how_far() const;
 
    bool front_facing() const;

    void draw(int cull) const;

  private:
    Color m_tint;
    pen_data m_line;
    bool m_fill;

    P pt1, pt2, pt3, pt4, center, direction, perp;
    double distance;

    path bd;
  };


  class by_distance {
  public:
    bool operator() (const facet&, const facet&);
    bool operator() (const facet*, const facet*);
  };
} // end of namespace
