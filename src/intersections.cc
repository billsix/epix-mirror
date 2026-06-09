/* 
 * intersections.cc -- ePiX's intersection operators
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
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
#include <cmath>

#include "constants.h"
#include "errors.h"

#include "circle.h"
#include "plane.h"
#include "segment.h"
#include "sphere.h"

#include "intersections.h"

namespace ePiX {

  static const double EPS(EPIX_EPSILON);

  Segment operator* (const Circle& arg1, const Circle& arg2)
  {
    if (arg1.malformed() || arg2.malformed())
      return Segment(true);

    const P p2(arg2.center());
    const P n2(arg2.perp());

    P dir(p2 - arg1.center()); // displacement between centers

    const double r1(arg1.radius());
    const double r2(arg2.radius());
    const double rad_diff(fabs(r2 - r1));
    const double rad_sum(r2 + r1);
    const double sep(norm(dir));

    if ( EPS < norm(arg1.perp()*arg2.perp()) || // non-coplanar
	 rad_sum <= sep || // separated
	 sep <= rad_diff ) // concentric, catches equality
      return Segment(true);

    // else
    dir *= 1.0/sep;

    const double COS(((r1-r2)*(r1+r2) - sep*sep)/(2*r2*sep));
    const double SIN(sqrt((1-COS)*(1+COS)));

    return Segment(p2 + r2*(COS*dir + SIN*(n2*dir)),
		   p2 + r2*(COS*dir - SIN*(n2*dir)));
  }


  Segment operator* (const Circle& circ,  const Plane& pl)
  {
    if (circ.malformed() || pl.malformed())
      return Segment(true);

    // else
    P bi_perp(circ.perp()*pl.perp());
    const double denom(norm(bi_perp));

    if (denom < EPS) // parallel
      return Segment(true);

    // else
    bi_perp *= 1.0/denom;

    const P dir(bi_perp*circ.perp()); // unit vector toward pl in circ plane

    // shortest distance from circ.center() to pl in circ plane
    const double x(-((circ.center() - pl.pt())|pl.perp())/(dir|pl.perp()));

    const double rad(circ.radius());

    if (rad <= fabs(x)) // disjoint
      return Segment(true);

    // else
    const P vec_y(sqrt((rad-x)*(rad+x))*bi_perp);
    const P midpt(circ.center() + x*dir);

    return Segment(midpt - vec_y, midpt + vec_y);
  }

  // Extend seg into a line, look for two crossings
  Segment operator* (const Circle& circ, const Segment& seg)
  {
    if (seg.malformed() || circ.malformed())
      return Segment(true);

    // else
    const P dir(seg.end2() - seg.end1());

    if (EPS < fabs(dir|circ.perp()))
      return Segment(true);

    // else
    P to_ctr(circ.center() - seg.end1());
    P perp(circ.perp()*dir);
	
    const double dist((to_ctr|perp)/norm(perp));
    const double rad(circ.radius());

    if (rad <= fabs(dist))
      return Segment(true);

    // else
    const P vec_x(circ.center() - (dist/norm(perp))*perp);
    const P vec_y((sqrt((rad-dist)*(rad+dist))/norm(dir))*dir);

    return Segment(vec_x + vec_y, vec_x - vec_y);
  }


  // cut plane of circ by S, intersect
  Segment operator* (const Circle& circ,  const Sphere& S)
  {
    Plane pl(circ.center(), circ.perp());
    return circ*(pl*S);
  }

  Segment operator* (const Plane& pl1, const Plane& pl2)
  {
    if (pl1.malformed() || pl2.malformed())
      return Segment(true);

    // else
    P N3((pl1.perp())*(pl2.perp()));
    const double temp(norm(N3));

    if (temp < EPS) // parallel
      return Segment(true);

    // else N3 non-zero, parallel to intersection
    N3 *= 1/temp; // normalize

    P perp((pl1.perp())*N3); // unit vector in pl, perp to intersection
    P pt(pl1.pt() + (((pl2.pt()-pl1.pt())|pl2.perp())/(perp|pl2.perp()))*perp);
    P ctr(pt %= N3); // closest pt to origin on line
    // P ctr(pt - (pt|N3)*N3);

    return Segment(ctr - EPIX_INFTY*N3, ctr + EPIX_INFTY*N3);
  }


  P operator* (const Plane& pl, const Segment& seg)
  {
    if (pl.malformed() || seg.malformed())
      epix_warning("Malformed argument(s) to Plane*Segment");

    const P tail(seg.end1());
    const P head(seg.end2());

    const double ptail((tail - pl.pt())|pl.perp());
    const double phead((head - pl.pt())|pl.perp());

    if (fabs(phead - ptail) < EPS)
      epix_warning("Plane parallel to Segment in intersection");

    return tail + (ptail/(ptail-phead))*(head-tail);
  }


  Circle  operator* (const Plane& pl, const Sphere& S)
  {
    if (pl.malformed() || S.malformed())
      return Circle(true);

    // else
    const double rad(S.radius());
    const double ht((pl.pt() - S.center())|pl.perp());

    if (rad <= fabs(ht)) // disjoint
      return Circle(true);

    else
      return Circle(S.center() + ht*pl.perp(),
		    sqrt((rad - ht)*(rad + ht)),
		    pl.perp());
  }


  P operator* (const Segment& seg1, const Segment& seg2)
  {
    if (seg1.malformed() || seg2.malformed())
      epix_warning("Malformed argument(s) in Segment intersection");

    const P p1(seg1.end1());
    const P dir1(seg1.end2() - p1);

    const P p3(seg2.end1());
    const P p4(seg2.end2());
    const P dir2(p4 - p3);

    P perp(dir1*dir2);
    double normal_length();

    // non-coplanar or parallel
    if (EPS < fabs((dir1*(p3 - p1)) | (p4 - p1)) || norm(perp) < EPS)
      epix_warning("Non-generic arguments in Segment intersection");

    // perp lies in plane of segments, is orthog to dir2
    perp *= dir2;

    // get t so that normal|(X - p3) = (normal|(p1 - p3 + t*dir1)) = 0.
    // note: X may not lie on either segment
    return p1 + ((perp|(p3-p1))/(perp|dir1))*dir1;
  }

  // extend seg into a line
  Segment operator* (const Segment& seg, const Sphere& S)
  {
    if (seg.malformed() || S.malformed())
      return Segment(true);

    // else
    P dir(seg.end2() - seg.end1());
    dir *= 1.0/norm(dir);

    const P posn(S.center() - seg.end1());
    const P perp(posn%dir);

    if (S.radius() <= norm(perp)) // disjoint
      return Segment(true);

    // else
    const double B(dir|posn);
    const double C((posn|posn)-pow(S.radius(), 2));
    const double discrim(sqrt(B*B - C)); // [sic]

    return Segment(seg.end1() + (B-discrim)*dir,
		   seg.end1() + (B+discrim)*dir);
  }

  Circle operator* (const Sphere& sph1, const Sphere& sph2)
  {
    if (sph1.malformed() || sph2.malformed())
      return Circle(true);

    // else
    const double r1(sph1.radius());
    const double r2(sph2.radius());
    P dir(sph2.center() - sph1.center());

    const double dist(norm(dir));

    // separated, tangent, or concentric
    if (r1+r2 <= dist || dist <= fabs(r2-r1))
      return Circle(true);

    // else
    const double x(0.5*(dist + (r1-r2)*(r1+r2)/dist));
    const P perp((1/dist)*dir);

    return Circle(sph1.center() + x*perp, sqrt((r1-x)*(r1+x)), perp);
  }


  // derived operators
  Segment operator* (const Plane& pl,   const Circle& circ)
  {
    return circ*pl;
  }

  Segment operator* (const Segment& seg, const Circle& circ)
  {
    return circ*seg;
  }

  Segment operator* (const Sphere& S,  const Circle& circ)
  {
    return circ*S;
  }

  P       operator* (const Segment& seg, const Plane& pl)
  {
    return pl*seg;
  }

  Circle  operator* (const Sphere& S,  const Plane& pl)
  {
    return pl*S;
  }

  Segment operator* (const Sphere& S,  const Segment& seg)
  {
    return seg*S;
  }
} // end of namespace
