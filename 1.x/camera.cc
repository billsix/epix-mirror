/* 
 * camera.cc -- frame, camera, and helpers
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc9
 * Last Change: June 23, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
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

// n.b. ePiX::trig functions are sensitive to current angle units

#include <iostream>
// #include <cstdlib>

#include "globals.h"
#include "functions.h"
#include "triples.h"
#include "camera.h"
#include "output.h"

namespace ePiX {

  // frame constructor, suitable for (sea,sky,eye) frames: Usually we
  // know the eye vector and want to preserve its direction. The frame
  // is guaranteed to be right-handed, and the first arg is immaterial.
  frame::frame(P arg1, P arg2, P arg3)
  {
    if (norm(arg2*arg3) < EPIX_EPSILON) // too nearly linearly dependent
      epix_error("Linearly dependent arguments to frame");

    // partial Gram-Schmidt
    arg3 *= 1/norm(arg3); // normalize eye

    arg2 = arg2%arg3; // orthogonalize sky_vector, preserving screen direction
    arg2 *= 1/norm(arg2); // and normalize

    frame1 = arg2*arg3;
    frame2 = arg2;
    frame3 = arg3;
  }


  extern epix_camera camera;

  // screen projection mappings ("lenses")

  // Point projection to sea-sky plane from point at given "distance"
  // from "target" along "eye" axis
  pair shadow(P arg)
  {
    P arg_vector = arg - camera.get_target();
	
    // get frame coordinates; "|" = dot product
    double u1=camera.sea()|arg_vector;
    double u2=camera.sky()|arg_vector;
    double u3=camera.eye()|arg_vector;

    double dist = recip(camera.get_range());

    // inversion occurs if camera closer than object to screen plane
    return pair(u1/(1-dist*u3), u2/(1-dist*u3));
  }

  pair fisheye(P arg) 
  {
    // vector from camera to arg, based at camera
    P arg_vector = arg - camera.get_viewpt();
    double scale = recip(norm(arg_vector))*camera.get_range();

    // radially project to sphere through target centered at camera
    P temp = scale*arg_vector;
    double u1=camera.sea()|temp;
    double u2=camera.sky()|temp;

    // orthogonal projection along eye()
    return pair(u1, u2);
  }

  pair bubble(P arg) 
  {
    // vector from camera to arg, based at camera
    P arg_vector = arg - camera.get_viewpt();
    double d = camera.get_range();
    double scale = d*recip(norm(arg_vector));

    // radially project to sphere through target centered at camera
    P temp = scale*arg_vector;
    // get coordinates in camera frame
    double u1=camera.sea()|temp;
    double u2=camera.sky()|temp;
    double u3=camera.eye()|temp;

    // stereographic projection from target antipode
    return (d/(d-u3))*pair(u1, u2);
  }


  // fix target, move viewpt radially along eye()
  void epix_camera::range(double d)
  {
    if (d == 0)
      d = EPIX_INFTY;

    distance = d;
    viewpt = target + d*eye();
  }

  // fix viewpt, move target radially along eye()
  void epix_camera::focus(double d)
  {
    if (d == 0)
      d = EPIX_INFTY;

    distance = d;
    target = viewpt + (-d)*eye();
  }

  // fix target, set viewpt arbitrarily
  void epix_camera::at(P arg) 
  {
    viewpt = arg;
    P temp = arg - get_target();
    double d = norm(temp);
    temp *= 1.0/d; // normalize

    double z_rad = norm(temp%E_3);
    double z_ht = temp|E_3;

    if (z_rad < EPIX_EPSILON) // too close to "z-axis" through target
      {
	if (z_ht >= 0)
	  orient = frame();

	else // z_ht < 0
	  orient = frame(-E_1, E_2, -E_3);
      }

    else // far enough from axis
      orient = frame(P(-temp.x2(), temp.x1(), 0), P(0,0,1), temp);

    // and in any case
    distance = d;
  }

  void viewpoint(P arg) { camera.at(arg); }
  void viewpoint(double a1, double a2, double a3) { camera.at(P(a1,a2,a3)); }

  void epix_camera::look_at(P arg) 
  {
    camera.target = arg;
    P temp = camera.viewpt - arg;
    double d = norm(temp);
    temp *= 1.0/d; // normalize

    double z_rad = norm(temp%E_3);
    double z_ht = temp|E_3;

    if (z_rad < EPIX_EPSILON) // too close to "z-axis" through target
      {
	if (z_ht >= 0)
	  camera.orient = frame();

	else // z_ht < 0
	  camera.orient = frame(-E_1, E_2, -E_3);
      }

    else // far enough from axis
      camera.orient = frame(P(-temp.x2(), temp.x1(), 0), P(0,0,1), temp);

    // and in any case
    camera.distance = d;
  }

} /* end of namespace */
