/* 
 * constants.h -- ePiX constants
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version EPIX_VERSION_NUMBER
 * Last Change: August 27, 2003
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

#ifndef _EPIX_CONSTANTS
#define _EPIX_CONSTANTS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#ifdef _EPIX_COMPILE_OUTPUT
namespace ePiX {
  const char *epix_version="EPIX_VERSION_NUMBER"; 
}
#endif

namespace ePiX3d {

  // ePiX's idea of too small/large (10^\pm 4)
  const double EPIX_EPSILON=0.0001;
  const double EPIX_INFTY=10000.0;
  const double EPIX_DBL_INFTY=10000000000.0; // 10^10

  // Output parameters
  const int EPIX_NUM_PTS=80;     // # of points in ellipses, splines
  const int EPIX_FILE_WIDTH=70;  // Width of output file

  // Sizes of markers et. al.
  const double EPIX_SPOT_DIAM=4;
  const double EPIX_DOT_DIAM=3;
  const double EPIX_DDOT_DIAM=2;
  const double EPIX_BOX_SIZE=4;
  const double EPIX_BBOX_SIZE=2;
  const double EPIX_ARROWHEAD_WIDTH=1.5; // Half-width of arrowheads in pt
  const double EPIX_ARROWHEAD_RATIO=5.5; // 2*Length/width ratio of arrowheads

  /*
   * In calculus plotting, determines the size of the increment dt
   * and the number of increments per point printed. A value of 100
   * is adequate for simple plotting, while a value larger than
   * 1000 is not likely to improve the accuracy noticeably.
   *
   * larger -> better accuracy, longer run
   */
  const int EPIX_ITERATIONS=200;

  // Dash length/dot separation parameters
  const double EPIX_DASH_LENGTH=5.0;
  const double EPIX_DOT_SEP=40.0;



  // Path styles, data plot markers, and label alignment types
  enum epix_path_style {SOLID, DASHED, DOTTED};

  enum epix_mark_type {PATH, CIRC, SPOT, RING, DOT, DDOT, PLUS, OPLUS, 
		       TIMES, OTIMES, DIAMOND, UP, DOWN, BOX, BBOX};

  enum epix_label_posn {c, r, tr, rt, t, tl, lt, l, bl, lb, b, br, rb};


  // coordinate axis tick marks
  enum epix_tick_type {TICK_NULL, H_AXIS, V_AXIS};

  // "vector field"-type objects
  enum epix_field_type {SLOPE, DART, VECTOR};

  // Shape error types...
  enum constructor_error_type {MALFORMED, MULTIPLICITY, COLLINEAR_PTS};
  enum join_error_type {TANGENT, PARALLEL, COINCIDENT, NON_COPLANAR,
			SEPARATED, CONCENTRIC};

  // and handlers
  struct constructor_error {
    constructor_error_type type;
    constructor_error(constructor_error_type x) { type = x; }
  };

  struct join_error {
    join_error_type type;
    join_error(join_error_type x) { type = x; }
  };

} /* end of namespace */

#endif /* _EPIX_CONSTANTS */
