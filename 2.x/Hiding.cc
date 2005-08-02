/* 
 * Hiding.cc -- ePiX2 classes (Shard, Layer) for hiding
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 01, 2005
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

#include <list>
#include <vector>

#include "Enums.h"
#include "Constants.h"
#include "Functions.h"

#include "Point.h"
#include "Vector.h"

#include "Edge.h"

#include "Color.h"
#include "Hiding.h"

namespace ePiX2 {

  Shard Shard::clip_by(const Vector& knife) const
  {
    const double eps=EPIX2_EPSILON; // half-thickness of cutting plane
    const Point on_knife = knife.tail();

    // frag convex => result of clipping is connected
    Shard clipped;

    clipped.set_normal(normal);

    std::list<Edge>::const_iterator curr ; // current edge
    std::vector<Point> cut_ends;

    // Examine heights of edges serially, check for crossings.
    // For clipping, a "crossing" occurs when max height > eps and 
    // min height < eps.
    double first_ht, second_ht, max_ht, min_ht;

    for (curr=boundary.begin(); curr!=boundary.end(); ++curr)
      {
	first_ht  = (knife|((*curr).first-on_knife));
	second_ht = (knife|((*curr).second-on_knife));

	max_ht=max(first_ht, second_ht);
	min_ht=min(first_ht, second_ht);

	if (max_ht > eps) // part of edge is in front
	  {
	    if (min_ht > eps) // no crossing
	      clipped.add_edge(*curr); // Shard::add_edge

	    else // at least one is behind
	      {
		Point cut=knife.perp_hits(*curr);
		cut_ends.push_back(cut);

		Edge E; // unclipped portion

		if (first_ht > eps) // crossing from the front
		  {
		    E = Edge((*curr).first, cut, 
			     (*curr).line_color, (*curr).drawn);
		    clipped.add_edge(E);

		    if (cut_ends.size() == 2) // found second crossing
		      {
			Edge the_cut; // undrawn boundary component
			the_cut = Edge(cut_ends.at(1), cut_ends.at(0), 
				       fill_color, true);

			clipped.add_edge(the_cut);
		      }
		  }

		else // crossing from the back
		  {
		    E = Edge(cut, (*curr).second, 
			     (*curr).line_color, (*curr).drawn);
		    
		    if (cut_ends.size() == 2) // found second crossing
		      {
			Edge the_cut;
			the_cut = Edge(cut_ends.at(0), cut_ends.at(1),
				       fill_color, true);

			clipped.add_edge(the_cut);
		      }

		    clipped.add_edge(E);
		  }
	      } // end of crossing
	  }  // end of partially in front

	else if (-eps < min_ht && max_ht < eps) // coplanar
	  clipped.add_edge(*curr);

	else // edge is behind, no action
	  ;
      } // all edges examined 

    clipped.set_solid(solid);
    clipped.set_line_color(line_color);
    clipped.set_fill_color(fill_color);

    return clipped;
  } // end of clip_by


  //// Layer's functions

  Layer::Layer(const Vector& N)
  {
    separator=N; // defaults to zero, initialized when first Shard is added

    front = 0;
    back  = 0;
  }


  // insert one Shard
  void Layer::insert(const Shard& frag)
  {
    if (frag.boundary.size() == 0)
      return;

    // else...
    if (norm(separator) < EPIX2_EPSILON)
      {
	separator = frag.normal;
	locals.push_back(frag);
	return;
      }

    // else...
    const double eps=EPIX2_EPSILON; // half-thickness of cutting plane
    const Point O=separator.tail();

    std::list<Edge>::const_iterator curr;
    double ht_curr, ht_min, ht_max;

    // compute vertices' heights w.r.t. separator
    for (curr=frag.boundary.begin(); curr!=frag.boundary.end(); ++curr)
      {
	// check first vertex manually
	if (curr == frag.boundary.begin())
	  {
	    ht_curr = ((((*curr).first) - O)|separator);
	    ht_max = max(0, ht_curr);
	    ht_min = min(0, ht_curr);
	  }

	ht_curr = ((((*curr).second) - O)|separator);

	ht_max = max(ht_max, ht_curr);
	ht_min = min(ht_min, ht_curr);
      }

    if (ht_min > -eps)
      {
	if (ht_max > eps) // in front wrt separator
	  {
	    if (front == 0)
	      {
		front  = new Layer;
		(*front) = Layer(frag.normal);
		front->locals.push_back(frag);
		return;
	      }

	    // else
	    front->insert(frag);
	    return;
	  }

	else // -eps < ht_min < ht_max < eps
	  {
	    locals.push_back(frag);
	    return;
	  }
      }

    else if (ht_max < eps) // ht_min <= -eps
      {
	if (back == 0)
	  {
	    back  = new Layer;
	    (*back) = Layer(frag.normal);
	    back->locals.push_back(frag);
	    return;
	  }

	// else
	back->insert(frag);
	return;
      }

    else // we cross
      {
	if (back == 0)
	  {
	    back  = new Layer;
	    *back = Layer(frag.normal);
	    back->locals.push_back(frag.clip_by(-separator));
	  }

	else
	  back->insert(frag.clip_by(-separator));

	if (front == 0)
	  {
	    front  = new Layer;
	    *front = Layer(frag.normal);
	    front->locals.push_back(frag.clip_by(separator));
	  }

	else
	  front->insert(frag.clip_by(separator));
      }

  } // end of insert(Layer*, Shard&)


  // add Shard(s) to tree
  void Layer::insert(std::list<Shard> fragments)
  {
    std::list<Shard>::iterator curr;

    for (curr=fragments.begin(); curr!=fragments.end(); ++curr)
      insert(*curr);
  }


  void Layer::clear(void)
  {
    if (back != 0)
      back->clear();

    if (front != 0)
      front->clear();

    locals.clear();
    delete back;
    delete front;
  }


} /* end of namespace */
