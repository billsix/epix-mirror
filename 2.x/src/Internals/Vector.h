/*** 
 ***  Vector.h -- epix2::Vector class and operators
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 12, 2006
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 ***/
/* 
 * Vector.h
 *
 * This file provides the Vector class, conceptually a displacement in 
 * 3-dimensional affine space, implemented as a basepoint and components.
 *
 *  Constructors
 *   - basepoint and components: Vector(tail, Vx1, Vx2, Vx3)
 *   - tail and head; Vector(tail, head)
 *   - Standard basis (at origin)
 *
 *  Position and momenta:
 *   - x1(), dx1(), etc.
 *   - translate tail to arg: move_to(Point arg)
 *   - projected length of (arg-tail): height(Point arg)
 *
 *  Point/Vector manipulators
 *   - head(), tail();
 *   - Point - Point = Vector; Point + Vector = Point;
 *   - convex linear combination: midpoint(Point, Point, double=0.5);
 *
 *  Algebraic operators for vectors with same basepoint
 *   - Vector sum, difference, scalar multiplication
 *   - Euclidean operations 
 *     dot product (v|w)
 *     norm
 *     cross product (v*w)
 *   - (In)equality, linear dependence
 *
 *  Affine operations with respect to line/plane through w.tail()
 *     orthogonalization (v%=w: v projected to w perp)
 *     reflection        (v^=w: v reflected in w perp)
 *     projection        (v&=w: v projected to w)
 */

#ifndef EPIX2_VECTOR
#define EPIX2_VECTOR

#include "Point.h"

namespace ePiX2 {
  //  class Edge; /***/
  class Vector {

  public:  
    // Cartesian constructor
    explicit Vector(const Point& tail = Origin, 
		    double dx1=0, double dx2=0, double dx3=0)
      : basepoint(tail), Vx1(dx1), Vx2(dx2), Vx3(dx3) { }

    Vector(const Point& tail, const Point& head);

    // component functions
    Point tail(void) const { return basepoint; }
    Point head(void) const;

    double x1(void) const { return basepoint.x1(); }
    double x2(void) const { return basepoint.x2(); }
    double x3(void) const { return basepoint.x3(); }

    double dx1(void) const { return Vx1; }
    double dx2(void) const { return Vx2; }
    double dx3(void) const { return Vx3; }

    // affine translation
    Vector& move_to(const Point arg) { basepoint = arg; return (*this); }

    // scalar multiplication
    Vector& operator*= (const double& c)
      {
	Vx1 *= c;
	Vx2 *= c;
	Vx3 *= c;

	return (*this); 
      }

    // vector addition/subtraction
    Vector& operator+= (const Vector arg);
    Vector& operator-= (const Vector arg);

    // cross product
    Vector& operator*= (const Vector arg);

    // dot product
    friend double operator| (const Vector arg1, const Vector arg2);

    // affine projection to line spanned by arg
    Vector& operator&= (Vector arg);

    // affine projection to plane perp to arg
    Vector& operator%= (Vector arg);

    // reflection in plane perp to arg
    Vector& operator^= (Vector arg);

    double height(const Point&) const;

    // intersection of perp with E
    //    Point perp_hits(const Point&, const Point&) const;
    //    Point perp_hits(const Edge& E) const;

  private:

    Point basepoint;
    double Vx1, Vx2, Vx3; // components

  }; // end of class Vector

  // Standard basis
  inline Vector E_1(const Point base=Origin) { return Vector(base, 1, 0, 0); }
  inline Vector E_2(const Point base=Origin) { return Vector(base, 0, 1, 0); }
  inline Vector E_3(const Point base=Origin) { return Vector(base, 0, 0, 1); }

  // Positions and components
  inline Point tail(const Vector arg) { return arg.tail(); }
  inline Point head(const Vector arg) { return arg.head(); }

  inline double dx1(const Vector arg) { return arg.dx1(); }
  inline double dx2(const Vector arg) { return arg.dx2(); }
  inline double dx3(const Vector arg) { return arg.dx3(); }

  // vector space operations; warning if vectors not based at same point
  inline Vector operator+ (Vector arg1, const Vector arg2) 
    {
      return arg1 += arg2;
    }
  inline Vector operator- (Vector arg1, const Vector arg2) 
    {
      return arg1 -= arg2;
    }
  inline Vector operator- (Vector arg) 
    {
      return arg *= -1;
    }

  // cross product
  inline Vector operator* (Vector arg1, const Vector arg2) 
    {
      return arg1 *= arg2;
    }

  // scalar multiplication
  inline Vector operator* (const double c, Vector v) 
    {
      return v *= c;
    }

  inline double norm(const Vector arg) { return sqrt(arg|arg); }

  // (in)equality
  bool operator == (const Vector arg1, const Vector arg2);
  bool operator != (const Vector arg1, const Vector arg2);

  // Linear dependence
  bool lin_dependent (const Vector&, const Vector&);
  bool lin_dependent (const Vector&, const Vector&, const Vector&);


  /* Affine operations */
  // project arg1 onto arg2
  inline Vector operator& (Vector arg1, const Vector arg2) 
    {
      return arg1 &= arg2;
    }

  // orthogonalization: project arg1 to arg2 perp
  inline Vector operator% (Vector arg1, const Vector arg2) 
    {
      return arg1 %= arg2;
    }

  // reflect arg1 in arg2 perp
  inline Vector operator^ (Vector arg1, const Vector arg2) 
    {
      return arg1 ^= arg2;
    }


  // Point - Point
  inline Vector operator- (const Point& head, const Point& tail)
    {
      return Vector(tail, head);
    }

  // Point +/- Vector
  inline Point operator+ (const Point tail, Vector displacement)
    {
      displacement.move_to(tail);
      return displacement.head();
    }

  inline Point operator- (const Point tail, Vector displacement)
    {
      displacement *= -1;
      displacement.move_to(tail);
      return displacement.head();
    }

  // convex linear combination
  Point midpoint(const Point arg1, const Point arg2, const double t=0.5);

} /* end of namespace */

#endif /* EPIX2_VECTOR */
