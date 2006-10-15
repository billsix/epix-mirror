/* 
 * circle.cc -- ePiX::circle class
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

#include <iostream>

#include "globals.h"
#include "errors.h"

#include "triples.h"

#include "circle.h"
#include "segment.h"
#include "frame.h"

#include "output.h"

#include "curves.h"

namespace ePiX {

  circle::circle(const P& arg1, const double arg2, const P& arg3)
    : ctr(arg1), rad(arg2), perp_to(arg3)
  {
    double temp(norm(arg3));
    if (temp < EPIX_EPSILON)
      {
	epix_warning("Degenerate circle normal, using (0,0,1)");
	perp_to=E_3;
      }

    else
      perp_to *= (1.0/temp); // normalize
  }

  // point-and-center constructor -- parallel to (x1,x2,0)-plane
  circle::circle(const P& center, const P& point)
    : ctr(center), rad(norm(point-center)), perp_to(E_3)
  { 
    if (fabs(E_3|(point-center)) > EPIX_EPSILON)
      epix_warning("Circle requested with point not in (x,y) plane");
  }

  // three-point circle constructor
  circle::circle(const P& pt1, const P& pt2, const P& pt3)
  {
    P D21(pt2-pt1), D31(pt3-pt1), D32(pt3-pt2), N(D21*D31);

    if (norm(D21) < EPIX_EPSILON ||
	norm(D31) < EPIX_EPSILON ||
	norm(D32) < EPIX_EPSILON)
      throw constructor_error(MULTIPLICITY);

    else if (norm(N) < EPIX_EPSILON)
      throw constructor_error(COLLINEAR_PTS);

    else
      {
	perp_to = (1.0/norm(N))*N;

	P q2(midpoint(pt1, pt2));
	P dir2(perp_to*(q2-pt1));

	P q3(midpoint(pt1, pt3));
	P dir3(perp_to*(q3-pt1));

	ctr = segment(q2, q2+dir2)*segment(q3, q3+dir3);
	rad = norm(ctr - pt1);
      }
  }
 
  P circle::center() const
  {
    return ctr;
  }
  double circle::radius() const
  {
    return rad;
  }
  P circle::perp() const
  {
    return perp_to;
  }

  // translation
  circle& circle::operator+= (const P& arg)
  {
    ctr += arg;
    return *this;
  }

  // scaling
  circle& circle::operator*= (const double& arg)
  {
    rad *= arg;
    return *this;
  }

  void circle::draw() const
  {
    double r(rad);
    P N(perp_to);
    frame axes; // standard basis

    if (EPIX_EPSILON <= norm(N*E_3))
      axes = frame(E_3*N, E_3, N);

    ellipse(ctr, r*(axes.sea()), r*(axes.sky()));
 
    end_stanza();
  }


  segment operator* (const segment& arg_seg, const circle& arg_circle)
  {
    P dir(arg_seg.end2() - arg_seg.end1());

    if ( fabs(dir|arg_circle.perp()) > EPIX_EPSILON )
      throw join_error(NON_COPLANAR);

    else
      {
	P to_ctr(arg_circle.center() - arg_seg.end1());
	P line_perp(arg_circle.perp()*dir);
	
	double dist((to_ctr|line_perp)/norm(line_perp));

	if (fabs(dist) > arg_circle.radius())
	  throw join_error(SEPARATED);

	else if (fabs(dist) == arg_circle.radius())
	  throw join_error(TANGENT);

	else
	  {
	    double y(sqrt(pow(arg_circle.radius(), 2) - dist*dist));
	    P unit_x(-(1.0/norm(line_perp))*line_perp);
	    P unit_y((1.0/norm(dir))*dir);

	    P temp1(dist*unit_x + y*unit_y);
	    P temp2(dist*unit_x - y*unit_y);
	    return segment(temp1, temp2) + arg_circle.center();
	  }
      }
  }

  segment operator* (const circle& arg1, const circle& arg2)
  {
    const P p2(arg2.center());
    const P n2(arg2.perp());

    P dir(p2 - arg1.center()); // displacement between centers

    const double r1(arg1.radius());
    const double r2(arg2.radius());
    const double rad_diff(fabs(r2 - r1));
    const double rad_sum(r2 + r1);
    const double sep(norm(dir));

    if (norm(arg1.perp()*arg2.perp()) > EPIX_EPSILON)
      throw join_error(NON_COPLANAR);

    else if (arg1.center() == arg2.center() && rad_diff < EPIX_EPSILON)
      throw join_error(COINCIDENT);
    
    else if ( fabs(sep - rad_sum ) < EPIX_EPSILON || 
	      fabs(sep - rad_diff) < EPIX_EPSILON )
      throw join_error(TANGENT);

    else if (sep > rad_sum)
      throw join_error(SEPARATED);

    else if (sep < rad_diff)
      throw join_error(CONCENTRIC);

    else
      {
	dir *= 1.0/sep;

	double COS((r1*r1 - r2*r2 - sep*sep)/(2*r2*sep));
	double SIN(sqrt(1-COS*COS));

	return segment(p2 + r2*(COS*dir + SIN*(n2*dir)),
		       p2 + r2*(COS*dir - SIN*(n2*dir)));
      }
  }

  segment operator* (const circle& arg_circle, const segment& arg_seg)
  {
    return arg_seg*arg_circle;
  }

  // affine operations: translation by a triple...
  circle operator+ (const circle& circ, const P& displace)
  {
    circle temp(circ);
    return temp += displace; 
  }

  // and scaling by a double
  circle operator* (const double& c, const circle& circ)
  {
    circle temp(circ);
    return temp *= c; 
  }
  circle operator* (const circle& circ, const double& c)
  {
    circle temp(circ);
    return temp *= c; 
  }
} /* end of namespace */
