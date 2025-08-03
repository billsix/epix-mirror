/* 
 * picture.h -- ePiX globals functions for picture interface
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.16
 * Last Change: September 11, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef EPIX_PICTURE
#define EPIX_PICTURE

#include <string>

namespace ePiX {

  class Color;
  class P;
  class picture_data;
  class screen;

  // "global" references
  picture_data& the_picture();

  double& xmin();
  double& xmax();
  double& xsize();

  double& ymin();
  double& ymax();
  double& ysize();

  double& tix();

  // preferred initialization functions
  // corners and dimensions, e.g. "4in x 15cm" or "4 x 6in"
  // (width units take precedence, dimension string must be passed by value)
  void picture(const P& arg1, const P& arg2, std::string);

  // same syntax as for picture dimensions
  void offset(std::string);

  //// Old global functions for manipulating the page markup region ////
  void bounding_box(const P&, const P&);

  void picture(double, double);
  void picture(const P&);

  void offset(double hoff, double voff);
  void offset(const P& arg);

  void unitlength(const std::string& units);

  // activate default screen
  void begin();
  // write the output file
  void end_picture();

  //// New global functions ////
  // the_picture().the_canvas
  screen& canvas();

  // specify active screen; pass non-const ref only to assign/compare pointer
  void activate(screen&);
  void deactivate(screen&);

  // add active or specified screen to the_picture
  void import();
  void inset(const P& sw, const P& ne);

  void import(const screen& child);
  void inset(const screen& child, const P& sw, const P& ne);

  void inset(const screen& child); // use child's corners

  // interpret child's coordinates as true pt on the page,
  // place origin at P
  void inlay(const screen& child, const P& loc);

  // interpret stated coord as true pt, the other as Cartesian
  void inlay_vertical(const screen& child, const P& loc);
  void inlay_horizontal(const screen& child, const P& loc);

  // set decorations on active screen
  void backing(const Color&);
  void border(const Color&, double); // width in pt
  void border(const Color&, const std::string&);

  void backing(); // use current fill style
  void border();  // use current line pen

  // write verbatim string to output file
  void write(const std::string&);

  void pre_write(const std::string&); // before picture header
  void post_write(const std::string&); // after picture footer

  // set output format
  void eepic_format();
  //  void mp_format();
  void tikz_format();
  void pst_format();

  // write in specified format to named output file
  void print_eepic(const std::string& filename);
  // void print_mp(const std::string& filename);
  void print_tikz(const std::string& filename);
  void print_pst(const std::string& filename);

  // allow user to override in-file format request on the command line
  inline void end()
    {
#ifdef EPIX_FMT_PSTRICKS
      pst_format();
#else
#ifdef EPIX_FMT_TIKZ
      tikz_format();
#else
#ifdef EPIX_FMT_EEPIC
      eepic_format();
#endif /* EPIX_FMT_EEPIC */
#endif /* EPIX_FMT_TIKZ */
#endif /* EPIX_FMT_PSTRICKS */
      end_picture();
    }

  // for implementors of true-size elements
  double pic_units_per_pt();   // exact
  double units_per_pic_unit(); // approximate, exact at true aspect ratio
  double pt_to_screen(double); // true length to screen length

  ////////////////////////////////////////////////////////////////
  ////                                                        ////
  ////    DEPRECATED global variables required for backward   ////
  ////    compatibility. Will be removed in a future release. ////
  ////    Use reference functions xmin() et. al.              ////
  ////                                                        ////
  ////////////////////////////////////////////////////////////////

  // bounding_box corners and dimensions
  extern double x_min, x_max, x_size, y_min, y_max, y_size;

} // end of namespace

#endif /* EPIX_PICTURE */
