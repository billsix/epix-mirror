/* 
 * globals.c -- ePiX's global variables and static functions to modify them
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
#include "globals.h"
#include "functions.h"

namespace ePiX {

  const std::string epix_version("EPIX_VERSION_NUMBER"); 

  // for backward compatibility, not in epix class
  double x_min, x_max, x_size, y_min, y_max, y_size;
  double tix(0);

  double pic_size;
  char *pic_unit;

  // Initialize global variables
  double epix::dashfill(0.5);
  double epix::separation(12);
  double epix::gray_depth(0.3);
  double epix::dotsize(3.0);

  double epix::angle_units(1.0);
  double epix::labelangle(0);

  bool epix::clipping(false);
  bool epix::cropping(false);
  bool epix::fill_paths(false);

  bool epix::using_pstricks(false);
  std::string epix::fillcolor("white");

  std::string epix::fontsize("normalsize");
  std::string epix::fontface("default");

  double epix::arrowwidth(1.5);
  double epix::arrowratio(5.5);
  double epix::arrowcamber(0.0);
  double epix::arrowfill(0.0);

  //     epix_output_type epix::OUTPUT_TYPE;
  //     std::string epix::font;
  epix_path_style epix::PATH_STYLE(SOLID);

  int epix::begin_count(0); // number of times begin() has been called


  //// epix::friend functions ////
  void dash_fill(double t)
  {
    epix::dashfill = snip_to(t,0.05,0.95);
  }
  void dash_length(double len)
  {
    epix::separation = snip_to(len,2,200);
  }

  void dot_sep(double len)
  {
    epix::separation = snip_to(len,2,200);
  }
  void dot_size(double diam)
  {
    epix::dotsize = snip_to(diam,0.5,256);
  }

  void gray(double depth)
  {
    epix::gray_depth = snip_to(depth, 0, 1);
  }

  void radians()
  {
    epix::angle_units = 1.0;
  }
  void degrees()
  {
    epix::angle_units = M_PI/180;
  }
  void revolutions()
  {
    epix::angle_units = 2*M_PI;
  }

    // radians per angle unit
  double angle(const double t)
  {
    return t*epix::angle_units;
  }

    // store label rotation angle internally in degrees
  void label_angle(double t)
  { 
    double temp(angle(t)/(2*M_PI)); // t in rotations
    temp -= 0.5; // shift half a turn
    temp -= ceil(temp); // map to (-1,0]
    temp += 0.5; // shift back
    epix::labelangle = 360*temp; // (-180, 180]
  }


    // set state variable
  void solid()
  {
    epix::PATH_STYLE = SOLID;
  }
  void dashed(double t)
  {
    epix::PATH_STYLE = DASHED;
    if ( t != 0) dash_fill(t); // by default, leave
  }
  void dotted(double t)
  {
    epix::PATH_STYLE = DOTTED; 
    if (t != 0) dot_size(t);
  }

  void clip(bool arg)
  {
    epix::clipping = arg;
  }
  void crop(bool arg)
  {
    epix::cropping = arg;
  }
  void fill(bool arg)
  {
    epix::fill_paths = arg;
  }
  void use_pstricks(bool arg)
  {
    epix::using_pstricks = arg;
  }

    // set font size and face
  void font_size(std::string arg)
  {
    epix::fontsize = arg;
  }
  void font_face(std::string arg)
  {
    epix::fontface = arg;
  }

  // set arrowhead parameters
  void arrow_width(double w)
  {
    epix::arrowwidth = fabs(0.5*w);
  }
  void arrow_ratio(double r)
  {
    epix::arrowratio = fabs(r);
  }
  void arrow_camber(double arg)
  {
    epix::arrowcamber = snip_to(arg,0,1);
  }
  void arrow_fill(double dens)
  {
    epix::arrowfill = snip_to(dens,0,1);
  }


  //// epix member functions ////
  double epix::get_arrow_width()
  {
    return epix::arrowwidth;
  }
  double epix::get_arrow_ratio()
  {
    return epix::arrowratio;
  }
  double epix::get_arrow_camber()
  {
    return epix::arrowcamber;
  }
  double epix::get_arrow_fill()
  {
    return epix::arrowfill;
  }

    // for internal use
  double epix::get_dashfill()
  {
    return epix::dashfill;
  }
  double epix::get_dashlength()
  {
    return epix::separation;
  }

  double epix::get_gray()
  {
    return epix::gray_depth;
  }
  double epix::get_dotsize()
  {
    return epix::dotsize;
  }
  double epix::full_turn()
  {
    return 2*M_PI/epix::angle_units;
  }
  double epix::get_labelangle() // in degrees
  {
    return epix::labelangle;
  }
  epix_path_style epix::path_style()
  {
    return epix::PATH_STYLE;
  }
} // end of namespace
