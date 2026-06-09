/* 
 * label_data.h -- ePiX::label_data class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.18
 * Last Change: September 16, 2007
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

/*
 * label_data comprises spatial location, screen offsets, text (contents),
 * mark type, and a label_style (q.v.). label_data can be constructed from
 * string-valued functions of position; examples are given below.
 */

#ifndef EPIX_LABEL
#define EPIX_LABEL

#include <string>

#include "enums.h"
#include "triples.h"

#include "pairs.h"

#include "marker_style.h"
#include "label_style.h"

namespace ePiX {

  class Camera;
  class Color;
  class screen;

  class label_data {
  public:
    label_data(const P& here, const P& offset,
	       const std::string& text, epix_mark_type mark=epix_mark_type::TEXT);

    // generate label text from user-specified function of 2 or 3 variables
    label_data(const P& here, const P& offset,
	       std::string f(double,double), epix_mark_type mark=epix_mark_type::TEXT);

    label_data(const P& here, const P& offset,
	       std::string f(double,double,double), epix_mark_type mark=epix_mark_type::TEXT);

    // marker; alignment affects only TICK marks
    label_data(const P& here, epix_mark_type mark, epix_label_posn align=epix_label_posn::c);

    // pass through to m_style
    label_data& text_color(const Color&);
    label_data& mask_color(const Color&);
    label_data& align_to(epix_label_posn);

    void draw() const;

  private:
    P m_here;
    pair m_offset;

    std::string m_text;
    epix_mark_type m_mark;

    marker_sizes m_sizes;
    label_state m_style;

    bool m_seen;
  }; // end of label_data class

} // end of namespace

#endif /* EPIX_LABEL */
