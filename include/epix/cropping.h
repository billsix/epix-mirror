/* 
 * cropping.h -- Implementation of ePiX's current drawing state.
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: January 07, 2007
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
#ifndef EPIX_CROPPING
#define EPIX_CROPPING

#include <string>

#include "edge_data.h"
#include "screen_mask.h"

namespace ePiX {

  class crop_state {
  public:
    crop_state(const screen_mask& M)
      : crop_state_mask(M.clone()) { }

    crop_state(const crop_state&);
    crop_state& operator= (const crop_state&);
    ~crop_state();

    std::list<edge2d>& crop_path(std::list<edge2d>&) const;
    std::list<edge2d>& crop_loop(std::list<edge2d>&) const;

  private:
    screen_mask* crop_state_mask;

  }; // end of class crop_state

} // end of namespace

#endif /* EPIX_CROPPING */
