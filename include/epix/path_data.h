/*
 * path_data.h -- ePiX implementation classes for polygons and paths
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2
 * Last Change5: May 04, 2008
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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
#ifndef EPIX_PATH_DATA
#define EPIX_PATH_DATA

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
class Sphere;

// sequence of vertices
class path_data {
 public:
  path_data();
  path_data(const std::vector<P>&, bool closed = false, bool filled = false);
  path_data(const std::list<edge3d>&, bool closed = false, bool filled = false);

  path_data* clone() const;

  // add points
  path_data& pt(const P&);

  // concatenate path_data segments
  path_data& operator+=(path_data);

  // concatenate, reversing second sequence
  path_data& operator-=(path_data);

  bool is_closed() const;
  bool is_filled() const;

  // make loop and prevent subsequent additions of points
  path_data& close();

  // (un)set fill flag if closed
  path_data& fill(const bool arg = true);

  path_data& clip();
  path_data& clip_to(const halfspace&);

  // assumes path already lies on sphere
  path_data& clip_to(const Sphere&, const P& viewpt, bool back = false);

  void photo(screen&, const Camera&, const Color& fill, const pen_data& line,
             const pen_data& base) const;

  std::vector<P> data() const;

  void draw() const;
  void draw(const Color&, const pen_data&) const;

 private:
  bool m_closed;
  bool m_filled;

  std::list<edge3d> m_data;

};  // end of class path_data

}  // namespace ePiX

#endif /* EPIX_PATH_DATA */
