/* 
 * verbatim.h -- ePiX screen implemenation of verbatim text
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX
 *
 * Version 1.1.7
 * Last Change: July 14, 2007
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

#ifndef EPIX_VERBATIM
#define EPIX_VERBATIM

#include <string>

#include "tile.h"

namespace ePiX {

  class format;
  class affine;
  class screen_mask;

  class verbatim : public tile {
  public:
    verbatim(const std::string& text="");

    verbatim& map_by(const affine&);
    verbatim& crop_to(const screen_mask&);

    verbatim* clone() const;

    bool is_empty() const;

    std::string print_to(const format&, const std::string&) const;
    void add_to_palette() const;

  private:
    std::string m_text;
  }; // end of class verbatim

} // end of namespace

#endif /* EPIX_VERBATIM */
