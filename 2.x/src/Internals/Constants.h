/*
 * Constants.h -- ePiX's internal constants
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 2.0pre
 * Last Change: January 14, 2006
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

#ifndef EPIX2_CONSTANTS
#define EPIX2_CONSTANTS

#include <cstdlib>

namespace ePiX2 {

  // ePiX's idea of too small/large (10^\pm 5)
  const double EPIX2_EPSILON=0.00001;
  const double EPIX2_INFTY=100000.0;

  // default line widths in pt
  const double PLAIN_WIDTH=0.6;
  const double BOLD_WIDTH=1.0;
  const double BBOLD_WIDTH=1.5;


  // deprecated, can be removed eventually
  // fraction of step size when clipping
  const double EPIX2_SEEK_SIZE=0.01;

  // Number of steps per point in calculus plots
  const int EPIX2_ITERATIONS=200;
} /* end of namespace */

#endif /* EPIX2_CONSTANTS */
