/*** 
 ***  Text2.h -- ePiX2 Screen text classes
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
 *** The classes Text_{Mark,Box}2, which represent markers and labels
 *** (respectively) in a Screen.
 ***
 *** A Text_Mark consists of a location, a marker type, and a style.
 *** A Text_Box consists of a location, horizontal and vertical offsets
 *** (of type Length), a string (the label contents), and a style.
 ***
 *** The 2-dimensional classes lie in the Tile_Base hierarchy.
 ***/
#ifndef EPIX2_TEXT2
#define EPIX2_TEXT2

#include <string>

#include "Pair.h"
#include "Camera.h"
#include "Length.h"
#include "Color.h"

#include "Styles.h"
#include "Tile.h"

namespace ePiX2 {
  class Format;
  class Text_Mark2 : public Tile_Base {
  public:
    Text_Mark2(void) { }
    Text_Mark2(const Pair& loc, const epix2_mark_type mt, const Text_Style&);

    Text_Mark2* map_by(const Pair_Map&) const;
    Text_Mark2* crop_to(const Rect&) const;
    Text_Mark2* clone(void) const;

    void filter_by(const Camera& cam) { m_style.filter_by(cam); }
    void add_edge(const Pair& from, const Pair& to, bool drawn) { }

    std::string print_to(const Format&) const;
    void add_colors_to(Screen&) const;

  private:
    Pair m_loc;
    epix2_mark_type m_mark;

    Text_Style m_style;
  }; // end of class Text_Mark2


  class Text_Box2 : public Tile_Base {
  public:
    Text_Box2(const Pair& loc=Pair(0,0), std::string msg="",
	      const Text_Style& ts=Text_Style(),
	      const Length& ho=Length(0), const Length& vo=Length(0))
      : m_loc(loc), m_msg(msg), m_style(ts), m_hoff(ho), m_voff(vo) { }

    Text_Box2* map_by(const Pair_Map&) const;
    Text_Box2* crop_to(const Rect&) const;
    Text_Box2* clone(void) const;

    void filter_by(const Camera& cam) { m_style.filter_by(cam); }
    void add_edge(const Pair& from, const Pair& to, bool drawn) { }

    std::string print_to(const Format&) const;
    void add_colors_to(Screen&) const;

  private:
    Pair m_loc;
    std::string m_msg;

    Text_Style m_style;

    Length m_hoff;
    Length m_voff;
  }; // end of class Text_Box2

} /* end of namespace */

#endif /* EPIX2_TEXT2 */
