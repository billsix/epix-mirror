/* 
 * Outline.cc -- ePiX2::Outline class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: January 29, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <list>
#include <vector>
#include <string>
#include <sstream>

#include "Pair.h"
#include "Functions.h"
#include "Length.h"
#include "Color.h"

#include "Screen.h"
#include "Output.h"
#include "Tile.h"

#include "Chop.h"
#include "Outline.h"

namespace ePiX2 {

  Outline::Outline(void)
    : m_line_color(Color::Unset), m_line_width(Length(0)),
      m_under_color(Color::Unset), m_under_width(Length(0)),
      m_cropped(false) { }

  Outline::Outline(const Color& pen, const Length& len, bool is_cropped)
    : m_line_color(pen), m_line_width(len),
      m_under_color(pen), m_under_width(Length(0)),
      m_cropped(is_cropped) { }

  Outline::Outline(const Color& pen1, const Length& len1,
		   const Color& pen2, const Length& len2, bool is_cropped)
    : m_line_color(pen1), m_line_width(len1),
      m_under_color(pen2), m_under_width(len2),
      m_cropped(is_cropped) { }


  Outline::Outline (const Color& pen, const Length& len,
		    const std::list<Edge2>& bd, bool cropped)
    : m_border(bd), m_line_color(pen), m_line_width(len),
      m_under_color(Color::Unset), m_under_width(Length(0)),
      m_cropped(cropped) { }

  Outline* Outline::clone(void) const
  {
    // Allocation
    return new Outline(*this);
  }

  Outline* Outline::map_by(const Pair_Map& f) const
  {
    // Allocation
    Outline* value = new Outline;

    // TO DO: Subdivide edges if f doesn't preserve lines
    for (Ep p=m_border.begin(); p!=m_border.end(); ++p)
      value->add_edge(f(p->m_from), f(p->m_to), (*p).is_drawn);

    value->m_line_color = m_line_color;
    value->m_line_width = m_line_width;

    value->m_under_color = m_under_color;
    value->m_under_width = m_under_width;

    value->m_cropped    = m_cropped;

    return value;
  }

  std::string Outline::print_to(const Format& fmt) const
  {
    if (!m_cropped)
      return fmt.print_outline(m_line_color, m_line_width,
			       m_under_color, m_under_width, vertices());

    // else
    std::ostringstream obuf;

    std::list<Pair> segment;
    bool in=false; // currently printing a visible segment?

    for (Ep p=m_border.begin(); p!=m_border.end(); ++p)
      {
	if ((*p).is_drawn)
	  {
	    if (!in) // start new segment
	      {
		segment.clear();
		in=true;
		segment.push_back((*p).m_from);
	      }

	    segment.push_back((*p).m_to);
	  }

	else
	  if (in) // end a segment
	    {
	      in=false;
	      obuf << fmt.print_outline(m_line_color, m_line_width,
					m_under_color, m_under_width, segment);
	      segment.clear();
	    }
	// else out and undrawn, do nothing
      } // examined all edges

    if (segment.size() != 0) // flush undrawn edges
      obuf << fmt.print_outline(m_line_color, m_line_width,
				m_under_color, m_under_width, segment);

    return obuf.str();
  }

  void Outline::add_colors_to(Screen& S) const
  {
    if (m_line_color != Color::Unset)
      S.add_color(m_line_color);

    if (m_under_color != Color::Unset)
      S.add_color(m_under_color);
  }


  std::list<Pair> Outline::vertices(void) const
  {
    std::list<Pair> V;

    // Assume Edges form a contiguous path
    for (Ep p=m_border.begin(); p!=m_border.end(); ++p)
      {
	if (p == m_border.begin())
	  V.push_back(p->m_from);

	V.push_back(p->m_to);
      }

    return V;
  }

  void Outline::snip(const Pair& pt, const Pair& N)
  {
    ePiX2::chop(pt, N, m_border, m_cropped);
  }


  Outline* Outline::crop_to(const Rect& mask) const
  {
    Outline* value = clone();

    value->snip(mask.t(), mask.t()+Pair(0,-1)); // top edge
    value->snip(mask.b(), mask.b()+Pair(0, 1)); // bottom edge
    value->snip(mask.r(), mask.r()+Pair(-1,0)); // right edge
    value->snip(mask.l(), mask.l()+Pair( 1,0)); // left edge

    /*
    if ((*value).m_border.size() == 0)
      return 0;
    else
    */
    return value;
  }

  // private
  void Outline::add_edge(const Edge2& E)
  {
    m_border.push_back(E);
  }

  // public
  void Outline::add_edge(const Pair& from, const Pair& to, bool drawn)
  {
    m_border.push_back(Edge2(from, to, drawn));
  }

} /* end of namespace */
