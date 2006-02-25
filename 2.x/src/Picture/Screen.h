/*** 
 *** Screen.h -- ePiX2::Screen class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 06, 2006
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
 *** This file provides the:
 ***   - Screen class, a maskable window
 ***/

#ifndef EPIX2_SCREEN
#define EPIX2_SCREEN

#include <set>
#include <list>

#include "Pair.h"
#include "Pair_Map.h"
#include "Color.h"
#include "Tile.h"

namespace ePiX2 {
  /* * * Screen.h * * */
  /* Underlayer
  class Matte {
  public:

  private:
    Color m_pen;
    Length m_width;

    Color m_tint;
    Color m_tint2;
  }; // end of class Matte
  */

  class Screen {
    friend class Picture;
  public:
    Screen(void) { }

    Screen(const Pair arg1, const Pair arg2);
    Screen(const Rect&);

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
    Screen& operator<< (const Tile&);

  private:
    Rect m_border;
    std::list<Tile> m_tiles;
    std::set<Color> m_pallet;
    // Matte m_matte

    void import_pallet(const Screen&);
  }; // end of Screen class

} /* end of namespace */

#endif /* EPIX2_SCREEN */
