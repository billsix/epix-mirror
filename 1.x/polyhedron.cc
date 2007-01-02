/* 
 * polyhedron.cc -- ePiX:: convex polyhedron operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 09, 2005
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
#include <cstdarg>

#include "triples.h"

#include "cropping.h"
#include "plane.h"
#include "segment.h"
#include "path.h"
#include "polyhedron.h"

namespace ePiX {

  // convex polygon vertex -- not intended for general use
  class cpv {
  public:
    cpv(const P& arg)
      : location(arg), angle(0) { }

    P posn() const
    {
      return location;
    }
    double get_angle() const
    {
      return angle;
    }

    // set angle with respect to origin, reference point, normal vector
    void set_angle(const P& origin, const P& pos_x, const P& normal)
    {
      P loc(location-origin);
      loc *= 1/norm(loc);

      P ref(pos_x-origin);
      ref *= 1/norm(ref);

      if ((loc|ref) < 0.)
	angle = M_PI/ePiX::angle(1) - 2*Asin(0.5*norm(loc+ref));
      else
	angle = 2*Asin(0.5*norm(loc-ref));

      if (((ref*loc)|normal) < 0) // wrong orientation
	angle *= -1;

    }

  private:
    P location;
    double angle;
  }; // end of class cpv

  class by_angle {
  public:
    bool operator() (const cpv& arg1, const cpv& arg2)
    {
      return (arg1.get_angle() < arg2.get_angle());
    }
  };

  one_skel::one_skel() { }

  one_skel::one_skel(unsigned int num_edges ...) // list of edges
  {
    std::vector<segment> tmp_edges;

    // get edges
    va_list ap;
    va_start(ap, num_edges);

    for (unsigned int i=0; i < num_edges; ++i)
      tmp_edges.push_back(*va_arg(ap, segment*));

    va_end(ap);

    swap(edges, tmp_edges);
  }

  void one_skel::transform(P Phi(P))
  {
    for (unsigned int i=0; i < edges.size(); ++i)
      {
	segment curr(edges.at(i));
	edges.at(i)=segment(Phi(curr.end1()), Phi(curr.end2()));
      }
  }

  void one_skel::section(const plane& knife)
  {
    std::vector<cpv> corners;

    // find/count points where we hit an edge
    unsigned int count=0;

    for (unsigned int i=0; i < edges.size(); ++i)
      {
	segment curr(edges.at(i));

	if (knife.separates(curr.end1(), curr.end2()))
	  {
	    ++count;
	    corners.push_back(cpv(knife*curr));
	  }
      }

    // hit at least a corner
    if (count > 2)
      {
	// kludge to build an origin to measure angles
	P tmp_ctr((1.0/3)*(corners.at(0).posn() +
			   corners.at(1).posn() + corners.at(2).posn()));

	cpv pos_x(corners.at(0));

	for (unsigned int i=1; i < count; ++i)
	  corners.at(i).set_angle(tmp_ctr, pos_x.posn(), knife.normal());

	sort(corners.begin(), corners.end(), by_angle());

	std::vector<vertex> data;
	data.push_back(vertex(corners.at(0).posn()));

	for (unsigned int i=1; i < corners.size(); ++i)
	  if (norm(corners.at(i-1).posn()-corners.at(i).posn()) > EPIX_EPSILON)
	    data.push_back(corners.at(i).posn());

	if (data.size() > 1)
	  {
	    path output=path(data, true); // closed polygon
	    output.draw();
	  }
      }
  } // end of section(knife)

} /* end of namespace */
