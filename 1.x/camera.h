/* 
 * camera.h -- Vectors and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc9
 * Last Change: June 23, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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
 * In geometry, a "frame" is a right-handed orthonormal basis, that is,
 * an ordered set of three mututally perpendicular unit vectors, oriented
 * according to the right-hand rule. A frame has nothing to do with
 * picture frames.
 *
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
 *   - the frame class, and routines for rotating a frame about the axes
 *     determined by its elements. (Note that there are no methods for
 *     rotating about fixed coordinate axes, and that rotations in R^3 do
 *     not generally commute.
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
 *
 */

#ifndef EPIX_CAMERA
#define EPIX_CAMERA

#include "functions.h"
#include "pairs.h"
#include "triples.h"

namespace ePiX {

  class frame {
  private:
    // orthonormal triple
    P frame1;
    P frame2;
    P frame3;

  public:
    //constructors
    frame(void) { frame1 = E_1; frame2 = E_2; frame3 = E_3; }

    // Gram-Schmidt
    frame(P arg1, P arg2, P arg3);

    // frame elements
    P sea() { return frame1; }
    P sky() { return frame2; }
    P eye() { return frame3; }

    // rotations about frame elements
    frame rotate_frame1(double angle)
      {
	P temp2 = frame2;
	P temp3 = frame3;

	frame2 = 
	  (ePiX::cos(angle)*(temp2)) - (ePiX::sin(angle)*(temp3));
	frame3 = 
	  (ePiX::sin(angle)*(temp2)) + (ePiX::cos(angle)*(temp3));

	return *this;
      }


    frame rotate_frame2(double angle)
      {
	P temp3 = frame3;
	P temp1 = frame1;

	frame3 = 
	  (ePiX::cos(angle)*(temp3)) - (ePiX::sin(angle)*(temp1));

	frame1 = 
	  (ePiX::sin(angle)*(temp3)) + (ePiX::cos(angle)*(temp1));

	return *this;
      }

    frame rotate_frame3(double angle)
      {
	P temp1 = frame1;
	P temp2 = frame2;

	frame1 = 
	  (ePiX::cos(angle)*(temp1)) - (ePiX::sin(angle)*(temp2));

	frame2 = 
	  (ePiX::sin(angle)*(temp1)) + (ePiX::cos(angle)*(temp2));

	return *this;
      }

  }; // end of class frame


  // screen projection mappings ("lenses")
  pair shadow(P arg);
  pair fisheye(P arg);
  pair bubble(P arg);


  class epix_camera {
  private:
    P viewpt;
    P target;
    frame  orient;
    double distance;
    pair (*screen_projection)(P);

  public:

    // defaults to orthog projection on (x,y) plane from pos z-axis
    epix_camera(void)
      { 
	viewpt = P(0,0,EPIX_INFTY);
	target = P(0,0,0);
	distance = EPIX_INFTY;
	orient = frame();

	screen_projection = shadow;
      }

    P get_viewpt(void) { return viewpt; }
    P get_target(void) { return target; }
    double get_range(void) { return distance; }

    // get frame vectors
    P sea(void) { return orient.sea(); }
    P sky(void) { return orient.sky(); }
    P eye(void) { return orient.eye(); }

    // adjust position, orientation, and target
    // pitch: rotate camera up/down
    void rotate_sea(double angle)
      { 
	orient = orient.rotate_frame1(angle); 
	viewpt = target + distance*orient.eye();
      }

    // yaw: rotate camera left/right
    void rotate_sky(double angle)
      { 
	orient = orient.rotate_frame2(angle); 
      	viewpt = target + distance*orient.eye();
      }

    // roll: rotate camera about viewing axis
    void rotate_eye(double angle)
      { orient = orient.rotate_frame3(angle); } // target unchanged

    // fix target, move viewpt radially along eye()
    void range(double d);
    // fix viewpt, move target radially along eye()
    void focus(double d);

    // fix target, set viewpt arbitrarily
    void at(P arg);
    friend void viewpoint(P arg);
    friend void viewpoint(double a1, double a2, double a3);

    // fix viewpt, set target arbitrarily
    void look_at(P arg);

    // set projection mapping
    void lens(pair proj(P)) { screen_projection = proj; }

    // project a point to the screen ("shoot a photo")
    pair operator() (const P arg) { return screen_projection(arg); }

  }; // end of class epix_camera 

  extern epix_camera camera;

} /* end of namespace */

#endif /* EPIX_CAMERA */
