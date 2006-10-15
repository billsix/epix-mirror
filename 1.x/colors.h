/*
 * colors.h -- ePiX color macros
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
#ifndef EPIX_COLORS
#define EPIX_COLORS

namespace ePiX {

  // ePiX provides rgb/cmyk colors via pstcol. The following are equivalent:
  //    red();
  //    rgb(1,0,0);
  //
  // cmyk(0,1,1,0); is visually equivalent but not the same.
  //
  // Densities outside the unit interval [0,1] are truncated, e.g., 
  //    rgb(2, -1, 0.3) = rgb(1, 0, 0.3).

  // red-green-blue
  void rgb(double r, double g, double b); // must pass args by value
  // cyan-magenta-yellow-black
  void cmyk(double c, double m, double y, double k);

  // primary colors
  void red(double d=1);
  void green(double d=1);
  void blue(double d=1);
  void white(double d=1);
  void black(double d=1);

  void cyan(double d=1);
  void magenta(double d=1);
  void yellow(double d=1);

} // end of namespace

#endif /* EPIX_COLORS */
