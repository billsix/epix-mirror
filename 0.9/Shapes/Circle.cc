/* 
 * circle.cc -- ePiX::circle class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc7
 * Last Change: April 18, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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

#include "exceptions.h"
#include "circle.h"
#include "segment.h"
#include "../lengths.h"
#include "../curves.h"

namespace ePiX {

  // three-point circle constructor
  circle::circle(triple pt1, triple pt2, triple pt3)
  {
    if (raw_len(pt2-pt1) < EPIX_EPSILON ||
	raw_len(pt3-pt1) < EPIX_EPSILON ||
	raw_len(pt3-pt2) < EPIX_EPSILON)
      throw constructor_error(MULTIPLICITY);

    else if (raw_len((pt2 - pt1)*(pt3 - pt1)) < EPIX_EPSILON)
      throw constructor_error(COLLINEAR_PTS);

    else
      {
	triple temp = (pt2 - pt1)*(pt3 - pt1);
	perp = (1.0/raw_len(temp))*temp;

	triple q1 = midpoint(pt1, pt2);
	triple dir1 = perp*q1;

	triple q2 = midpoint(pt1, pt3);
	triple dir2 = perp*q2;

	ctr = intersect(segment(q1, q1+dir1), segment(q2, q2+dir2));
	rad = norm(ctr - pt1);
      }
  }

  // Intersection operators
  segment operator * (segment& arg_seg, circle& arg_circle)
  {
    triple dir = arg_seg.end2() - arg_seg.end1();

    if ( (dir|arg_circle.normal()) != 0)
      throw join_error(NON_COPLANAR);

    else
      {
	triple to_ctr = arg_circle.center() - arg_seg.end1();
	triple line_perp = arg_circle.normal()*dir;
	
	double dist = (to_ctr|line_perp)/(line_perp|line_perp);

	triple P1 = arg_seg.end1();
	triple P2 = arg_seg.end2();

	if (fabs(dist) > arg_circle.radius())
	  throw join_error(SEPARATED);

	else if (fabs(dist) == arg_circle.radius())
	  throw join_error(TANGENT);

	else
	  {
	    double y = sqrt(pow(arg_circle.radius(), 2) - dist*dist);
	    triple unit_x = -(1.0/raw_len(line_perp))*line_perp;
	    triple unit_y = (1.0/raw_len(dir))*dir;

	    triple temp1 = arg_circle.center() + dist*unit_x + y*unit_y;
	    triple temp2 = arg_circle.center() + dist*unit_x - y*unit_y;
	    return segment(arg_circle.center()+temp1, 
			   arg_circle.center()+temp2);
	  }
      }
  }

  segment operator * (circle& arg1, circle& arg2)
  {
    triple p1 = arg1.center();
    triple p2 = arg2.center();
    double r1 = arg1.radius();
    double r2 = arg2.radius();
    double separation = norm(p2 - p1);

    if (arg1.normal() != arg2.normal() && arg1.normal() != -arg2.normal())
      throw join_error(NON_COPLANAR);

    else if (arg1.center() == arg2.center() && arg1.radius() == arg2.radius())
      throw join_error(COINCIDENT);
    
    else if (separation == r1+r2 || separation == fabs(r2 - r1))
      throw join_error(TANGENT);

    else if (separation > r1+r2)
      throw join_error(SEPARATED);

    else if (separation < fabs(r2 - r1))
      throw join_error(CONCENTRIC);

    else
      {
	double dir_len = norm(p2 - p1);
	triple dir_unit = (1.0/dir_len)*(p2-p1);
	triple unit_perp = arg2.normal();

	double cos_theta = (r1*r1 - r2*r2 - dir_len*dir_len)/(2*r2*dir_len);
	double sin_theta = sqrt(1-cos_theta*cos_theta);

	triple temp1 = -r2*(cos_theta*dir_unit+sin_theta*(unit_perp*dir_unit));
	triple temp2 = -r2*(cos_theta*dir_unit-sin_theta*(unit_perp*dir_unit));

	return segment(p2+temp1, p2+temp2);
      }
  }

  void draw(circle& arg)
  {
    triple axis1, axis2;
    double r = arg.radius();

    // arg.normal() is a unit vector
    if (norm(arg.normal() - E_3) < EPIX_EPSILON || 
	norm(arg.normal() + E_3) < EPIX_EPSILON)
      {
	axis1 = r*E_1;
	axis2 = r*E_2;
      }
    else
      {
	const triple N = arg.normal();
	triple temp = P(0,0,1);
	axis1 = temp/N; // orthogonalize
	axis1 *= r/norm(axis1);

	axis2 = (arg.normal())*axis1; // cross product
      }

    ellipse(arg.center(), axis1, axis2);
  }

} /* end of namespace */
