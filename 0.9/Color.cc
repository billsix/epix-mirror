/*
 * Color.cc -- ePiX color handling
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

#include <cstdlib>
#include "Color.h"
#include "functions.h"
#include "Errors.h"

namespace ePiX3d {

  extern EPIX_COLOR_MODEL Color_model;

  void Color_spec::color_model(EPIX_COLOR_MODEL model) { Color_model = model; }

  // red-green-blue
  void Color_spec::set_color(double r, double g, double b)
  {
    if ( Color_model == RGB )
      {
	density_red   = clip_to_unit(r);
	density_green = clip_to_unit(g);
	density_blue  = clip_to_unit(b);
      }
    else if ( Color_model == CMYK )
      {
	epix_warning("RGB color specified under CMYK color model");
	density_cyan    = clip_to_unit(0.5*(b+g));
	density_magenta = clip_to_unit(0.5*(r+b));
	density_yellow  = clip_to_unit(0.5*(r+g));
      }
    else
      ; // do nothing
  }

  void Color_spec::set_color(double c, double m, double y, double k)
  {
    if ( Color_model == CMYK )
      {
	density_cyan    = clip_to_unit(c);
	density_magenta = clip_to_unit(m);
	density_yellow  = clip_to_unit(y);
	density_black   = clip_to_unit(k);
      }
    else if (Color_model == RGB )
      {
	epix_warning("CMYK color specified under RGB color model");
	density_red   = clip_to_unit(0.5*(m+y));
	density_green = clip_to_unit(0.5*(c+y));
	density_blue  = clip_to_unit(0.5*(m+c));
      }
    else
      ; // do nothing
  }

  // primary colors
  void Color_spec::red(double d)   { set_color(d,0,0); }
  void Color_spec::green(double d) { set_color(0,d,0); }
  void Color_spec::blue(double d)  { set_color(0,0,d); }
  void Color_spec::white(void)
  {
    ( Color_model == RGB ) ? set_color(1,1,1) : set_color(1,1,1,0);
  }
  
  void Color_spec::cyan(double d)    { set_color(d,0,0,0); }
  void Color_spec::magenta(double d) { set_color(0,d,0,0); }
  void Color_spec::yellow(double d)  { set_color(0,0,d,0); }
  void Color_spec::black(void)
  {
    ( Color_model == RGB ) ? set_color(0,0,0) : set_color(0,0,0,1);
  }

} /* end of namespace */
