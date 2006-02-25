/* 
 * Text2.cc -- ePiX2 Screen text
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 12, 2006
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

#include <string>
#include <sstream>

#include "Pair.h"
#include "Pair_Map.h"
#include "Point.h"
#include "Length.h"
#include "Camera.h"

#include "Color.h"

#include "Screen.h"
#include "Output.h"

#include "Tile.h"
#include "Text2.h"

namespace ePiX2 {

  Text_Mark2::Text_Mark2(const Pair& loc, 
			 const epix2_mark_type mt, 
			 const Text_Style& style)
    : m_loc(loc), m_mark(mt), m_style(style)
  { 
    m_style.m_align=c; // markers centered
  }


  Text_Mark2* Text_Mark2::map_by(const Pair_Map& f) const
  {
    // Allocate
    Text_Mark2* value = clone();
    value->m_loc = f(m_loc);

    return value;
  }

  Text_Mark2* Text_Mark2::crop_to(const Rect& mask) const
  {
    if (mask.hides(m_loc))
      return 0;

    // Allocate
    return clone();
  }

  Text_Mark2* Text_Mark2::clone(void) const
  {
    // Allocate
    return new Text_Mark2(*this);
  }

  std::string Text_Mark2::print_to(const Format& fmt) const
  {
    return fmt.print_mark(m_loc, m_mark,
			  m_style.m_text, m_style.m_fill, m_style.m_edge,
			  m_style.m_pad, m_style.m_bord);
  }

  void Text_Mark2::add_colors_to(Screen& S) const
  {
    if (m_style.m_text != Color::Unset)
      S.add_color(m_style.m_text);

    if (m_style.m_fill != Color::Unset)
      S.add_color(m_style.m_fill);

    if (m_style.m_edge != Color::Unset)
      S.add_color(m_style.m_edge);
  }


  /* * * Text_Box functions * * */
  Text_Box2* Text_Box2::map_by(const Pair_Map& f) const
  {
    // Allocate
    Text_Box2* value = clone();
    value->m_loc = f(m_loc);

    return value;
  }

  Text_Box2* Text_Box2::crop_to(const Rect& mask) const
  {
    if (mask.hides(m_loc))
      return 0;

    return clone();
  }

  Text_Box2* Text_Box2::clone(void) const
  {
    // Allocate
    return new Text_Box2(*this);
  }

  std::string Text_Box2::print_to(const Format& fmt) const
  {
    return fmt.print_box(m_loc, m_msg,
			 m_style.m_text, m_style.m_fill, m_style.m_edge,
			 m_style.m_pad, m_style.m_bord,
			 m_style.m_align, m_style.m_font, m_style.m_angle);
  }

  void Text_Box2::add_colors_to(Screen& S) const
  {
    if (m_style.m_text != Color::Unset)
      S.add_color(m_style.m_text);

    if (m_style.m_fill != Color::Unset)
      S.add_color(m_style.m_fill);

    if (m_style.m_edge != Color::Unset)
      S.add_color(m_style.m_edge);
  }

} /* end of namespace */
