/*
 * arrow_data.h -- ePiX implementation class for arrows
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
#ifndef EPIX_ARROW_DATA
#define EPIX_ARROW_DATA

#include <list>
#include <vector>

#include "triples.h"
#include "edge_data.h"

namespace ePiX {

class Camera;
class Color;
class halfspace;
class pen_data;
class screen;

// sequence of vertices
class arrow_data {
 public:
  arrow_data(const std::vector<P>&, const P& base, const P& tip,
             double scale = 1);

  arrow_data& clip();
  arrow_data& clip_to(const halfspace&);

  void photo(screen&, const Camera&, const pen_data&, const pen_data&) const;

  void draw() const;
  void draw(const pen_data&, const pen_data&) const;

 private:
  // head data
  P m_base, m_tip;
  double m_scale;  // size multiplier
  bool m_head_seen;

  // shaft
  std::list<edge3d> m_shaft;
};  // end of class arrow_data

}  // namespace ePiX

#endif /* EPIX_ARROW_DATA */
