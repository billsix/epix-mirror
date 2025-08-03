/*
 * surface.h -- Shaded surface plotting
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
    scenery();
    scenery(P F(double, double), const domain& R);
    scenery(P F(double, double, double), const domain& R);

    // third arg is RGB-valued function of position
    scenery(P F(double, double), const domain& R,
	    P color(double, double, double));

    scenery(P F(double, double, double), const domain& R,
	    P color(double, double, double));

    // third arg is RGB-valued function of domain location
    scenery(P F(double, double), const domain& R,
	    P color(double, double));

    scenery(P F(double, double, double), const domain& R,
	    P color(double, double));

    scenery(const scenery&);
    scenery& operator= (const scenery&);
    ~scenery();

    scenery& add(P F(double, double), const domain& R);
    scenery& add(P F(double, double, double), const domain& R);

    scenery& add(P F(double, double), const domain_list&);
    scenery& add(P F(double, double, double), const domain_list&);

    // color-dependent
    // location-colored
    scenery& add(P F(double, double), const domain& R,
		 P color(double, double, double));
    scenery& add(P F(double, double, double), const domain& R,
		 P color(double, double, double));

    scenery& add(P F(double, double), const domain_list&,
		 P color(double, double, double));
    scenery& add(P F(double, double, double), const domain_list&,
		 P color(double, double, double));

    // domain-colored
    scenery& add(P F(double, double), const domain& R,
		 P color(double, double));
    scenery& add(P F(double, double, double), const domain& R,
		 P color(double, double));

    scenery& add(P F(double, double), const domain_list&,
		 P color(double, double));
    scenery& add(P F(double, double, double), const domain_list&,
		 P color(double, double));

    // remove backward-pointing faces? 0=no, -1=front, 1=back
    scenery& cull(int);
    void draw(int cull=0) const;

  private:
    int m_cull;

    std::list<facet*> m_data;
  }; // end of class scenery


  //// global functions ////
  // except as noted, dim(R) must be 2
  // cosine-shaded surface with fake z-buffered hidden object removal
  void surface(P F(double, double), const domain& R, int cull=0);

  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R, int cull=0);

  // plot multiple slices
  void surface(P F(double, double, double), const domain_list& R, int cull=0);

  // Surface from revolving the curve (f(t),g(t)) about the x-axis...
  void surface_rev(double f(double), double g(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes=24, int cull=0);

  void surface_rev(double f(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes=24, int cull=0);

  // or about the specified axis, with specified angle range
  void surface_rev(double f(double), double g(double), 
		   const domain& R, const frame& coords=frame(), int cull=0);



  //// location-colored versions ////
  void surface(P F(double, double), const domain& R,
	       P color(double, double, double), int cull=0);

  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R,
	       P color(double, double, double), int cull=0);

  // plot multiple slices
  void surface(P F(double, double, double), const domain_list& R,
	       P color(double, double, double), int cull=0);

  // Surface from revolving the curve (f(t),g(t)) about the x-axis...
  void surface_rev(double f(double), double g(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes,
		   P color(double, double, double), int cull=0);

  void surface_rev(double f(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes,
		   P color(double, double, double), int cull=0);

  // or about the specified axis, with specified angle range
  void surface_rev(double f(double), double g(double), 
		   const domain& R, P color(double, double, double),
		   const frame& coords=frame(), int cull=0);


  //// domain-colored versions ////
  void surface(P F(double, double), const domain& R,
	       P color(double, double), int cull=0);

  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R,
	       P color(double, double), int cull=0);

  // plot multiple slices
  void surface(P F(double, double), const domain_list& R,
	       P color(double, double, double), int cull=0);

  // Surface from revolving the curve (f(t),g(t)) about the x-axis...
  void surface_rev(double f(double), double g(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes,
		   P color(double, double), int cull=0);

  void surface_rev(double f(double),
		   double t_min, double t_max, 
		   int latitudes, int longitudes,
		   P color(double, double), int cull=0);

  // or about the specified axis, with specified angle range
  void surface_rev(double f(double), double g(double), 
		   const domain& R, P color(double, double),
		   const frame& coords=frame(), int cull=0);
} // end of namespace ePiX
