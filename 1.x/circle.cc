/* 
 * circle.cc -- ePiX::circle class
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

#include <iostream>

#include "globals.h"
#include "triples.h"

#include "circle.h"
#include "segment.h"
#include "camera.h"

#include "output.h"
// #include "lengths.h"
// #include "objects.h"
#include "curves.h"

namespace ePiX {

  void circle::draw()
  {
    double r = this->rad;
    P N = this->perp_to;
    frame axes;

    if (norm(N*E_3) < EPIX_EPSILON)
      axes = frame();
    else
      axes = frame(E_3*N, E_3, N);

    ellipse(this->ctr, r*(axes.sea()), r*(axes.sky()));
 
    end_stanza();
  }
  /*
  void draw(circle arg)
  {
  arg.draw();
  }
  */

  // point-and-center constructor -- parallel to (x1,x2,0)-plane
  circle::circle(P center, P point) 
  { 
    if (fabs(E_3|(point-center)) > EPIX_EPSILON)
      throw constructor_error(MALFORMED);
    else
      {
	ctr = center;
	rad = norm(point-center);
	perp_to = E_3;
      }
  }
  // three-point circle constructor
  circle::circle(P pt1, P pt2, P pt3)
  {
    if (norm(pt2-pt1) < EPIX_EPSILON ||
	norm(pt3-pt1) < EPIX_EPSILON ||
	norm(pt3-pt2) < EPIX_EPSILON)
      throw constructor_error(MULTIPLICITY);

    else if (norm((pt2 - pt1)*(pt3 - pt1)) < EPIX_EPSILON)
      throw constructor_error(COLLINEAR_PTS);

    else
      {
	P temp = (pt2 - pt1)*(pt3 - pt1);
	perp_to = (1.0/norm(temp))*temp;

	P q2 = midpoint(pt1, pt2);
	P dir2 = perp_to*(q2-pt1);

	P q3 = midpoint(pt1, pt3);
	P dir3 = perp_to*(q3-pt1);

	ctr = segment(q2, q2+dir2)*segment(q3, q3+dir3);
	rad = norm(ctr - pt1);
      }
  }
 
  segment& operator * (const segment& arg_seg, const circle& arg_circle)
  {
    P dir = arg_seg.end2() - arg_seg.end1();

    if ( fabs(dir|arg_circle.perp()) > EPIX_EPSILON )
      throw join_error(NON_COPLANAR);

    else
      {
	P to_ctr = arg_circle.center() - arg_seg.end1();
	P line_perp = arg_circle.perp()*dir;
	
	double dist = (to_ctr|line_perp)/norm(line_perp);

	if (fabs(dist) > arg_circle.radius())
	  throw join_error(SEPARATED);

	else if (fabs(dist) == arg_circle.radius())
	  throw join_error(TANGENT);

	else
	  {
	    double y = sqrt(pow(arg_circle.radius(), 2) - dist*dist);
	    P unit_x = -(1.0/norm(line_perp))*line_perp;
	    P unit_y = (1.0/norm(dir))*dir;

	    P temp1 = dist*unit_x + y*unit_y;
	    P temp2 = dist*unit_x - y*unit_y;
	    return segment(temp1, temp2) += arg_circle.center();
	  }
      }
  }

  segment& operator * (const circle& arg1, const circle& arg2)
  {
    P p1 = arg1.center();
    P p2 = arg2.center();
    P n1 = arg1.perp();
    P n2 = arg2.perp();

    const double r1 = arg1.radius();
    const double r2 = arg2.radius();
    const double rad_diff = fabs(r2 - r1);
    const double rad_sum = r2 + r1;
    const double separation = norm(p2 - p1);

    if ((n1 != n2) && (n1 != -n2))
      throw join_error(NON_COPLANAR);

    else if (arg1.center() == arg2.center() && rad_diff < EPIX_EPSILON)
      throw join_error(COINCIDENT);
    
    else if ( fabs(separation - rad_sum ) < EPIX_EPSILON || 
	      fabs(separation - rad_diff) < EPIX_EPSILON )
      throw join_error(TANGENT);

    else if (separation > rad_sum)
      throw join_error(SEPARATED);

    else if (separation < rad_diff)
      throw join_error(CONCENTRIC);

    else
      {
	P dir_unit = (1.0/separation)*(p2-p1);

	double cos_theta 
	  = (r1*r1 - r2*r2 - separation*separation)/(2*r2*separation);
	double sin_theta = sqrt(1-cos_theta*cos_theta);

	P temp1 = r2*(cos_theta*dir_unit+sin_theta*(n2*dir_unit));
	P temp2 = r2*(cos_theta*dir_unit-sin_theta*(n2*dir_unit));

	return segment(temp1, temp2) += p2;
      }
  }

} /* end of namespace */
