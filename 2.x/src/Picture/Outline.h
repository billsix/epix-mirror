/*** 
 ***  Outline.h -- ePiX2::Outline class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 11, 2006
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
 *** Outline  - Unfilled shape (path-like objects)
 ***/
#ifndef EPIX2_OUTLINE
#define EPIX2_OUTLINE

#include <list>
#include <string>

#include "Pair.h"
#include "Pair_Map.h"
#include "Color.h"
#include "Length.h"

#include "Tile.h"

namespace ePiX2 {
  class Outline : public Tile_Base {
  public:
    Outline(void);
    Outline (const Color&, const Length&, bool);

    Outline (const Color&, const Length&,
	     const Color&, const Length&, bool);

    // for Silhouettes
    Outline (const Color&, const Length&, const std::list<Edge2>&, bool);

    Outline* clone(void) const;
    Outline* map_by(const Pair_Map&) const;
    Outline* crop_to(const Rect&) const;

    void add_edge(const Pair& from, const Pair& to, bool drawn=true);

    std::string print_to(const Format&) const;
    void add_colors_to(Screen&) const;

    std::list<Pair> vertices(void) const;

  private:
    typedef std::list<Edge2>::const_iterator Ep; // Edge2 pointer

    std::list<Edge2> m_border;

    Color  m_line_color;
    Length m_line_width;

    Color  m_under_color;
    Length m_under_width;

    bool m_cropped; // some edge(s) undrawn?

    void snip(const Pair&, const Pair&);
    void add_edge(const Edge2&);
  }; // end of class Outline

} /* end of namespace */

#endif /* EPIX2_OUTLINE */
