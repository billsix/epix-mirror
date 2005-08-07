/*** 
 ***  Hiding.h -- ePiX2 internal classes for hidden object removal
 ***
 ***  When a world Object is drawn in the screen, it is broken into 
 ***  "Shards", which are assembled into a binary space partition
 ***  tree. Then, depending on the location of the viewer, the tree
 ***  is traversed, and each node's data is projected to a Tile.
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 06, 2005
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
 ***   Shard - A *convex* planar polygon or polyline and draw functions
 ***   Layer - Binary space partition tree node and algorithms
 ***
 ***          .        
 ***    Front .  Back
 ***          .        
 ***   *------*- - - -o
 ***   |xxxxxx:       
 ***   |xxxxxx:       |
 ***   |xxxxxx:       
 ***   |xxxxxx:       |
 ***   |xxxxxx:       
 ***   *------*- - - -o
 ***          .
 ***          .        
 ***          .
 ***
 ***   . . . Cutting plane
 ***   ----- Visible edge
 ***   - - - Edge of back portion
 ***     :   Cut edge (undrawn)
 ***     *   Visible vertex
 ***     o   Vertex of "back" portion
 ***     x   Interior of front portion
 *** 
 ***   A Shard provides functions for adding a Shard to the existing tree,
 ***   for orienting the tree according to a specified viewpoint, and
 ***   for drawing the entire tree in the following styles:
 ***
 ***   - render:      Interiors shaded according to lighting
 ***   - render_flat: Interiors white
 ***   - x_ray:       All edges of equal line width
 ***/


#ifndef EPIX2_HIDING
#define EPIX2_HIDING

#include <list>

#include "Enums.h"
#include "Color.h"
#include "Point.h"
#include "Vector.h"

namespace ePiX2 {

  /* * * Hiding.h * * */

  class Shard {

    friend class Layer;
    friend class Picture;

  public:

    Shard(void)
      {
	fill_color=Color(1,1,1,rgb); // white
	line_color=Color(0,0,0,rgb); // black
      }

    // build, modify; used by Object::shatter() and Layer functions
    void add_edge(Edge E) { boundary.push_back(E); }
    void set_normal(const Vector& N) { normal=N; }
    void set_solid(bool arg) { solid=arg; }

    void set_line_color(const Color& col) { line_color=col; }
    void set_fill_color(const Color& col) { fill_color=col; }

    Shard clip_by(const Vector& knife) const;

    bool operator< (const Shard& arg);

  private:

    // Data members

    // Object::shatter() must guarantee outward-oriented faces for
    // convex bodies, and that Edges of each face form a convex loop
    std::list<Edge> boundary;

    Vector            normal; // for hiding, shading

    double min_distance, max_distance; // viewer-dependent

    // fill data:
    bool  solid;
    Color fill_color;
    Color line_color;
    // boundary width, style;
    // if solid, interior color/shading

  }; // end of class Shard


  class Layer {

    friend class Picture;

  public:

    Layer(const Vector& sep=Vector(Origin));

    void insert(const Shard&);
    void insert(std::list<Shard>);

  private:

    Vector           separator; // cutting plane
    std::list<Shard> locals;

    Layer* front;
    Layer* back;

    // clip frag to plane with oriented normal knife, place in node
    //void clip_insert(Layer* node, Shard& frag, const Vector& knife);

    void clear(void);

  }; // end of Layer


  /*
  class Speck : public Shard {

  public:
    Speck(void) { }    
    Speck(const Point v1, epix2_mark_type t) : vtx1(v1) { TYPE=t; }

  private:

    epix2_mark_type TYPE;

  }; // end of class Speck

  class Label : public Shard, public Object {

  public:

    // initialize
    Label(const Point& arg, const Pair& offset, std::string msg,
	  epix2_align_type ALIGN)
      : here(arg) { TYPE=t; }

    Glyph* scale_by(const Affine_Map& map);

    void project(void);

  private:

    Point here;
    Pair true_offset; // in pt
    std::string text;
    epix2_align_type TYPE;

  }; // end of class Label
  */

} /* end of namespace */

#endif /* EPIX2_HIDING */
