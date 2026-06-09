/* 
 * path_data.cc -- ePiX implementation classes for polygons and paths
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.17
 * Last Change: June 26, 2017
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2017
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
#include <algorithm>

#include "errors.h"
#include "constants.h"

#include "triples.h"
#include "camera.h"
#include "sphere.h"

#include "edge_data.h"
#include "active_screen.h"
#include "clipping.h"

#include "paint_style.h"

#include "halfspace.h"
#include "pen_line.h"
#include "pen_fill.h"

#include "screen_data.h"
#include "screen.h"

#include "path_data.h"

namespace ePiX {

  path_data::path_data() : m_closed(false), m_filled(false) { }

  path_data::path_data(const std::vector<P>& pv, bool closed, bool filled)
    : m_closed(closed), m_filled(closed && filled)
  {
    if (pv.size() == 1)
      m_data.push_back(edge3d(pv.at(0), pv.at(0), true)); // will delete later

    else
      for (unsigned int i=0; i<pv.size()-1; ++i)
	m_data.push_back(edge3d(pv.at(i), pv.at(i+1), true));
  }

  path_data::path_data(const std::list<edge3d>& data, bool closed, bool filled)
    : m_closed(closed), m_filled(closed && filled), m_data(data) { }


  path_data* path_data::clone() const
  {
    return new path_data(*this);
  }


  // join to arg; no effect if we're closed
  path_data& path_data::pt(const P& arg)
  {
    if (0 < m_data.size())
      {
	const P hd((--m_data.end())->head()); // terminal point
	m_data.push_back(edge3d(hd, arg, true));
      }

    else
      m_data.push_back(edge3d(arg, arg, true));

    return *this;
  }


  // concatenate path_data segments
  path_data& path_data::operator+= (path_data pd)
  {
    if (!m_closed)
      m_data.splice(m_data.end(), pd.m_data);

    return *this;
  }

  // concatenate, reversing second sequence
  path_data& path_data::operator-= (path_data pd)
  {
    if (!m_closed)
      {
	for (auto & ep : pd.m_data)
	  ep.reverse();   // swap tails and heads

	pd.m_data.reverse(); // reverse list order
	m_data.splice(m_data.end(), pd.m_data);
      }

    return *this;
  }

  bool path_data::is_closed() const
  {
    return m_closed;
  }

  bool path_data::is_filled() const
  {
    return m_filled;
  }


  // set flag and close up if necessary
  path_data& path_data::close()
  {
    if (0 < m_data.size())
      {
	m_closed=true;

	const P tail_pt((m_data.begin())->tail());
	const P head_pt((--m_data.end())->head());

	if (tail_pt != head_pt)
	  m_data.push_back(edge3d(head_pt, tail_pt, true));
      }

    return *this;
  }

  // no action unless we're closed
  path_data&  path_data::fill(const bool arg)
  {
    if (m_closed)
      m_filled=arg;
    return *this;
  }


  path_data& path_data::clip()
  {
    if (m_filled)
      the_clip_box().clip_loop(m_data);

    else
      the_clip_box().clip_path(m_data);

    return *this;
  }

  path_data& path_data::clip_to(const halfspace& knife)
  {
    if (m_filled)
      knife.clip_loop(m_data);
    else
      knife.clip_path(m_data);
    return *this;
  }


  path_data& path_data::clip_to(const Sphere& S, const P& viewpt,
				bool back)
  {
    const double rad(S.radius());

    P dir(S.center() - viewpt);
    const double dist(norm(dir));

    if (rad < dist) // viewpt is outside
      {
	dir *= 1/dist; // unit vector from center to viewpt
	double x(rad*rad/dist);
	
	halfspace knife(S.center() - x*dir, dir);

	if (back)
	  knife.reverse();

	clip_to(knife);
      }

    else
      epix_warning("Can't clip path to sphere from interior viewpoint");

    return *this;
  }


  void path_data::photo(screen& scr, const Camera& mycam, const Color& fill,
			const pen_data& line, const pen_data& base) const
  {
    path_data tmp_data(*this);

    tmp_data.clip(); // clip_box
    if (mycam.needs_clip())
      tmp_data.clip_to(mycam.clip_plane());

    std::list<edge2d> edges;
    unsigned int edge_ct(0); // don't print filled unless >= 3 non-null edges

    for (auto p=tmp_data.m_data.begin();
	 p != tmp_data.m_data.end(); ++p)
      {
	edge2d tmp(mycam((*p).tail()), mycam((*p).head()), (*p).is_seen());

	if (!tmp.is_null()) // endpoints not equal
	  {
	    ++edge_ct;
	    edges.push_back(tmp);
	  }
      }

    // draw fill bordered by base, then re-draw border with line/base pens
    if (m_filled && 3 <= edge_ct)
      {
	scr.m_screen->add_tile(pen_fill(mycam(fill),
					line.seen_through(mycam), edges));
	if (line.width() < base.width())
	  scr.m_screen->add_tile(pen_line(line.seen_through(mycam),
					  base.seen_through(mycam), edges));
      }

    else if (1 <= edge_ct)
      {
	// print edges in groups
	auto p(edges.begin());
	std::list<edge2d> path_buf;

	int count(0);
	while (p != edges.end())
	  {
	    path_buf.push_back(*p);
	    p = edges.erase(p);

	    if (++count == 250 || p == edges.end()) // Magic number
	      {
		scr.m_screen->add_tile(pen_line(line.seen_through(mycam),
						base.seen_through(mycam),
						path_buf));
		path_buf.clear();
		count = 0;
	      }
	  }
      }
  }

  std::vector<P> path_data::data() const
  {
    std::list<P> tmp;
    auto ep(m_data.begin());

    // first point is duplicated by constructor
    while (ep != m_data.end())
      {
	tmp.push_back((*ep).head());
	++ep;
      }

    std::vector<P> value;
    value.assign(tmp.begin(), tmp.end());

    return value;
  }

  void path_data::draw() const
  {
    photo(*active_screen(), cam(), the_paint_style().fill_color(),
	  the_paint_style().line_pen(), the_paint_style().base_pen());
  }

  void path_data::draw(const Color& col, const pen_data& pen) const
  {
    photo(*active_screen(), cam(), col, pen, Xfine());
  }
} // end of namespace
