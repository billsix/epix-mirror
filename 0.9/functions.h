/*
 * functions.h -- ePiX utility functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 0.9.0rc1
 * Last Change: January 27, 2003
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

#ifndef _EPIX3d_FUNCTIONS
#define _EPIX3d_FUNCTIONS

#include <cmath>

namespace ePiX3d {

  // identity maps
  template<class T> T Id(T);

  // miscellaneous functions with singularities removed
  double Sinx (double);
  double Sgn (double);
  double Recip (double);
  double cb (double);

  double clip_to_unit(double);

  // mostly non-printing utility functions
  int gcd (int, int);
  double inf (double f(double), double, double);
  double sup (double f(double), double, double);

  // ePiX::trig functions
  double cos(double);
  double sin(double);
  double tan(double);
  double sec(double);

  // and their inverses
  double acos(double);
  double asin(double);
  double atan(double);

  // variable-base log
  //  double log(double);

} /* end of namespace */

#endif /* _EPIX3d_FUNCTIONS */
