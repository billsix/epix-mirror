/*
 * geometry.h 
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc9
 * Last Change: June 23, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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

#ifndef EPIX_GEOMETRY
#define EPIX_GEOMETRY

#include "triples.h"
#include "sphere.h"
#include "functions.h"

namespace ePiX {

  // Flag for type of projection to the sphere
  enum sphere_proj_type {RADIAL, STEREO_N, STEREO_S};

  // recommended for internal use only
  void draw_sphereplot(double f1(double), double f2(double), double f3(double),
		       double t_min, double t_max, int num_pts, bool hidden, 
		       sphere_proj_type TYPE, const sphere& S);

  void draw_sphereplot(P Phi(double), double t_min, double t_max, 
		       int num_pts, bool hidden, sphere_proj_type TYPE,
		       const sphere& S);

  // stereographic projection; path first projected vertically
  inline void frontplot_N(double f1(double), double f2(double),
			  double t_min, double t_max, int num_pts, 
			  const sphere& S = sphere())
  { draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, true, STEREO_N, S); }

  inline void backplot_N(double f1(double), double f2(double),
			 double t_min, double t_max, int num_pts,
			 const sphere& S = sphere())
  { draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, false, STEREO_N, S); }

  inline void frontplot_S(double f1(double), double f2(double),
			  double t_min, double t_max, int num_pts, 
			  const sphere& S = sphere())
  { draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, true, STEREO_S, S); }

  inline void backplot_S(double f1(double), double f2(double),
			 double t_min, double t_max, int num_pts, 
			 const sphere& S = sphere())
  { draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, false, STEREO_S, S); }


  // Radial projection from center
  inline void frontplot_R(P phi(double), double t_min, double t_max, 
			  int num_pts, const sphere& S = sphere())
  { draw_sphereplot(phi, t_min, t_max, num_pts, true, RADIAL, S); }

  inline void backplot_R(P phi(double), double t_min, double t_max, 
			 int num_pts, const sphere& S = sphere())
  { draw_sphereplot(phi, t_min, t_max, num_pts, false, RADIAL, S); }


  // Hyperbolic lines
  void hyperbolic_line (P, P);
  void disk_line (P, P);

} /* end of namespace */

#endif /* EPIX_GEOMETRY */
