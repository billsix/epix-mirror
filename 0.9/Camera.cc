/* 
 * Camera.cc -- camera and methods
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 01, 2003
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

// n.b. ePiX3d::trig functions are sensitive to current angle units

#include "Position.h"
#include "Vector.h"
#include "Pair.h"
#include "Camera.h"
#include "functions.h"

namespace ePiX3d {

  // screen projection mappings ("lenses")

  extern Camera camera;

  // Point projection to sea-sky plane from point at given "distance"
  // from "target" along "eye" axis
  Pair shadow(Position arg)
  {
    Vector arg_vector = Vector(Position(), 
			       arg.x1() - camera.target.x1(),
			       arg.x2() - camera.target.x2(),
			       arg.x3() - camera.target.x3());
	
    // get Frame coordinates; "|" = dot product
    double u1=camera.orientation.sea()|arg_vector;
    double u2=camera.orientation.sky()|arg_vector;
    double u3=camera.orientation.eye()|arg_vector;

    // if Distance = 0, dist (the reciprocal distance) is also 0
    double dist = Recip(camera.distance);

    return Pair(u1/(1+dist*u3), u2/(1+dist*u3));
  }

  Pair fisheye(Position arg) 
  {
    //    double d = camera.distance;
    //    Vector cam_eye = camera.orientation.eye();

    // vector from camera to arg, based at origin
    Vector arg_vector = Vector(Position(), 
			       arg.x1() - camera.target.x1(),
			       arg.x2() - camera.target.x2(),
			       arg.x3() - camera.target.x3());

    double length = norm(arg_vector);

    if (fabs(length) < EPIX_EPSILON)
      return Pair(0,0);

    else
      {
	// radially project to sphere of radius d centered at camera
	Vector temp = (1/length)*arg_vector;
	double u1=camera.orientation.sea()|temp;
	double u2=camera.orientation.sky()|temp;
	//	double u3=-camera.orientation.eye()|temp;
	return Pair(u1, u2);
	//	return (std::acos(u3/d)/sqrt(d*d-u3*u3))*Pair(u1,u2);
      }
  }

} /* end of namespace */
