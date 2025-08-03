/* 
 * pen_fill.cc -- ePiX::pen_fill class, for filled screen elements
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX
 *
 * Version 1.1.11
 * Last Change: August 12, 2007
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

#ifndef EPIX_PEN_FILL
#define EPIX_PEN_FILL

#include <list>
#include <string>

#include "edge_data.h"

#include "Color.h"
#include "path_style.h"
#include "tile.h"

namespace ePiX {

  class length;
  class screen_mask;

  class pen_fill : public tile {
  public:
    pen_fill();

    pen_fill(const Color&, const pen_data&, const std::list<edge2d>&);

    pen_fill(const Color&, const pen_data&, double theta,
	     const std::list<edge2d>&);

    pen_fill* clone() const;

    pen_fill& map_by(const affine&);
    pen_fill& crop_to(const screen_mask&);

    bool is_empty() const;

    std::string print_to(const format&, const std::string&) const;
    void add_to_palette() const;

  private:
    Color    m_tint;
    pen_data m_line;

    path_state m_style;

    std::list<edge2d> m_border; // filled boundary (loop)
    std::list<edge2d> m_edges;  // visible boundary (path)
  }; // end of class pen_fill

} // end of namespace

#endif /* EPIX_PEN_FILL */
