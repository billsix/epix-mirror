/*
 * clipping.cc -- Implementation of ePiX's current drawing state
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
#include <list>

#include "constants.h"
#include "triples.h"
#include "functions.h"
#include "clipping.h"

namespace ePiX {

using Hp = std::list<halfspace>::const_iterator;
static const double INF(EPIX_INFTY);

clip_state::clip_state(const P& arg1, const P& arg2) {
  double min1(min(arg1.x1(), arg2.x1())), max1(max(arg1.x1(), arg2.x1()));
  double min2(min(arg1.x2(), arg2.x2())), max2(max(arg1.x2(), arg2.x2()));
  double min3(min(arg1.x3(), arg2.x3())), max3(max(arg1.x3(), arg2.x3()));

  double avg1(0.5 * (min1 + max1)), avg2(0.5 * (min2 + max2)),
      avg3(0.5 * (min3 + max3));

  // inward-pointing faces of clip_box
  m_walls.push_back(halfspace(P(max1, avg2, avg3), P(-1, 0, 0)));
  m_walls.push_back(halfspace(P(min1, avg2, avg3), P(1, 0, 0)));

  m_walls.push_back(halfspace(P(avg1, max2, avg3), P(0, -1, 0)));
  m_walls.push_back(halfspace(P(avg1, min2, avg3), P(0, 1, 0)));

  m_walls.push_back(halfspace(P(avg1, avg2, max3), P(0, 0, -1)));
  m_walls.push_back(halfspace(P(avg1, avg2, min3), P(0, 0, 1)));
}

clip_state::clip_state() {
  m_walls.push_back(halfspace(P(INF, 0, 0), P(-1, 0, 0)));
  m_walls.push_back(halfspace(P(-INF, 0, 0), P(1, 0, 0)));

  m_walls.push_back(halfspace(P(0, INF, 0), P(0, -1, 0)));
  m_walls.push_back(halfspace(P(0, -INF, 0), P(0, 1, 0)));

  m_walls.push_back(halfspace(P(0, 0, INF), P(0, 0, -1)));
  m_walls.push_back(halfspace(P(0, 0, -INF), P(0, 0, 1)));
}

clip_state& clip_state::add_face(const P& loc, const P& perp) {
  if (EPIX_EPSILON < norm(perp)) m_extras.push_back(halfspace(loc, perp));

  return *this;
}

clip_state& clip_state::clear_extras() {
  m_extras.clear();

  return *this;
}

bool clip_state::clips(const P& arg) const {
  for (const auto& m_wall : m_walls)
    if (m_wall.clips(arg)) return true;

  for (const auto& m_extra : m_extras)
    if (m_extra.clips(arg)) return true;

  return false;
}

// return reference to clipped argument
std::list<edge3d>& clip_state::clip_path(std::list<edge3d>& L) const {
  for (const auto& m_wall : m_walls) m_wall.clip_path(L);

  for (const auto& m_extra : m_extras) m_extra.clip_path(L);

  return L;
}

std::list<edge3d>& clip_state::clip_loop(std::list<edge3d>& L) const {
  for (const auto& m_wall : m_walls) m_wall.clip_loop(L);

  for (const auto& m_extra : m_extras) m_extra.clip_loop(L);

  return L;
}

clip_state& the_clip_box() {
  static auto* the_clip_state(new clip_state());
  return *the_clip_state;
}
}  // namespace ePiX
