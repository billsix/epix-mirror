/*
 * Sline.cc -- ePiX::Sline class (spherical arc)
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 1.2.17
 * Last Change: June 30, 2017
 */

/*
 * Copyright (C) 2017
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

#include "triples.h"
#include "geometry.h"

#include "Sline.h"

namespace ePiX {
Sline::Sline(const P& tail, const P& head)
    : m_tail(tail), m_head(head), m_malformed(false) {
  double mt(norm(tail)), mh(norm(head));
  if (fabs(mt - 1) > EPIX_EPSILON) {
    epix_warning("First argument of Sline is not a unit vector");
    if (EPIX_EPSILON < mt && mt < EPIX_INFTY)
      m_tail *= 1.0 / mt;
    else
      m_malformed = true;
  }

  if (fabs(mh - 1) > EPIX_EPSILON) {
    epix_warning("Second argument of Sline is not a unit vector");
    if (EPIX_EPSILON < mh && mh < EPIX_INFTY)
      m_head *= 1.0 / mh;
    else
      m_malformed = true;
  }

  m_pole = m_tail * m_head;
  double mp(norm(m_pole));
  if (mp < EPIX_EPSILON) {
    epix_warning("Collinear arguments in Sline");
    m_pole = P(0, 0, 0);
    m_malformed = true;
  }

  else  // well-formed; normalize pole
    m_pole *= 1.0 / mp;
}  // end of Sline::Sline

bool Sline::malformed() const { return m_malformed; }

P Sline::pole() const {
  if (m_malformed) epix_warning("Pole of malformed Sline requested");

  return m_pole;
}

double Sline::cosine() const { return (m_tail | m_head); }

P Sline::operator*(const Sline& arg) const {
  if (m_malformed || arg.m_malformed) {
    epix_warning("Malformed argument(s) in Sline intersection");
    return P(0, 0, 0);
  } else if (this->collinear(arg)) {
    epix_warning("Collinear argument(s) in Sline intersection");
    return P(0, 0, 0);
  }

  // else
  P val(m_pole * arg.m_pole);
  val *= 1.0 / norm(val);

  if (0 < (m_tail | val))
    return val;

  else
    return -val;
}

P Sline::reflect(const P& arg) const {
  return arg - 2 * (m_pole | arg) * m_pole;
}

Sline Sline::reflect(const Sline& arg) const {
  return Sline(this->reflect(arg.m_tail), this->reflect(arg.m_head));
}

bool Sline::collinear(const Sline& arg) const {
  return m_malformed || arg.m_malformed ||
         norm(m_pole * arg.m_pole) < EPIX_EPSILON;
}

bool Sline::cuts(const Sline& arg) const  // we and arg cross
{
  return ((m_tail | arg.m_pole) * (m_head | arg.m_pole) < 0 &&
          (m_pole | arg.m_tail) * (m_pole | arg.m_head) < 0);
}

void Sline::draw() const {
  this->draw_front();
  this->draw_back();
}

void Sline::draw_front() const  // front arc
{
  front_arc(m_tail, m_head);
}

void Sline::draw_back() const  // back arc
{
  back_arc(m_tail, m_head);
}

void Sline::draw_line() const {
  this->line_back();
  this->line_front();
}

void Sline::line_front() const { front_line(m_tail, m_head); }

void Sline::line_back() const  // back line
{
  back_line(m_tail, m_head);
}
}  // namespace ePiX
