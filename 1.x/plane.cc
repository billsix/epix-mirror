/* 
 * plane.cc -- ePiX::plane class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 09, 2006
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

#include "globals.h"
#include "errors.h"

#include "circle.h"
#include "polyhedron.h"
#include "segment.h"
#include "sphere.h"

#include "path.h"
#include "curves.h"
#include "cropping.h"
#include "plane.h"

namespace ePiX {

  plane::plane(const P& point, const P& normal)
    : m_pt(point), m_perp(normal)
  { 
    double temp(norm(normal));

    if (temp < EPIX_EPSILON)
      {
	epix_warning("Degenerate plane normal, using (0,0,1)");
	m_perp=E_3;
      }

    else
      m_perp *= 1.0/temp;
  }

  plane::plane(const P& p1, const P& p2, const P& p3)
    : m_pt(p1)
  {
    P perp((p3-p1)*(p2-p1));
    double norm_perp(norm(perp));

    if (norm_perp < EPIX_EPSILON)
      throw constructor_error(COLLINEAR_PTS);

    else
      m_perp = (1/norm_perp)*perp;
  }


  P plane::normal() const
  {
    return m_perp;
  }

  plane& plane::reverse() 
  { 
    m_perp *= -1;
    return *this;
  }

  plane& plane::operator += (const P& arg)
  {
    m_pt += arg;
    return *this;
  }

  double plane::height(const P& arg) const
  {
    return (arg-m_pt)|m_perp;
  }

  bool plane::contains(const P& arg) const
  { 
    return (fabs(height(arg)) < EPIX_EPSILON); 
  }

  bool plane::separates(const P& arg1, const P& arg2) const
  {
    return (height(arg1)*height(arg2) <= 0 );
  }

  bool plane::parallel_to (const plane& arg) const
  {
    return (norm(m_perp*arg.m_perp)<EPIX_EPSILON);
  }

  bool plane::operator== (const plane& arg) const
  {
    // normals parallel and arg.m_pt lies in *this
    return ( parallel_to(arg) && ((m_pt - arg.m_pt)|m_perp) < EPIX_EPSILON);
  }

  P operator* (const plane& knife, const segment& seg)
  {
    P tail(seg.end1()), head(seg.end2());

    double ptail(knife.height(tail)), phead(knife.height(head));

    return tail + (ptail/(ptail-phead))*(head-tail);
  }

  // draw Line of intersection between non-parallel planes
  void plane::operator* (const plane& P1) const
  {
    P N3((P1.m_perp)*m_perp);
    double temp(norm(N3));

    if (temp < EPIX_EPSILON)
      throw join_error(PARALLEL);

    else  // N3 non-zero, parallel to intersection
      {
	N3 *= 1/temp; // normalize

	P perp((P1.m_perp)*N3); // unit vector in P1, perp to intersection
	P point(P1.m_pt + (((m_pt-P1.m_pt)|m_perp)/(perp|m_perp))*perp);

	Line(point, point+N3); // draw line through points
      }
  }

  // intersection
  circle plane::operator* (const sphere& S) const
  {
    double rad(S.radius());
    // signed dist from S.ctr to *this
    double height((m_pt - S.center())|m_perp);

    if (rad < fabs(height))
      throw join_error(SEPARATED);

    else if (rad == fabs(height))
      throw join_error(TANGENT);

    else
      return circle(S.center() + height*m_perp,
		    sqrt((rad - height)*(rad + height)),
		    m_perp);
  }

  void plane::draw() const
  {
    // clip_box vertices
    P vert000(clip1_min(), clip2_min(), clip3_min());
    P vert100(clip1_max(), clip2_min(), clip3_min());
    P vert010(clip1_min(), clip2_max(), clip3_min());
    P vert110(clip1_max(), clip2_max(), clip3_min());

    P vert001(clip1_min(), clip2_min(), clip3_max());
    P vert101(clip1_max(), clip2_min(), clip3_max());
    P vert011(clip1_min(), clip2_max(), clip3_max());
    P vert111(clip1_max(), clip2_max(), clip3_max());

    segment edge00(vert000, vert001);
    segment edge01(vert001, vert011);
    segment edge02(vert011, vert010);
    segment edge03(vert010, vert000);

    segment edge04(vert000, vert100);
    segment edge05(vert001, vert101);
    segment edge06(vert011, vert111);
    segment edge07(vert010, vert110);

    segment edge08(vert100, vert101);
    segment edge09(vert101, vert111);
    segment edge10(vert111, vert110);
    segment edge11(vert110, vert100);

    // and edges
    one_skel walls(12, &edge00, &edge01, &edge02, &edge03, &edge04, &edge05,
		   &edge06, &edge07, &edge08, &edge09, &edge10, &edge11);

    walls.section(*this);
  } // end of plane::draw()

} /* end of namespace */
