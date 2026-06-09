/* 
 * picture_data.h -- ePiX::picture implementation class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.7
 * Last Change: July 14, 2007
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
 * The picture_data class was designed with three primary goals:
 *
 * (i) Encapsulate state data formerly scattered (and inconsistently
 *   structured) through multiple files,
 *
 * (ii) Provide some of the flexibility of building a scene in memory
 *   instead of writing data immediately to the output,
 *
 * (iii) Ensure compatibility with existing syntax.
 *
 * A picture_data comprises two screens (canvas -- the bounding_box, and
 * page -- the picture box), unitlength and offsets, state data
 * (clip_box; angle units; styles for labels, painting, and arrowheads),
 * and a set of colors so that color dclarations in the output file can
 * be made using named colors.
 *
 * Dozens of global and member functions need to access and modify
 * the_picture(). Instead of making these functions friends, our data
 * (except for the color palette) are public. Under the circumstances,
 * this seemed the Path of Least Evil.
 */
#ifndef EPIX_PICTURE_DATA
#define EPIX_PICTURE_DATA

#include <string>
#include <list>
#include <set>

#include "verbatim.h"

#include "length.h"
#include "Color.h"
#include "screen.h"

namespace ePiX {

  class format;

  class picture_data {
  public:
    // rely on members' default constructors
    picture_data();

    picture_data(const picture_data&);
    picture_data& operator= (const picture_data&);
    ~picture_data();

    picture_data& add_to_palette(const Color&);

    void set_format(const format&);

    // output requires knowledge of unitlength; all other ops handled by data
    void print();
    void print_to(const format&);
    void print_to(const std::string& filename, const format&);

    void  pre_write(const std::string&);
    void post_write(const std::string&);

    //// public data (true size, offsets, and screens) ////
    length the_unitlength;
    length the_hoffset, the_voffset;

    screen the_canvas;
    screen the_page;


  private:
    format* m_format;
    std::set<Color> m_palette;
    // user-specified strings to be written before/after start of picture
    std::list<verbatim> m_pre_writes;
    std::list<verbatim> m_post_writes;
  }; // end of class picture_data

} // end of namespace

#endif /* EPIX_PICTURE_DATA */
