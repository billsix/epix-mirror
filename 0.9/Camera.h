/* 
 * Camera.h -- Vectors and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 01, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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
 *   - the Camera class and routines:
 *       void rotate_sea(double); // change Camera orientation
 *       void rotate_sky(double); // about respective axes
 *       void rotate_eye(double);
 *
 *       void range(double d); // set distance from Camera to target
 *       void look_at(Position arg); // set target (center of field of view)
 *       void lens(pair (*screen_projection(Position))); // set projection mapping
 *
 *       pair shoot(P arg); // project a point to the screen ("take a photo")
 *
 */

#ifndef _EPIX3d_CAMERA
#define _EPIX3d_CAMERA

#include "Position.h"
#include "Vector.h"
#include "Frame.h"
#include "Pair.h"
#include "functions.h"

namespace ePiX3d {

  class Camera {
  private:
    Frame orientation;
    double distance;
    Position target;
    Pair (*screen_projection)(Position);

  public:

    Camera() 
      { 
	screen_projection = shadow; // orthog projection when distance = 0
      }

    double get_range(void) { return distance; }
    Position get_target(void) { return target; }

    // pitch: rotate camera up/down
    void rotate_sea(double angle)
      { 
	orientation = orientation.rotate_frame1(angle);
      }

    // yaw: rotate camera left/right
    void rotate_sky(double angle)
      { 
	orientation = orientation.rotate_frame2(angle);
      }

    // roll: rotate camera about viewing axis
    void rotate_eye(double angle)
      { 
	orientation = orientation.rotate_frame3(angle);
      }

    // get/set distance from camera to origin
    void range(double d) { distance = d; }

    // set target (center of field of view)
    void look_at(Position arg) { target = arg; }

    // set projection mapping
    void lens(Pair proj(Position)) { screen_projection = proj; }

    // project a point to the screen ("shoot a photo")
    Pair shoot(Position arg) { return screen_projection(arg); }

    // screen projection mappings ("lenses")
    friend Pair shadow(Position arg);
    friend Pair fisheye(Position arg);

  }; // end of class epix3d_camera 

} /* end of namespace */

#endif /* _EPIX3d_CAMERA */
