/* 
 * state.h -- ePiX global functions to control current drawing state
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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

/*
 * This file contains the (old) user interface governing:
 *
 *  [] Angular mode
 *  [] Clipping and cropping
 *  [] Font size and face, label angle, mask color
 *  [] Gray depth, path filling, fill color
 *  [] Whether to use PSTricks macros
 *  [] Arrowhead parameters
 *  [] Line style, dash length, dot size
 *  [] Line width
 *  [] Line/text color
 */

#ifndef EPIX_STATE
#define EPIX_STATE

#include <string>

namespace ePiX {

  class Color;
  class P;

  // set camera position
  void viewpoint(const P&);
  void viewpoint(double, double, double);

  // Set angular mode
  void radians();
  void degrees();
  void revolutions();

  // number of current angle units in one full turn
  double full_turn();

  // set clip box
  void clip(bool arg=true); // no effect
  void clip_box(); // default (very large)
  void clip_box(const P& arg1, const P& arg2);
  void clip_to(const P& arg);
  void clip_box(const P& arg);

  // add a clip face; perp points inward
  void clip_face(const P& loc, const P& perp);

  // add parallel clip planes
  void clip_slice(const P& loc, const P& perp);
  void clip_slice(const P& loc, const P& perp, double thickness);

  // loc = origin
  void clip_slice(const P& perp);
  void clip_slice(const P& perp, double thickness);

  // remove user-specified clip planes, preserve clip box settings
  void clip_restore();

  // set flag on active screen; contents unchanged
  void set_crop(bool arg=true);

  // crop contents of active screen
  void crop();

  //// New cropping functions ////
  // set crop mask of active screen
  void crop_rectangle(const P&, const P&);
  void crop_ellipse(const P&, const P&);
  void crop_diamond(const P&, const P&);

  void crop_rectangle();
  void crop_ellipse();
  void crop_diamond();

  void crop_box(const P&, const P&);
  void crop_box(); // (re)set crop box to bounding box

  // Set label attributes
  void font_size(const std::string& arg="nsz");
  void font_face(const std::string& arg="rm");

  void label_color(const Color&);

  void label_angle(double t=0);
  void label_mask(const Color& col);
  void label_mask();

  void label_pad(std::string);
  void label_border(const Color&, double);
  void label_border(const Color&, std::string);
  void label_border(const Color&);

  void label_border(double);
  void label_border(std::string);
  void no_label_border();

  // Gray depth, path filling, [fill color]
  void gray(double depth=0.3);
  void fill(const Color&); // set fill color
  void fill(bool arg=true); // set fill flag
  void nofill();
  // void fill_color(const std::string& arg);

  // Arrowhead parameters
  void arrow_width(double w=3);
  void arrow_ratio(double r=5.5);
  void arrow_inset(double arg=0);
  void arrow_fill(bool arg=true);

  // Dash pattern length
  void dash_size(double len=0);
  void dot_sep(double len=0);

  // Dot and box marker size
  void dot_size(double diam=0);

  // Tick length
  void tick_size(double len=0);

  // Line style
  void line_style(std::string style="-"); // solid by default

  void solid();
  void dashed();
  void dotted();

  // Line width
  void pen(const std::string&);
  void pen(double);

  void bbold();
  void  bold();
  void plain();

  // Line width and color
  void bbold(const Color&);
  void  bold(const Color&);
  void plain(const Color&);

  void pen(const Color&);
  void pen(const Color&, double);
  void pen(const Color&, std::string);

  void base(const Color&);
  void base(const Color&, double);
  void base(const Color&, std::string);

  // Line and text color
  // ePiX provides rgb/cmyk colors via xcolor. The following are equivalent:
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

  // P arguments, for easy function control
  void rgb(const P&);
  void cmyk(const P&);

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

#endif /* EPIX_STATE */
