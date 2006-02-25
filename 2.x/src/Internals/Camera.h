/*** 
 *** Camera.h -- epix2::Camera class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 09, 2006
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
 ***/
/*
 * Camera.h
 *
 * ePiX2 uses a camera metaphor to render Pictures. A camera provides
 * functions to set the location, orientation, target (center of the
 * view field) and range (distance to the target), filter, and lens:
 *
 *   - Change orientation (angles in current units)
 *      tilt(angle): rotate up/down (pitch)
 *      pan(angle):  rotate left/right (yaw)
 *      roll(angle); rotate about viewing axis
 *   - Change target, viewpoint, and location
 *      range(double d): fix target, move viewpt radially
 *      focus(double d): fix viewpt, move target radially
 *      at(const Point&): fix target, set viewpt
 *      look_at(const Point&): fix viewpt, set target
 *   - Set lens
 *      perspective() (default), orthog(), fisheye(), bubble()
 *
 * The implementation data consists of:
 *   - a Basis ({sea, sky, eye}, with the intuitive meanings:)
 *   - a viewpoint
 *   - a target (at the center of the field of view)
 *   - the distance from the viewpoint to the target
 *   - a "filter" color, White by default, for color separation, etc.
 *   - A Lens (below)
 *
 * The viewer sits at given <distance> from the <target>, in the
 * direction of the <eye>. The vectors <sea> and <sky> point
 * horizontally to the right and vertically upward.
 *
 * Conceptually, a Lens provides a mapping Point -> Pair. In the
 * implementation, the Camera ("body") passes the orientation, target,
 * and distance to the Lens operator. The Camera contains a pointer to
 * Lens, a base class. Adding a new Lens entails:
 *   - Declaring a new child of Camera::Lens
 *   - Adding a pointer-managing function to set the lens
 */

#ifndef EPIX2_CAMERA
#define EPIX2_CAMERA

#include "Constants.h"
#include "Point.h"
#include "Basis.h"
#include "Color.h"

namespace ePiX2 {

  class Pair;
  class Vector;

  class Camera {

    class Lens {
    public:
      // lens map(arg, orient, viewpt, distance)
      virtual Pair operator() (const Point&, 
	 const Basis&, const Point&, const double) const = 0;
      virtual bool is_linear(void) const = 0; // lens preserves lines?
      virtual ~Lens(void) { }
    }; // end of class Lens_base

  public:
    Camera(const Point vpt = Point(0,0,EPIX2_INFTY));
    ~Camera(void);

    // adjust position, orientation, and target
    void tilt(const double angle); // rotate up/down (pitch)
    void pan(const double angle);  // rotate left/right (yaw)
    void roll(const double angle); // rotate about viewing axis

    // fix target, move viewpt radially along eye()
    void range(double d);
    // fix viewpt, move target radially along eye()
    void focus(double d);

    void at(const Point& arg);      // fix target, set viewpt
    void look_at(const Point& arg); // fix viewpt, set target

    void filter(const Color& col) { the_filter = col; }

    // set lens
    void perspective(void); // default
    void orthog(void); // faster
    void fisheye(void);
    void bubble(void);

    // Camera maps
    Color operator() (const Color) const; // filter
    Pair operator() (const Point) const; // lens map
    bool is_linear(void); // lens preserves lines?

    // situation
    Vector eye(void) const;
    Point viewpt(void) const;

  private:

    Point the_viewpt; // center of projection
    Point the_target; // mapped to Screen origin
    Basis the_orient; // our {sea, sky, eye}
    double the_distance;

    Color the_filter;
    void adjust(const Point& vpt, const Point& tgt);
    Lens* the_lens;

    // actual lenses
    class Perspective : public Lens {
    public:
      Pair operator()
	(const Point&, const Basis&, const Point&, const double) const;
      bool is_linear(void) const { return true; }
    }; // end of class Perspective

    class Orthog : public Lens {
    public:
      Pair operator()
	(const Point&, const Basis&, const Point&, const double) const;
      bool is_linear(void) const { return true; }
    }; // end of class Orthog

    class Fisheye : public Lens {
    public:
      Pair operator()
	(const Point&, const Basis&, const Point&, const double) const;
      bool is_linear(void) const { return false; }
    }; // end of class Fisheye

    class Bubble : public Lens {
    public:
      Pair operator()
	(const Point&, const Basis&, const Point&, const double) const;
      bool is_linear(void) const { return false; }
    }; // end of class Bubble

  }; // end of class Camera

} /* end of namespace */

#endif /* EPIX2_CAMERA */
