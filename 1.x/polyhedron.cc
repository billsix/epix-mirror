/* 
 * polyhedron.cc -- ePiX:: convex polyhedron operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.4
 * Last Change: March 13, 2005
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
#include <cstdarg>

#include "triples.h"
#include "camera.h"
#include "cropping.h"
#include "plane.h"
#include "segment.h"
#include "polyhedron.h"
#include "path.h"

namespace ePiX {

  extern epix_camera camera;

  // convex polygon vertex -- not intended for general use
  class cpv {
  private:
    P location;
    double angle;

  public:
    cpv(const P arg) : location(arg), angle(0) {}

    P posn(void) const { return location; }
    double get_angle(void) const { return angle; }

    // set angle with respect to origin, reference point, normal vector
    void set_angle(P origin, P pos_x, P normal)
    {
      P loc=location-origin;
      loc *= 1/norm(loc);
      P ref=pos_x-origin;
      ref *= 1/norm(ref);
      if (norm(loc-ref) < EPIX_EPSILON)
	angle=0;
      else if (norm(loc+ref) < EPIX_EPSILON)
	angle=M_PI/ePiX::angle(1);
      else
	{
	  double temp=Acos(loc|ref);
	  if (((ref*loc)|normal) < 0) // wrong orientation
	    temp *= -1;

	  angle=temp; // between -pi and pi
	}
    }
  }; // end of class cpv

  class by_angle {
  public:
    bool operator() (const cpv& arg1, const cpv& arg2)
    {
      return (arg1.get_angle() < arg2.get_angle());
    }
  };

  one_skel::one_skel(void)
  {
    std::vector<segment> edges;
  }

  one_skel::one_skel(unsigned int num_edges ...) // list of edges
  {
    std::vector<segment> edges;
    edges.reserve(num_edges);

    // get edges
    va_list ap;
    va_start(ap, num_edges);

    for (unsigned int i=0; i < num_edges; ++i)
      this->add_edge(*va_arg(ap, segment*));

    va_end(ap);
  }

  void one_skel::transform(P Phi(P))
  {
    segment curr;
    for (unsigned int i=0; i < edges.size(); ++i)
      {
	curr=edges.at(i);
	edges.at(i)=join(Phi(curr.end1()), Phi(curr.end2()));
      }
  }

  void one_skel::section(const plane& knife)
  {
    std::vector<cpv> corners;
    segment curr;

    // find/count points where we hit an edge
    unsigned int count=0;

    for (unsigned int i=0; i < edges.size(); ++i)
      {
	curr=this->edges.at(i);

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
	P tmp_ctr = (1.0/3)*(corners.at(0).posn() +
			     corners.at(1).posn() + corners.at(2).posn());

	cpv pos_x = corners.at(0);

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
