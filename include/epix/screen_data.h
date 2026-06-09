/* 
 * screen_data.h -- ePiX::screen class implementation
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.18
 * Last Change: September 16, 2007
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
 * This file implements the screen class, a Cartesian rectangle into
 * which objects are drawn. This file is not part of epix.h.
 */
#ifndef EPIX_SCREEN_DATA
#define EPIX_SCREEN_DATA

#include <string>
#include <list>

#include "pairs.h"

namespace ePiX {

  class affine;
  class Color;
  class format;
  class length;
  class pen_fill;
  class pen_line;
  class screen_mask;
  class tile;

  class screen_data {
  public:
    screen_data(const pair&, const pair&);

    screen_data(const screen_data&);
    screen_data& operator= (const screen_data&);
    ~screen_data();

    screen_data* clone() const;

    screen_data& crop_to(const screen_mask&);
    screen_data& crop();


    screen_data& set_mask(const screen_mask&);
    screen_data& set_crop(bool);

    // cut out and return a region
    screen_data extract(const pair&, const pair&) const;
    screen_data extract_diamond(const pair&, const pair&) const;
    screen_data extract_ellipse(const pair&, const pair&) const;

    // inset and import
    screen_data& paste(const screen_data& child, const screen_mask& inset);
    screen_data& paste(const screen_data& child);

    screen_data& clear();

    screen_data& border(const Color&, const std::string&);
    screen_data& backing(const Color&);

    screen_data& border();
    screen_data& backing();

    void add_tile(const tile&);
    void add_tile(tile*);

    // wrap=true -> encapsulate in a picture, for legends
    std::string print_to(const format&, const length&, bool wrap=false) const;

    // affine operations on contents (not border, backing)
    void   apply(const affine& af);
    void   shift(const pair& arg);

    // rotate by theta about ctr
    void  rotate(double theta, const pair& ctr = pair(0,0));

    // reflect in angle-theta line through ctr
    void reflect(double theta, const pair& ctr = pair(0,0));

    // scale coord direction(s) fixing ctr
    void h_scale(double, const pair& ctr=pair(0,0));
    void v_scale(double, const pair& ctr=pair(0,0));
    void   scale(double, const pair& ctr=pair(0,0));

    // shear, fixing ctr, e.g. h_shear(sc) : (x,y) -> (x+sc*y, y)
    void h_shear(double sc, const pair& ctr=pair(0,0));
    void v_shear(double sc, const pair& ctr=pair(0,0));

    pair tr() const;
    pair tl() const;
    pair bl() const;
    pair br() const;

    pair  t() const;
    pair  b() const;
    pair  l() const;
    pair  r() const;
    pair  c() const;

    double h_min()  const;
    double h_max()  const;
    double h_size() const;
    double h_avg()  const;

    double v_min()  const;
    double v_max()  const;
    double v_size() const;
    double v_avg()  const;

  private:
    pair m_bl;
    pair m_tr;

    screen_mask* m_mask;
    pen_line* m_border;
    pen_fill* m_backing;

    bool m_crop;

    std::list<tile*> m_tiles;
  }; // end of class screen_data

} // end of namespace

#endif /* EPIX_SCREEN_DATA */
