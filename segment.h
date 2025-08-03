/* 
 * segment.h -- ePiX::Segment class
 * 
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.5
 * Last Change: May 04, 2008
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 *   This file provides:
 *
 *   The Segment class (unordered pair of points), and operators:
 *    - end1(), end2() (no guaranteed order)
 *    - Translation operators shift(P), move_to(P)
 *    - Stretch about midpoint: scale(double)
 *    - draw(double) (ePiX line)
 *    - midpoint();
 */

#ifndef EPIX_SEGMENT
#define EPIX_SEGMENT

#include "triples.h"

namespace ePiX {

  class Segment {
  public:
    Segment(const P&, const P&); // endpoints

    Segment(bool); // malformed Segment for intersection operators

    // Segment ends are not distinguished geometrically,
    // so these functions should normally be used in tandem.
    P end1() const;
    P end2() const;

    bool malformed() const;

    // translate
    Segment& shift(const P&);
    Segment& move_to(const P&);

    Segment& scale(double);

    P midpoint(double t=0.5) const;

    void draw(double stretch=0) const;

  private:
    P m_endpt1;
    P m_endpt2;

    bool m_malformed;
  }; // end of Segment class

} // end of namespace
#endif /* EPIX_SEGMENT */
