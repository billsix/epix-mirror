/* 
 * Tile.cc -- ePiX2::Tile and Affine_Map class functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 29, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#include <iostream>
#include <algorithm>

#include "Pair.h"
#include "Point.h"
#include "Color.h"
#include "Screen.h"
#include "Output.h"
#include "Tile.h"

namespace ePiX2 {

  using std::cout;
  
  Affine_Map::Affine_Map(const Screen& parent, const Screen& child)
  {
    bl_parent = parent.bl();
    bl_child  =  child.bl();

    h_scale = parent.h_size()/child.h_size();
    v_scale = parent.v_size()/child.v_size();
  }

  Pair Affine_Map::operator() (const Pair arg) const
  {	
    return bl_parent+((arg - bl_child)&Pair(h_scale, v_scale)); 
  }

  Screen_Edge Affine_Map::operator() (const Screen_Edge E) const
  {
    return Screen_Edge(this->operator()(E.first), 
		       this->operator()(E.second),
		       E.line_color, E.drawn);
  }

  void Glyph::map_to(Screen& screen, const Affine_Map& map) 
  {
    // create copy of this
    Glyph* temp = new Glyph;
    (*temp) = (*this);

    temp->here = map(here); // adjust location

    screen.glyphs.push_back(*temp);
    screen.add_tile_ptr(temp);
  }

  // TO DO
  void Glyph::print(void)
  {
    print_line("\\put");

    raw_print(here);
    // TO DO: switch on glyph type, write LaTeX code
    std::cout << "{\\makebox(0,0)[c]{\\rule{2pt}{2pt}}}";
  }

  void Outline::map_to(Screen& screen, const Affine_Map& map) 
  {
    // create copy of this
    Outline* temp = new Outline;

    std::list<Screen_Edge>::iterator p;

    for (p=border.begin(); p != border.end(); ++p)
      temp->add_edge(map(*p)); // adjust location

    screen.outlines.push_back(*temp);
    screen.add_tile_ptr(temp);
  }


  // draw visible edges
  void Outline::print(void)
  {
    for_each(border.begin(), border.end(), 
	     std::bind2nd(std::mem_fun_ref(&Screen_Edge::print), false));
  }

  void Region::map_to(Screen& screen, const Affine_Map& map)
  {
    // create copy of this
    Region* temp = new Region;

    std::list<Screen_Edge>::iterator p;

    for (p=border.begin(); p != border.end(); ++p)
      temp->add_edge(map(*p)); // adjust location

    temp->fill_color=fill_color;
    // TO DO: copy fill style

    screen.regions.push_back(*temp);
    screen.add_tile_ptr(temp);
  }

  void Region::print(void)
  {
    if (border.size() > 2)
      {
	std::list<Screen_Edge>::iterator p;

	if (fillcolor_isnt(fill_color))
	  std::cout << std::endl << "\\psset{fillcolor=" 
		    << fill_color.name() << "}";

	if (fillstyle_isnt("solid"))
	  print_line("\\psset{fillstyle=solid}");

	print_line("\\pspolygon");
	for(p = border.begin(); p != border.end(); ++p)
	  {
	    if (p==border.begin())
	      raw_print((*p).first);

	    raw_print((*p).second);
	  }

	for_each(border.begin(), border.end(), 
		 std::bind2nd(std::mem_fun_ref(&Screen_Edge::print), false));
      }
  }

} /* end of namespace */
