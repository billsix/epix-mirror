/* 
 * crop_algorithms.h -- clipping/cropping templates
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.9
 * Last Change: July 31, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * This file defines list<edge> manipulating template functions and
 * associated utilities. Each chop function returns a reference to its
 * list<edge> argument.
 *
 * double height(perp, base, arg)
 * PT cut_location(perp, base, tail, head)
 * cull(edges)     // remove edges whose tail and head are equal
 * loopify(edges)  // replace contiguous unseen edges with one seen edge
 * chop_path(perp, base, edges) // cull to a halfspace/halfplane
 * chop_loop(perp, base, edges) // same, for loops
 */

#ifndef EPIX_CROP_ALGO
#define EPIX_CROP_ALGO

#include <list>

#include "constants.h"

namespace ePiX {

  // perp assumed to be *unit*
  template <typename PT>
    double height(const PT& perp, const PT& base, const PT& arg)
    {
      //      return (perp|(operator-(arg, base)));
      return (perp|(arg - base));
    }

  // no check for parallel
  template <typename PT>
    PT cut_location(const PT& perp, const PT& base, const PT& p1, const PT& p2)
    {
      //      PT dir(operator-(p2, p1)); // -dir to base
      //      return p1 + (((operator-(base, p1))|perp)/(dir|perp))*dir;
      PT dir(p2 - p1); // -dir to base
      return p1 + (((base - p1)|perp)/(dir|perp))*dir;
    }

  // remove edges whose tail, head are equal
  template <typename PT>
    std::list<edge_data<PT> >& cull(std::list<edge_data<PT> >& edges)
    {
      typename std::list<edge_data<PT> >::iterator ep(edges.begin());

      while (ep != edges.end())
	{
	  if ((*ep).is_null())
	    ep = edges.erase(ep);

	  else
	    ++ep;
	}

      return edges;
    }

  template <typename PT>
    std::list<edge_data<PT> >& loopify(std::list<edge_data<PT> >& edges)
    {
      if (edges.begin() == edges.end())
	return edges;

      typename std::list<edge_data<PT> >::iterator ep(edges.begin());
      bool in_path(false); // currently in a visible sublist?

      // After first pass, ep points to first visible edge
      while (ep != edges.end())
	{
	  if (!(*ep).is_seen()) // invisible edge
	    {
	      ep=edges.erase(ep);
	      in_path=false;
	      continue;
	    }

	  // else visible edge
	  if (in_path)
	    {
	      ++ep;
	      continue;
	    }

	  // else
	  in_path=true;
	  if (ep != edges.begin()) // join our tail to previous head
	    {
	      typename std::list<edge_data<PT> >::iterator prev(ep);
	      --prev;
	      ep = edges.insert(ep, edge_data<PT>((*prev).head(),
						  (*ep).tail(),
						  true));
	      ++ep;
	      ++ep;
	    }
	}

      // all edges examined; if we contain multiple edges, close final cut
      ep = edges.begin();
      // contains at most one edge
      if (ep == edges.end())
	; // return below

      else if (++ep == edges.end())
	edges.erase(edges.begin());

      else
        {
          ep = edges.end();
	  --ep;
          edges.push_back(edge_data<PT>((*ep).head(),
					(*edges.begin()).tail(),
                                        true));
        }

      return edges;
    } // end of loopify()


  // return result of clipping edges to half space/plane through base
  // with unit inward normal perp
  template <typename PT>
    std::list<edge_data<PT> >& chop_path(const PT& perp, const PT& base,
					 std::list<edge_data<PT> >& edges)
    {
      typename std::list<edge_data<PT> >::iterator ep(edges.begin());

      while(ep != edges.end())
	{
	  if (!(*ep).is_seen())
	    {
	      ++ep;
	      continue;
	    }

	  // else
	  const PT tail((*ep).tail());
	  const PT head((*ep).head());

	  const double ht_tail(height(perp, base, tail));
	  const double ht_head(height(perp, base, head));

	  // non-crossings:
	  // unclipped
	  if (-EPIX_EPSILON < ht_tail && -EPIX_EPSILON < ht_head)
	    {
	      ++ep;
	      continue;
	    }

	  // clipped
	  else if (ht_tail <= 0 && ht_head <= 0)
	    {
	      (*ep).seen(false);
	      ++ep;
	      continue;
	    }

	  // else one end below -EPS, one above 0; split edge
	  const PT crossing(cut_location(perp, base, tail, head));

	  // visibility of segments
	  bool vis_tail(false), vis_head(true);

	  if (0 < ht_tail)
	    {
	      vis_tail=true;
	      vis_head=false;
	    }

	  ep = edges.erase(ep);
	  ep=edges.insert(ep, edge_data<PT>(crossing, head, vis_head));
	  ep=edges.insert(ep, edge_data<PT>(tail, crossing, vis_tail));
	  ++ep;
	  ++ep;
	} // all edges examined

      return cull(edges);
    } // end of chop_path()


  template <typename PT>
    std::list<edge_data<PT> >& chop_loop(const PT& perp, const PT& base,
			    std::list<edge_data<PT> >& edges)
    {
      return loopify(chop_path(perp, base, edges));
    }
} // end of namespace

#endif /* EPIX_CROP_ALGO */
