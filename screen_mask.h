/* 
 * screen_mask.h -- ePiX::screen_mask interface, for screen cropping
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.8
 * Last Change: July 17, 2007
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

#ifndef EPIX_SCREEN_MASK
#define EPIX_SCREEN_MASK

#include <list>
#include <string>

#include "edge_data.h"

namespace ePiX {

  class pair;

  class Color;

  class length;
  class pen_fill;
  class pen_line;

  class screen_mask {
  public:
    virtual ~screen_mask() { }

    virtual screen_mask* clone() const = 0;

    // alignment points
    pair tr() const;
    pair tl() const;
    pair bl() const;
    pair br() const;

    pair  t() const;
    pair  b() const;
    pair  l() const;
    pair  r() const;
    pair  c() const;

    // cropping facilities (for tiles: glyph, pen_line, pen_fill resp.)
    virtual bool crops(const pair& arg) const = 0; // false if arg visible
    virtual std::list<edge2d>& crop_path(std::list<edge2d>&) const = 0;
    virtual std::list<edge2d>& crop_loop(std::list<edge2d>&) const = 0;

    // extreme and average coordinates, dimensions
    virtual double h_min()  const = 0;
    virtual double h_max()  const = 0;
    virtual double h_size() const = 0;
    virtual double h_avg()  const = 0;

    virtual double v_min()  const = 0;
    virtual double v_max()  const = 0;
    virtual double v_size() const = 0;
    virtual double v_avg()  const = 0;

    // convert us to screen border or backing
    virtual pen_line border(const Color&, const length&) const = 0;
    virtual pen_fill backing(const Color&) const = 0;

    virtual pen_line border() const = 0;
    virtual pen_fill backing() const = 0;
  }; // end of class screen_mask

} // end of namespace

#endif /* EPIX_SCREEN_MASK */
