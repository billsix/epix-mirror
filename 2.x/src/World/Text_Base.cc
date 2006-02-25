/* 
 * Text_Base.cc -- ePiX2 Labels and Markers
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
#include "Point.h"
#include "Length.h"
#include "Camera.h"

#include "Color.h"

#include "Screen.h"
#include "Output.h"

#include "Tile.h"
#include "Styles.h"
#include "Text_Base.h"

namespace ePiX2 {

  /* * * Text_Mark functions * * */
  Text_Mark3::Text_Mark3(const Point& loc, const epix2_mark_type mt,
			 const Text_Style& style)
    : m_loc(loc), m_mark(mt), m_style(style)
  { 
    m_style.m_align=c; // markers centered
  }

  Tile_Base* Text_Mark3::shoot(const Camera& cam) const
  {
    // Allocate
    Text_Mark2* value = new Text_Mark2(cam(m_loc), m_mark, m_style);

    value->filter_by(cam);
    return value;
  }


  /* * * Text_Box functions * * */
  void Text_Box3::text(const Color& col)   { m_style.m_text=col; }
  void Text_Box3::fill(const Color& col)   { m_style.m_fill=col; }
  void Text_Box3::edge(const Color& col)   { m_style.m_edge=col; }

  void Text_Box3::pad(const double p)      { m_style.m_pad=Length(p); }
  void Text_Box3::pad(const std::string p) { m_style.m_pad=Length(p); }
  void Text_Box3::pad(const Length& len)   { m_style.m_pad=len; }

  void Text_Box3::edge(const double p)     { m_style.m_bord=Length(p); }
  void Text_Box3::edge(const std::string p){ m_style.m_bord=Length(p); }
  void Text_Box3::edge(const Length& len)  { m_style.m_bord=len; }

  void Text_Box3::align(epix2_align_type t) { m_style.m_align=t; }
  void Text_Box3::font(epix2_font_type t)   { m_style.m_font=t; }
  void Text_Box3::rotate(double t)          { m_style.m_angle=t; }

  void Text_Box3::offset(const Pair& off)
  {
    m_hoff = Length(off.x1());
    m_voff = Length(off.x2());
  }

  void Text_Box3::offset(const double hoff, const double voff)
  {
    m_hoff = Length(hoff);
    m_voff = Length(voff);
  }


  Tile_Base* Text_Box3::shoot(const Camera& cam) const
  {
    // Allocate
    Text_Box2* value = new Text_Box2(cam(m_loc), m_msg, m_style,
				     m_hoff, m_voff);

    value->filter_by(cam);
    return value;
  }

} /* end of namespace */
