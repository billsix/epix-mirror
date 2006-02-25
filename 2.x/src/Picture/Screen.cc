/* 
 * Screen.cc -- ePiX2::Screen class and classes for importing
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 04, 2006
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

#include <iostream>
#include <list>
#include <algorithm>

#include "Pair.h"
#include "Color.h"
#include "Tile.h"
#include "Screen.h"

namespace ePiX2 {

  typedef std::list<Tile>::const_iterator Tp;

  Screen::Screen(const Pair arg1, const Pair arg2) :
    m_border(Rect(arg1, arg2)) { }

  Screen::Screen(const Rect& bbox) : m_border(bbox) { }

  // Should be unnecessary; each "add_tile" call imports colors
  void Screen::import_pallet(const Screen& child)
  {
    std::set<Color>::const_iterator s;
    for(s=child.m_pallet.begin(); s!=child.m_pallet.end(); ++s)
      m_pallet.insert(*s);
  }

  // superimpose a layer by affine scaling
  void Screen::paste(const Screen& child)
  {
    if (this != &child)
      {
	Pair_Map affine=Affine(m_border, child.m_border);

	// import tiles
	for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	  add_tile((*p)->map_by(affine));

	//	import_pallet(child);
      }
  }

  // map child to inset
  void Screen::paste(const Screen& child, const Rect& inset)
  {
    if (this != &child)
      {
	Pair_Map affine=Affine(inset, child.m_border);

	for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	  add_tile((*p)->map_by(affine));
      }
  }

  // map child.cut to inset
  void Screen::paste(const Screen& child, const Rect& cut, const Rect& inset)
  {
    if (this != &child)
      {
	Pair_Map affine=Affine(inset, cut);

	for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	  {
	    Tile temp((*p)->crop_to(cut));
	    if (temp.operator->() != 0) // vertices survived cropping
	      add_tile(temp->map_by(affine));
	  }
      }
  }


  // or by a specified Pair_Map
  void Screen::paste(const Screen& child, const Pair_Map& f)
  {
    if (this != &child)
      for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	add_tile((*p)->map_by(f));
  }

  void Screen::paste(const Screen& child, const Rect& cut, const Pair_Map& f)
  {
    if (this != &child)
      for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	add_tile(((*p)->map_by(f))->crop_to(cut));
  }

  void Screen::clear(void)
  {
    m_tiles.clear();
  }

  void Screen::crop(void)
  {
    std::list<Tile> visibles;

    for (Tp curr=m_tiles.begin(); curr!=m_tiles.end(); ++curr)
      {
	Tile crop((*curr)->crop_to(m_border));

	if (crop.operator->() != 0)
	  visibles.push_back(crop);
      }

    //    m_tiles.clear();
    m_tiles = visibles;
  }

  void Screen::add_color(const Color& col)
  {
    m_pallet.insert(col);
  }

  void Screen::add_tile(const Tile& T)
  {
    m_tiles.push_back(T);
    T->add_colors_to(*this);
  }

  Screen& Screen::operator<< (const Tile& T) 
  { 
    add_tile(T);
    return (*this);
  }

} /* end of namespace */
