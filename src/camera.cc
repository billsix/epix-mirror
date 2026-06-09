/* 
 * camera.cc -- ePiX::camera functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.21
 * Last Change: September 22, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <cmath>

#include "errors.h"
#include "constants.h"

#include "pairs.h"
#include "triples.h"
#include "frame.h"

#include "functions.h"

#include "halfspace.h"

#include "lens.h"
#include "camera.h"

namespace ePiX {

  // Need to initialize frame argument for Apple gcc; otherwise cam()
  // gets null vectors in frame()...
  Camera::Camera()
    : the_viewpt(P(0,0,EPIX_INFTY)), the_target(P(0,0,0)), 
      the_orient(frame(E_1, E_2, E_3)),
      the_distance(EPIX_INFTY), the_clip_range(MIN_CLIP),
      the_filter(Neutral()), the_lens(new Perspective()) { }

  // viewpt, target, orient
  Camera::Camera(const P& vpt)
    : the_viewpt(vpt), the_target(P(0,0,0)), the_clip_range(MIN_CLIP),
      the_filter(Neutral())
  {
    if (the_viewpt == P(0,0,0))
      {
	epix_warning("Cannot initialize Camera at Origin, using (0, 0, oo)");
	the_viewpt = P(0, 0, EPIX_INFTY);
      }
    P viewer(the_viewpt - the_target);
    the_distance = norm(viewer);

    viewer *= recip(norm(viewer));

    // viewer is "eye", if possible E_3 is "sky"
    P tmp_sea(E_3*viewer);

    if (norm(tmp_sea) < EPIX_EPSILON) // we're on the "z-axis through vpt"
      tmp_sea = E_1;

    // eye*sea = sky
    the_orient = frame(tmp_sea, viewer*tmp_sea, viewer);
    adjust();

    the_lens = new Perspective();
  }


  Camera::Camera(const Camera& cam)
    : the_viewpt(cam.the_viewpt), the_target(cam.the_target),
      the_orient(cam.the_orient), the_distance(cam.the_distance),
      the_clip_range(cam.the_clip_range), the_filter(cam.the_filter),
      the_lens(cam.the_lens->clone()) { }

  Camera& Camera::operator= (const Camera& cam)
  {
    Lens* tmp_lens(cam.the_lens->clone());
    the_lens = tmp_lens;

    the_viewpt = cam.the_viewpt;
    the_target = cam.the_target;
    the_orient =cam.the_orient;
    the_distance = cam.the_distance;
    the_clip_range = cam.the_distance;

    the_filter = cam.the_filter;

    return *this;
  }

  Camera::~Camera()
  {
    delete the_lens;
  }

  // rotate up/down
  Camera& Camera::tilt(double angle)
  {
    the_orient.rot1(-angle);
    the_target = the_viewpt + (-the_distance)*the_orient.eye();
    return *this;
  }

  // rotate left/right
  Camera& Camera::pan(double angle)
  {
    the_orient.rot2(-angle);
    the_target = the_viewpt + (-the_distance)*the_orient.eye();
    return *this;
  }

  // rotate about viewing axis
  Camera& Camera::roll(double angle)
  { 
    the_orient.rot3(-angle); // target unchanged
    return *this;
  }

  // fix target, move viewpt radially along eye()
  Camera& Camera::range(double d)
  {
    if (d == 0)
      d = EPIX_INFTY;

    the_distance = d;
    the_viewpt = the_target + d*the_orient.eye();

    return *this;
  }

  // fix viewpt, move target radially along eye()
  Camera& Camera::focus(double d)
  {
    if (d == 0)
      d = EPIX_INFTY;

    the_distance = d;
    the_target = the_viewpt - d*the_orient.eye();

    return *this;
  }

  // clip everything behind this
  Camera& Camera::clip_range(double dist)
  {
    the_clip_range = (dist < MIN_CLIP ? MIN_CLIP : dist);
    return *this;
  }

  // fix target, set viewpt
  Camera& Camera::at(const P& vpt)
  {
    the_viewpt = vpt;
    adjust();

    return *this;
  }

  // fix viewpt, set target
  Camera& Camera::look_at(const P& tgt)
  {
    the_target = tgt;
    adjust();

    return *this;
  }

  Camera& Camera::at(double x1, double x2, double x3)
  {
    the_viewpt = P(x1, x2, x3);
    adjust();

    return *this;
  }

  // fix viewpt, set target
  Camera& Camera::look_at(double x1, double x2, double x3)
  {
    the_target = P(x1, x2, x3);
    adjust();

    return *this;
  }

  Camera& Camera::filter(const Color& filter)
  {
    the_filter=filter;
    return *this;
  }

  Camera& Camera::perspective()
  {
    delete the_lens;
    the_lens = new Perspective;
    return *this;
  }

  Camera& Camera::orthog()
  {
    delete the_lens;
    the_lens = new Orthog;
    return *this;
  }

  Camera& Camera::fisheye()
  {
    delete the_lens;
    the_lens = new Fisheye;
    return *this;
  }

  Camera& Camera::bubble()
  {
    delete the_lens;
    the_lens = new Bubble;
    return *this;
  }

  Color Camera::operator() (const Color& in_color) const
  {
    Color temp = the_filter;
    return temp.filter(in_color);
  }

  // lens interface
  pair Camera::operator() (const P& arg) const
  {
    return (*the_lens)(arg, the_orient, the_viewpt, the_distance);
  }

  bool Camera::is_linear() const
  {
    return (*the_lens).is_linear();
  }

  bool Camera::needs_clip() const
  {
    return (*the_lens).needs_clip();
  }


  halfspace Camera::clip_plane() const
  {
    const P N(-the_orient.eye());
    return halfspace(the_viewpt + the_clip_range*N, N);
  }

  P Camera::eye() const
  { 
    return the_orient.eye();
  }

  P Camera::viewpt() const
  {
    return the_viewpt;
  }

  // private function
  void Camera::adjust()
  {
    P temp(the_target - the_viewpt);
    the_distance = norm(temp);

    if (the_distance < EPIX_EPSILON)
      {
	epix_warning("Cannot make viewpoint and target the same, no action");
	return;
      }

    // else
    temp *= -1.0/the_distance; // reverse direction

    double z_ht(temp|E_3);

    // too close to "z-axis" through target?
    if (sqrt((1-z_ht)*(1+z_ht)) < EPIX_EPSILON)
      {
        if (z_ht >= 0)
          the_orient = frame();

        else // z_ht < 0
          the_orient = frame(-E_1, E_2, -E_3);
      }

    else // far enough from axis
      the_orient = frame(P(-temp.x2(), temp.x1(), 0), E_3, temp);
  }


  Camera& cam()
  {
    static Camera* the_camera(new Camera());
    return *the_camera;
  }

  Camera& camera(cam());

} // end of namespace
