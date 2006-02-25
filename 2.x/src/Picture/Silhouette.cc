/* 
 * Silhouette.cc -- ePiX2 Silhouette class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 04, 2006
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
#include "Silhouette.h"

namespace ePiX2 {

  Silhouette::Silhouette(void)
    : m_line_color(Color::Unset), m_line_width(Length(0)),
      m_fill_color(Color::Unset),
      m_filled(true), m_cropped(false) { }

  Silhouette::Silhouette(const Color& pen, const Length& len,
			 const Color& paint, bool is_filled, bool is_cropped)
    : m_line_color(pen), m_line_width(len), m_fill_color(paint),
      m_filled(is_filled), m_cropped(is_cropped) { }


  Silhouette* Silhouette::clone(void) const
  {
    // Allocation
    return new Silhouette(*this);
  }

  Silhouette* Silhouette::map_by(const Pair_Map& f) const
  {
    // Allocation
    Silhouette* value = new Silhouette;

    // TO DO: Subdivide edges if f doesn't preserve lines
    for (Ep p=m_border.begin(); p!=m_border.end(); ++p)
      value->add_edge(f(p->m_from), f(p->m_to), (*p).is_drawn);

    value->m_line_color = m_line_color;
    value->m_line_width = m_line_width;

    value->m_fill_color = m_fill_color;
    value->m_filled     = m_filled;
    value->m_cropped    = m_cropped;

    return value;
  }

  std::string Silhouette::print_to(const Format& fmt) const
  {
    if (m_border.size() < 3) // must be at least a triangle
      return "";

    std::ostringstream obuf;

    obuf << fmt.print_sil(m_line_color, m_line_width, m_fill_color,
			  m_filled, m_cropped, vertices());

    if (m_cropped) // re-draw boundary
      {
	Outline my_bd(m_line_color, m_line_width, m_border, m_cropped);
	obuf << my_bd.print_to(fmt);
      }

    return obuf.str();
  }

  void Silhouette::add_colors_to(Screen& S) const
  {
    if (m_line_color != Color::Unset)
      S.add_color(m_line_color);

    if (m_fill_color != Color::Unset)
      S.add_color(m_fill_color);
  }

  std::list<Pair> Silhouette::vertices(void) const
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

  void Silhouette::chop(const Pair& pt, const Pair& N)
  {
    ePiX2::chop(pt, N, m_border, m_cropped);
  }


  Silhouette* Silhouette::crop_to(const Rect& mask) const
  {
    Silhouette* value = clone();

    value->chop(mask.t(), mask.t()+Pair(0,-1)); // top edge
    value->chop(mask.b(), mask.b()+Pair(0, 1)); // bottom edge
    value->chop(mask.r(), mask.r()+Pair(-1,0)); // right edge
    value->chop(mask.l(), mask.l()+Pair( 1,0)); // left edge
    /*
    if ((*value).m_border.size() == 0)
      return 0;
    else
    */
    return value;
  }

  // private
  void Silhouette::add_edge(const Edge2& E)
  {
    m_border.push_back(E);
  }

  // public
  void Silhouette::add_edge(const Pair& from, const Pair& to, bool drawn)
  {
    m_border.push_back(Edge2(from, to, drawn));
  }

} /* end of namespace */
