/* 
 * Vector.h -- Vectors and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 27, 2003
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

/*
 * A Vector is conceptually a displacement in (3-dimensional) object
 * space. Vectors are represented as a basepoint and a triple of
 * numbers. ePiX works in non-Euclidean coordinate systems, and
 * therefore must emphasize the distinction between locations (Position)
 * and displacements (Vector). In coordinate systems other than
 * Cartesian, a basepoint *must* be specified when defining a vector.
 *
 * Mathematically, a Position is an element of the manifold R^3, while
 * a Vector is an element of the tangent bundle, TR^3. Positions and
 * Vectors are always "in Cartesian object coordinates". Non-Cartesian
 * coordinate systems are handled in the classes P and V.
 *
 * This file provides:
 *   - Cartesian constructor (basepoint defaults to origin)
 *   - Position-valued head and tail operators, component functions
 *   - Algebraic operators for vectors at a point, including 
 *       -- vector space operations (v1+-v2, c*v)
 *       -- dot, cross, and componentwise products (|, *, and &)
 *       -- orthogonalization (v1 % v2 -- "division with remainder")
 *       -- (in)equality
 *       -- norm
 *   - Position-and-Vector operators, including:
 *       -- Position - Position (head - tail)
 *       -- Position + Vector (translate Vector to Position and compute head)
 *       -- midpoint (convex linear combination)
 */

#ifndef _EPIX3d_VECTOR
#define _EPIX3d_VECTOR

#include <string>
#include <cmath>

#include "constants.h"
#include "Position.h"

namespace ePiX3d {

  void epix_warn(std::string);

  class Vector {

  private:
    Position basepoint;
    double dX1, dX2, dX3;

  public:  
    // Cartesian constructor
    Vector(Position tail = Position(), 
	   double dx1=0, double dx2=0, double dx3=0)
      { 
	basepoint = tail;
	dX1 = dx1; dX2 = dx2; dX3 = dx3; 
      }

    // component functions
    Position& tail() { return basepoint; }
    //    Position& head() {} // implement...
    double x1() const { return basepoint.x1(); }
    double x2() const { return basepoint.x2(); }
    double x3() const { return basepoint.x3(); }

    double dx1() const { return dX1; }
    double dx2() const { return dX2; }
    double dx3() const { return dX3; }

    // scalar multiplication
    Vector& operator*= (const double& c)
    {
      dX1 *= c;
      dX2 *= c;
      dX3 *= c;

      return (*this); 
    }

    // increment operators
    Vector& operator+= (Vector arg)
    {
      if (tail() != arg.tail())
	epix_warn("Sum of vectors based at different points");

      dX1 += arg.dX1;
      dX2 += arg.dX2;
      dX3 += arg.dX3;
    
      return (*this); 
    }

    Vector& operator-= (Vector arg)
    {
      if (tail() != arg.tail())
	epix_warn("Difference of vectors based at different points");
  
      dX1 -= arg.dX1;
      dX2 -= arg.dX2;
      dX3 -= arg.dX3;

      return (*this); 
    }

    // cross product
    Vector& operator*= (Vector arg)
    {
      if (tail() != arg.tail())
	epix_warn("Cross product of vectors based at different points");

      Vector temp = *this;

      dX1 = (temp.dX2)*(arg.dX3) - (temp.dX3)*(arg.dX2);
      dX2 = (temp.dX3)*(arg.dX1) - (temp.dX1)*(arg.dX3);
      dX3 = (temp.dX1)*(arg.dX2) - (temp.dX2)*(arg.dX1);
      return (*this);
    }

    // dot product
    double operator|= (Vector arg)
    {
      if (tail() != arg.tail())
	epix_warn("Dot product of vectors based at different points");

      double result;
      result += (dX1*arg.dX1);
      result += (dX2*arg.dX2);
      return result += (dX3*arg.dX3);
    }

    // componentwise product
    Vector& operator&= (Vector arg)
    {
      if (tail() != arg.tail())
	epix_warn("Componentwise product of vectors based at different points");

      dX1 *= arg.dX1;
      dX2 *= arg.dX2;
      dX3 *= arg.dX3;

      return (*this); 
    }

    // orthogonalization
    Vector& operator%= (Vector arg)
    {
      if (tail() != arg.tail())
	epix_warn("Orthogonalization of vectors based at different points");

      double norm = 
	(arg.dX1)*(arg.dX1) + (arg.dX2)*(arg.dX2) + (arg.dX3)*(arg.dX3);

      if ( fabs(norm) < EPIX_EPSILON ) 
	return (*this);
      else 
	{
	  double c = (dX1*arg.dX1 + dX2*arg.dX2 + dX3*arg.dX3)/norm;
	  dX1 -= c*arg.dX1;
	  dX2 -= c*arg.dX1;
	  dX3 -= c*arg.dX1;
	  return *this;
	}
    }

  }; // end of class Vector

  // non-member versions
  inline Position tail(Vector arg) { return arg.tail(); }
  //  inline Position head(Vector arg) { return arg.head(); }
  inline double dx1(Vector arg) { return arg.dx1(); }
  inline double dx2(Vector arg) { return arg.dx2(); }
  inline double dx3(Vector arg) { return arg.dx3(); }

  // vector space operators
  Vector& operator+ (Vector arg1, const Vector arg2);
  Vector& operator- (Vector arg1, const Vector arg2);
  Vector& operator- (Vector arg); // unary minus
  Vector& operator* (Vector arg1, const Vector arg2); // cross product
  Vector& operator* (const double c, Vector arg); // scalar multiplication

  // miscellaneous operators
  Vector& operator& (Vector arg1, const Vector arg2); // componentwise product
  Vector& operator% (Vector arg1, const Vector arg2); // orthogonalization
  double operator| (Vector arg1, Vector arg2); // dot product

  double norm (Vector arg);

  // (in)equality
  inline bool operator == (Vector arg1, Vector arg2)
  {
    return ((arg1.dx1() == arg2.dx1()) && 
	    (arg1.dx2() == arg2.dx2()) && 
	    (arg1.dx3() == arg2.dx3()) &&
	    (arg1.tail() == arg2.tail()));
  }
 
  inline bool operator != (Vector arg1, Vector arg2)
  {
    return ((arg1.tail() != arg2.tail()) || 
	    (arg1.dx1() != arg2.dx1()) || 
	    (arg1.dx2() != arg2.dx2()) || 
	    (arg1.dx3() != arg2.dx3()) );
  }

  // Position and Vector affine operations
  // increment operators
  Position operator+=(const Position tail, const Vector displacement)
    {
      Position temp = Position(tail.x1() + displacement.dx1(),
			       tail.x2() + displacement.dx2(),
			       tail.x3() + displacement.dx3());
      return temp;
    }

  Vector operator-=(const Position head, const Position tail)
    {
      Vector temp = Vector(tail, 
			   head.x1()-tail.x1(),
			   head.x2()-tail.x2(),
			   head.x3()-tail.x3());
      return temp;
    }

  // binary operators
  inline Position operator+(Position tail, Vector displacement)
    {
      tail += displacement;
      return tail;
    }

  inline Position& operator+(Vector displacement, Position tail)
    {
      tail += displacement;
      return tail;
    }

  inline Vector operator-(Position head, Position tail)
    {
      head -= tail;
      return Vector(tail, head.x1(), head.x2(), head.x3());
    }

  // convex linear combination
  inline Position midpoint(Position arg1, Position arg2, double t=0.5)
    {
      return arg1 + t*(arg2-arg1);
    }

} /* end of namespace */

#endif /* _EPIX3d_VECTORS */
