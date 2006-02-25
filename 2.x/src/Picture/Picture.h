/*** 
 ***  Picture.h -- epix2::Picture class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 08, 2006
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 ***/
/*
 * This file provides the Picture class, a printable rectangular region
 * with true width and height, border and background, and markup abiity.
 *
 */
#ifndef EPIX2_PICTURE
#define EPIX2_PICTURE

#include <string>
#include <set>

#include "Length.h"
#include "Pair.h"
#include "Color.h"

#include "Output.h"
#include "Screen.h"

namespace ePiX2 {
  /* * * Picture.h * * */
  class Picture {
  public:
    // corners and dimensions, e.g. "4in x 15cm" or "4 x 6in"
    Picture(const Pair arg1, const Pair arg2, std::string);

    // superimpose a layer by affine scaling
    void paste(const Screen& child);
    void paste(const Screen& child, const Rect& inset);
    void paste(const Screen& child, const Rect& cutout, const Rect& inset);

    // or by a specified Pair_Map
    void paste(const Screen& child, const Pair_Map&);
    void paste(const Screen& child, const Rect&, const Pair_Map&);

    void clear(void); // blank screen
    void crop(void);  // cull elements outside mask

    void add_color(const Color&);
    void add_tile(const Tile&);
    Picture& operator<< (const Tile&);

    void width(const Length& w)  { m_width = w; }
    void height(const Length& h) { m_height = h; }

    void print_to(const char* filename, const Format& fmt);

    Screen& screen(void) { return m_data; }

  private:
    Screen m_data;
    Screen m_page; // one corner at (0,0)

    Length m_width;
    Length m_height;

    Length m_hoff;
    Length m_voff;

  }; // end of class Picture

} /* end of namespace */

#endif /* EPIX2_PICTURE */
