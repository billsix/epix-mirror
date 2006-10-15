/* 
 * camera.h -- the ePiX camera class
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

/*
 * ePiX uses a camera metaphor to view points in R^3. The camera consists
 * of:
 *   - a frame ({sea, sky, eye}, with the intuitive meanings:)
 *   - a point to look at (the "target")
 *   - the distance from the camera to the target
 *   - a projection mapping from R^3 to R^2 (the "lens")
 *
 * While specific behavior is determined by the lens, the idea is that the
 * viewer sits at given <distance> from the <target>, in the direction of
 * the <eye>. The vectors <sea> and <sky> point horizontally to the right
 * and vertically upward on the screen. A <distance> of zero is taken to
 * mean "infinitely far away"; for example, point projection (<shadow>)
 * becomes orthogonal projection along the <eye> axis. Again, a specific
 * lens can override this behavior, for example by ignoring the distance.
 *
 *
 * This file provides:
 *
 *   - the camera class and routines:
 *       void rotate_sea(double); // change camera orientation
 *       void rotate_sky(double); // about respective axes
 *       void rotate_eye(double);
 *
 *       void range(double d); // set distance from camera to origin
 *       void look_at(P arg); // set target (center of field of view)
 *       void lens(pair (*screen_projection(P))); // set projection mapping
 *
 *       pair shoot(P arg); // project a point to the screen ("take a photo")
 */

#ifndef EPIX_CAMERA
#define EPIX_CAMERA

#include "pairs.h"
#include "triples.h"
#include "frame.h"

namespace ePiX {

  // screen projection mappings ("lenses")
  pair shadow(const P& arg);
  pair fisheye(const P& arg);
  pair bubble(const P& arg);

  class epix_camera {
  public:
    // defaults to orthog projection on (x,y) plane from pos z-axis
    epix_camera();

    P get_viewpt() const;
    P get_target() const;
    double get_range() const;

    // get frame vectors
    P sea() const;
    P sky() const;
    P eye() const;

    // adjust position, orientation, and target
    // pitch: rotate camera up/down
    epix_camera& rotate_sea(double angle);

    // yaw: rotate camera left/right
    epix_camera& rotate_sky(double angle);

    // roll: rotate camera about viewing axis
    epix_camera& rotate_eye(double angle);

    // fix target, move viewpt radially along eye()
    epix_camera& range(double d);
    // fix viewpt, move target radially along eye()
    epix_camera& focus(double d);

    // fix target, set viewpt arbitrarily
    epix_camera& at(const P& arg);
    epix_camera& at(const double, const double, const double);

    // fix viewpt, set target arbitrarily
    epix_camera& look_at(const P& arg);
    epix_camera& look_at(const double, const double, const double);

    // set projection mapping
    epix_camera& lens(pair proj(const P&));

    // project a point to the screen ("shoot a photo")
    pair operator() (const P&) const;

  private:
    P viewpt;
    P target;
    frame  orient;
    double distance;
    pair (*screen_projection)(const P&);

  }; // end of class epix_camera 

  extern epix_camera camera;

  // set global camera's location
  void viewpoint(const P&);
  void viewpoint(double a1, double a2, double a3);

} // end of namespace

#endif /* EPIX_CAMERA */
