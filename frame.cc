/* 
 * frame.cc -- ePiX orthonormal basis
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: January 10, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
#include "constants.h"
#include "functions.h"

#include "errors.h"
#include "triples.h"
#include "frame.h"

namespace ePiX {

  // standard basis
  frame::frame()
    : m_e1(E_1), m_e2(E_2), m_e3(E_3) { }

  // frame constructor, suitable for (sea,sky,eye) frames: Usually we
  // know the eye vector and want to preserve its direction. The frame
  // is guaranteed to be right-handed, and the first arg is immaterial.
  frame::frame(P arg1, P arg2, P arg3)
  {
    if (norm(arg2*arg3) < EPIX_EPSILON) // too nearly linearly dependent
      epix_error("Linearly dependent arguments to frame");

    // partial Gram-Schmidt
    arg3 *= 1/norm(arg3); // normalize eye

    arg2 %= arg3; // orthogonalize sky_vector, preserving screen direction
    arg2 *= 1/norm(arg2); // and normalize

    m_e1 = arg2*arg3;
    m_e2 = arg2;
    m_e3 = arg3;
  }


  frame& frame::rot1(double angle)
  {
    P temp2(m_e2);
    P temp3(m_e3);

    m_e2 = (Cos(angle)*(temp2)) - (Sin(angle)*(temp3));
    m_e3 = (Sin(angle)*(temp2)) + (Cos(angle)*(temp3));

    return *this;
  }


  frame& frame::rot2(double angle)
  {
    P temp3(m_e3);
    P temp1(m_e1);

    m_e3 = (Cos(angle)*(temp3)) - (Sin(angle)*(temp1));
    m_e1 = (Sin(angle)*(temp3)) + (Cos(angle)*(temp1));

    return *this;
  }

  frame& frame::rot3(double angle)
  {
    P temp1(m_e1);
    P temp2(m_e2);

    m_e1 = (Cos(angle)*(temp1)) - (Sin(angle)*(temp2));
    m_e2 = (Sin(angle)*(temp1)) + (Cos(angle)*(temp2));

    return *this;
  }

  // frame elements
  P frame::sea() const
  {
    return m_e1;
  }
  P frame::sky() const
  {
    return m_e2;
  }
  P frame::eye() const
  {
    return m_e3;
  }
} // end of namespace
