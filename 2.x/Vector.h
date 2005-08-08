/*** 
 ***  Vector.h -- epix2::Vector class and operators
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 01, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 ***
 ***   This file provides:
 ***
 *** A Vector is conceptually a displacement in 3-dimensional affine
 *** space. Vectors are represented as a basepoint and a triple of numbers.
 *** This file provides:constructors
 ***   - basepoint and components: Vector(tail, Vx1, Vx2, Vx3)
 ***   - tail and head; Vector(tail, head)
 ***   - Standard basis (at origin) and orthonormal frame (arbitrary basepoint)
 ***  Component functions
 ***   - x1(), dx1(), etc.
 ***  Point/Vector manipulators
 ***   - head(), tail();
 ***   - move_to(Point arg) (translate tail to arg)
 ***   - Point - Point = Vector; Point + Vector = Point;
 ***  algebraic operators (for vectors with same basepoint)
 ***   - Vector space operations (sum, diff, scalar mult)
 ***   - (In)equality, linear dependence
 ***   - Euclidean operations (dot product (v|w), norm, 
 ***     orthogonalization (v%w))
 ***   - Misc. operations (cross product (v*w), componentwise product (v&w))
 ***/

#ifndef EPIX2_VECTOR
#define EPIX2_VECTOR

#include "Point.h"
#include "Errors.h"

namespace ePiX2 {

  class Edge; /***/

  /* * * Vector.h * * */

  class Vector {

  private:

    Point basepoint;
    double Vx1, Vx2, Vx3; // components

  public:  
    // Cartesian constructor
    Vector(const Point& tail = Origin, 
	   double dx1=0, double dx2=0, double dx3=0)
      : basepoint(tail), Vx1(dx1), Vx2(dx2), Vx3(dx3) { }

    Vector(const Point& tail, const Point& head)
      {
	basepoint = tail;
	Vx1 = head.Px1-tail.Px1;
	Vx2 = head.Px2-tail.Px2;
	Vx3 = head.Px3-tail.Px3;
      }

    // component functions
    Point tail(void) const { return basepoint; }
    Point head(void) const
      { 
	double x1 = basepoint.Px1 + Vx1;
	double x2 = basepoint.Px2 + Vx2;
	double x3 = basepoint.Px3 + Vx3;

	return Point(x1, x2, x3);
      }

    double x1(void) const { return basepoint.Px1; }
    double x2(void) const { return basepoint.Px2; }
    double x3(void) const { return basepoint.Px3; }

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

    // increment operators
    Vector& operator+= (const Vector arg)
      {
	if (basepoint != arg.basepoint)
	  epix_warning("Implicit parallel translation");

	Vx1 += arg.Vx1;
	Vx2 += arg.Vx2;
	Vx3 += arg.Vx3;
    
	return (*this); 
      }

    Vector& operator-= (const Vector arg)
      {
	if (basepoint != arg.basepoint)
	  epix_warning("Implicit parallel translation");
  
	Vx1 -= arg.Vx1;
	Vx2 -= arg.Vx2;
	Vx3 -= arg.Vx3;

	return (*this); 
      }

    // cross product
    Vector& operator*= (const Vector arg)
      {
	if (basepoint != arg.basepoint)
	  epix_warning("Implicit parallel translation");

	Vector temp = *this;

	Vx1 = (temp.Vx2)*(arg.Vx3) - (temp.Vx3)*(arg.Vx2);
	Vx2 = (temp.Vx3)*(arg.Vx1) - (temp.Vx1)*(arg.Vx3);
	Vx3 = (temp.Vx1)*(arg.Vx2) - (temp.Vx2)*(arg.Vx1);
	return (*this);
      }

    // dot product
    friend double operator| (const Vector arg1, const Vector arg2);

    // projection to arg
    Vector& operator&= (Vector arg)
      {
	if (basepoint != arg.basepoint)
	  {
	    epix_warning("Implicit translation in projection");
	    arg.move_to(basepoint);
	  }

	double c = ((*this)|arg)/(arg|arg);

	Vx1 = c*arg.Vx1;
	Vx2 = c*arg.Vx2;
	Vx3 = c*arg.Vx3;

	return (*this); 
      }

    // projection to arg(perp)
    Vector& operator%= (Vector arg)
      {
	if (basepoint != arg.basepoint)
	  {
	    epix_warning("Implicit translation in orthogonal projection");
	    arg.move_to(basepoint);
	  }

	double c = ((*this)|arg)/(arg|arg);

	Vx1 -= c*arg.Vx1;
	Vx2 -= c*arg.Vx2;
	Vx3 -= c*arg.Vx3;
	return *this;
      }

    double height(const Point&) const;

    // intersection of perp with E
    Point perp_hits(const Point&, const Point&) const;
    Point perp_hits(const Edge& E) const;

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
  inline Vector operator+ (const Vector arg1, const Vector arg2) 
    { 
      Vector temp  = arg1;
      return temp += arg2; 
    }
  inline Vector operator- (const Vector arg1, const Vector arg2) 
    { 
      Vector temp  = arg1;
      return temp -= arg2; 
    }
  inline Vector operator- (const Vector arg) 
    { 
      Vector temp  = arg;
      return temp *= -1; 
    }

  // cross product
  inline Vector operator* (const Vector arg1, const Vector arg2) 
    { 
      Vector temp  = arg1;
      return temp *= arg2; 
    }

  // scalar multiplication
  inline Vector operator* (const double c, const Vector v) 
    { 
      Vector temp  = v;
      return temp *= c; 
    }

  // componentwise product
  inline Vector operator& (const Vector arg1, const Vector arg2) 
    { 
      Vector temp  = arg1;
      return temp &= arg2; 
    }

  // orthogonalization: subtract arg2 component from arg1
  inline Vector operator% (const Vector arg1, const Vector arg2) 
    { 
      Vector temp  = arg1;
      return temp %= arg2; 
    }

  inline double norm(const Vector arg) { return sqrt(arg|arg); }

  // (in)equality
  inline bool operator == (const Vector arg1, const Vector arg2)
    {
      return (arg1.tail() == arg2.tail() && norm(arg2-arg1) < EPIX2_EPSILON);
    }
 
  inline bool operator != (const Vector arg1, const Vector arg2)
    {
      return !(arg1 == arg2);
    }

  // Point and Vector affine operations

  // head - tail = Vector(tail, head)
  inline Vector operator- (const Point& head, const Point& tail)
    {
      return Vector(tail, head);
    }

  // binary operators
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
  inline Point midpoint(const Point arg1, const Point arg2, const double t=0.5)
    {
      return arg1 + t*(arg2-arg1);
    }

  // Linear dependence
  bool lin_dependent (const Vector&, const Vector&);
  bool lin_dependent (const Vector&, const Vector&, const Vector&);

} /* end of namespace */

#endif /* EPIX2_VECTOR */
