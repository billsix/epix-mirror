/* 
 * Vector.cc -- ePiX2::Vector class and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: December 20, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

#include "Constants.h"
#include "Errors.h"
#include "Functions.h"
//#include "Edge.h"
#include "Vector.h"

namespace ePiX2 {

  Vector::Vector(const Point& tail, const Point& head)
  {
    basepoint = tail;
    Vx1 = head.x1()-tail.x1();
    Vx2 = head.x2()-tail.x2();
    Vx3 = head.x3()-tail.x3();
  }

  // component functions
  Point Vector::head(void) const
  { 
    double x1 = basepoint.x1() + Vx1;
    double x2 = basepoint.x2() + Vx2;
    double x3 = basepoint.x3() + Vx3;

    return Point(x1, x2, x3);
  }

  // increment operators
  Vector& Vector::operator+= (const Vector arg)
  {
    if (basepoint != arg.basepoint)
      epix_warning("Implicit parallel translation");

    Vx1 += arg.Vx1;
    Vx2 += arg.Vx2;
    Vx3 += arg.Vx3;
    
    return (*this); 
  }

  Vector& Vector::operator-= (const Vector arg)
  {
    if (basepoint != arg.basepoint)
      epix_warning("Implicit parallel translation");
  
    Vx1 -= arg.Vx1;
    Vx2 -= arg.Vx2;
    Vx3 -= arg.Vx3;

    return (*this); 
  }

  // cross product
  Vector& Vector::operator*= (const Vector arg)
  {
    if (basepoint != arg.basepoint)
      epix_warning("Implicit parallel translation");

    Vector temp = *this;

    Vx1 = (temp.Vx2)*(arg.Vx3) - (temp.Vx3)*(arg.Vx2);
    Vx2 = (temp.Vx3)*(arg.Vx1) - (temp.Vx1)*(arg.Vx3);
    Vx3 = (temp.Vx1)*(arg.Vx2) - (temp.Vx2)*(arg.Vx1);
    return (*this);
  }

  // projection to arg
  Vector& Vector::operator&= (Vector arg)
  {
    /** Commented versions work only if arg.tail() == tail() **/
    /*
    if (basepoint != arg.basepoint)
      {
	epix_warning("Implicit translation in projection");
	arg.move_to(basepoint);
      }

    double denom=(arg|arg);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    double c = ((*this)|arg)/denom;

    Vx1 = c*arg.Vx1;
    Vx2 = c*arg.Vx2;
    Vx3 = c*arg.Vx3;
    */
    double denom=(arg|arg);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    Vector temp = (head()&arg) - (tail()&arg);
    *this = temp;
    return (*this); 
  }

  // projection to arg(perp)
  Vector& Vector::operator%= (Vector arg)
  {
    /*
    if (basepoint != arg.basepoint)
      {
	epix_warning("Implicit translation in orthogonal projection");
	arg.move_to(basepoint);
      }

    double denom=(arg|arg);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    double c = ((*this)|arg)/(arg|arg);

    Vx1 -= c*arg.Vx1;
    Vx2 -= c*arg.Vx2;
    Vx3 -= c*arg.Vx3;
    return *this;
    */
    double denom=(arg|arg);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    Vector temp = (head()%arg) - (tail()%arg);
    *this = temp;
    return *this;
  }

  // reflection in arg(perp)
  Vector& Vector::operator^= (Vector arg)
  {
    /*
    if (basepoint != arg.basepoint)
      {
	epix_warning("Implicit translation in reflection");
	arg.move_to(basepoint);
      }

    double denom=(arg|arg);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    double c = 2*((*this)|arg)/denom;

    Vx1 -= c*arg.Vx1;
    Vx2 -= c*arg.Vx2;
    Vx3 -= c*arg.Vx3;
    return *this;
    */
    double denom=(arg|arg);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    Vector temp = (head()^arg) - (tail()^arg);
    *this = temp;
    return *this;
  }

  bool operator == (const Vector arg1, const Vector arg2)
  {
    return (arg1.tail() == arg2.tail() && norm(arg2-arg1) < EPIX2_EPSILON);
  }

  bool operator != (const Vector arg1, const Vector arg2)
  {
    return !(arg1 == arg2);
  }
 
  double operator| (const Vector arg1, const Vector arg2)
  {
    if (arg1.basepoint != arg2.basepoint)
      epix_warning("Implicit translation in dot product");

    return 
      (arg1.Vx1)*(arg2.Vx1) + 
      (arg1.Vx2)*(arg2.Vx2) + 
      (arg1.Vx3)*(arg2.Vx3);
  }

  Point midpoint(const Point arg1, const Point arg2, const double t)
    {
      return arg1 + t*(arg2-arg1);
    }

  // Linear dependence
  bool lin_dependent (const Vector& v1, const Vector& v2)
  {
    return (norm(v1*v2) <= EPIX2_EPSILON*norm(v1)*norm(v2));
  }

  bool lin_dependent (const Vector& v1, const Vector& v2, const Vector& v3)
  {
    return (fabs((v1*v2)|v3) <= EPIX2_EPSILON*norm(v1)*norm(v2)*norm(v3));
  }


  double Vector::height(const Point& arg) const
  {
    Vector N = (*this);
    N *= recip(norm(N)); // unit, or 0
    return N|(arg-N.tail());
  }

  // intersection of perp with E
  /*
  Point Vector::perp_hits(const Point& q1, const Point& q2) const
  {
    Vector dir=q2-q1;
    dir.move_to(basepoint);
    double t=-((q1-basepoint)|(*this))/(dir|(*this));

    return q1 + t*dir;
  }
  */

  /*
  Point Vector::perp_hits(const Edge& E) const
  {
    return perp_hits(E.first, E.second);
  }
  */

}; /* end of namespace ePiX2 */
