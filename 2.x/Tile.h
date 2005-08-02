/*** 
 ***  Tile.h -- epix2::Tile class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 27, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 *** Affine_Map - mapping class for importing, etc.
 *** Tile    - The base for the screen elements
 *** Glyph   - Labels and Markers in the Screen
 *** Outline - Image of a Path or curve-like Object
 *** Region  - Image of a filled or surface-like Object
 ***/


#ifndef EPIX2_TILE
#define EPIX2_TILE

#include <iostream>
#include <list>

#include "Pair.h"
#include "Edge.h"
#include "Color.h"
#include "Enums.h"

namespace ePiX2 {

  class Screen; /***/

  /* * * Tile.h * * */

  class Affine_Map {

  public:

    Affine_Map(const Screen& parent, const Screen& child);
    Pair operator() (const Pair) const;
    Screen_Edge operator() (const Screen_Edge) const;

  private:
    Pair bl_parent;
    Pair bl_child;
    double h_scale, v_scale;

  }; // end of class Affine_Map


  // Every drawable element in the Screen is a Tile
  class Tile {

  public:

    virtual ~Tile(void) { }

    virtual void map_to(Screen&, const Affine_Map&) = 0;

    virtual void print(void) = 0;

  }; // end of class Tile


  // The image of a Label or Marker in the Screen
  class Glyph : public Tile {

  public:

    // initialize
    Glyph(Pair arg=Pair(0,0), epix2_mark_type t=MK_BOX) : here(arg) { TYPE=t; }

    void map_to(Screen& screen, const Affine_Map& map);

    void print(void);

  private:

    Pair here;
    Pair true_offset; // in pt
    epix2_mark_type TYPE;

    // Style attributes to be filled in

    //      Color text_color;
    //      Color background_color;
    //      Fill_Style background_style;
    //      std::string font_style;
    //      double box_padding; // true pt

    //      double label_angle;
    //      double shear_angle;
    //      epix2_alignment_type align;

  }; // end of class Glyph


  // Outline represents curves in the Screen, and is the parent of Region
  // Color is handled Edge by Edge
  class Outline : public Tile {

  public:

    void map_to(Screen& screen, const Affine_Map& map);
    void print(void);

    void add_edge(const Screen_Edge& E)
      {
	border.push_back(E);
      }

    // void set_line_width(std::string);

    std::list<Screen_Edge> border;

  protected:

    double line_width;

  }; // end of class Outline

  // Outline with filling
  class Region : public Outline {

  public:

    void map_to(Screen& screen, const Affine_Map& map);
    void print(void);

    void set_fill_color(const Color& col) { fill_color=col; }
    // void set_fill_style(Fill_Style);

    private:

    Color fill_color;

    // Fill_Style fill_style;

  }; // end of class Region

} /* end of namespace */

#endif /* EPIX2_TILE */
