/*
 * segment.cc -- ePiX::Segment class and mathematical operators
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.5
 * Last Change: May 04, 2008
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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

#include "triples.h"

#include "path.h"
#include "segment.h"

namespace ePiX {

Segment::Segment(const P& p1, const P& p2)
    : m_endpt1(p1), m_endpt2(p2), m_malformed(false) {}

Segment::Segment(bool tag) : m_malformed(true) {}

P Segment::end1() const { return m_endpt1; }

P Segment::end2() const { return m_endpt2; }

bool Segment::malformed() const { return m_malformed; }

// translate
Segment& Segment::shift(const P& arg) {
  if (!m_malformed) {
    m_endpt1 += arg;
    m_endpt2 += arg;
  }
  return *this;
}

Segment& Segment::move_to(const P& arg) {
  if (!m_malformed) {
    const P dX(arg - 0.5 * (m_endpt1 + m_endpt2));
    m_endpt1 += dX;
    m_endpt2 += dX;
  }
  return *this;
}

Segment& Segment::scale(double c) {
  if (!m_malformed) {
    const P ctr(0.5 * (m_endpt1 + m_endpt2));

    m_endpt1 = ctr + c * (m_endpt1 - ctr);
    m_endpt2 = ctr + c * (m_endpt2 - ctr);
  }
  return *this;
}

P Segment::midpoint(double t) const {
  return m_endpt1 + t * (m_endpt2 - m_endpt1);
}

void Segment::draw(double stretch) const {
  if (m_malformed) return;

  // else
  path temp(m_endpt1, m_endpt2, stretch);

  temp.draw();
}
}  // namespace ePiX
