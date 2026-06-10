/*
 * arrow_data.cc -- ePiX arrow class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
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
#include <cmath>

#include "triples.h"
#include "functions.h"

#include "camera.h"

#include "edge_data.h"
#include "path_data.h"
#include "clipping.h"

#include "arrow_style.h"
#include "pen_data.h"
#include "paint_style.h"

#include "halfspace.h"
#include "pen_line.h"
#include "pen_arrow.h"

#include "screen_data.h"
#include "screen.h"
#include "active_screen.h"

#include "arrow_data.h"

namespace ePiX {

arrow_data::arrow_data(const std::vector<P>& pv, const P& base, const P& tip,
                       double scale)
    : m_base(base),
      m_tip(tip),
      m_scale(scale),
      m_head_seen(!the_clip_box().clips(m_tip)) {
  for (unsigned int i = 0; i < pv.size() - 1; ++i)
    m_shaft.push_back(edge3d(pv.at(i), pv.at(i + 1), true));

  the_clip_box().clip_path(m_shaft);
}

arrow_data& arrow_data::clip_to(const halfspace& knife) {
  knife.clip_path(m_shaft);
  if (knife.clips(m_tip)) m_head_seen = false;

  return *this;
}

void arrow_data::photo(screen& scr, const Camera& mycam, const pen_data& line,
                       const pen_data& under) const {
  arrow_data tmp_data(*this);
  if (mycam.needs_clip()) tmp_data.clip_to(mycam.clip_plane());

  std::list<edge2d> edges;
  for (auto p = tmp_data.m_shaft.begin(); p != tmp_data.m_shaft.end(); ++p) {
    edge2d tmp(mycam((*p).tail()), mycam((*p).head()), (*p).is_seen());

    if (!tmp.is_null())  // endpoints not equal
      edges.push_back(tmp);
  }

  // draw shaft
  scr.m_screen->add_tile(
      pen_line(line.seen_through(mycam), under.seen_through(mycam), edges));

  // and arrowhead
  if (m_head_seen) {
    const P dir(m_tip - m_base);
    const P to_cam(mycam.viewpt() - m_base);

    const double sin_th(norm(dir * to_cam) / (norm(dir) * norm(to_cam)));

    scr.m_screen->add_tile(
        pen_arrow(mycam(m_base), mycam(m_tip), line.seen_through(mycam),
                  under.seen_through(mycam), m_scale, sin_th, m_head_seen));
  }
}

void arrow_data::draw() const {
  photo(*active_screen(), cam(), the_paint_style().line_pen(),
        the_paint_style().base_pen());
}

void arrow_data::draw(const pen_data& p1, const pen_data& p2) const {
  photo(*active_screen(), cam(), p1, p2);
}
}  // namespace ePiX
