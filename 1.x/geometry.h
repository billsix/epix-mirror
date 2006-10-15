/*
 * geometry.h 
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 09, 2006
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

#ifndef EPIX_GEOMETRY
#define EPIX_GEOMETRY

namespace ePiX {
  class P;
  class sphere;

  // stereographic projection; path first projected vertically
  void frontplot_N(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const sphere& S = sphere());

  void backplot_N(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts,
		  const sphere& S = sphere());

  void frontplot_S(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const sphere& S = sphere());

  void backplot_S(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts, 
		  const sphere& S = sphere());

  // Radial projection from center
  void frontplot_R(P phi(double), double t_min, double t_max, 
		   int num_pts, const sphere& S = sphere());

  void backplot_R(P phi(double), double t_min, double t_max, 
		  int num_pts, const sphere& S = sphere());

  // Hyperbolic lines
  void hyperbolic_line (const P&, const P&);
  void disk_line (const P&, const P&);

} // end of namespace

#endif /* EPIX_GEOMETRY */
