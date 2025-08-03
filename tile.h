/* 
 * tile.h -- ePiX::screen object base class (tile)
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: January 14, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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
#ifndef EPIX_TILE
#define EPIX_TILE

#include <string>

namespace ePiX {

  class format;
  class length;
  class affine;
  class screen;
  class screen_mask;

  class tile {
  public:
    virtual ~tile();

    virtual tile* clone() const = 0;
    virtual tile& map_by(const affine&) = 0;
    virtual tile& crop_to(const screen_mask&) = 0;

    virtual bool is_empty() const = 0;

    // pass length for (e.g.) dash length, marker size
    virtual std::string print_to(const format&, const std::string&) const = 0;
    virtual void add_to_palette() const = 0;
  }; // end of class tile

} // end of namespace

#endif /* EPIX_TILE */
