/* 
 * Camera.cc -- Camera functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 29, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#include "Pair.h"
#include "Point.h"
#include "Vector.h"
#include "Edge.h"
#include "Basis.h"
#include "Constants.h"
#include "Functions.h"
#include "Errors.h"
#include "Camera.h"

namespace ePiX2 {

  // viewpt, target, orient
  Camera::Camera(const Point vpt, epix2_lens_type lens)
    : the_viewpt(vpt), the_target(Origin), the_lens(lens)
  {
    the_distance = norm(vpt-Origin);
    the_orient   = Basis();
    the_orient.move_to(vpt);
  }

  // tilt: rotate up/down
  void Camera::rotate_sea(const double angle)
  {
    the_orient.rotate_sea(angle);
    the_target = the_viewpt + (-the_distance)*eye();
  }

  // pan: rotate left/right
  void Camera::rotate_sky(const double angle)
  {
    the_orient.rotate_sky(angle);
    the_target = the_viewpt + (-the_distance)*eye();
  }

  // roll: rotate about viewing axis
  void Camera::rotate_eye(const double angle)
  { 
    the_orient.rotate_eye(angle); // target unchanged
  }

  // N.B. Update if new lenses are added
  bool Camera::is_linear(void)
  {
    return (the_lens == orthogonal || 
	    the_lens == perspective);
  }

  // point projection lens
  Pair Camera::operator() (const Point& arg) const
  {
    Vector arg_vector = arg - the_viewpt;
    Pair image;

   // get arg's location in our coordinates
    double u1 = sea()|arg_vector;
    double u2 = sky()|arg_vector;
    double u3 = eye()|arg_vector;

    double temp=0;

    switch(the_lens) {
    case orthogonal:

      image = Pair(u1, u2);
      break;

    case perspective:

      image = (the_distance*fabs(recip(u3)))*Pair(u1, u2);
      break;


    case fisheye:

      // radial projection to camera sphere, then orthogonal projection
      temp = the_distance*recip(norm(arg_vector));
      image = temp*Pair(u1, u2);
      break;

    case bubble:

      // radial projection to camera sphere, then stereographic projection
      temp = the_distance*recip(norm(arg_vector));
      image = 2*temp*recip(the_distance-u3)*Pair(u1, u2);
      break;

    } // end of switch(lens)

    return image;

  } // end of Pair operator()

  Screen_Edge Camera::operator() (const Edge& arg) const
  {
    return Screen_Edge(Camera::operator() (arg.first), 
		       Camera::operator() (arg.second), 
		       arg.line_color, arg.drawn);
  }

  // fix target, move viewpt radially along eye()
  void Camera::range(double d)
  {
    if (d == 0)
      d = EPIX2_INFTY;

    the_distance = d;
    the_viewpt = the_target + d*eye();
    the_orient.move_to(the_viewpt);
  }

  // fix viewpt, move target radially along eye()
  void Camera::focus(double d)
  {
    if (d == 0)
      d = EPIX2_INFTY;

    the_distance = d;
    the_target = the_viewpt + (-d)*eye();
  }

  // private function
  void Camera::adjust_camera(const Point& vpt, const Point& tgt)
  {
    Vector temp = tgt - vpt;
    the_distance = norm(temp);

    temp *= -recip(the_distance); // normalize, reverse direction

    double z_rad = norm(temp%E_3(vpt));
    double z_ht = (temp|E_3(vpt));

    if (z_rad < EPIX2_EPSILON) // too close to "z-axis" through target
      {
        if (z_ht >= 0)
          the_orient = Basis(vpt,  E_1(vpt), E_2(vpt),  E_3(vpt));

        else // z_ht < 0
          the_orient = Basis(vpt, -E_1(vpt), E_2(vpt), -E_3(vpt));
      }

    else // far enough from axis
      the_orient = Basis(vpt, Vector(vpt, -temp.dx2(), temp.dx1(), 0), 
			 E_3(vpt), temp);
  }

  // fix target, set viewpt
  void Camera::at(const Point& vpt)
  {
    the_viewpt = vpt;
    const Point& tgt = the_target;
    adjust_camera(vpt, tgt);
  }

  // fix viewpt, set target
  void Camera::look_at(const Point& tgt)
  {
    the_target = tgt;
    const Point& vpt = the_viewpt;
    adjust_camera(vpt, tgt);
  }

} /* end of namespace */
