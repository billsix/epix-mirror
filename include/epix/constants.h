/* 
 * constants.h -- ePiX's hidden compile-time constants
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.5
 * Last Change: June 14, 2008
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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

#ifndef EPIX_CONSTANTS
#define EPIX_CONSTANTS

#include "length.h"

namespace ePiX {

  // See lengths.cc for units conversion factors

  //// Style Parameters -- min, default, max ////
  // standard pen widths
  const length PLAIN_WIDTH("0.4pt");
  const length  BOLD_WIDTH("0.8pt");
  const length BBOLD_WIDTH("1.2pt");


  // dot diam in pt
  const double MIN_DOT(1);
  const double AVG_DOT(4);
  const double MAX_DOT(36);

  // page length of dash pattern in pt
  const double MIN_SEP(4);
  const double AVG_SEP(12);
  const double MAX_SEP(36);

  // axis tick half-lengths
  const double MIN_TICK(1);
  const double AVG_TICK(3);
  const double MAX_TICK(8);

  // arrow head parameters
  const double MIN_AWIDTH(1);
  const double AVG_AWIDTH(3);
  const double MAX_AWIDTH(12);

  const double MIN_ARATIO(1);
  const double AVG_ARATIO(5.5);
  const double MAX_ARATIO(8);

  const double MIN_INSET(-1);
  const double AVG_INSET(0);
  const double MAX_INSET(0.95); // not 1, so head can be hatched


  // too small/large
  const double EPIX_EPSILON(0.000001); // 10^-6
  const double EPIX_INFTY(100000.0);  // 10^5

  // Numerical constants
  const double PI_180(0.01745329251994329577);
  const double TWO_PI(6.28318530717958647692);

  // smallest distance from camera to clipping plane
  const double MIN_CLIP(0.01);

  // No more than this many minor ticks per segment in log axis
  const unsigned int MAX_LOG_BASE(100);

  // For output
  const unsigned int DEF_PRECISION(6);
  const unsigned int MAX_PRECISION(20);

  // In calculus plotting, the interval between adjacent points is too large
  // for accuracy. Divide each such interval into this many subintervals
  const unsigned int EPIX_ITERATIONS(200);

  // Output-formatting constants:
  const unsigned int EPIX_NUM_PTS(120); // # of points in ellipses, splines
  const unsigned int EPIX_FILE_WIDTH(70);  // Width of output file

} // end of namespace

#endif /* EPIX_CONSTANTS */
