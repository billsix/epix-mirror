/* 
 * sphere.cc -- ePiX::sphere class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc14
 * Last Change: July 26, 2004
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

#include "plane.h"
#include "circle.h"
#include "curves.h"
#include "cropping.h"

namespace ePiX {

  extern epix_camera camera;

  plane::plane(P p1, P p2, P p3) 
  { 
    P perp=(p3-p1)*(p2-p1);
    double norm_perp=norm(perp);
    if (norm_perp < EPIX_EPSILON)
      throw constructor_error(COLLINEAR_PTS);

    else
      {
	pt = p1;
	N = (1/norm_perp)*perp;
      }
  }

  // draw Line of intersection between non-parallel planes
  void plane::operator* (const plane P1)
  {
    const plane P2=*this;

    P N3=(P1.N)*(P2.N);
    double temp=norm(N3);

    if (temp<EPIX_EPSILON)
      throw join_error(PARALLEL);

    else  // N3 non-zero, parallel to intersection
      {
	N3 *= 1/temp; // normalize

	P perp=(P1.N)*N3; // unit vector in P1, perp to intersection
	P point=P1.pt + ((((P2.pt-P1.pt)|(P2.N))/(perp|(P2.N)))*perp);

	Line(point, point+N3); // draw line through points
      }
  }

  // intersection
  circle plane::operator* (const sphere& S) const
    {
      double rad=S.radius();
      P perp=(*this).N;
      // signed dist from S.ctr to *this
      double height = ((*this).pt-S.center())|perp;

      if (rad<fabs(height))
      throw join_error(SEPARATED);

      else if (rad == fabs(height))
      throw join_error(TANGENT);

      else
	return circle(S.center()+height*perp,
		      sqrt(rad*rad-height*height), perp);
    }

  void plane::draw()
  {
    // outward-oriented clip_box faces
    plane face1_min = plane(P(clip1_min(),0,0), -E_1);
    plane face1_max = plane(P(clip1_max(),0,0),  E_1);

    plane face2_min = plane(P(0,clip2_min(),0), -E_2);
    plane face2_max = plane(P(0,clip2_max(),0),  E_2);

    plane face3_min = plane(P(0,0,clip3_min()), -E_3);
    plane face3_max = plane(P(0,0,clip3_max()),  E_3);

    try {
      (*this)*face1_min;
      (*this)*face1_max;
    }
    catch (join_error PARALLEL) { };

    try {
      (*this)*face2_min;
      (*this)*face2_max;
    }
    catch (join_error PARALLEL) { };

    try {
      (*this)*face3_min;
      (*this)*face3_max;
    }
    catch (join_error PARALLEL) { };
  }

} /* end of namespace */
