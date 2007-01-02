/*
 * surface.h -- Shaded surface plotting
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 1.1
 * Last Change: January 01, 2007
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
#include <list>

#include "triples.h"
#include "frame.h"

#include "path.h"

namespace ePiX 
{
  class domain;
  class domain_list;
  class facet;

  // manipulable collection of surface-like objects
  class scenery {
  public:
    scenery() { };
    scenery(P F(double, double), const domain& R);
    scenery(P F(double, double, double), const domain& R);

    scenery(const scenery&);
    scenery& operator= (const scenery&);
    ~scenery();

    scenery& add(P F(double, double), const domain& R);
    scenery& add(P F(double, double, double), const domain& R);

    scenery& add(P F(double, double), const domain_list&);
    scenery& add(P F(double, double, double), const domain_list&);

    void draw(); // not const: must sort facets

  private:
    std::list<facet*> m_data;
  }; // end of class scenery

  // except as noted, dim(R) must be 2
  // cosine-shaded surface with fake z-buffered hidden object removal
  void surface(P F(double, double), const domain& R);

  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R);

  // plot multiple slices
  void surface(P F(double, double, double), const domain_list& R);

  // Surface from revolving the curve (f(t),g(t)) about the x-axis...
  void surface_rev(double f(double), double g(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes=24);

  void surface_rev(double f(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes=24);

  // or about the specified axis, with specified angle range
  void surface_rev(double f(double), double g(double), 
		   const domain& R, const frame& coords=frame());
} // end of namespace ePiX
