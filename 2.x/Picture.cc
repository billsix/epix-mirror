/* 
 *  Picture.cc -- epix2::Picture class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 06, 2005
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

/*
 * This file provides:
 *
 *  Picture class
 */

#include <iostream>
#include <list>
#include <functional>

#include "Enums.h"
#include "Camera.h"
#include "Tile.h"
#include "Screen.h"
#include "Hiding.h"
#include "Object.h"
#include "Output.h"
#include "Picture.h"

namespace ePiX2 {

  Picture::Picture(void)
  {
    camera=Camera();
    screen=Screen();

    shards = new Layer;
  }

  Picture::~Picture(void)
  {
    delete shards;
  }

  // modify scenery
  Picture& Picture::operator<< (Object_Base& obj)
  {
    //    obj.shatter(); unnec just yet; first remove fully hidden Objects
    scenery.push_back(&obj);
    return *this;
  }

  //  void Picture::add(Object_Base* obj) { scenery.push_back(obj); }

  void Picture::empty(void)
  {
    scenery.clear();
  }

  /* Obsolete
  void Picture::add_shard(Shard frag) 
  { 
    shards->insert(frag);
  }
  */

  void Picture::clear_shard_tree(void)
  {
    shards->clear();
  }

  // import our screen to specified page rectangle
  void paste(Picture& world, const Pair& arg1, const Pair& arg2)
  {
    Screen::the_page.import(world.screen, arg1, arg2);
  }


  // get visual output; general steps:
  // 1. build_shard_tree
  // 2. project requested parts of picture to screen
  // 3. print screen to page
 
  // map scenery to screen
  void Picture::photo(epix2_shade_type shaded)
  {
    build_shard_tree(); // shatter everything and build tree

    // draw in screen
    draw_tree(*shards, shaded);
    //    shards->face_draw(*this, shaded);
    //    shards->edge_draw(*this);
    //    shards->label_draw(*this); TO DO
  }


  // map screen to page
  void Picture::print(void)
  {
    Screen::the_page.import(this->screen);
    Screen::the_page.print();
  }


  // draw directly in screen
  void Picture::draw(Tile* tile) 
  { 
    Affine_Map ident(screen, screen);
    tile->map_to(screen, ident); 
  }

  // blank the screen
  void Picture::clear(void)  
  { 
    screen.clear();
  }

  void Picture::crop(void)   { screen.crop(); }
  void Picture::nocrop(void) { screen.nocrop(); }


  void Picture::clip(void)   { clipping=true; }
  void Picture::noclip(void) { clipping=false; }

  // set screen
  void Picture::bounding_box(const Pair sw, const Pair ne)
  {
    screen.set_corners(sw,ne);
  }

  void Picture::lens(epix2_lens_type L)
  {
    camera.set_lens(L);
  }

  // modify camera
  void Picture::view_from(const Point& arg) 
  { 
    camera.at(arg);
  }
  void Picture::view_from(const double a1, const double a2, const double a3)
  {
    Point vpt(a1,a2,a3);
    camera.at(vpt);
  }

  Point Picture::viewpt(void) const
  {
    return camera.viewpt();
  }

  void Picture::look_at(const Point& arg) { camera.look_at(arg); }
  void Picture::look_at(const double a1, const double a2, const double a3)
  {
    camera.look_at(Point(a1, a2, a3));
  }

  void Picture::range(const double d) { camera.range(d); }

  void Picture::pan(const double th)  { camera.rotate_sky(th); }
  void Picture::tilt(const double th) { camera.rotate_sea(th); }
  void Picture::roll(const double th) { camera.rotate_eye(th); }

  /*
  // merge tree front to back into shards
  void Picture::merge(Layer* tree) 
  {
    double ht_vpt = ((tree->separator)|(viewpt() - (tree->separator.tail())));

    if (ht_vpt >= -EPIX2_EPSILON)
      {
	if (tree->front != 0)
	  merge(tree->front);

	  shards->insert(tree->locals);

	if (tree->back != 0)
	  merge(tree->back);
      }

    else // if (ht_vpt < -EPIX2_EPSILON)
      {
	if (tree->back != 0)
	  merge(tree->back);

	if (tree->locals.size() > 0)
	  shards->insert(tree->locals);

	if (tree->front != 0)
	  merge(tree->front);
      }
  }
  */

