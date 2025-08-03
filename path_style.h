/* 
 * path_style.h -- ePiX's class for solid/dashed/dotted paths
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.7
 * Last Change: July 13, 2007
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

/*
 * This file implements ePiX's styles for dashed and dotted paths. The
 * data are a string containing dashes (-), spaces ( ), and dots (.),
 * each of which represents a fixed portion of a path segment, e.g.
 *
 * Solid:  "-"
 * Dashed: "-  -"
 * Dotted: " . "
 * Custom: "- . -- . -", etc.
 *
 * The class in this file implements backend for user-level global
 * functions defined in state.h.
 *
 * This header is not part of epix.h.
 */
#ifndef EPIX_PATH_STYLE
#define EPIX_PATH_STYLE

#include <string>
#include <vector>

namespace ePiX {

  class path_state {
  public:
    path_state();
    path_state(std::string);

    path_state& separation(double len); // dist in pt between adjacent elements

    double separation() const;
    unsigned int natural_length() const;
    std::vector<double> breakpts() const;

    bool is_solid() const;

  private:
    double m_separation;    // true length of pattern
    std::vector<double> m_breakpoints; // subdivision of [0,1]

    unsigned int m_natural_length; // number of chars in defining string
    bool m_solid;
  }; // end of class path_state

  path_state& the_path_style();

} // end of namespace

#endif /* EPIX_PATH_STYLE */
