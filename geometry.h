/*
 * geometry.h 
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.1.6
 * Last Change: June 28, 2007
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

#ifndef EPIX_GEOMETRY
#define EPIX_GEOMETRY

#include "frame.h"
#include "sphere.h"

namespace ePiX {
  class P;

  // latitude, longitude lines with hiding
  void latitude(double lat, double long_min, double long_max,
		const Sphere& S=Sphere(), const frame& coords=frame());

  void longitude(double lngtd, double lat_min, double lat_max,
		 const Sphere& S=Sphere(), const frame& coords=frame());

  void back_latitude(double lat, double long_min, double long_max,
		     const Sphere& S=Sphere(), const frame& coords=frame());

  void back_longitude(double lngtd, double lat_min, double lat_max,
		      const Sphere& S=Sphere(), const frame& coords=frame());


  // stereographic projection; path first projected vertically
  void frontplot_N(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const Sphere& S = Sphere());

  void backplot_N(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts,
		  const Sphere& S = Sphere());

  void frontplot_S(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const Sphere& S = Sphere());

  void backplot_S(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts, 
		  const Sphere& S = Sphere());

  // Radial projection from center
  void frontplot_R(P phi(double), double t_min, double t_max, 
		   int num_pts, const Sphere& S = Sphere());

  void backplot_R(P phi(double), double t_min, double t_max, 
		  int num_pts, const Sphere& S = Sphere());


  // All sphere line functions treat point arguments as unscaled
  // displacements from the center of the sphere, not as absolute
  // spatial locations.  If the center or radius of the sphere
  // changes, the line automatically adjusts as expected.

  // front portion of spherical segment joining p1 to p2
  void front_arc(const P& p1, const P& p2, const Sphere& S = Sphere());
  void  back_arc(const P& p1, const P& p2, const Sphere& S = Sphere());

  // half-line joining p1 to -p1 through p2
  void front_arc2(const P&, const P&, const Sphere& S=Sphere());
  void  back_arc2(const P&, const P&, const Sphere& S=Sphere());

  // great circle through p1 and p2
  void front_line(const P&, const P&, const Sphere& S=Sphere());
  void  back_line(const P&, const P&, const Sphere& S=Sphere());

  void front_triangle(const P&, const P&, const P&, const Sphere& S=Sphere());
  void  back_triangle(const P&, const P&, const P&, const Sphere& S=Sphere());

  // spherical polyhedra
  void front_tetra(const Sphere& S=Sphere(), const frame& coords=frame());
  void  back_tetra(const Sphere& S=Sphere(), const frame& coords=frame());

  void front_cube(const Sphere& S=Sphere(), const frame& coords=frame());
  void  back_cube(const Sphere& S=Sphere(), const frame& coords=frame());

  void front_octa(const Sphere& S=Sphere(), const frame& coords=frame());
  void  back_octa(const Sphere& S=Sphere(), const frame& coords=frame());

  void front_dodeca(const Sphere& S=Sphere(), const frame& coords=frame());
  void  back_dodeca(const Sphere& S=Sphere(), const frame& coords=frame());

  void front_icosa(const Sphere& S=Sphere(), const frame& coords=frame());
  void  back_icosa(const Sphere& S=Sphere(), const frame& coords=frame());

  // Hyperbolic lines
  void hyperbolic_line (const P&, const P&);
  void disk_line (const P&, const P&);

} // end of namespace

#endif /* EPIX_GEOMETRY */
