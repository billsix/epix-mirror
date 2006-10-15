/*
 * surface.h -- Shaded surface plotting
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 1.0.15
 * Last Change: October 10, 2006
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
#include "triples.h"
#include "frame.h"

#include "path.h"

namespace ePiX 
{
  class domain;

  class facet {
  public:
    // create quasi-planar region bounded by path segments in f(R)
    facet(P f(double u, double v), const domain& R, double u0, double v0);

    // for surfaces of rotation
    facet(double f(double), double g(double), const domain& R,
	  double u0, double v0, const frame& coords=frame());

    double how_far(void) const;
 
    bool front_facing(void) const;

    void draw(void);

  private:
    path bd;
    P pt1, pt2, pt3, pt4, center, direction;
    double distance;
  };


  class by_distance {
  public:
    bool operator() (const facet&, const facet&);
  };


  // cosine-shaded surface with fake z-buffered hidden object removal
  void surface(P F(double, double), const domain& R);

  // Surface from revolving the curve (f(t),g(t)) about the x-axis...
  void surface_rev(double f(double), double g(double),
		   double min_x, double max_x, 
		   int latitudes, int longitudes=24);

  // or about the specified axis, with specified angle range
  void surface_rev(double f(double), double g(double), 
		   const domain& R, const frame& coords=frame());
} // end of namespace ePiX
