/*** 
 ***  Text_Box.h -- ePiX2 Marker and Label templates
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: February 12, 2006
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
 ***
 *** This file provides:
 ***
 *** The classes Text_{Mark,Box}3, which represent markers and labels
 *** (respectively) in dimension 3.
 ***
 *** A Text_Mark consists of a location, a marker type, and a style.
 *** A Text_Box consists of a location, horizontal and vertical offsets
 *** (of type Length), a string (the label contents), and a style.
 ***
 ***/
#ifndef EPIX2_TEXT_BASE
#define EPIX2_TEXT_BASE

#include <string>

#include "Point.h"
#include "Camera.h"
#include "Length.h"
#include "Color.h"
#include "Tile.h"

#include "Styles.h"
#include "Text2.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Marker data classes * * */
  class Text_Mark3 : public Object_Base {
  public:
    Text_Mark3(const Point& loc=Origin, const epix2_mark_type mt=MK_NONE,
	       const Text_Style& style=Text_Style());

    Tile_Base* shoot(const Camera& cam) const;

  private:
    Point m_loc;
    epix2_mark_type m_mark;

    Text_Style m_style;
  }; // end of class Text_Mark3


  /* * * Label data * * */
  class Text_Box3 : public Object_Base {
  public:
    Text_Box3(const Point& loc=Origin, std::string msg="",
	      const Text_Style& ts=Text_Style(),
	      const Length& ho=Length(0), const Length& vo=Length(0))
      : m_loc(loc), m_msg(msg), m_style(ts), m_hoff(ho), m_voff(vo) { }

    void text(const Color&); // text color, etc.
    void fill(const Color&);
    void edge(const Color&);

    void pad(const double);
    void pad(const std::string);
    void pad(const Length&);

    void edge(const double);
    void edge(const std::string);
    void edge(const Length&);

    void align(epix2_align_type);
    void font(epix2_font_type);
    void rotate(double);

    void h_offset(const Length& len) { m_hoff = len; }
    void v_offset(const Length& len) { m_voff = len; }

    // set offsets in true pt
    void offset(const Pair& off);
    void offset(const double hoff, const double voff);

    Tile_Base* shoot(const Camera& cam) const;

  private:
    Point m_loc;
    std::string m_msg;

    Text_Style m_style;

    Length m_hoff;
    Length m_voff;
  }; // end of class Text_Box3

} /* end of namespace */

#endif /* EPIX2_TEXT_BASE */
