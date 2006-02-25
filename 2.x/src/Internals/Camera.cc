/* 
 * Camera.cc -- Camera functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: January 09, 2006
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

// n.b. ePiX::trig functions are sensitive to current angle units

#include "Pair.h"
#include "Point.h"
#include "Vector.h"
#include "Basis.h"
#include "Constants.h"
#include "Functions.h"
#include "Errors.h"
#include "Camera.h"

namespace ePiX2 {

  // viewpt, target, orient
  Camera::Camera(const Point vpt)
    : the_viewpt(vpt), the_target(Origin), the_filter(White())
  {
    the_distance = norm(vpt-Origin);
    the_orient   = Basis();
    the_orient.move_to(vpt);

    adjust(the_viewpt, the_target);

    the_filter=White();
    the_lens = new Perspective();
  }

  Camera::~Camera(void) { delete the_lens; }

  void Camera::perspective(void)
  {
    delete the_lens;
    the_lens = new Perspective;
  }

  void Camera::orthog(void)
  {
    delete the_lens;
    the_lens = new Orthog;
  }

  void Camera::fisheye(void)
  {
    delete the_lens;
    the_lens = new Fisheye;
  }

  void Camera::bubble(void)
  {
    delete the_lens;
    the_lens = new Bubble;
  }

  // rotate up/down
  void Camera::tilt(const double angle)
  {
    the_orient.rotate_sea(-angle);
    the_target = the_viewpt + (-the_distance)*the_orient.eye();
  }

  // rotate left/right
  void Camera::pan(const double angle)
  {
    the_orient.rotate_sky(-angle);
    the_target = the_viewpt + (-the_distance)*the_orient.eye();
  }

  // rotate about viewing axis
  void Camera::roll(const double angle)
  { 
    the_orient.rotate_eye(-angle); // target unchanged
  }

  bool Camera::is_linear(void)
  {
    return (*the_lens).is_linear();
  }


  Color Camera::operator() (const Color in_color) const
  {
    return the_filter(in_color);
  }

  // lens interface
  Pair Camera::operator() (const Point arg) const
  {
    return (*the_lens)(arg, the_orient, the_viewpt, the_distance);
  }

  Vector Camera::eye(void) const
  { 
    return the_orient.eye();
  }

  Point Camera::viewpt(void) const
  {
    return the_viewpt;
  }


  /* * * lens implementations * * */
  Pair Camera::Perspective::operator()
    (const Point& arg, const Basis& orient, const Point& viewpt,
     const double dist) const
  {
    Vector arg_vector = arg - viewpt;

    // get arg's location in our coordinates
    double u1 = orient.sea()|arg_vector;
    double u2 = orient.sky()|arg_vector;
    double u3 = orient.eye()|arg_vector;
    return (dist*fabs(recip(u3)))*Pair(u1, u2);
  } // end of Camera::Perspective::operator()

  Pair Camera::Orthog::operator()
    (const Point& arg, const Basis& orient, const Point& viewpt,
     const double dist) const
  {
    Vector arg_vector = arg - viewpt;
    double u1 = orient.sea()|arg_vector;
    double u2 = orient.sky()|arg_vector;

    return Pair(u1, u2);
  } // end of Camera::Orthog::operator()

  Pair Camera::Fisheye::operator()
    (const Point& arg, const Basis& orient, const Point& viewpt,
     const double dist) const
  {
    Vector arg_vector = arg - viewpt;
    double u1 = orient.sea()|arg_vector;
    double u2 = orient.sky()|arg_vector;
    //    double u3 = orient.eye()|arg_vector;

    // radial projection to camera sphere, then orthogonal projection
    return dist*recip(norm(arg_vector))*Pair(u1, u2);
  } // end of Camera::Fisheye::operator()

  Pair Camera::Bubble::operator()
    (const Point& arg, const Basis& orient, const Point& viewpt,
     const double dist) const
  {
    Vector arg_vector = arg - viewpt;
    arg_vector *= recip(norm(arg_vector));

    double u1 = orient.sea()|arg_vector;
    double u2 = orient.sky()|arg_vector;
    double u3 = orient.eye()|arg_vector;

    // radial projection to camera sphere, then stereographic projection
    return 2*dist*recip(1-u3)*Pair(u1, u2);
  } // end of Camera::Bubble::operator()


  // fix target, move viewpt radially along eye()
  void Camera::range(double d)
  {
    if (d == 0)
      d = EPIX2_INFTY;

    the_distance = d;
    the_viewpt = the_target + d*the_orient.eye();
    the_orient.move_to(the_viewpt);
  }

  // fix viewpt, move target radially along eye()
  void Camera::focus(double d)
  {
    if (d == 0)
      d = EPIX2_INFTY;

    the_distance = d;
    the_target = the_viewpt + (-d)*the_orient.eye();
  }

  // private function
  void Camera::adjust(const Point& vpt, const Point& tgt)
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
    adjust(vpt, tgt);
  }

  // fix viewpt, set target
  void Camera::look_at(const Point& tgt)
  {
    the_target = tgt;
    const Point& vpt = the_viewpt;
    adjust(vpt, tgt);
  }

} /* end of namespace */
