/* 
 * camera.cc -- frame, camera, and helpers
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 10, 2006
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

// #include <iostream>
// #include <cstdlib>

#include "globals.h"
#include "functions.h"
#include "triples.h"

#include "frame.h"
#include "camera.h"

namespace ePiX {

  extern epix_camera camera;

  // screen projection mappings ("lenses")

  // Point projection to sea-sky plane from point at given "distance"
  // from "target" along "eye" axis
  pair shadow(const P& arg)
  {
    P arg_vector(arg - camera.get_target());
	
    // get frame coordinates; "|" = dot product
    double u1(camera.sea()|arg_vector);
    double u2(camera.sky()|arg_vector);
    double u3(camera.eye()|arg_vector);

    double dist(recip(camera.get_range()));

    // inversion occurs if camera closer than object to screen plane
    return pair(u1/(1-dist*u3), u2/(1-dist*u3));
  }

  pair fisheye(const P& arg) 
  {
    // vector from camera to arg, based at camera
    P arg_vector(arg - camera.get_viewpt());
    double scale(recip(norm(arg_vector))*camera.get_range());

    // radially project to sphere through target centered at camera
    P temp(scale*arg_vector);
    double u1(camera.sea()|temp);
    double u2(camera.sky()|temp);

    // orthogonal projection along eye()
    return pair(u1, u2);
  }

  pair bubble(const P& arg)
  {
    // vector from camera to arg, based at camera
    P arg_vector(arg - camera.get_viewpt());
    double d(camera.get_range());
    double scale(d*recip(norm(arg_vector)));

    // radially project to sphere through target centered at camera
    P temp(scale*arg_vector);
    // get coordinates in camera frame
    double u1(camera.sea()|temp);
    double u2(camera.sky()|temp);
    double u3(camera.eye()|temp);

    // stereographic projection from target antipode
    return (d/(d-u3))*pair(u1, u2);
  }

  // camera functions
  epix_camera::epix_camera()
    : viewpt(P(0,0,EPIX_INFTY)), target(P(0,0,0)), orient(frame()),
      distance(EPIX_INFTY), screen_projection(shadow) { }

  P epix_camera::get_viewpt() const
  {
    return viewpt;
  }
  P epix_camera::get_target() const
  {
    return target;
  }
  double epix_camera::get_range() const
  {
    return distance;
  }

  // get frame vectors
  P epix_camera::sea() const
  {
    return orient.sea();
  }
  P epix_camera::sky() const
  {
    return orient.sky();
  }
  P epix_camera::eye() const
  {
    return orient.eye();
  }

  epix_camera& epix_camera::rotate_sea(double angle)
  { 
    orient.rot1(angle); 
    viewpt = target + distance*orient.eye();

    return *this;
  }

  // yaw: rotate camera left/right
  epix_camera& epix_camera::rotate_sky(double angle)
  { 
    orient.rot2(angle); 
    viewpt = target + distance*orient.eye();

    return *this;
  }

  // roll: rotate camera about viewing axis
  epix_camera& epix_camera::rotate_eye(double angle)
  { 
    orient.rot3(angle); // target unchanged

    return *this;
  }


  // fix target, move viewpt radially along eye()
  epix_camera& epix_camera::range(double d)
  {
    if (d == 0)
      d = EPIX_INFTY;

    distance = d;
    viewpt = target + d*eye();

    return *this;
  }

  // fix viewpt, move target radially along eye()
  epix_camera& epix_camera::focus(double d)
  {
    if (d == 0)
      d = EPIX_INFTY;

    distance = d;
    target = viewpt + (-d)*eye();

    return *this;
  }

  // fix target, set viewpt arbitrarily
  epix_camera& epix_camera::at(const P& arg) 
  {
    viewpt = arg;
    P temp(arg - get_target());
    double d(norm(temp));
    temp *= 1.0/d; // normalize

    double z_rad(norm(temp%E_3));
    double z_ht(temp|E_3);

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

    return *this;
  }

  epix_camera& epix_camera::at(const double a1,
			       const double a2,
			       const double a3)
  {
    return at(P(a1,a2,a3));
  }

  epix_camera& epix_camera::look_at(const P& arg) 
  {
    camera.target = arg;
    P temp(camera.viewpt - arg);
    double d(norm(temp));
    temp *= 1.0/d; // normalize

    double z_rad(norm(temp%E_3));
    double z_ht(temp|E_3);

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

    return *this;
  }

  epix_camera& epix_camera::look_at(const double a1,
				    const double a2,
				    const double a3)
  {
    return look_at(P(a1,a2,a3));
  }


  // set projection mapping
  epix_camera& epix_camera::lens(pair proj(const P&))
  {
    screen_projection = proj;
    return *this;
  }

  // project a point to the screen ("shoot a photo")
  pair epix_camera::operator() (const P& arg) const
  {
    return screen_projection(arg);
  }

  // global functions
  void viewpoint(const P& arg)
  {
    ePiX::camera.at(arg);
  }
  void viewpoint(double a1, double a2, double a3)
  {
    ePiX::camera.at(a1,a2,a3);
  }

} /* end of namespace */
