/* 
 * legend.h -- Plot/figure legend class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.11
 * Last Change: August 16, 2007
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
#ifndef EPIX_LEGEND
#define EPIX_LEGEND

#include <string>

#include "enums.h"

#include "Color.h"

namespace ePiX {

  class legend_item;
  class P;

  class legend {
  public:
    legend();

    legend(const legend&);

    legend& operator=(const legend&);

    ~legend();

    // add items; get key style from drawing state
    legend& fill_item(const std::string&); // solid color
    legend& path_item(const std::string&); // line
    legend& mark_item(epix_mark_type, const std::string&); // marker

    // decorations
    legend& backing(const Color&);
    legend& border(const Color&, double);
    legend& border(double); // line width in pt

    // item attributes
    legend& item_border(const Color&, double);
    legend& item_border(double);

    legend& label_skip(double); // 
    legend& key_size(double);   // size of legend keys in pt

    void draw(const P& loc, const P& offset=P(0,0),
	      epix_label_posn align=epix_label_posn::tr) const;

  private:
    Color m_backing;
    Color m_bord;
    double m_bord_width;

    Color m_item_bord;
    double m_item_bord_width;

    double m_key_size;
    double m_label_skip;

    std::list<legend_item*> m_items;
  }; // end of legend class

} // end of namespace

#endif /* EPIX_LEGEND */