  // shard mangling
  void Picture::set_shard_distance(Object_Base* obj)
  {
    std::list<Shard>::iterator frag_ptr;  // current shard
    std::list<Edge>::const_iterator curr; // current edge

    for (frag_ptr  = obj->fragments.begin(); 
	 frag_ptr != obj->fragments.end(); ++frag_ptr)
      {
	Point curr_vertex;
	double curr_dist;
	double max_dist, min_dist; // initialize on first edge

	for (curr  = (*frag_ptr).boundary.begin(); 
	     curr != (*frag_ptr).boundary.end(); ++curr)
	  {
	    if (curr == (*frag_ptr).boundary.begin())
	      {
		curr_vertex = (*curr).first;
		curr_dist = norm(curr_vertex - camera.viewpt());
		max_dist=min_dist=curr_dist;
	      }

	    curr_vertex = (*curr).second;
	    curr_dist = norm(curr_vertex - camera.viewpt());

	    max_dist=max(curr_dist, max_dist);
	    min_dist=min(curr_dist, min_dist);
	  }

	// all edges examined, set distances
	(*frag_ptr).max_distance = max_dist;
	(*frag_ptr).min_distance = min_dist;
      } // all shards examined

  } // end of set_shard_distance


  void Picture::remove_backfaces(Object_Base* obj)
  {
    if (obj->closed_oriented) // else do nothing
      {
	std::list<Shard>::iterator frag_ptr = obj->fragments.begin();
	Vector temp_N;

	while (frag_ptr != obj->fragments.end())
	  {
	    temp_N = (*frag_ptr).normal;

	    if ((temp_N|(camera.viewpt()-temp_N.tail())) < EPIX2_EPSILON)
	      frag_ptr = obj->fragments.erase(frag_ptr);

	    else
	      ++frag_ptr;
	  }
      }
  } // end of remove_backfaces()

  // private functions to turn all our Object_Bases into lists of Shards
  void Picture::build_shard_tree(void)
  {
    std::list<Shard> all_fragments;

    std::list<Object_Base*>::iterator obj;

    //    for (obj=scenery.begin(); obj!=scenery.end(); ++obj)
    //      set_distance(*obj); // distance from viewpt to object

    //    sort(scenery.begin(), scenery.end(), by_distance);

    for (obj=scenery.begin(); obj!=scenery.end(); ++obj)
      {
	(*obj)->shatter();       // break *obj into shards
	remove_backfaces(*obj);  // remove obvious invisibles
	set_shard_distance(*obj);// compute min/max dist to viewer

	all_fragments.merge((*obj)->fragments); // global temporary shard list
      }

    shards->insert(all_fragments);
  }

  // map a single Shard to the screen; TO DO -- make outline an unsolid region
  void Picture::map_to_tile(const Shard& frag, epix2_shade_type shaded)
  {
    Silhouette shadow;
    std::list<Edge>::const_iterator curr;

    for(curr=frag.boundary.begin(); curr!=frag.boundary.end(); ++curr)
      shadow.add_edge(camera(*curr)); // Tile::add_edge

    if (shaded == SHADE_NONE)
      shadow.set_solid(false);

    else
      shadow.set_solid(frag.solid);

    if (frag.solid && (shaded != SHADE_NONE)) // set fillcolor
      {
	// TO DO: Handle back coloring
	Vector view_dir=camera.viewpt() - frag.normal.tail();
	double c=(frag.normal|view_dir)/(norm(frag.normal)*norm(view_dir));
	double dens=1;

	// Magic formula: (3+cos^2(theta))/4 interpolates color 
	// densities between 0.75 and 1, discards face orientation
	if (shaded == SHADE_SOLID)
	  dens = 0.25*(3+c*c);

	shadow.set_fill_color(frag.fill_color*dens);
      }

    screen.silhouettes.push_back(shadow);

    // get address of element just pushed
    std::list<Silhouette>::iterator last = --screen.silhouettes.end();
    screen.add_tile_ptr(&(*last));

  } // end of map_to_tile()


  // private functions to draw portions of the tree
  void Picture::draw_back(const Layer& tree, epix2_shade_type shaded)
  {
    if ( tree.back != 0)
      draw_tree(*(tree.back), shaded);
  }

  void Picture::draw_front(const Layer& tree, epix2_shade_type shaded)
  {
    if ( tree.front != 0)
      draw_tree(*(tree.front), shaded);
  }

  void Picture::draw_this(const Layer& tree, epix2_shade_type shaded)
  {
    std::list<Shard>::const_iterator p;

    for (p=tree.locals.begin(); p!=tree.locals.end(); ++p)
      map_to_tile(*p, shaded);
  }


  // shaded faces
  void Picture::draw_tree(const Layer& tree, epix2_shade_type shaded)
  {
    double ht_vpt = (tree.separator|(viewpt() - (tree.separator.tail())));

    if (ht_vpt > EPIX2_EPSILON)
      {
	draw_back (tree, shaded);
	draw_this (tree, shaded);
	draw_front(tree, shaded);
      }

    else if (ht_vpt < -EPIX2_EPSILON)
      {
	draw_front(tree, shaded);
	draw_this (tree, shaded);
	draw_back (tree, shaded);
      }

    else // viewer is edge-on
      {
	draw_back (tree, shaded);
	draw_front(tree, shaded);
      }
  }

} // end of namespace
