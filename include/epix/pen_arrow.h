/*
 * pen_arrow.cc -- ePiX::pen_arrow class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.8
 * Last Change: July 19, 2007
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

#ifndef EPIX_PEN_ARROW
#define EPIX_PEN_ARROW

#include <list>
#include <string>

#include "edge_data.h"
#include "pen_data.h"
#include "tile.h"

namespace ePiX {

class affine;
class screen_mask;

// arrowhead tile class
class pen_arrow : public tile {
 public:
  pen_arrow(const pair& tail, const pair& head, const pen_data& line,
            const pen_data& base, double scale, double sin_th, bool head_seen);

  pen_arrow* clone() const override;

  pen_arrow& map_by(const affine&) override;
  pen_arrow& crop_to(const screen_mask&) override;

  bool is_empty() const override;

  std::string print_to(const format&, const std::string&) const override;
  void add_to_palette() const override;

 private:
  edge2d m_shaft;

  pen_data m_line;
  pen_data m_base;

  double m_scale;    // overall scale of head
  double m_shorten;  // foreshortening factor

  bool m_head_seen;

  arrowhead_state m_style;
};  // end of class pen_arrow

}  // namespace ePiX

#endif /* EPIX_PEN_ARROW */
