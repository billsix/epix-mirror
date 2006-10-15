/* 
 * geometry.cc -- spherical and hyperbolic geometry
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

#include <math.h>
#include <vector>

#include "globals.h"
#include "errors.h"

#include "triples.h"

#include "path.h"

#include "sphere.h"

#include "curves.h"

#include "geometry.h"

namespace ePiX {

  // Flag for type of projection to the sphere
  enum sphere_proj_type {RADIAL, STEREO_N, STEREO_S};

  // Spherical geometry
  P proj_to_sphere(const P& arg, const sphere& S, sphere_proj_type TYPE)
  {
    P O(S.center());
    double rad(S.radius());
    P loc(arg - O); // location relative to O

    if (TYPE == RADIAL)
      return O + (rad/norm(loc))*loc;

    else if (TYPE == STEREO_N)
      {
	P temp(loc%E_3);
	double rho(temp|temp);
	return O + (rad/(rho+1))*P(2*temp.x1(), 2*temp.x2(), rho-1);
      }

    else if (TYPE == STEREO_S)
      {
	P temp(loc%E_3);
	double rho(temp|temp);
	return O + (rad/(rho+1))*P(2*temp.x1(), 2*temp.x2(), 1-rho);
      }

    else // Return center on erroneous projection type
      return O;
  }

  void draw_sphereplot(double f1(double),double f2(double),double f3(double), 
		       double t_min, double t_max, int num_pts, bool hidden, 
		       sphere_proj_type TYPE, const sphere& S)
  {
    std::vector<vertex> data(num_pts+1);
    double t(t_min);
    const double dt((t_max - t_min)/num_pts);

    for (int i=0; i <= num_pts; ++i, t += dt)
      data.at(i) = vertex(proj_to_sphere(P(f1(t), f2(t), f3(t)), S, TYPE));

  path temp(data, false);
  temp.draw(S, hidden);
  } // end of draw_sphereplot

  void draw_sphereplot(P Phi(double), double t_min, double t_max, 
		       int num_pts, bool hidden, sphere_proj_type TYPE,
		       const sphere& S)
  {
    std::vector<vertex> data(num_pts+1);
    double t(t_min);
    const double dt((t_max - t_min)/num_pts);

    for (int i=0; i <= num_pts; ++i, t += dt)
      data.at(i) = vertex(proj_to_sphere(Phi(t), S, TYPE));

  path temp(data, false);
  temp.draw(S, hidden);
  } // end of draw_sphereplot


  void frontplot_N(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, true, STEREO_N, S);
  }

  void backplot_N(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts,
		  const sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, false, STEREO_N, S);
  }

  void frontplot_S(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, true, STEREO_S, S);
  }

  void backplot_S(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts, 
		  const sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, false, STEREO_S, S);
  }


  // Radial projection from center
  void frontplot_R(P phi(double), double t_min, double t_max, 
		   int num_pts, const sphere& S)
  {
    draw_sphereplot(phi, t_min, t_max, num_pts, true, RADIAL, S);
  }

  void backplot_R(P phi(double), double t_min, double t_max, 
		  int num_pts, const sphere& S)
  {
    draw_sphereplot(phi, t_min, t_max, num_pts, false, RADIAL, S);
  }


  // Hyperbolic lines in upper half space
  // For compatibility with 2-D geometry, the boundary is the (x1,x3)-plane

  void hyperbolic_line(const P& tail, const P& head)
  {
    if ( (tail.x2() < 0) || (head.x2() < 0) )
      epix_warning("Endpoint not in upper half-space");

    P sh_tail(tail%E_2); // shadow of tail
    P sh_head(head%E_2);
    double ht_tail(tail|E_2), ht_head(head|E_2);
    double dist(norm(sh_head - sh_tail)); // dist btw projections to boundary

    if (dist < EPIX_EPSILON)
      line(tail, head);

    else
      {
	// use similar triangles to find center; get basis; draw arc
	double diff((ht_head - ht_tail)*(ht_head + ht_tail)/dist);
	double frac(0.5*(diff + dist));
	P center((1-frac/dist)*sh_tail + (frac/dist)*sh_head);
	P e1(tail - center);
	double rad(norm(e1));

	P e2(E_2%e1);
	e2 *= rad/norm(e2);
	double theta(Acos(((head-center)|e1)/(rad*rad)));

	ellipse(center, e1, e2, 0, theta);
      }
  } // end of hyperbolic_line


  // Lines in Poincare disk model.
  //
  // Consider the "positive" portion of the standard hyperboloid of two 
  // sheets: x^2 + y^2 + 1 = z^2, z>0, and consider copies of the unit
  // disk in the planes z=0 (D0) and z=1 (D1). The Klein model of the disk
  // is gotten by stereographic projection from the origin to D1, while the
  // Poincare model is gotten by stereographic projection from (0,0,-1) to
  // D0. Appropriate compositions of these projection maps are hyperbolic
  // isometries. The algorithm for drawing lines in the disk model is to
  // find the images of the endpoints in the Klein model, draw the line
  // between them, and map this line back to the Poincare model. Because
  // the isometry is "square-root-like" in the radial direction at the
  // unit circle, the points on the Klein line are spaced quadratically
  // close together at the endpoints of the segment (the variable "s") so
  // their images will be roughly equally-spaced in the Poincare model.
  // There is no visual harm if one or both endpoints are far from the
  // circle, and the result is acceptable if both points are on or near the
  // circle. The number of points to draw is determined both by the true
  // distance between the endpoints and by how close they are to the circle.

  P poincare_klein(P pt)
  {
    return (2.0/(1+(pt|pt)))*pt;
  }

  P klein_poincare(P pt)
  {
    return (1.0/(1+sqrt(1-(pt|pt))))*pt;
  }

  P p_line(const P& tail, const P& head, double t)
  {
    double s(0.5*(1+std::cos(M_PI*t))); // s in [0,1]

    P current((s*poincare_klein(tail)) + ((1-s)*poincare_klein(head)));
    return klein_poincare(current);
  }

  void disk_line(const P& tail, const P& head)
  {
    const int N(EPIX_NUM_PTS);
    std::vector<vertex> data(N+1);

    double t(0);

    for (int i=0; i <= N; ++i, t += 1.0/N)
      data.at(i) = vertex(p_line(tail, head, t));

    path temp(data, false);
    temp.draw();
  }

} /* end of namespace */

