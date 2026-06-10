/*
 * affine.h -- ePiX::affine class (affine screen maps)
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.21
 * Last Change: September 23, 2007
 *
 *
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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
 * An affine map is constructed from the images of the "standard
 * triple" (0,0), (1,0), and (0,1).
 *
 * An affine map may be (post)-composed with translation, rotation,
 * reflection, scaling along either or both coordinate axes, shearing,
 * and may be inverted. An optional pair argument, defaulting to
 * (0,0), gives the center of the operation (if appropriate).
 *
 * An affine map may be post-composed via member function, and
 * pre-composed with operator syntax. The evaluation operator
 * returns the image of the argument.
 */

#ifndef EPIX_AFFINE
#define EPIX_AFFINE

#include "pairs.h"
#include "triples.h"

namespace ePiX {

class affine {
 public:
  // the identity map
  affine();

  // images of (1,0), (0,1), (0,0)
  affine(const pair&, const pair&, const pair& loc = pair(0, 0));
  affine(const P&, const P&, const P& loc = P(0, 0));

  // post-operations
  // translate by arg
  affine& shift(const pair& arg);
  affine& shift(const P& arg);

  // rotate by theta about ctr
  affine& rotate(double theta, const pair& ctr = pair(0, 0));
  affine& rotate(double theta, const P& ctr);

  // reflect in angle-theta line through ctr
  affine& reflect(double theta, const pair& ctr = pair(0, 0));
  affine& reflect(double theta, const P& ctr);

  // scale coord direction(s) fixing ctr
  affine& h_scale(double, const pair& ctr = pair(0, 0));
  affine& v_scale(double, const pair& ctr = pair(0, 0));
  affine& scale(double, const pair& ctr = pair(0, 0));

  affine& h_scale(double, const P& ctr);
  affine& v_scale(double, const P& ctr);
  affine& scale(double, const P& ctr);

  // shear, fixing ctr
  affine& h_shear(double, const pair& ctr = pair(0, 0));
  affine& v_shear(double, const pair& ctr = pair(0, 0));

  affine& h_shear(double, const P& ctr);
  affine& v_shear(double, const P& ctr);

  // post-compose
  affine& postcomp(const affine&);

  affine& invert();

  // evaluation
  pair operator()(const pair&) const;
  pair operator()(const P&) const;

  // pre-compose
  affine operator()(const affine&) const;

  bool reverses_orientation() const;

 private:
  pair m_00;
  pair m_10;
  pair m_01;
};
}  // namespace ePiX

#endif /* EPIX_AFFINE */
