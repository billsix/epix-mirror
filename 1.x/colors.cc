/* 
 * colors.cc -- ePiX color macros
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
#include <iostream>

#include "functions.h"
#include "colors.h"

namespace ePiX {

  // Red-Blue-Green
  void rgb(double r, double g, double b)
  {
    r = clip_to_unit(r);
    g = clip_to_unit(g);
    b = clip_to_unit(b);
    fprintf (stdout, "\n\\color[rgb]{%.2f,%.2f,%.2f}", r, g, b);
  }

  // Cyan-Magenta-Yellow-Black
  void cmyk(double c, double m, double y, double k)
  {
    c = clip_to_unit(c);
    m = clip_to_unit(m);
    y = clip_to_unit(y);
    k = clip_to_unit(k);
    fprintf (stdout, "\n\\color[cmyk]{%.2f,%.2f,%.2f,%.2f}", c, m, y, k);
  }

  // primary colors
  void red(double d)
  {
    rgb(d,0,0);
  }
  void green(double d)
  {
    rgb(0,d,0);
  }
  void blue(double d)
  {
    rgb(0,0,d);
  }
  void white(double d)
  {
    rgb(d,d,d);
  }
  void black(double d)
  {
    rgb(1-d,1-d,1-d);
  }

  void cyan(double d)
  {
    cmyk(d,0,0,0);
  }
  void magenta(double d)
  {
    cmyk(0,d,0,0);
  }
  void yellow(double d)
  {
    cmyk(0,0,d,0);
  }
} // end of namespace
