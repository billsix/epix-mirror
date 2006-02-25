/*** 
 ***  Tile.h -- ePiX2 Screen Tile hierarchy
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: February 4, 2006
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
 *** Tile_Base   - The base for the screen elements
 *** Tile        - Tile_Base handle class
 ***
 *** Outline     - Unfilled shape (curve-like objects)
 *** Silhouette  - Filled shape (surface-like objects)
 ***
 *** The classes Text_Marker (a single character) and Text_Box (an aligned
 *** label, with copious style data) are declared in a separate file but
 *** lie in the same hierarchy.
 ***/
#ifndef EPIX2_TILE
#define EPIX2_TILE

#include <string>
#include <list>

#include "Handle.h"
#include "Pair.h"

namespace ePiX2 {
  // Every drawable element in the Screen is a Tile
  class Pair_Map;
  class Format;
  class Screen;
  class Tile_Base {
  public:
    virtual ~Tile_Base(void) { }

    virtual Tile_Base* map_by(const Pair_Map&) const = 0;
    virtual Tile_Base* crop_to(const Rect&) const = 0;
    virtual Tile_Base* clone(void) const = 0;

    // for Outline/Silhouette dynamic binding; others define { }
    virtual void add_edge(const Pair&, const Pair&, bool drawn) = 0;

    virtual std::string print_to(const Format&) const = 0;
    virtual void add_colors_to(Screen&) const = 0;
  }; // end of class Tile_Base


  typedef Handle<Tile_Base> Tile;

  class Edge2 {
    friend class Outline;
    friend class Silhouette;

    friend void chop(const Pair&, const Pair&, std::list<Edge2>&, bool&);

  public:
    Edge2(const Pair& from, const Pair& to, bool drawn)
      : m_from(from), m_to(to), is_drawn(drawn) { }

  private:
    Pair m_from, m_to;
    bool is_drawn;
  }; // end of class Edge2

} /* end of namespace */

#endif /* EPIX2_TILE */
