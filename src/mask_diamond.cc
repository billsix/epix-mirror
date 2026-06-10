/*
 * mask_diamond.cc -- ePiX diamond crop mask
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.10
 * Last Change: August 09, 2007
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
#include <cmath>

#include "functions.h"
// #include "errors.h"

#include "constants.h"

#include "pairs.h"

#include "Color.h"

#include "paint_style.h"
#include "length.h"

#include "pen_line.h"
#include "pen_fill.h"

#include "edge_data.h"
#include "screen_crop.h"

#include "screen_mask.h"
#include "mask_diamond.h"

namespace ePiX {

mask_diamond::mask_diamond(const pair& arg1, const pair& arg2)
    : m_hmin(min(arg1.x1(), arg2.x1())),
      m_hmax(max(arg1.x1(), arg2.x1())),
      m_vmin(min(arg1.x2(), arg2.x2())),
      m_vmax(max(arg1.x2(), arg2.x2())) {
  m_border.push_back(edge2d(l(), b(), true));
  m_border.push_back(edge2d(b(), r(), true));
  m_border.push_back(edge2d(r(), t(), true));
  m_border.push_back(edge2d(t(), l(), true));
}

mask_diamond* mask_diamond::clone() const { return new mask_diamond(*this); }

// corners and midpoints
double mask_diamond::h_min() const { return m_hmin; }
double mask_diamond::v_min() const { return m_vmin; }

double mask_diamond::h_max() const { return m_hmax; }
double mask_diamond::v_max() const { return m_vmax; }

double mask_diamond::h_avg() const { return 0.5 * (m_hmin + m_hmax); }
double mask_diamond::v_avg() const { return 0.5 * (m_vmin + m_vmax); }

double mask_diamond::h_size() const { return m_hmax - m_hmin; }
double mask_diamond::v_size() const { return m_vmax - m_vmin; }

// diamond-specific functions
bool mask_diamond::crops(const pair& arg) const {
  const double x(fabs(arg.x1() - h_avg()) / h_size());
  const double y(fabs(arg.x2() - v_avg()) / v_size());

  return !((x + y) <= 0.5);
}

std::list<edge2d>& mask_diamond::crop_path(std::list<edge2d>& L) const {
  return crop_path_2nd(m_border, L);
}

std::list<edge2d>& mask_diamond::crop_loop(std::list<edge2d>& L) const {
  return crop_loop_2nd(m_border, L);
}

pen_line mask_diamond::border(const Color& col, const length& len) const {
  return pen_line(pen_data(col, len), Xfine(), m_border);
}

pen_fill mask_diamond::backing(const Color& col) const {
  return pen_fill(col, pen_data(col, PLAIN_WIDTH), 0, m_border);
}

pen_line mask_diamond::border() const {
  return pen_line(the_paint_style().line_pen(), the_paint_style().base_pen(),
                  m_border);
}

pen_fill mask_diamond::backing() const {
  const Color& col(the_paint_style().fill_color());
  return pen_fill(col, pen_data(col, PLAIN_WIDTH), 0, m_border);
}
}  // namespace ePiX
