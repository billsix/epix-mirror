/* 
 * halfspace.h -- ePiX::halfspace class for clipping
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.8
 * Last Change: July 17, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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
 * This file provides the halfspace class for path clipping.
 * A halfspace contains a base point p and a unit vector n, and defines the
 * affine half space (x-p)|n > 0; n points "inward".
 */
#ifndef EPIX_HALFSPACE
#define EPIX_HALFSPACE

#include <list>

#include "triples.h"
#include "edge_data.h"

namespace ePiX {

  class halfspace {
  public:
    halfspace(const P& base, const P& perp);

    halfspace& reverse();

    bool clips(const P&) const;

    std::list<edge3d>& clip_path(std::list<edge3d>&) const;
    std::list<edge3d>& clip_loop(std::list<edge3d>&) const;

  private:
    P m_base;
    P m_perp;
  }; // end of class halfspace
} // end of namespace

#endif /* EPIX_HALFSPACE */
