/*** 
 ***  Picture.h -- epix2::Picture class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 07, 2005
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
 ***  Picture class
 ***/

#ifndef EPIX2_PICTURE
#define EPIX2_PICTURE

#include <list>

#include "Enums.h"
#include "Camera.h"
#include "Screen.h"

namespace ePiX2 {
  class Pair;   /***/
  class Point;  /***/
  class Object_Base; /***/
  class Shard;  /***/
  class Tile;   /***/
  class Layer;  /***/

  /* * * Picture.h * * */

  class Picture
    {
      //      friend class Shard;
    public:

      Picture(void);
      //      Picture(const Picture&); // copy shards
      ~Picture(void); // delete shards

      // modify scenery
      Picture& operator<< (Object_Base&);
      //      void add(Object_Base*);
      void empty(void); // clear all Object_Bases

      void clip(void);
      void noclip(void);


      // import world's screen to specified rectangle
      void import(Picture& world, const Pair& arg1, const Pair& arg2);

      // map scenery to screen
      void photo(epix2_shade_type shaded=SHADE_SOLID); // face-normal shading
      void flash(void) { photo(SHADE_FLAT); } // flat shading
      void x_ray(void) { photo(SHADE_NONE); } // edges only, no hiding

      void print(void); // map screen to page
      void draw(Tile*); // draw directly in screen
      void clear(void); // blank the screen

      // (un)set flag; action taken only at develop
      void crop(void);    
      void nocrop(void);


      // access screen
      void bounding_box(const Pair, const Pair);

      // access camera
      void lens(epix2_lens_type);

      void view_from(const Point&);
      void view_from(const double, const double, const double);
      Point viewpt(void) const;

      void look_at(const Point&);
      void look_at(const double, const double, const double);
      void range(const double);

      void pan(const double);  // side to side
      void tilt(const double); // up and down
      void roll(const double);

      //      void add_shard(Shard frag);

      // map our screen's bounding box to specified page rectangle
      friend void paste(Picture&, const Pair&, const Pair&);  

    private:

      bool clipping;

      Camera camera;
      Screen screen;

      std::list<Object_Base*> scenery;

      Layer* shards; // binary space partition tree root

      //      void merge(Layer*);

      void remove_backfaces(Object_Base*);
      void set_shard_distance(Object_Base*); // compute min/max dist to viewer

      void clear_shard_tree(void);
      void build_shard_tree(void);

      void map_to_tile(const Shard& frag, epix2_shade_type shaded);

      void draw_tree(const Layer&, epix2_shade_type shaded=SHADE_SOLID);

      // helpers for draw_tree
      void draw_back (const Layer&, epix2_shade_type shaded=SHADE_SOLID);
      void draw_front(const Layer&, epix2_shade_type shaded=SHADE_SOLID);
      void draw_this (const Layer&, epix2_shade_type shaded=SHADE_SOLID);


      // Appearance attributes -- to be filled in
      //	   double    line_width;
      //	   line_type line_style;

      //	   Bkg_Type background; (solid, gradient, image...)
      //	   std::string font_size;
      //	   std::string font_family;	   
      //	   std::string caption;

      //	   Color font_color;
      //	   Color line_color;
      //	   Color fill_color;

    }; // end of class Picture

} /* end of namespace */

#endif /* EPIX2_PICTURE */
