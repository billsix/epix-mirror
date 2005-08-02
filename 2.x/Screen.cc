/* 
 * Screen.cc -- ePiX2::Screen class and classes for importing
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 31, 2005
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
#include <list>
#include <algorithm>

#include "Pair.h"
#include "Color.h"
#include "Tile.h"
#include "Picture.h"
#include "Screen.h"

#include "Output.h"

namespace ePiX2 {

  Screen::Screen(const Pair& arg1, const Pair& arg2) : corner_rad(0)
  {
    low_left =Pair(min(arg1.x1(), arg2.x1()), min(arg1.x2(), arg2.x2()));
    top_right=Pair(max(arg1.x1(), arg2.x1()), max(arg1.x2(), arg2.x2()));

    shape = MASK_RECT;
    cropping=false;
  }

  void Screen::crop(void)
  {
    cropping=true;
  }
  void Screen::nocrop(void)
  {
    cropping=false;
  }

  Screen Screen::the_page=Screen();

  // re-set corners
  void Screen::set_corners(const Pair& arg1, const Pair& arg2)
  {
    low_left =Pair(min(arg1.x1(), arg2.x1()), min(arg1.x2(), arg2.x2()));
    top_right=Pair(max(arg1.x1(), arg2.x1()), max(arg1.x2(), arg2.x2()));
  }    

  // set corner radius for rounded rectangular border
  void Screen::set_radius(const double rad)
  {
    corner_rad = rad;

    if ( rad < 0) // too small
      corner_rad=0; 

    else // if too large
      {
	double temp =  0.5*min(h_size(), v_size());
	if (temp < rad)
	  corner_rad = temp;
      }
  }

  // inside bounding box test
  bool Screen::is_onscreen(const Pair arg)
  {
    const double x = arg.x1();
    const double y = arg.x2();
    return ((h_min() <= x) && (x <= h_max()) && 
	    (v_min() <= y) && (y <= v_max()));
  }

  // N.B. negated return statements
  bool Screen::is_masked(Pair arg)
  {
    const double x = arg.x1();
    const double y = arg.x2();
    const double width  = h_size();
    const double height = v_size();
    const double radius = corner_rad;

    Pair P1 = r(); // midpoints in ccw order from right
    Pair P2 = t();
    Pair P3 = l();
    Pair P4 = b();

    switch(shape) {
    case MASK_RECT:
      if (radius == 0) // no need to handle rounded corners
	return !is_onscreen(arg); // i.e., is_offscreen(arg)

      else
	{
	  return !(// not in union of sets:
		   // rectangle with left, right sides shaved off
		   ((h_min()+radius <= x) && (x <= h_max()-radius) && 
		    (v_min() <= y) && (y <= v_max()))
		   || // top right (quarter) circle
		   (norm(arg - Pair(h_max()-radius, v_max()-radius)) < radius)
		   || // top left (quarter) circle
		   (norm(arg - Pair(h_min()+radius, v_max()-radius)) < radius)
		   || // bottom right (quarter) circle
		   (norm(arg - Pair(h_min()+radius, v_min()+radius)) < radius)
		   || // bottom left (quarter) circle
		   (norm(arg - Pair(h_max()-radius, v_min()+radius)) < radius)
		   || // rectangle with top, bottom shaved off
		   ((h_min() <= x) && (x <= h_max()) && 
		    (v_min()+radius <= y) && (y <= v_max()-radius))
		   );
	} // end of case RECT
      break;

    case MASK_ELLIPSE:
      if (width == height) // circular mask of radius width/2
	return !(norm(arg - c()) <= 0.5*width);

      else {
	// half-separation of foci
	double focus  = 0.5*sqrt(fabs(width*width - height*height));

	Pair focus1, focus2;
	if (width > height)
	  {
	    focus1 = c() + Pair(focus,0);
	    focus2 = c() - Pair(focus,0);
	  }
	else
	  {
	    focus1 = c() + Pair(0,focus);
	    focus2 = c() - Pair(0,focus);
	  }

	// sum of distances to foci > twice the major axis
	return !(norm(arg-focus1) + norm(arg-focus2) <= max(width,height));
      } // end case ELLIPSE
      break;

    case MASK_DIAMOND:
      return !(((J(P2-P1) | (arg-P1)) >= 0) &&
	       ((J(P3-P2) | (arg-P2)) >= 0) &&
	       ((J(P4-P3) | (arg-P3)) >= 0) &&
	       ((J(P1-P4) | (arg-P4)) >= 0));
      break;

    default: // unrounded rectangle
      return !is_onscreen(arg);
      break;
    } // end of switch(shape)
  } // end of is_masked


  void Screen::add_tile_ptr(Tile* element) 
  { 
    tiles.push_back(element); 
  }

  void Screen::import(Screen& child)
  {
    if (&child == &Screen::the_page)
      ; // do nothing

    else // import to page
      {
	Affine_Map affine(*this, child);
	std::list<Tile*>::iterator p;

	for(p=child.tiles.begin(); p!= child.tiles.end(); ++p)
	  (*p)->map_to(*this, affine);
      }
  }


  void Screen::import(Screen& child, const Pair& arg1, const Pair& arg2)
  {
    Screen temp(arg1, arg2);
    Affine_Map affine(temp, child);
    std::list<Tile*>::iterator p;

    for(p=child.tiles.begin(); p!= child.tiles.end(); ++p)
      (*p)->map_to(*this, affine);
  }


  void Screen::clear(void) 
  { 
    glyphs.clear(); 
    outlines.clear(); 
    regions.clear(); 

    tiles.clear(); 
  }

  // set page size
  void picture(const double h_arg, const double v_arg)
  {
    Screen::the_page.set_corners(Pair(0,0), Pair(h_arg, v_arg));
  }

  void picture(const Pair& arg)
  {
    Screen::the_page.set_corners(Pair(0,0), arg);
  }

  void offset(const double h_off, const double v_off)
  {
    Pair off=Pair(h_off, v_off);
    Screen::the_page.low_left -= off;
    Screen::the_page.top_right -= off;
  }

  void offset(const Pair& off)
  {
    Screen::the_page.low_left -= off;
    Screen::the_page.top_right -= off;
  }

  // print contents to output stream
  // TO DO: specify output stream/file
  void Screen::print(void)
  {
    if (this!=&Screen::the_page)
      Screen::the_page.import(*this);

    // skeletal picture header
    std::cout << "%%  ePiX-2.0pre";
    comment("Cartesian bounding box: ");

    std::cout << "["
	      << h_min() << "," << h_max() << "] x ["
	      << v_min() << "," << v_max() << "]";

    print_line("\\setlength{\\unitlength}{1in}");
    print_line("\\begin{picture}");

    raw_print(Pair(Screen::the_page.h_size(),Screen::the_page.v_size()));
    raw_print(Screen::the_page.bl());

    Color::print_pallet();

    // TO DO -- handle user-specified parameters; N.B. Magic number 0.6
    print_line("\\psset{unit=1in,linewidth=0.6pt,linearc=0.2pt}");

    for_each(Screen::the_page.tiles.begin(), 
	     Screen::the_page.tiles.end(), 
	     std::mem_fun(&Tile::print));

    print_line("\\end{picture}\n");

  } // end of print

  // corners and midpoints
  // N.B. min vs. mid
  double Screen::h_min() const { return low_left.x1(); }
  double Screen::v_min() const { return low_left.x2(); }

  double Screen::h_max() const { return top_right.x1(); }
  double Screen::v_max() const { return top_right.x2(); }

  // N.B. mid vs. min
  double Screen::h_mid() const { return 0.5*(top_right.x1()+low_left.x1()); }
  double Screen::v_mid() const { return 0.5*(top_right.x2()+low_left.x2()); }

  double Screen::h_size() const { return top_right.x1()-low_left.x1(); }
  double Screen::v_size() const { return top_right.x2()-low_left.x2(); }

  // alignment points
  Pair Screen::tr() const { return Pair(h_max(), v_max()); }
  Pair Screen::tl() const { return Pair(h_min(), v_max()); }
  Pair Screen::bl() const { return Pair(h_min(), v_min()); }
  Pair Screen::br() const { return Pair(h_max(), v_min()); }

  Pair Screen::t() const { return Pair(h_mid(), v_max()); }
  Pair Screen::b() const { return Pair(h_mid(), v_min()); }
  Pair Screen::l() const { return Pair(h_min(), v_mid()); }
  Pair Screen::r() const { return Pair(h_max(), v_mid()); }
  Pair Screen::c() const { return Pair(h_mid(), v_mid()); }

} /* end of namespace */
