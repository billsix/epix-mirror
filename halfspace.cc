/* 
 * halfspace.cc -- ePiX::halfspace class for clipping
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
#include <list>

#include "triples.h"
#include "edge_data.h"
#include "crop_algorithms.h"

#include "halfspace.h"

namespace ePiX {

  halfspace::halfspace(const P& base, const P& perp)
    : m_base(base), m_perp((1.0/norm(perp))*perp) { }

  halfspace& halfspace::reverse()
  {
    m_perp *= -1;
    return *this;
  }

  bool halfspace::clips(const P& arg) const
  {
    return height(m_perp, m_base, arg) < -EPIX_EPSILON;
  }

  std::list<edge3d>& halfspace::clip_path(std::list<edge3d>& L) const
  {
    return chop_path<P>(m_perp, m_base, L);
  }

  std::list<edge3d>& halfspace::clip_loop(std::list<edge3d>& L) const
  {
    return chop_loop<P>(m_perp, m_base, L);
  }
} // end of namespace
