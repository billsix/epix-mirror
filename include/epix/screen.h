/* 
 * screen.h -- ePiX::screen class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.18
 * Last Change: September 15, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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

/*
 * This file defines the screen class interface, a Cartesian rectangle
 * into which objects are drawn. Object-creating commands add to the
 * the "global" screen the_picture(). Users may create and activate
 * additional screens, then import their content to the_picture(). 
 *
 * Constructor:
 *   screen(P(a,c), P(b,d));  // corners of bounding box
 *
 * Markup functions:
 *   crop();         // remove elements outside bounding box
 *   crop_to(mask);  // or specified mask
 *   paste(screen);  // overlay contents of screen
 *   paste(screen, P(a,c), P(b,d));  // or inset into specified rectangle
 *
 * Decorations:
 *   border(Color, "1pt"); // set border color/width
 *   backing(Color);       // and backing color
 *
 * This file wraps the implementation, which is hidden from the user.
 */
#ifndef EPIX_SCREEN
#define EPIX_SCREEN

#include <string>

#include "triples.h"

namespace ePiX {

  class Color;
  class affine;
  class screen_data;
  class P;

  // global function
  void write(const std::string&);

  class screen {
    // There's no user-visible form for screen contents, so
    // we give these classes access to our implementation.
    friend class arrow_data;
    friend class label_data;
    friend class legend;
    friend class path_data;
    friend class picture_data;
    friend void write(const std::string&);

  public:
    screen();
    screen(const P&, const P&);

    screen(const screen&);
    screen& operator= (const screen&);
    ~screen();

    // Set mask; one named function for each type of screen_mask
    screen& crop_mask_rectangle(const P&, const P&);
    screen& crop_mask_ellipse(const P&, const P&);
    screen& crop_mask_diamond(const P&, const P&);

    screen& crop_mask_rectangle();
    screen& crop_mask_ellipse();
    screen& crop_mask_diamond();

    // return selected region
    screen extract(const P&, const P&) const;
    screen extract_diamond(const P&, const P&) const;
    screen extract_ellipse(const P&, const P&) const;

    // set flag only
    screen& set_crop(bool arg=true);

    // crop contents to current mask
    screen& crop();

    // inset and import
    screen& paste(const screen& child, const P& inset_sw, const P& inset_ne);
    screen& paste(const screen& child);

    screen& clear();

    // border(Color, length)?
    screen& border(const Color&, double);
    screen& border(const Color&, const std::string&);
    screen& backing(const Color&);

    screen& border();
    screen& backing();

    //// pass-through functions ////
    // affine operations on contents (not border, backing)
    screen&  apply(const affine& f);

    // convenience operators for single affine ops
    screen&   shift(const P& arg);

    // rotate by theta about ctr
    screen&  rotate(double theta, const P& ctr = P(0,0));

    // reflect in angle-theta line through ctr
    screen& reflect(double theta, const P& ctr = P(0,0));

    // scale coord direction(s) fixing ctr
    screen& h_scale(double, const P& ctr=P(0,0));
    screen& v_scale(double, const P& ctr=P(0,0));
    screen&   scale(double, const P& ctr=P(0,0));

    // shear, fixing ctr, e.g. h_shear(sc) : (x,y) -> (x+sc*y, y)
    screen&  h_shear(double sc, const P& ctr=P(0,0));
    screen&  v_shear(double sc, const P& ctr=P(0,0));

    P tr() const;
    P tl() const;
    P bl() const;
    P br() const;

    P  t() const;
    P  b() const;
    P  l() const;
    P  r() const;
    P  c() const;

    double h_min()  const;
    double h_max()  const;
    double h_size() const;
    double h_avg()  const;

    double v_min()  const;
    double v_max()  const;
    double v_size() const;
    double v_avg()  const;

  private:
    screen_data* m_screen;

  }; // end of class screen

} // end of namespace

#endif /* EPIX_SCREEN */
