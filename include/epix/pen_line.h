/* 
 * pen_line.h -- ePiX::pen_line class, for line-like objects
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX
 *
 * Version 1.1.3
 * Last Change: March 04, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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

#ifndef EPIX_PEN_LINE
#define EPIX_PEN_LINE

#include <list>
#include <string>

#include "edge_data.h"
#include "pen_data.h"
#include "path_style.h"
#include "tile.h"

namespace ePiX {

  class length;
  class affine;
  class screen_mask;

  class pen_line : public tile {
  public:
    pen_line();
    pen_line(const pen_data&, const pen_data&, const std::list<edge2d>&);

    pen_line* clone() const;

    pen_line& map_by(const affine&);
    pen_line& crop_to(const screen_mask&);

    bool is_empty() const;

    std::string print_to(const format&, const std::string&) const;
    void add_to_palette() const;

    void add(const edge2d&);

  private:
    pen_data m_line;
    pen_data m_base;

    path_state m_style;

    std::list<edge2d> m_border;

  }; // end of class pen_line

} // end of namespace

#endif /* EPIX_PEN_LINE */
