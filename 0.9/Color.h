/*
 * Color.h -- ePiX color handling
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 0.9.0rc1
 *
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

#ifndef _EPIX3d_COLOR
#define _EPIX3d_COLOR

#include <cstdlib>

namespace ePiX3d {

  /*
   * ePiX provides "pstcol" style colors using RGB (default) or CMYK
   * specifications.  Every figure element has a color, which defaults
   * to black, and when appropriate is inherited by child elements.
   *
   * Color specifications cause PostScript specials to be written to
   * the DVI file, and may not be displayed properly by some previewers.
   */

  /*
   * This file provides:
     - The Color_spec class, a collection of seven color densities

     - The set_color() function for setting color densities
     - Primary colors: red(), green(), blue(), cyan(), magenta(), etc.
   */

  enum EPIX_COLOR_MODEL {BW, RGB, CMYK};

  // color data
  class Color_spec {

  private:
    static EPIX_COLOR_MODEL Color_model;

    // all 0 by default
    double density_red;
    double density_green;
    double density_blue;

    double density_cyan;
    double density_magenta;
    double density_yellow;
    double density_black;

  public:

    static void color_model(EPIX_COLOR_MODEL);

    Color_spec(void)
      {
	density_red = density_green = density_blue = 0;
	density_cyan = density_magenta = density_yellow = density_black = 0;
      }

    // red-green-blue
    void set_color(double r, double g, double b);
    void set_color(double c, double m, double y, double k);

    // primary colors
    void red(double d=1);
    void green(double d=1);
    void blue(double d=1);
    void white(void);

    void cyan(double d=1);
    void magenta(double d=1);
    void yellow(double d=1);
    void black(void);

  }; /* end of class color_spec */
} /* end of namespace */

#endif /* _EPIX3d_COLOR */
