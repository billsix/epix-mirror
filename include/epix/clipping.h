/* 
 * clipping.h -- ePiX's clip box
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.13
 * Last Change: August 23, 2007
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
 * The class in this file implements backend for user-level global
 * functions defined in state.h. This header is not part of epix.h.
 */
#ifndef EPIX_CLIPPING
#define EPIX_CLIPPING

#include <string>

#include "edge_data.h"
#include "halfspace.h"

namespace ePiX {

  class P;

  class clip_state {
  public:
    // create inward-pointing faces of clip_box
    clip_state(const P& arg1, const P& arg2);
    clip_state(); // very large box by default

    // add and remove faces
    clip_state& add_face(const P& loc, const P& perp);
    clip_state& clear_extras();

    bool clips(const P&) const;

    // return reference to clipped argument
    std::list<edge3d>& clip_path(std::list<edge3d>&) const;
    std::list<edge3d>& clip_loop(std::list<edge3d>&) const;

  private:
    std::list<halfspace> m_walls;  // always present

    std::list<halfspace> m_extras; // user-added
  }; // end of class clip_state

  clip_state& the_clip_box();

} // end of namespace

#endif /* EPIX_CLIPPING */
