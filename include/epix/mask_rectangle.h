/* 
 * mask_rectangle.h -- ePiX:: rectangular screen_mask for cropping
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.15
 * Last Change: September 05, 2007
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

#ifndef EPIX_MASK_RECTANGLE
#define EPIX_MASK_RECTANGLE

#include <list>
#include <string>

#include "edge_data.h"
#include "screen_mask.h"

namespace ePiX {

  class pair;

  class Color;

  class length;
  class pen_fill;
  class pen_line;

  class mask_rectangle : public screen_mask {
  public:
    // mask_rectangle() { }
    mask_rectangle(const pair& arg1, const pair& arg2);

    mask_rectangle* clone() const;
    double h_min()  const;
    double h_max()  const;
    double h_size() const;
    double h_avg()  const;

    double v_min()  const;
    double v_max()  const;
    double v_size() const;
    double v_avg()  const;

    bool crops(const pair& arg) const; // false if arg visible
    std::list<edge2d>& crop_path(std::list<edge2d>&) const;
    std::list<edge2d>& crop_loop(std::list<edge2d>&) const;

    pen_line  border(const Color&, const length&) const;
    pen_fill backing(const Color&) const;

    pen_line  border() const;
    pen_fill backing() const;

  private:
    double m_hmin, m_hmax;
    double m_vmin, m_vmax;

    std::list<edge2d> m_border;
  }; // end of class mask_rectangle

} // end of namespace

#endif /* EPIX_MASK_RECTANGLE */
