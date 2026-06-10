/*
 * sphere.cc -- ePiX::Sphere class and mathematical operators
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.9
 * Last Change: July 30, 2007
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

#include "errors.h"

#include "triples.h"

#include "functions.h"

#include "camera.h"
#include "circle.h"

#include "sphere.h"

namespace ePiX {

Sphere::Sphere(const P& ctr, double rad)
    : m_ctr(ctr), m_rad(rad), m_malformed(false) {}

Sphere::Sphere(const P& ctr, const P& pt)
    : m_ctr(ctr), m_rad(norm(pt - ctr)), m_malformed(false) {}

P Sphere::center() const { return m_ctr; }

double Sphere::radius() const { return m_rad; }

bool Sphere::malformed() const { return m_malformed; }

// translation
Sphere& Sphere::shift(const P& arg) {
  if (!m_malformed) m_ctr += arg;
  return *this;
}

Sphere& Sphere::move_to(const P& arg) {
  if (!m_malformed) m_ctr = arg;
  return *this;
}

Sphere& Sphere::scale(double& arg) {
  if (!m_malformed) m_rad *= arg;
  return *this;
}

void Sphere::draw() const {
  if (m_malformed) return;

  // else
  P location(cam().viewpt());

  P dir(m_ctr - location);
  double dist(norm(dir));

  if (dist <= m_rad)
    epix_warning("Cannot draw sphere enclosing camera");

  else {
    dir *= 1 / dist;
    double x(m_rad * m_rad / dist);

    Circle temp(m_ctr - x * dir, sqrt((m_rad - x) * (m_rad + x)), dir);
    temp.draw();
  }
}  // end of Sphere::photo()

//// global functions ////
void sphere(const P& ctr, double rad) {
  Sphere S(ctr, rad);
  S.draw();
}

void sphere(const P& ctr, const P& pt) {
  Sphere S(ctr, pt);
  S.draw();
}
}  // namespace ePiX
