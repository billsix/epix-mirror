/* 
 * mask_ellipse.h -- ePiX:: elliptical screen_mask for cropping
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

#ifndef EPIX_MASK_ELLIPSE
#define EPIX_MASK_ELLIPSE

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

  class mask_ellipse : public screen_mask {
  public:
    // mask_ellipse() { }
    mask_ellipse(const pair& arg1, const pair& arg2);

    mask_ellipse* clone() const override;
    double h_min()  const override;
    double h_max()  const override;
    double h_size() const override;
    double h_avg()  const override;

    double v_min()  const override;
    double v_max()  const override;
    double v_size() const override;
    double v_avg()  const override;

    bool crops(const pair& arg) const override; // false if arg visible
    std::list<edge2d>& crop_path(std::list<edge2d>&) const override;
    std::list<edge2d>& crop_loop(std::list<edge2d>&) const override;

    pen_line  border(const Color&, const length&) const override;
    pen_fill backing(const Color&) const override;

    pen_line  border() const override;
    pen_fill backing() const override;

  private:
    double m_hmin, m_hmax;
    double m_vmin, m_vmax;

    std::list<edge2d> m_border;
  }; // end of class mask_ellipse

} // end of namespace

#endif /* EPIX_MASK_ELLIPSE */
