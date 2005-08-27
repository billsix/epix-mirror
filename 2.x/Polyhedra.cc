/* 
 * Polyhedra.cc -- ePiX2 Mesh and Polyhedron classes
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 08, 2005
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

#include "Color.h"

#include "Polyhedra.h"

namespace ePiX2 {

  Polygon_Edge::Polygon_Edge(const Point& p1, const Point& p2, const Vector& N)
    : tail(p1), head(p2)
  {
    Vector tmp=N;
    tmp.move_to(p1);

    Vector dir = p2 - p1;
    dir *= recip(norm(dir));

    outward = dir*tmp;
  }

  // void constructor
  Polygon::Polygon(const Vector& N) : normal(N)
  {
    double len=norm(N);
    Point O = normal.tail();

    if (len < EPIX2_EPSILON)
      {
	epix_warning("Polygon requested with null normal, using E_3");
	normal = E_3(O);
      }

    else // normalize
      normal *= 1.0/len;

    const double root3=sqrt(3);
    // the equilateral triangle this far from O lies outside INFTY box
    const double Inf=2*root3*EPIX2_INFTY;

    // get RH basis for N.perp
    Vector bas1, bas2;

    if ((normal|E_3(O)) > 0.7)
      bas1=(E_1(O)%normal);

    else
      bas1=(E_3(O)%normal);

    bas1 *= 1.0/norm(bas1);
    bas2  = normal*bas1; // normalized

    // initial three vertices and outward normals
    Point v1 = O + Inf*bas1;
    Point v2 = O - 0.5*Inf*(bas1-root3*bas2);
    Point v3 = O - 0.5*Inf*(bas1+root3*bas2);

    boundary.push_back(Polygon_Edge(v1, v2, normal));
    boundary.push_back(Polygon_Edge(v2, v3, normal));
    boundary.push_back(Polygon_Edge(v3, v1, normal));

    clip_by( E_1(Point( EPIX2_INFTY,0,0)));
    clip_by(-E_1(Point(-EPIX2_INFTY,0,0)));

    clip_by( E_2(Point( 0, EPIX2_INFTY,0)));
    clip_by(-E_2(Point( 0,-EPIX2_INFTY,0)));

    clip_by( E_3(Point( 0, 0, EPIX2_INFTY)));
    clip_by(-E_3(Point( 0, 0,-EPIX2_INFTY)));

  } // end of Polygon(void)


  Polygon::Polygon (const Point& v1, const Point& v2, const Point& v3)
  {
    normal = (v2-v1)*(v3-v1);

    normal *= recip(norm(normal));

    boundary.push_back(Polygon_Edge(v1, v2, normal));
    boundary.push_back(Polygon_Edge(v2, v3, normal));
    boundary.push_back(Polygon_Edge(v3, v1, normal));
  }

  Polygon::Polygon (const Point& v1, const Point& v2, 
		    const Point& v3, const Point& v4)
  {
    normal = (v2-v1)*(v3-v1);

    normal *= recip(norm(normal));

    boundary.push_back(Polygon_Edge(v1, v2, normal));
    boundary.push_back(Polygon_Edge(v2, v3, normal));
    boundary.push_back(Polygon_Edge(v3, v4, normal));
    boundary.push_back(Polygon_Edge(v4, v1, normal));
  }

  Polygon::Polygon (const Point& v1, const Point& v2, const Point& v3,
		    const Point& v4, const Point& v5)
  {
    normal = (v2-v1)*(v3-v1);

    normal *= recip(norm(normal));

    boundary.push_back(Polygon_Edge(v1, v2, normal));
    boundary.push_back(Polygon_Edge(v2, v3, normal));
    boundary.push_back(Polygon_Edge(v3, v4, normal));
    boundary.push_back(Polygon_Edge(v4, v5, normal));
    boundary.push_back(Polygon_Edge(v5, v1, normal));
  }

  Polygon::Polygon (const Point& v1, const Point& v2, const Point& v3,
		    const Point& v4, const Point& v5, const Point& v6)
  {
    normal = (v2-v1)*(v3-v1);

    normal *= recip(norm(normal));

    boundary.push_back(Polygon_Edge(v1, v2, normal));
    boundary.push_back(Polygon_Edge(v2, v3, normal));
    boundary.push_back(Polygon_Edge(v3, v4, normal));
    boundary.push_back(Polygon_Edge(v4, v5, normal));
    boundary.push_back(Polygon_Edge(v5, v6, normal));
    boundary.push_back(Polygon_Edge(v6, v1, normal));
  }


  Polygon& Polygon::clip_by(const Vector& knife)
  {
    const double eps=EPIX2_EPSILON; // half-thickness of cutting plane
    const Point on_knife = knife.tail();

    std::list<Polygon_Edge>::iterator curr=boundary.begin(); // current edge
    std::vector<Point> cut_ends; // don't set size; use to check for cut

    // Examine heights of edges serially, check for crossings.
    // For clipping, a "crossing" occurs when max height > eps and
    // min height < eps.
    double first_ht, second_ht, max_ht, min_ht;
    Point curr_tail, curr_head;

    while (curr!=boundary.end())
      {
	curr_tail = (*curr).tail;
	curr_head = (*curr).head;

        first_ht  = (knife|(curr_tail - on_knife));
        second_ht = (knife|(curr_head - on_knife));

        max_ht=max(first_ht, second_ht);
        min_ht=min(first_ht, second_ht);

	if (max_ht < eps) // no crossing, do nothing
	  ++curr;

	else if (min_ht > eps) // edge is behind
	  curr=boundary.erase(curr);

	else // trim edge, add new edge if necessary
	  {
	    Point cut=knife.perp_hits(curr_tail, curr_head);
	    cut_ends.push_back(cut);

	    Polygon_Edge E; // unclipped portion

	    curr = boundary.erase(curr); // increment curr

	    if (first_ht < -eps) // crossing into knife
	      {
		E = Polygon_Edge(curr_tail, cut, normal);
		boundary.insert(curr, E);

		if (cut_ends.size() == 2) // found second crossing
		  boundary.insert(curr, 
				  Polygon_Edge(cut_ends.at(1), 
					       cut_ends.at(0), normal));
	      }

	    else // crossing away from knife
	      {
		E = Polygon_Edge(cut, curr_head, normal);

		if (cut_ends.size() == 2) // found second crossing
		  boundary.insert(curr, 
				  Polygon_Edge(cut_ends.at(0),
					       cut_ends.at(1), normal));

		boundary.insert(curr, E);
	      }
	  } // end of crossing
      } // all edges examined

    return *this;
  } // end of clip_by


  // add vertex, return new convex hull; if vtx is inside, no action
  Polygon& Polygon::add(const Point& vtx)
  {
    std::list<Polygon_Edge>::iterator curr=boundary.begin();

    // project vtx to our plane
    Vector loc = vtx - normal.tail();
    loc %= normal;
    Point new_vtx = loc.head();

    bool need_to_close=false; // insert closing edge?

    Point start=(*curr).tail; // in case we need to close at end
    Point curr_vtx;

    while (curr != boundary.end())
      {
	curr_vtx = (*curr).tail;
	
	if (((new_vtx-curr_vtx)|(*curr).outward) <= 0) // back edge
	  {
	    if (need_to_close)
	      {
		boundary.insert(curr, Polygon_Edge(new_vtx, curr_vtx, normal));
		need_to_close=false;
	      }
	    ++curr;
	  }

	else // front edge
	  {
	    curr = boundary.erase(curr);

	    if (!need_to_close) // connect vtx
	      {
		need_to_close=true;
		boundary.insert(curr, Polygon_Edge(curr_vtx, new_vtx, normal));
	      }
	  }

      } // all edges examined

    if (need_to_close) // still not closed
      boundary.push_back(Polygon_Edge(new_vtx, start, normal));

    return *this;
  }

  void Polygon::shatter(void)
  {
    if (boundary.size() < 2) // not enough edges
      return;

    // else compute transformed vertices
    Point curr_tail, curr_head;
    std::list<Polygon_Edge>::const_iterator curr;

    Shard face;

    for (curr=boundary.begin(); curr!=boundary.end(); ++curr)
      {
	curr_tail = the_orient.coords((*curr).tail, the_scale);
	curr_head = the_orient.coords((*curr).head, the_scale);

	face.add_edge(Edge(curr_tail, curr_head, get_line_color(),true));
      }

    face.set_normal(normal);

    face.set_solid(solid);
    face.set_line_color(get_line_color());
    face.set_fill_color(get_fill_color());

    fragments.push_back(face);
  }


  // cube with enormous sides
  Polyhedron::Polyhedron(void)
  {
    closed_oriented=true;
    /*
    const double sz = EPIX2_INFTY;

    Point v_nnn(-sz,-sz,-sz), v_nnp(-sz,-sz, sz);
    Point v_pnn( sz,-sz,-sz), v_pnp( sz,-sz, sz);

    Point v_npn(-sz, sz,-sz), v_npp(-sz, sz, sz);
    Point v_ppn( sz, sz,-sz), v_ppp( sz, sz, sz);


//    Polygon F_1n(-E_1(Point(-sz,0,0))), F_1p(E_1(Point(sz,0,0)));
//    Polygon F_2n(-E_2(Point(0,-sz,0))), F_2p(E_2(Point(0,sz,0)));
//    Polygon F_3n(-E_3(Point(0,0,-sz))), F_3p(E_3(Point(0,0,sz)));

    Polygon F_1n(v_nnn, v_nnp, v_npp, v_npn);
    Polygon F_1p(v_ppp, v_pnp, v_pnn, v_npn);

    Polygon F_2n(v_nnn, v_pnn, v_pnp, v_nnp);
    Polygon F_2p(v_ppp, v_ppn, v_npn, v_npp);

    Polygon F_3n(v_nnn, v_npn, v_ppn, v_pnn);
    Polygon F_3p(v_ppp, v_npp, v_nnp, v_pnp);

    faces.push_back(F_1n);
    faces.push_back(F_2n);
    faces.push_back(F_3n);

    faces.push_back(F_1p);
    faces.push_back(F_2p);
    faces.push_back(F_3p);
    */
  }


  Polyhedron& Polyhedron::operator<< (const Polygon F)
  {
    faces.push_back(F);
    return *this;
  }

  // remove half-space pointed to by N
  Polyhedron& Polyhedron::clip_by(const Vector& knife)
  {
    std::list<Polygon>::iterator f;
    for(f=faces.begin(); f!=faces.end(); ++f)
      (*f).clip_by(knife);

    return *this;
  }

  /*
    Polyhedron& chop_by(const Knife&);  // remove half-space pointed to by N
    Polyhedron& carve_by(const Knife&); // half-space without cutting plane
    Polyhedron& slice_by(const Knife&); // planar slice

    Polyhedron& add_vertex(const Point&);
  */
  void Polyhedron::shatter(void)
  {
    std::list<Polygon>::iterator f;
    for(f=faces.begin(); f!=faces.end(); ++f)
      (*f).shatter();

    // for_each(faces.begin(), faces.end(), std::mem_fun_ref(&Polygon::shatter));
  }

} /* end of namespace */
