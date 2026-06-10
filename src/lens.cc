/*
 * lens.cc -- ePiX::lens functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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
// #include <cmath>

#include "pairs.h"
#include "triples.h"
#include "frame.h"

#include "lens.h"

namespace ePiX {

Lens::~Lens() = default;

pair Perspective::operator()(const P& arg, const frame& orient, const P& viewpt,
                             double dist) const {
  P arg_vector(arg - viewpt);

  // get arg's location in our coordinates
  double u1(orient.sea() | arg_vector);
  double u2(orient.sky() | arg_vector);
  double u3(orient.eye() | arg_vector);

  return (-dist / u3) * pair(u1, u2);
}  // end of Camera::Perspective::operator()

bool Perspective::is_linear() const { return true; }

bool Perspective::needs_clip() const { return true; }

Perspective* Perspective::clone() const { return new Perspective(*this); }

pair Orthog::operator()(const P& arg, const frame& orient, const P& viewpt,
                        double dist) const {
  P arg_vector(arg - viewpt);
  double u1(orient.sea() | arg_vector);
  double u2(orient.sky() | arg_vector);

  return pair(u1, u2);
}  // end of Camera::Orthog::operator()

bool Orthog::is_linear() const { return true; }

bool Orthog::needs_clip() const { return true; }

Orthog* Orthog::clone() const { return new Orthog(*this); }

pair Fisheye::operator()(const P& arg, const frame& orient, const P& viewpt,
                         double dist) const {
  P arg_vector(arg - viewpt);
  arg_vector *= 1.0 / (norm(arg_vector));

  double u1(orient.sea() | arg_vector);
  double u2(orient.sky() | arg_vector);

  // radial projection to camera sphere, then orthogonal projection
  return dist * pair(u1, u2);
}  // end of Camera::Fisheye::operator()

bool Fisheye::is_linear() const { return false; }

bool Fisheye::needs_clip() const { return false; }

Fisheye* Fisheye::clone() const { return new Fisheye(*this); }

pair Bubble::operator()(const P& arg, const frame& orient, const P& viewpt,
                        double dist) const {
  P arg_vector(arg - viewpt);
  arg_vector *= 1.0 / (norm(arg_vector));

  double u1(orient.sea() | arg_vector);
  double u2(orient.sky() | arg_vector);
  double u3(orient.eye() | arg_vector);

  // radial projection to camera sphere, then stereographic projection
  return (2.0 * dist / (1 - u3)) * pair(u1, u2);
}  // end of Camera::Bubble::operator()

bool Bubble::is_linear() const { return false; }

bool Bubble::needs_clip() const { return false; }

Bubble* Bubble::clone() const { return new Bubble(*this); }
}  // namespace ePiX
