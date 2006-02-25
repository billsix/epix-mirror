/* 
 *  Picture.cc -- epix2::Picture class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: January 08, 2006
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

#include <fstream>
#include <sstream>

#include "Pair.h"
#include "Pair_Map.h"
#include "Errors.h"

#include "Tile.h"
#include "Output.h"

#include "Screen.h"
#include "Picture.h"

namespace ePiX2 {

  /*
   * The "sz" argument is expected to look like "4in x 10cm" or "4 x 6 in",
   * specifically a double, followed by *either* a two-letter unitlength
   * or an "x", followed by a double *and* a two-letter unitlength. Spaces
   * are unimportant, as is initial or trailing garbage. Badly malformed
   * input (e.g., "4,6 in") may confuse the parsing code on some platforms.
   */
  Picture::Picture(const Pair arg1, const Pair arg2, std::string sz)
    : m_data(Screen(arg1, arg2))
  {
    std::string units1;
    std::string units2;

    double len1, len2;

    // find first digit
    std::string::size_type i=sz.find_first_of("0123456789.");

    if (i == std::string::npos)
      epix_warning("Invalid width in Picture initializer");

    else
      {
	sz.erase(0, i); // remove initial garbage
	const char* tmp = sz.c_str();
	char* arg;
	len1 = strtod(tmp, &arg);
	sz=arg;
      }

    // expect either unitlength or multiplication character
    i=sz.find_first_not_of(" ");
    std::string::size_type j=sz.find_first_of("Xx*");
    if (i != j)
      units1 = sz.substr(i, 2);

    // next digit
    i=sz.find_first_of("0123456789.");

    if (i == std::string::npos)
      epix_warning("Invalid height in Picture initializer");

    else
      {
	sz.erase(0, i);
	const char* tmp = sz.c_str();
	char* arg;
	len2 = strtod(tmp, &arg);
	sz=arg;
	i=sz.find_first_not_of(" ");
	units2 = sz.substr(i, 2);
      }

    if (units1 == "") // e.g. sz = "4x6in"
      units1 = units2;

    std::stringstream buf1;
    buf1 << len1 << units1;
    m_width=Length(buf1.str());

    std::stringstream buf2;
    buf2 << len2 << units2;
    m_height=Length(buf2.str());

    Length tmp=m_height;
    tmp.to(units1); // convert height to width units

    m_page=Screen(Pair(0,0), Pair(len1, tmp.magnitude())); // "actual size"
  }


  // superimpose a layer by affine scaling
  void Picture::paste(const Screen& child)
  {
    m_data.paste(child);
  }

  void Picture::paste(const Screen& child, const Rect& inset)
  {
    m_data.paste(child, inset);
  }

  void Picture::paste(const Screen& child, const Rect& cutout, 
		      const Rect& inset)
  {
    m_data.paste(child, cutout, inset);
  }

  // or by a specified Pair_Map
  void Picture::paste(const Screen& child, const Pair_Map& f)
  {
    m_data.paste(child, f);
  }

  void Picture::paste(const Screen& child, const Rect& cut, const Pair_Map&f)
  {
    m_data.paste(child, cut, f);
  }

  void Picture::clear(void)
  {
    m_data.clear();
  }

  void Picture::crop(void)
  {
    m_data.crop();
  }

  void Picture::add_color(const Color& col)
  {
    m_data.add_color(col);
  }

  void Picture::add_tile(const Tile& T)
  {
    m_data.add_tile(T);
  }

  Picture& Picture::operator<< (const Tile& T)
  {
    m_data.add_tile(T);
    return *this;
  }

  void Picture::print_to(const char* filename, const Format& fmt)
  {
    std::ofstream outfile(filename);

    if (!outfile.is_open())
      {
	// TO DO: More sophisticated handling...
	std::stringstream err_msg;
	err_msg << "Cannot open file \"" << filename << "\"";
	epix_warning(err_msg.str());
      }

    // TO DO: shift by offsets
    m_page.paste(m_data);

    outfile << fmt.pic_header(m_page.m_border.bl(), m_page.m_border.tr(),
			      m_width, m_page.m_pallet);

    // TO DO: Underlayer
    std::list<Tile>::const_iterator curr;
    for (curr = m_page.m_tiles.begin(); curr != m_page.m_tiles.end(); ++curr)
      outfile << (*curr)->print_to(fmt);


    outfile << fmt.pic_footer();
    outfile.close();
  }
} // end of namespace
