/*
 * pen_arrow.cc -- ePiX::pen_arrow class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
 *
 *
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

#include <sstream>
#include <list>  // for arrow shaft cropping

#include "pairs.h"
#include "affine.h"
#include "screen_mask.h"

#include "length.h"

#include "edge_data.h"

#include "Color.h"
#include "arrow_style.h"
#include "label_style.h"
#include "marker_style.h"  // for bbox marker

#include "format.h"
#include "picture_data.h"
#include "picture.h"

#include "tile.h"
#include "pen_arrow.h"

namespace ePiX {
using enum epix_mark_type;
pen_arrow::pen_arrow(const pair& tail, const pair& head, const pen_data& line,
                     const pen_data& base, double scale, double sin_th,
                     bool head_seen)
    : m_shaft(tail, head, true),
      m_line(line),
      m_base(base),
      m_scale(scale),
      m_shorten(sin_th),
      m_head_seen(head_seen),
      m_style(the_arrowhead_style()) {}

pen_arrow* pen_arrow::clone() const { return new pen_arrow(*this); }

pen_arrow& pen_arrow::map_by(const affine& f) {
  m_shaft = edge2d(f(m_shaft.tail()), f(m_shaft.head()), m_shaft.is_seen());

  return *this;
}

pen_arrow& pen_arrow::crop_to(const screen_mask& M) {
  // update head visibility; never re-mark as visible
  if (M.crops(m_shaft.head())) m_head_seen = false;

  // screen masks are convex, so we're cropped iff an end is cropped
  if (M.crops(m_shaft.head()) || M.crops(m_shaft.tail())) {
    std::list<edge2d> shaft;
    shaft.push_back(m_shaft);
    M.crop_path(shaft);

    // search for visible segment; will find no more than one
    bool still_seen(false);

    for (auto ep = shaft.begin(); ep != shaft.end(); ++ep)
      if ((*ep).is_seen()) {
        still_seen = true;
        m_shaft = *ep;
      }

    if (!still_seen) {
      m_head_seen = false;
      m_shaft.seen(false);
    }
  }

  return *this;
}

bool pen_arrow::is_empty() const {
  return !(m_head_seen && m_shaft.is_seen()) || m_line.color().is_unset();
}

/*
 *
 * 1: base, 2: foot, 3: tip, 4: ear_right, 5: ear_left
 *
         5
          ...
.........  .....
.........1  2.....3
.........  .....
          ...
         4
*        <-height->
*
*        <--> inset (fraction of height)
*
*/
std::string pen_arrow::print_to(const format& fmt,
                                const std::string& len) const {
  if (is_empty()) return "";

  // else
  std::stringstream obuf;

  if (m_shaft.tail() == m_shaft.head())  // draw a ddot
  {
    label_state my_state(the_label_style());
    my_state.text_color(m_line.color());

    obuf << fmt.print_mark(m_shaft.tail(), pair(0, 0), DDOT, the_mark_size(),
                           my_state, len);
  }

  else {
    const pair head(m_shaft.head());
    const double stretch(length("1pt").to(len).magnitude());

    pair dir(head - m_shaft.tail());
    double dist(norm(dir));

    const pair unit_dir((stretch / dist) * dir);
    const pair unit_perp(J(unit_dir));

    const double wd(m_scale * m_style.width());
    const double ht(m_style.ratio() * wd * m_shorten);

    const pair base(head - ht * unit_dir);
    const pair foot(head - (1 - m_style.inset()) * ht * unit_dir);

    const pair ear_left(base + wd * unit_perp);
    const pair ear_right(base - wd * unit_perp);

    std::list<edge2d> bd;
    bd.push_back(edge2d(foot, ear_right, true));
    bd.push_back(edge2d(ear_right, head, true));
    bd.push_back(edge2d(head, ear_left, true));
    bd.push_back(edge2d(ear_left, foot, true));

    obuf << fmt.print_fill(bd, pair(0, 0), m_line.color(), m_line, len);
  }

  return obuf.str();
}

void pen_arrow::add_to_palette() const {
  if (!is_empty())
    the_picture().add_to_palette(m_line.color()).add_to_palette(m_base.color());
}
}  // namespace ePiX
