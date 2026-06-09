/*
 * facet.cc -- Utility classes for shaded surface plotting
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.2
 * Last Change: December 06, 2007
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
#include "functions.h"

#include "frame.h"
#include "camera.h"

#include "Color.h"
#include "paint_style.h"

#include "facet.h"

namespace ePiX {

  facet::facet(P f(double, double),
	       double u0, double v0,
	       double du, double dv,
	       const unsigned int N1, const unsigned int N2)
    : m_tint(the_paint_style().fill_color()),
      m_line(the_paint_style().line_pen()),
      m_fill(the_paint_style().fill_flag()),
      pt1(f(u0,         v0)),
      pt2(f(u0 + N1*du, v0)),
      pt3(f(u0 + N1*du, v0 + N2*dv)),
      pt4(f(u0,         v0 + N2*dv)),
      center(0.25*(pt1 + pt2 + pt3 + pt4)),
      direction(center-cam().viewpt()),
      distance(norm(direction))
  {
    perp = ((pt1 - center)*(pt2 - center));

    if (norm(perp) < EPIX_EPSILON)
      perp = (pt3 - center)*(pt4 - center);

    perp *= recip(norm(perp));

    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i*du, v0));

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + N1*du, v0 + j*dv));

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + (N1-i)*du, v0 + N2*dv));

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0, v0 + (N2-j)*dv));

    bd.close().fill(!m_tint.is_unset());
  }


  // facet constructor for f(double, double, double)
  facet::facet(P f(double, double, double),
	       double u0, double v0, double w0,
	       double du, double dv, double dw,
	       const unsigned int N1, const unsigned int N2)
    : m_tint(the_paint_style().fill_color()),
      m_line(the_paint_style().line_pen()),
      m_fill(the_paint_style().fill_flag()),
      pt1(f(u0, v0, w0))
  {
    if (du == 0) // (y,z)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0, v0 + i*dv, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0 + N1*dv, w0 + j*dw));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0, v0 + (N1-i)*dv, w0 + N2*dw));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0, w0 + (N2-j)*dw));

	// use corners to approximate distance to camera
	pt2 = f(u0, v0 + N1*dv, w0);
	pt3 = f(u0, v0 + N1*dv, w0 + N2*dw);
	pt4 = f(u0, v0,         w0 + N2*dw);
      }

    else if (dv == 0) // (x,z)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + i*du, v0, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0 + N1*du, v0, w0 + j*dw));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + (N1-i)*du, v0, w0 + N2*dw));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0, w0 + (N2-j)*dw));

	// use corners to approximate distance to camera
	pt2 = f(u0 + N1*du, v0, w0);
	pt3 = f(u0 + N1*du, v0, w0 + N2*dw);
	pt4 = f(u0,         v0, w0 + N2*dw);
      }

    else if (dw == 0) // (x,y)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + i*du, v0, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0 + N1*du, v0 + j*dv, w0));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + (N1-i)*du, v0 + N2*dv, w0));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0 + (N2-j)*dv, w0));

	// use corners to approximate distance to camera
	pt2 = f(u0 + N1*du, v0,         w0);
	pt3 = f(u0 + N1*du, v0 + N2*dv, w0);
	pt4 = f(u0,         v0 + N2*dv, w0);
      }

    else
      epix_error("Bad call to facet constructor"); // exits

    bd.close().fill(!m_tint.is_unset());

    center = 0.25*(pt1 + pt2 + pt3 + pt4);

    direction = center-cam().viewpt();
    distance = norm(direction);

    perp = ((pt1 - center)*(pt2 - center));

    if (norm(perp) < EPIX_EPSILON)
      perp = (pt3 - center)*(pt4 - center);

    perp *= recip(norm(perp));

  } // end of facet(f(double, double, double), ...)


  // for surface of rotation
  facet::facet(double f(double), double g(double),
	       double u0, double v0,
	       double du, double dv,
	       const unsigned int N1, const unsigned int N2,
	       const frame& coords)
    : m_tint(the_paint_style().fill_color()),
      m_line(the_paint_style().line_pen()),
      m_fill(the_paint_style().fill_flag()),
      pt1(f(u0)*coords.sea() +
	  g(u0)*Cos(v0)*coords.sky() +
	  g(u0)*Sin(v0)*coords.eye()),
      pt2(f(u0 + N1*du)*coords.sea() +
	  g(u0 + N1*du)*Cos(v0)*coords.sky() +
	  g(u0 + N1*du)*Sin(v0)*coords.eye()),
      pt3(f(u0 + N1*du)*coords.sea() +
	  g(u0 + N1*du)*Cos(v0 + N2*dv)*coords.sky() +
	  g(u0 + N1*du)*Sin(v0 + N2*dv)*coords.eye()),
      pt4(f(u0)*coords.sea() +
	  g(u0)*Cos(v0 + N2*dv)*coords.sky() +
	  g(u0)*Sin(v0 + N2*dv)*coords.eye()),
      center(0.25*(pt1 + pt2 + pt3 + pt4)),
      direction(center-cam().viewpt()), distance(norm(direction))
  {
    perp = ((pt1 - center)*(pt2 - center));

    if (norm(perp) < EPIX_EPSILON)
      perp = (pt3 - center)*(pt4 - center);

    perp *= recip(norm(perp));

    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i*du)*coords.sea() +
	    g(u0 + i*du)*Cos(v0)*coords.sky() +
	    g(u0 + i*du)*Sin(v0)*coords.eye());

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + N1*du)*coords.sea() +
	    g(u0 + N1*du)*Cos(v0 + j*dv)*coords.sky() +
	    g(u0 + N1*du)*Sin(v0 + j*dv)*coords.eye());

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + (N1-i)*du)*coords.sea() +
	    g(u0 + (N1-i)*du)*Cos(v0 + N2*dv)*coords.sky() +
	    g(u0 + (N1-i)*du)*Sin(v0 + N2*dv)*coords.eye());

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0)*coords.sea() +
	    g(u0)*Cos(v0 + (N2-j)*dv)*coords.sky() +
	    g(u0)*Sin(v0 + (N2-j)*dv)*coords.eye());

    bd.close().fill(!m_tint.is_unset());
  }

  //// Color-dependent constructors
  facet::facet(P f(double, double),
	       double u0, double v0,
	       double du, double dv,
	       const unsigned int N1, const unsigned int N2,
	       const Color& tint)
    : m_tint(tint), m_line(the_paint_style().line_pen()),
      m_fill(the_paint_style().fill_flag()),
      pt1(f(u0,         v0)),
      pt2(f(u0 + N1*du, v0)),
      pt3(f(u0 + N1*du, v0 + N2*dv)),
      pt4(f(u0,         v0 + N2*dv)),
      center(0.25*(pt1 + pt2 + pt3 + pt4)),
      direction(center-cam().viewpt()),
      distance(norm(direction))
  {
    perp = ((pt1 - center)*(pt2 - center));

    if (norm(perp) < EPIX_EPSILON)
      perp = (pt3 - center)*(pt4 - center);

    perp *= recip(norm(perp));

    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i*du, v0));

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + N1*du, v0 + j*dv));

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + (N1-i)*du, v0 + N2*dv));

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0, v0 + (N2-j)*dv));

    bd.close().fill(!m_tint.is_unset());
  }


  // facet constructor for f(double, double, double)
  facet::facet(P f(double, double, double),
	       double u0, double v0, double w0,
	       double du, double dv, double dw,
	       const unsigned int N1, const unsigned int N2,
	       const Color& tint)
    : m_tint(tint), m_line(the_paint_style().line_pen()),
      m_fill(the_paint_style().fill_flag()),
      pt1(f(u0, v0, w0))
  {
    if (du == 0) // (y,z)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0, v0 + i*dv, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0 + N1*dv, w0 + j*dw));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0, v0 + (N1-i)*dv, w0 + N2*dw));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0, w0 + (N2-j)*dw));

	// use corners to approximate distance to camera
	pt2 = f(u0, v0 + N1*dv, w0);
	pt3 = f(u0, v0 + N1*dv, w0 + N2*dw);
	pt4 = f(u0, v0,         w0 + N2*dw);
      }

    else if (dv == 0) // (x,z)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + i*du, v0, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0 + N1*du, v0, w0 + j*dw));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + (N1-i)*du, v0, w0 + N2*dw));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0, w0 + (N2-j)*dw));

	// use corners to approximate distance to camera
	pt2 = f(u0 + N1*du, v0, w0);
	pt3 = f(u0 + N1*du, v0, w0 + N2*dw);
	pt4 = f(u0,         v0, w0 + N2*dw);
      }

    else if (dw == 0) // (x,y)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + i*du, v0, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0 + N1*du, v0 + j*dv, w0));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + (N1-i)*du, v0 + N2*dv, w0));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0 + (N2-j)*dv, w0));

	// use corners to approximate distance to camera
	pt2 = f(u0 + N1*du, v0,         w0);
	pt3 = f(u0 + N1*du, v0 + N2*dv, w0);
	pt4 = f(u0,         v0 + N2*dv, w0);
      }

    else
      epix_error("Bad call to facet constructor"); // exits

    bd.close().fill(!m_tint.is_unset());

    center = 0.25*(pt1 + pt2 + pt3 + pt4);

    direction = center-cam().viewpt();
    distance = norm(direction);

    perp = ((pt1 - center)*(pt2 - center));

    if (norm(perp) < EPIX_EPSILON)
      perp = (pt3 - center)*(pt4 - center);

    perp *= recip(norm(perp));

  } // end of facet(f(double, double, double), ...)


  // for surface of rotation
  facet::facet(double f(double), double g(double),
	       double u0, double v0,
	       double du, double dv,
	       const unsigned int N1, const unsigned int N2,
	       const Color& tint, const frame& coords)
    : m_tint(tint), m_line(the_paint_style().line_pen()),
      m_fill(the_paint_style().fill_flag()),
      pt1(f(u0)*coords.sea() +
	  g(u0)*Cos(v0)*coords.sky() +
	  g(u0)*Sin(v0)*coords.eye()),
      pt2(f(u0 + N1*du)*coords.sea() +
	  g(u0 + N1*du)*Cos(v0)*coords.sky() +
	  g(u0 + N1*du)*Sin(v0)*coords.eye()),
      pt3(f(u0 + N1*du)*coords.sea() +
	  g(u0 + N1*du)*Cos(v0 + N2*dv)*coords.sky() +
	  g(u0 + N1*du)*Sin(v0 + N2*dv)*coords.eye()),
      pt4(f(u0)*coords.sea() +
	  g(u0)*Cos(v0 + N2*dv)*coords.sky() +
	  g(u0)*Sin(v0 + N2*dv)*coords.eye()),
      center(0.25*(pt1 + pt2 + pt3 + pt4)),
      direction(center-cam().viewpt()), distance(norm(direction))
  {
    perp = ((pt1 - center)*(pt2 - center));

    if (norm(perp) < EPIX_EPSILON)
      perp = (pt3 - center)*(pt4 - center);

    perp *= recip(norm(perp));

    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i*du)*coords.sea() +
	    g(u0 + i*du)*Cos(v0)*coords.sky() +
	    g(u0 + i*du)*Sin(v0)*coords.eye());

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + N1*du)*coords.sea() +
	    g(u0 + N1*du)*Cos(v0 + j*dv)*coords.sky() +
	    g(u0 + N1*du)*Sin(v0 + j*dv)*coords.eye());

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + (N1-i)*du)*coords.sea() +
	    g(u0 + (N1-i)*du)*Cos(v0 + N2*dv)*coords.sky() +
	    g(u0 + (N1-i)*du)*Sin(v0 + N2*dv)*coords.eye());

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0)*coords.sea() +
	    g(u0)*Cos(v0 + (N2-j)*dv)*coords.sky() +
	    g(u0)*Sin(v0 + (N2-j)*dv)*coords.eye());

    bd.close().fill(!m_tint.is_unset());
  }


  facet* facet::clone() const
  {
    return new facet(*this);
  }

  double facet::how_far() const { return distance; }

  bool facet::front_facing() const
  {
    return (-direction|perp) > -EPIX_EPSILON;
  }

  // N.B. We assume the fill state is stored by our caller
  void facet::draw(int cull) const
  {
    if (( cull ==  1 && front_facing() ) || ( cull == -1 && !front_facing() ))
      return;

    // else
    Color paint(m_tint);
    if (paint.is_unset())
      paint = White();

    Color ink(m_line.color());
    if (ink.is_unset())
      ink = paint;

    if (m_fill)
      {
	// calculate cosine^2 of normal angle
	// Magic formula (simulated ambient lighting)
	const double dens(0.5*(1+pow(perp|(recip(distance)*direction), 2)));

	paint *= dens;
	ink   *= dens;
      }

    bd.draw(paint, pen_data(ink, m_line.width()));
  } // end of facet::draw(bool, int)


  bool by_distance::operator() (const facet& arg1, const facet& arg2)
  {
    return arg1.how_far() > arg2.how_far();
  }

  bool by_distance::operator() (const facet* arg1, const facet* arg2)
  {
    return arg1->how_far() > arg2->how_far();
  }
} // end of namespace
