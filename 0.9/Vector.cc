/* 
 * Vector.cc -- Vectors and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 01, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "Position.h"
#include "Vector.h"

namespace ePiX3d {

  // vector space operations; error if vectors not based at same point
  Vector& operator+ (Vector arg1, const Vector arg2) { return arg1 += arg2; }
  Vector& operator- (Vector arg1, const Vector arg2) { return arg1 -= arg2; }
  Vector& operator- (Vector arg) { return arg *= -1; }

  // cross product
  Vector& operator* (Vector arg1, const Vector arg2) { return arg1 *= arg2; }

  // scalar multiplication
  Vector& operator* (const double c, Vector v) { return v *= c; }

  // dot product and norm
  double operator| (Vector arg1, Vector arg2) 
  { 
    return 
      (arg1.dx1())*(arg2.dx1()) + 
      (arg1.dx2())*(arg2.dx2()) + 
      (arg1.dx3())*(arg2.dx3());
  }

  double norm(const Vector arg) { return sqrt(arg|arg); }

  // componentwise product
  Vector& operator& (Vector arg1, const Vector arg2) { return arg1 &= arg2; }

  // orthogonalization: subtract arg2 component from arg1
  Vector& operator% (Vector arg1, const Vector arg2) { return arg1 %= arg2; }

  // Position and Vector affine operations

} /* end of namespace */

