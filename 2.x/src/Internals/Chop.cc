/*** 
 ***  Chop.cc -- ePiX2 clipping/snipping function template
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: February 4, 2006
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
 ***/

/*
 * A chop-able class (Silhouette, Outline, Shard...) must contain members
 *
 *   std::list<Edge> m_border; // delineating edges
 *   bool m_cropped;           // true if some edge(s) undrawn
 *
 * The caller's Edge list is replaced by the Edge list of the chopped
 * object, and if chopping occurs, the bool is set to true.
 *
 * The "Pt" arguments are the tail and head of a Vector normal to the
 * cutting line/plane. The Vector points *toward* the chopped portion.
 *
 * "Pt" must provide affine operations (addition, scalar mult, subtraction,
 * and a diff of "Pt"s must have an inner product operator "|").
 *
 * An "Edge" must be constructible from two "Pt"s and a bool.
 */

#include <vector>

#include "Constants.h"
#include "Functions.h"

#include "Object_Rep.h"

#include "Chop.h"

namespace ePiX2 {

  void chop(const Point& tail, const Point& head, 
	    std::list<Edge3>& border,
	    bool& is_chopped)
  {

    if (border.size() == 0)
      return;

    const double eps=EPIX2_EPSILON; // half-thickness of cutting plane

    std::list<Edge3>::const_iterator curr; // current edge
    std::vector<Point> cut_ends;
    std::list<Edge3> chopped;

    // Examine heights of edges serially, check for crossings.
    // For clipping, a "crossing" occurs when max height > eps and 
    // min height < eps.
    double from_ht, to_ht, max_ht, min_ht;

    for (curr=border.begin(); curr!=border.end(); ++curr)
      {
	// drop null edges
	if ((*curr).m_from == (*curr).m_to)
	  continue;

	from_ht = ((head-tail)|((*curr).m_from - tail));
	to_ht   = ((head-tail)|((*curr).m_to - tail));

	max_ht = max(from_ht, to_ht);
	min_ht = min(from_ht, to_ht);

	if (min_ht > eps) // no crossing
	  chopped.push_back(*curr);

	else if ( -eps <= min_ht && max_ht <= eps) // edge lies in cut plane
	  {
	    chopped.push_back(*curr);
	    // update latest cut
	    if (cut_ends.size() == 0)
	      cut_ends.push_back((*curr).m_to);
	    else
	      cut_ends.at(cut_ends.size()-1) = (*curr).m_to;
	  }

	else // part of edge is outside
	  {
	    if (max_ht > eps) // part of edge is inside
	      {
		double t = from_ht/(from_ht - to_ht);
		Point cut = (*curr).m_from + t*((*curr).m_to - (*curr).m_from);
		cut_ends.push_back(cut);

		if (from_ht > eps) // crossing from inside
		  {
		    // unchopped portion
		    chopped.push_back(Edge3((*curr).m_from, cut, true));

		    if (cut_ends.size() == 2) // found second crossing
		      if (cut_ends.at(0) != cut_ends.at(1))
			{
			  chopped.push_back(Edge3(cut_ends.at(1),
						 cut_ends.at(0),
						 false));
			  is_chopped=true;
			}
		  }

		else // crossing from outside
		  {
		    if (cut_ends.size() == 2) // found second crossing
		      if (cut_ends.at(0) != cut_ends.at(1))
			{
			  chopped.push_back(Edge3(cut_ends.at(0),
						 cut_ends.at(1),
						 false));
			  is_chopped=true;
			}

		    chopped.push_back(Edge3(cut, (*curr).m_to, true));
		  }
	      } // end of crossing; else edge is outside, no action
	  } // end of edge partially outside
      } // all edges examined 

    // If last Edge hits knife from outside, need to close up border...
    if (cut_ends.size() == 1) // final edge outside, touches knife
      {
	--curr; // back up
	if (cut_ends.at(0) != (*curr).m_to)
	  {
	    chopped.push_back(Edge3(cut_ends.at(0), (*curr).m_to, false));
	    is_chopped=true;
	  }
      }

    border = chopped;
  }

  // Pair version
  void chop(const Pair& tail, const Pair& head, 
	    std::list<Edge2>& border,
	    bool& is_chopped)
  {
    if (border.size() == 0)
      return;

    const double eps=EPIX2_EPSILON; // half-thickness of cutting plane

    std::list<Edge2>::const_iterator curr; // current edge
    std::vector<Pair> cut_ends;
    std::list<Edge2> chopped;

    // Examine heights of edges serially, check for crossings.
    // For clipping, a "crossing" occurs when max height > eps and 
    // min height < eps.
    double from_ht, to_ht, max_ht, min_ht;

    for (curr=border.begin(); curr!=border.end(); ++curr)
      {
	// drop null edges
	if ((*curr).m_from == (*curr).m_to)
	  continue;

	from_ht = ((head-tail)|((*curr).m_from - tail));
	to_ht   = ((head-tail)|((*curr).m_to - tail));

	max_ht = max(from_ht, to_ht);
	min_ht = min(from_ht, to_ht);

	if (min_ht > eps) // no crossing
	  chopped.push_back(*curr);

	else if ( -eps <= min_ht && max_ht <= eps) // edge lies in cut plane
	  {
	    chopped.push_back(*curr);
	    // update latest cut
	    if (cut_ends.size() == 0)
	      cut_ends.push_back((*curr).m_to);
	    else
	      cut_ends.at(cut_ends.size()-1) = (*curr).m_to;
	  }

	else // part of edge is outside
	  {
	    if (max_ht > eps) // part of edge is inside
	      {
		double t = from_ht/(from_ht - to_ht);
		Pair cut = (*curr).m_from + t*((*curr).m_to - (*curr).m_from);
		cut_ends.push_back(cut);

		if (from_ht > eps) // crossing from inside
		  {
		    // unchopped portion
		    chopped.push_back(Edge2((*curr).m_from, cut, true));

		    if (cut_ends.size() == 2) // found second crossing
		      if (cut_ends.at(0) != cut_ends.at(1))
			{
			  chopped.push_back(Edge2(cut_ends.at(1),
						 cut_ends.at(0),
						 false));
			  is_chopped=true;
			}
		  }

		else // crossing from outside
		  {
		    if (cut_ends.size() == 2) // found second crossing
		      if (cut_ends.at(0) != cut_ends.at(1))
			{
			  chopped.push_back(Edge2(cut_ends.at(0),
						 cut_ends.at(1),
						 false));
			  is_chopped=true;
			}

		    chopped.push_back(Edge2(cut, (*curr).m_to, true));
		  }
	      } // end of crossing; else edge is outside, no action
	  } // end of edge partially outside
      } // all edges examined 

    // If last Edge hits knife from outside, need to close up border...
    if (cut_ends.size() == 1) // final edge outside, touches knife
      {
	--curr; // back up
	if (cut_ends.at(0) != (*curr).m_to)
	  {
	    chopped.push_back(Edge2(cut_ends.at(0), (*curr).m_to, false));
	    is_chopped=true;
	  }
      }

    border = chopped;
  }

} // end of namespace
