/*** 
 *** Camera.h -- epix2::Camera class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 31, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 ***
 ***
 *** ePiX2 uses a camera metaphor to render Pictures. A Camera consists
 *** of:
 ***   - a Basis ({sea, sky, eye}, with the intuitive meanings:)
 ***   - a viewpoint
 ***   - a target (at the center of the field of view)
 ***   - the distance from the viewpoint to the target
 ***   - a projection mapping (the "lens")
 ***
 *** The viewer sits at given <distance> from the <target>, in the
 *** direction of the <eye>. The vectors <sea> and <sky> point
 *** horizontally to the right and vertically upward.
 ***/

#ifndef EPIX2_CAMERA
#define EPIX2_CAMERA

#include "Enums.h"
#include "Constants.h"
#include "Basis.h"

namespace ePiX2 {

  class Pair;   /***/
  class Point;  /***/
  class Edge;   /***/
  class Screen_Edge; /***/
  class Vector; /***/

  /* * * Camera.h * * */

  class Camera {

  public:

    Camera(const Point vpt = Point(0,0,EPIX2_INFTY),
	   epix2_lens_type lens=perspective);

    // get position and orientation
    Point viewpt(void) const { return the_viewpt; }
    Point target(void) const { return the_target; }

    Vector sea(void) const { return the_orient.sea(); }
    Vector sky(void) const { return the_orient.sky(); }
    Vector eye(void) const { return the_orient.eye(); }

    Pair operator() (const Point& arg) const;
    Screen_Edge operator() (const Edge& arg) const;

    epix2_lens_type lens(void) { return the_lens; }
    void set_lens(epix2_lens_type T) { the_lens=T; }
    bool is_linear(void); // lens preserves lines?

    // adjust position, orientation, and target
    // pitch: rotate camera up/down
    void rotate_sea(const double angle);
    // yaw: rotate camera left/right
    void rotate_sky(const double angle);
    // roll: rotate camera about viewing axis
    void rotate_eye(const double angle);

    // fix the_target, move the_viewpt radially along eye()
    void range(double d);
    // fix the_viewpt, move the_target radially along eye()
    void focus(double d);

    // fix the_target, set the_viewpt
    void at(const Point& arg);

    // fix the_viewpt, set the_target
    void look_at(const Point& arg);

  private:
    Point the_viewpt; // center of projection
    Point the_target; // mapped to Screen origin
    Basis the_orient; // our {sea, sky, eye}
    double the_distance;

    epix2_lens_type the_lens;

    void adjust_camera(const Point& vpt, const Point& tgt);

  }; // end of class Camera

} /* end of namespace */

#endif /* EPIX2_CAMERA */
