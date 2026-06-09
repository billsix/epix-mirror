/*
 * Sline.h -- ePiX::Sline class (spherical arc)
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.2.17
 * Last Change: June 29, 2017
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

#ifndef EPIX_SLINE
#define EPIX_SLINE

#include "frame.h"
#include "sphere.h"

namespace ePiX {
  class P;

  class Sline {
  public:
    Sline(const P& tail, const P& head);

    bool malformed() const;
    P pole() const; // normalization of tail * head
    double cosine() const; // cosine of subtended arc
    P operator * (const Sline&) const; // this.pole() * pole()

    P reflect(const P&) const;
    Sline reflect(const Sline&) const;

    bool collinear(const Sline&) const;
    bool cuts(const Sline&) const; // we cross arg

    void draw() const; // entire arc
    void draw_front() const; // front arc
    void draw_back() const; // back arc

    void draw_line() const; // entire line
    void line_front() const; // front line
    void line_back() const; // back line

  private:
    P m_tail;
    P m_head;
    P m_pole;
    bool m_malformed; // tail, head collinear
  }; // end of Sline class
} // end of namespace

#endif /* EPIX_SLINE */
