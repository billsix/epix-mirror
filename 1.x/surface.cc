/*
 * surface.cc -- Shaded surface plotting
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 1.0.13
 * Last Change: September 8, 2006
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

#include "triples.h"
#include "frame.h"

#include "camera.h"

#include "domain.h"
#include "curves.h"
#include "output.h"
#include "surface.h"

namespace ePiX {
  // for parametric surface
  facet::facet(P f(double u, double v), const domain& R, double u0, double v0)
  {
    unsigned int N1(R.fine.n1()/R.coarse.n1());
    unsigned int N2(R.fine.n2()/R.coarse.n2());

    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i * R.dx1(), v0));

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + R.step1(), v0 + j * R.dx2()));

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + R.step1() - i * R.dx1(), v0 + R.step2()));

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0, v0 + R.step2() - j * R.dx2()));

    bd.close().fill();

    // use corners to approximate distance to camera
    pt1 = f(u0, v0);
    pt2 = f(u0 + R.step1(), v0);
    pt3 = f(u0 + R.step1(), v0 + R.step2());
    pt4 = f(u0, v0 + R.step2());
    center = 0.25*(pt1 + pt2 + pt3 + pt4);

    direction = center-camera.get_viewpt();
    distance = norm(direction);
  }

  // for surface of rotation
  facet::facet(double f(double), double g(double), const domain& R,
	       double u0, double v0, const frame& coords)
  {
    unsigned int N1(R.fine.n1()/R.coarse.n1());
    unsigned int N2(R.fine.n2()/R.coarse.n2());

    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i * R.dx1())*coords.sea() +
	    g(u0 + i * R.dx1())*Cos(v0)*coords.sky() +
	    g(u0 + i * R.dx1())*Sin(v0)*coords.eye());

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + R.step1())*coords.sea() +
	    g(u0 + R.step1())*Cos(v0 + j * R.dx2())*coords.sky() +
	    g(u0 + R.step1())*Sin(v0 + j * R.dx2())*coords.eye());

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + R.step1() - i * R.dx1())*coords.sea() +
	    g(u0 + R.step1() - i * R.dx1())*Cos(v0 + R.step2())*coords.sky() +
	    g(u0 + R.step1() - i * R.dx1())*Sin(v0 + R.step2())*coords.eye());

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0)*coords.sea() +
	    g(u0)*Cos(v0 + R.step2() - j * R.dx2())*coords.sky() +
	    g(u0)*Sin(v0 + R.step2() - j * R.dx2())*coords.eye());

    bd.close().fill();

    // use corners to approximate distance to camera
    pt1 = f(u0)*coords.sea() +
      g(u0)*Cos(v0)*coords.sky() +
      g(u0)*Sin(v0)*coords.eye();

    pt2 = f(u0 + R.step1())*coords.sea() +
      g(u0 + R.step1())*Cos(v0)*coords.sky() +
      g(u0 + R.step1())*Sin(v0)*coords.eye();

    pt3 = f(u0 + R.step1())*coords.sea() +
      g(u0 + R.step1())*Cos(v0 + R.step2())*coords.sky() +
      g(u0 + R.step1())*Sin(v0 + R.step2())*coords.eye();

    pt4 = f(u0)*coords.sea() +
      g(u0)*Cos(v0 + R.step2())*coords.sky() +
      g(u0)*Sin(v0 + R.step2())*coords.eye();

    center = 0.25*(pt1 + pt2 + pt3 + pt4);

    direction = center-camera.get_viewpt();
    distance = norm(direction);
  }

  double facet::how_far() const { return distance; }

  bool facet::front_facing(void) const
  {
    return ((camera.get_viewpt()-center)|(pt3-center)) > -EPIX_EPSILON;
  }

  void facet::draw(void)
  {
    bool my_fill=epix::fill_paths; // get fill state
    if (my_fill) // calculate cosine^2 of normal angle
      {
	P normal((pt2 - pt1)*(pt4 - pt1));
	normal *= recip(norm(normal));

	gray(0.75*(1-pow(normal|(recip(distance)*direction), 2)));
      }

    fill();
    bd.draw();
    fill(my_fill); // restore state
  }


  bool by_distance::operator() (const facet& arg1, const facet& arg2)
  {
    return arg1.how_far() > arg2.how_far();
  }

  void surface(P F(double, double), const domain& R)
  {
    if (R.dx3() > 0)
      epix_warning("surface() ignores domain's third coordinate");

    std::vector<facet> mesh;

    for (int i=0; i<R.coarse.n1(); ++i)
      for (int j=0; j<R.coarse.n2(); ++j)
	mesh.push_back(facet(F, R,
			     R.corner1.x1() + i*R.step1(),
			     R.corner1.x2() + j*R.step2()));

    sort(mesh.begin(), mesh.end(), by_distance());

    for (unsigned int i=0; i<mesh.size(); ++i)
      mesh.at(i).draw();
  }


  void surface_rev(double f(double), double g(double), 
		   double min_x, double max_x, int i_max, int j_max)
  {
    domain R(P(min_x, 0), P(max_x, angle(2*M_PI)),
	     mesh(i_max, j_max), mesh(i_max, j_max));

    surface_rev(f, g, R);
  }

  void surface_rev(double f(double), double g(double), 
		   const domain& R, const frame& coords)
  {
    if (R.dx3() > 0)
      epix_warning("surface_rev() ignores domain's third coordinate");

    std::vector<facet> mesh;

    for (int i=0; i<R.coarse.n1(); ++i)
      for (int j=0; j<R.coarse.n2(); ++j)
	mesh.push_back(facet(f, g, R,
			     R.corner1.x1() + i*R.step1(),
			     R.corner1.x2() + j*R.step2(), coords));

    sort(mesh.begin(), mesh.end(), by_distance());

    for (unsigned int i=0; i<mesh.size(); ++i)
      mesh.at(i).draw();
  }
} // end of namespace

