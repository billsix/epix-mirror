/* 
 * picture_data.cc -- ePiX::picture implementation class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.14
 * Last Change: May 13, 2014
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2014
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
#include <iostream>
#include <fstream>
#include <sstream>

#include "errors.h"
#include "utils.h"
#include "Color.h"

#include "verbatim.h"

#include "format.h"
#include "eepic.h" // default format
#include "picture.h"

#include "screen_data.h"
#include "picture_data.h"

namespace ePiX {

  picture_data::picture_data()
    : m_format(new eepic()), m_palette() { }


  picture_data::picture_data(const picture_data& pd)
    : the_unitlength(pd.the_unitlength),
      the_hoffset(pd.the_hoffset), the_voffset(pd.the_voffset),
      the_canvas(pd.the_canvas), the_page(pd.the_page),
      m_format(pd.m_format->clone()), m_palette(pd.m_palette),
      m_pre_writes(pd.m_pre_writes), m_post_writes(pd.m_post_writes) { }


  picture_data& picture_data::operator= (const picture_data& pd)
  {
    if (this != &pd)
      {
	format* tmp_fmt(pd.m_format->clone());

	// okay, safe to muck with our data
	the_unitlength = pd.the_unitlength;
	the_hoffset = pd.the_hoffset;
	the_voffset = pd.the_voffset;

	the_canvas = pd.the_canvas;
	the_page = pd.the_page;

	delete m_format;
	m_format = tmp_fmt;

	m_palette = pd.m_palette;

	m_pre_writes  = pd.m_pre_writes;
	m_post_writes = pd.m_post_writes;
      }

    return *this;
  } // end of copy assignment

  picture_data::~picture_data()
  {
    delete m_format;
  }


  picture_data& picture_data::add_to_palette(const Color& col)
  {
    if (!col.is_unset())
      m_palette.insert(col);
    return *this;
  }

  void picture_data::set_format(const format& fmt)
  {
    m_format = fmt.clone();
  }

  void picture_data::print()
  {
    print_to(*m_format);
  }

  void picture_data::print_to(const format& fmt)
  {
    the_page.clear();
    the_page.paste(the_canvas);

    const pair offset(the_hoffset.magnitude(), the_voffset.magnitude());

    std::cout << fmt.file_comments(the_unitlength.units(),
				   the_hoffset, the_voffset,
				   xmin(), xmax(), ymin(), ymax(),
				   the_page.h_size(), the_page.v_size());

    for (auto p = m_pre_writes.begin();
	 p != m_pre_writes.end(); ++p)
      std::cout << (*p).print_to(fmt, the_unitlength.units());

    // Need to build m_palette before writing picture header
    std::string my_screen(the_page.m_screen->print_to(fmt, the_unitlength));

    // the_page.br() = (0,0), so tr() is size
    std::cout << fmt.pic_header(the_page.tr(), offset,
				m_palette, the_unitlength.units())

	      << my_screen

	      << fmt.pic_footer();

    for (auto p = m_post_writes.begin();
	 p != m_post_writes.end(); ++p)
      std::cout << (*p).print_to(fmt, the_unitlength.units());
  }

  void picture_data::print_to(const std::string& filename, const format& fmt)
  {
    const char* fn(filename.c_str());

    std::ofstream output(fn);
    if (!output)
      {
	epix_warning("Cannot open file " + filename);
	return;
      }

    // else
    the_page.clear();
    the_page.paste(the_canvas);

    const pair offset(the_hoffset.magnitude(), the_voffset.magnitude());

    output << fmt.file_comments(the_unitlength.units(),
				the_hoffset, the_voffset,
				xmin(), xmax(), ymin(), ymax(),
				the_page.h_size(), the_page.v_size());

    for (auto p = m_pre_writes.begin();
	 p != m_pre_writes.end(); ++p)
      output << (*p).print_to(fmt, the_unitlength.units());

    // Need to build m_palette before writing picture header
    std::string my_screen(the_page.m_screen->print_to(fmt, the_unitlength));

    // the_page.br() = (0,0), so tr() is size
    output << fmt.pic_header(the_page.tr(), offset,
				m_palette, the_unitlength.units())

	      << my_screen

	      << fmt.pic_footer();

    for (auto p = m_post_writes.begin();
	 p != m_post_writes.end(); ++p)
      output << (*p).print_to(fmt, the_unitlength.units());

    output.close();
  }

  void picture_data::pre_write(const std::string& text)
  {
    m_pre_writes.push_back(verbatim(text));
  }

  void picture_data::post_write(const std::string& text)
  {
    m_post_writes.push_back(verbatim(text));
  }
} // end of namespace
