/* 
 * legend.cc -- ePiX legend class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.1
 * Last Change: September 28, 2007
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
// #include <cmath>
#include <string>
#include <sstream>
#include <list>

#include "constants.h"

#include "triples.h"

#include "Color.h"

#include "length.h"

#include "camera.h"

#include "curves.h"

#include "screen.h"
#include "active_screen.h"
#include "screen_data.h"

#include "label_data.h"
#include "markers.h"

#include "legend_item.h"
#include "legend_tile.h"
#include "legend.h"

namespace ePiX {

  legend::legend()
    : m_backing(White()),
      m_bord(Black()), m_bord_width(PLAIN_WIDTH.magnitude()),
      m_item_bord(Black()), m_item_bord_width(0.5*PLAIN_WIDTH.magnitude()),
      m_key_size(12), m_label_skip(6) { }

  legend::legend(const legend& L)
    : m_backing(L.m_backing), m_bord(L.m_bord), m_bord_width(L.m_bord_width),
      m_item_bord(L.m_item_bord), m_item_bord_width(L.m_item_bord_width),
      m_key_size(L.m_key_size), m_label_skip(L.m_label_skip)
  {
    for (auto m_item : L.m_items)
      m_items.push_back(m_item->clone());
  }

  legend& legend::operator=(const legend& L)
  {
    if (&L != this)
      {
	std::list<legend_item*> tmp;
	for (auto m_item : L.m_items)
	  tmp.push_back(m_item->clone());

	m_backing = L.m_backing;
	m_bord = L.m_bord;
	m_bord_width = L.m_bord_width;

	m_item_bord = L.m_item_bord;
	m_item_bord_width = L.m_item_bord_width;

	m_key_size = L.m_key_size;
	m_label_skip = L.m_label_skip;

	swap(m_items, tmp);
      }

    return *this;
  }

  legend::~legend()
  {
    for (auto & m_item : m_items)
      delete m_item;
  }

  // add items
  legend& legend::fill_item(const std::string& text)
  {
    m_items.push_back(new ePiX::fill_item(text));
    return *this;
  }

  legend& legend::path_item(const std::string& text)
  {
    m_items.push_back(new ePiX::path_item(text));
    return *this;
  }

  legend& legend::mark_item(epix_mark_type mark, const std::string& text)
  {
    m_items.push_back(new ePiX::mark_item(mark, text));
    return *this;
  }

  // decorations
  legend& legend::backing(const Color& col)
  {
    m_backing = col;
    return *this;
  }

  legend& legend::border(const Color& col, double len)
  {
    m_bord = col;
    m_bord_width = len;

    return *this;
  }

  legend& legend::border(double len)
  {
    m_bord = Black();
    m_bord_width = len;

    return *this;
  }

  // item attributes
  legend& legend::item_border(const Color& col, double len)
  {
    m_item_bord = col;
    m_item_bord_width = len;

    return *this;
  }

  legend& legend::item_border(double len)
  {
    m_item_bord = Black();
    m_item_bord_width = len;

    return *this;
  }

  legend& legend::key_size(double len)
  {
    m_key_size = len;
    return *this;
  }

  legend& legend::label_skip(double len)
  {
    m_label_skip = len;
    return *this;
  }

  void legend::draw(const P& loc, const P& off, epix_label_posn align) const
  {
    auto ip(m_items.begin());
    if (ip == m_items.end())
      return;

    // else
    label_state ls(the_label_style());
    ls.mask_color(m_backing);
    ls.label_border(pen_data(m_bord, length(m_bord_width)));
    ls.align_to(align);

    active_screen()->m_screen
      ->add_tile(legend_tile(pair(loc.x1(), loc.x2()),
			     pair(off.x1(), off.x2()),
			     align,
			     ls.seen_through(cam()),
			     pen_data(cam()(m_item_bord),
				      length(m_item_bord_width)),
			     m_key_size, m_label_skip, m_items));
  }
} // end of namespace
