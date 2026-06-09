/* 
 * mask_ellipse.cc -- ePiX ellipse crop mask
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
//#include "errors.h"

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
#include "mask_ellipse.h"

namespace ePiX {

  mask_ellipse::mask_ellipse(const pair& arg1, const pair& arg2)
    : m_hmin(min(arg1.x1(), arg2.x1())),
      m_hmax(max(arg1.x1(), arg2.x1())),
      m_vmin(min(arg1.x2(), arg2.x2())),
      m_vmax(max(arg1.x2(), arg2.x2()))
  {
    const double dt(2*M_PI/EPIX_NUM_PTS);

    const pair ctr(0.5*(m_hmin + m_hmax), 0.5*(m_vmin + m_vmax));

    const double hsz(0.5*(m_hmax - m_hmin));
    const double vsz(0.5*(m_vmax - m_vmin));

    for (unsigned int i=0; i<EPIX_NUM_PTS; ++i)
      m_border.push_back(edge2d(ctr + pair(hsz*std::cos(i*dt),
					   vsz*std::sin(i*dt)),
				ctr + pair(hsz*std::cos((i+1)*dt),
					   vsz*std::sin((i+1)*dt)), true));
  }

  mask_ellipse* mask_ellipse::clone() const
  {
    return new mask_ellipse(*this);
  }

  // corners and midpoints
  double mask_ellipse::h_min() const { return m_hmin; }
  double mask_ellipse::v_min() const { return m_vmin; }

  double mask_ellipse::h_max() const { return m_hmax; }
  double mask_ellipse::v_max() const { return m_vmax; }

  double mask_ellipse::h_avg() const { return 0.5*(m_hmin+m_hmax); }
  double mask_ellipse::v_avg() const { return 0.5*(m_vmin+m_vmax); }

  double mask_ellipse::h_size() const { return m_hmax - m_hmin; }
  double mask_ellipse::v_size() const { return m_vmax - m_vmin; }

  // ellipse-specific functions
  bool mask_ellipse::crops(const pair& arg) const
  {
    const double dx((arg.x1() - h_avg())/h_size());
    const double dy((arg.x2() - v_avg())/v_size());
    return !(dx*dx + dy*dy <= 0.25);
  }

  std::list<edge2d>& mask_ellipse::crop_path(std::list<edge2d>& L) const
  {
    return crop_path_2nd(m_border, L);
  }

  std::list<edge2d>& mask_ellipse::crop_loop(std::list<edge2d>& L) const
  {
    return crop_loop_2nd(m_border, L);
  }

  pen_line mask_ellipse::border(const Color& col, const length& len) const
  {
    return pen_line(pen_data(col, len), Xfine(), m_border);
  }

  pen_fill mask_ellipse::backing(const Color& col) const
  {
    return pen_fill(col, pen_data(col, PLAIN_WIDTH), 0, m_border);
  }

  pen_line mask_ellipse::border() const
  {
    return pen_line(the_paint_style().line_pen(),
		    the_paint_style().base_pen(), m_border);
  }

  pen_fill mask_ellipse::backing() const
  {
    const Color col(the_paint_style().fill_color());
    return pen_fill(col, pen_data(col, PLAIN_WIDTH), 0, m_border);
  }
} // end of ePiX namespace
