/*** 
 *** Screen.h -- ePiX2::Screen class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 19, 2005
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
 *** This file provides the:
 ***   - Screen class, a maskable window
 ***/

#ifndef EPIX2_SCREEN
#define EPIX2_SCREEN

#include <list>

#include "Functions.h"
#include "Pair.h"
#include "Enums.h"
#include "Tile.h"

namespace ePiX2 {

  /* * * Screen.h * * */

  class Screen {

  public:

    static Screen the_page;

    //    Screen(void);
    Screen(const Pair& sw=Pair(0,0), const Pair& ne=Pair(0,0));
    void set_corners(const Pair&, const Pair&);

    void set_shape(const epix2_screen_shape SHAPE) { shape = SHAPE; }
    void set_radius(const double);

    bool is_onscreen(const Pair);
    bool is_offscreen(const Pair arg) { return !is_onscreen(arg); }

    bool is_masked(const Pair arg);

    // import to our viewfinder...
    void import(Screen&  child);
    // ...or specified rectangle
    void import(Screen&  child, const Pair&, const Pair&);

    // set page size and offset
    friend void picture(const double, const double);
    friend void picture(const Pair&);

    friend void offset(const double, const double);
    friend void offset(const Pair&);

    void print(void); // to output
    void clear(void); // blank screen

    void crop(void);
    void nocrop(void);


    double h_min(void) const;
    double v_min(void) const;

    double h_max(void) const;
    double v_max(void) const;

    double h_size(void) const;
    double v_size(void) const;

    // alignment points
    Pair tr(void) const;
    Pair tl(void) const;
    Pair bl(void) const;
    Pair br(void) const;

    Pair  t(void) const;
    Pair  b(void) const;
    Pair  l(void) const;
    Pair  r(void) const;
    Pair  c(void) const;

    int tile_count(void) { return tiles.size(); }

    // actual data; ordering stored privately in tiles
    std::list<Glyph> glyphs;
    std::list<Outline> outlines;
    std::list<Region> regions;
    void add_tile_ptr(Tile*);

  private:

    Pair low_left;
    Pair top_right;

    epix2_screen_shape shape;
    double corner_rad; // radius of rounded corners
    bool cropping;

    std::list<Tile*> tiles;

    // N.B. mid vs. min
    double h_mid(void) const;
    double v_mid(void) const;

  }; // end of Screen class

} /* end of namespace */

#endif /* EPIX2_SCREEN */
