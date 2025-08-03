/* 
 * sphere.h -- ePiX::Sphere class
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

/*
 *   This file provides:
 *
 *   The Sphere class (center, radius), and operators:
 *    - Sphere += P (translate by <P>)
 *    - Sphere *= double (scale about center)
 *    - draw() (ePiX line)
 *    - center();
 *    - radius();
 *
 *   Hiding test, latitude and longitudes on a sphere.
 */
#ifndef EPIX_SPHERE
#define EPIX_SPHERE

#include "triples.h"

namespace ePiX {

  class Sphere {
  public:
    Sphere(const P& ctr=P(0,0,0), double rad=1);
    Sphere(const P& ctr, const P& pt);

    P center() const;
    double radius() const;

    bool malformed() const; // always returns false, currently

    // translation
    Sphere& shift(const P&);
    Sphere& move_to(const P&);

    // scale radius
    Sphere& scale(double&);

    void draw() const;

  private:
    P m_ctr;
    double m_rad;

    bool m_malformed;
  }; // end of Sphere class


  // global horizon-drawing functions
  void sphere(const P& ctr=P(0,0,0), double rad=1);
  void sphere(const P& ctr, const P& pt);

} // end of namespace

#endif /* EPIX_SPHERE */
