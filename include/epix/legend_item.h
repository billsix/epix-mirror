/* 
 * legend_item.h -- ePiX legend_item base class and implementations
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.11
 * Last Change: August 17, 2007
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
#ifndef EPIX_LEGEND_ITEM
#define EPIX_LEGEND_ITEM

#include <string>

#include "enums.h"
#include "Color.h"
#include "pen_data.h"

#include "label_data.h"
#include "markers.h"

namespace ePiX {

  class format;

  // used to represent legend items both in "space" and in the screen
  class legend_item {
  public:
    virtual ~legend_item() = default;
    virtual legend_item* clone() const = 0;

    virtual std::string value() const = 0;
    virtual std::string key(const format& fmt, double sz,
			    const pen_data& bpen,
			    const std::string& len) const = 0;

    double key_coord(double sz, const std::string& len) const;
  }; // end of base class


  // key is a solid filled square in current color
  class fill_item : public legend_item {
  public:
    fill_item(const std::string& text);
    fill_item* clone() const override;

    std::string value() const override;
    std::string key(const format&, double,
		    const pen_data&,
		    const std::string& len) const override;

  private:
    Color m_fill;
    std::string m_text;
  }; // end of class fill_item


  // key is a line segment in current line style
  class path_item : public legend_item {
  public:
    path_item(const std::string& text);
    path_item* clone() const override;

    std::string value() const override;
    std::string key(const format&, double sz, const pen_data&,
		    const std::string& len) const override;

  private:
    pen_data m_line;
    pen_data m_base;

    std::string m_text;
  }; // end of class path_item


  // key is a marker in the current style
  class mark_item : public legend_item {
  public:
    mark_item(epix_mark_type mark, const std::string& text);
    mark_item* clone() const override;

    std::string value() const override;
    std::string key(const format&, double sz, const pen_data&,
		    const std::string& len) const override;

  private:
    epix_mark_type m_mark;
    label_state m_style;
    marker_sizes m_sizes;

    std::string m_text;
  }; // end of class mark_item

} // end of namespace

#endif /* EPIX_LEGEND_ITEM */
