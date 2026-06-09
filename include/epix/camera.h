/* 
 * camera.h -- ePiX::Camera
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.21
 * Last Change: September 22, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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

/*
 * A Camera has a location, orientation, lens, and color filter.  The
 * viewer sits at given <distance> from the <target>, in the direction
 * of the <eye>. The vectors <sea> and <sky> point horizontally to the
 * right and vertically upward on the screen.  The viewing angle is
 * controlled by changing the viewer, target, and/or the distance
 * between them.
 *
 * The implementation data comprise:
 *   - a frame ({sea, sky, eye}, with the intuitive meanings:)
 *   - a viewpoint
 *   - a target (at the center of the field of view)
 *   - the distance from the viewpoint to the target
 *   - a "filter" color, Neutral by default, for color separation, etc.
 *   - a Lens
 *
 * A Lens is a mapping P -> pair. In the implementation, the Camera
 * ("body") passes the orientation, target, and distance to the Lens
 * operator.
 */

#ifndef EPIX_CAMERA
#define EPIX_CAMERA

#include "triples.h"
#include "frame.h"
#include "Color.h"

namespace ePiX {

  class halfspace;
  class Lens;
  class pair;

  class Camera {
  public:
    Camera();
    Camera(const P& vpt);

    Camera(const Camera&);
    Camera& operator= (const Camera&);
    ~Camera();

    // rotate
    Camera& tilt(double angle); // up/down (pitch)
    Camera& pan(double angle);  // left/right (yaw)
    Camera& roll(double angle); // about viewing axis

    // fix target, move viewpt radially along eye()
    Camera& range(double);
    // fix viewpt, move target radially along eye()
    Camera& focus(double);

    // set clip distance
    Camera& clip_range(double);

    Camera& at(const P& arg);      // fix target, set viewpt
    Camera& look_at(const P& arg); // fix viewpt, set target

    // alternative syntax
    Camera& at(double, double, double);
    Camera& look_at(double, double, double);

    Camera& filter(const Color&);

    // set lens
    Camera& perspective(); // default
    Camera& orthog(); // faster
    Camera& fisheye();
    Camera& bubble();

    // Functions for internal use
    // Camera maps
    Color operator() (const Color&) const; // filter
    pair  operator() (const P&) const; // lens map
    bool  is_linear() const; // lens preserves lines?

    bool needs_clip() const; // lens needs scene pre-clipping
    halfspace clip_plane() const;

    // situation
    P eye() const;
    P viewpt() const;

  private:
    P the_viewpt; // center of projection
    P the_target; // mapped to Screen origin
    frame the_orient; // our {sea, sky, eye}
    double the_distance;
    double the_clip_range;

    Color the_filter;
    void adjust(); // re-orient according to target, viewpt
    Lens* the_lens;
  }; // end of class Camera

  // global Camera
  Camera& cam();
  extern Camera& camera;
} // end of namespace

#endif /* EPIX_CAMERA */
