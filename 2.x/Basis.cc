/* 
 * Basis.cc -- ePiX2::Basis class and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 31, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#include "Vector.h"
#include "Errors.h"
#include "Constants.h"
#include "Functions.h"
#include "Basis.h"

namespace ePiX2 {

  Basis::Basis(Point loc, Vector v1, Vector v2, Vector v3) 
    : location(loc)
  {
    v1=v1.move_to(loc);
    v2=v2.move_to(loc);
    v3=v3.move_to(loc);

    if (lin_dependent(v1,v2,v3)) // N.B. no graceful attempt to comply
      {
	epix_warning("Linearly independent vectors in basis");
	basis1 = E_1(loc);
	basis2 = E_2(loc);
	basis3 = E_3(loc);
      }

    else
      {
	// Gram-Schmidt constructor, suitable for (sea,sky,eye) bases: 
	// Usually we know sky and eye, and want to preserve eye's direction.
	v3 *= 1/norm(v3);

	v2 = v2%v3;       // orthogonalize
	v2 *= 1/norm(v2); // and normalize

	v1 = (v1%v3)%v2;
	v1 *= 1/norm(v1);

	basis1 = v1;
	basis2 = v2;
	basis3 = v3;
      }
  } // end of Basis()

  // rotations about basis elements; separated from rotate() for efficiency
  void Basis::rotate_sea(double angle)
  {
    Vector temp2 = basis2;
    Vector temp3 = basis3;

    basis2 = (Cos(angle)*(temp2)) - (Sin(angle)*(temp3));
    basis3 = (Sin(angle)*(temp2)) + (Cos(angle)*(temp3));
  }

  void Basis::rotate_sky(double angle)
  {
    Vector temp3 = basis3;
    Vector temp1 = basis1;

    basis3 = (Cos(angle)*(temp3)) - (Sin(angle)*(temp1));
    basis1 = (Sin(angle)*(temp3)) + (Cos(angle)*(temp1));
  }

  void Basis::rotate_eye(double angle)
  {
    Vector temp1 = basis1;
    Vector temp2 = basis2;

    basis1 = (Cos(angle)*(temp1)) - (Sin(angle)*(temp2));
    basis2 = (Sin(angle)*(temp1)) + (Cos(angle)*(temp2));
  }

  void Basis::rotate(const double angle, const Vector& axis)
  {
    if ( fabs(axis|axis) < EPIX2_EPSILON )
      epix_warning("Null axis of rotation, no action"); // return at bottom

    else
      {
	Point old_O = here();
	Point ctr   = axis.tail(); // origin of rotation
	Vector arm  = old_O-ctr;   // our location from center of rotation

	// rotate basis about parallel axis and translate to new location
	Vector tmp_axis = axis;
	tmp_axis.move_to(old_O);

	Basis old_coords; // ON basis with third axis tmp_axis
	Basis new_coords; // same, rotated backward

	if (lin_dependent(tmp_axis, E_3(old_O)))
	  old_coords=Basis(old_O, E_1(old_O), E_2(old_O), tmp_axis);
	else
	  old_coords=Basis(old_O, E_2(old_O), E_3(old_O), tmp_axis);

	// rotate basis backward and use to compute coordinates
	new_coords=old_coords;
	new_coords.rotate_eye(-angle);

	basis1 = (basis1|new_coords.basis1)*old_coords.basis1 +
                 (basis1|new_coords.basis2)*old_coords.basis2 + 
                 (basis1|new_coords.basis3)*old_coords.basis3;

	basis2 = (basis2|new_coords.basis1)*old_coords.basis1 +
                 (basis2|new_coords.basis2)*old_coords.basis2 + 
                 (basis2|new_coords.basis3)*old_coords.basis3;

	basis3 = (basis3|new_coords.basis1)*old_coords.basis1 +
                 (basis3|new_coords.basis2)*old_coords.basis2 + 
                 (basis3|new_coords.basis3)*old_coords.basis3;

	old_coords.move_to(ctr);
	new_coords.move_to(ctr);
	Vector new_arm 
          = (arm|new_coords.basis1)*old_coords.basis1 +
            (arm|new_coords.basis2)*old_coords.basis2 + 
            (arm|new_coords.basis3)*old_coords.basis3;
	  
	// translate everyone to new home
	location=new_arm.head();
	basis1.move_to(location);
	basis2.move_to(location);
	basis3.move_to(location);
      }

  } // end of Basis::rotate(double, Vector)

  void Basis::reflect(const Vector& axis)
  {
    if ( fabs(axis|axis) < EPIX2_EPSILON ) // reflect through tail()
      {
	location = axis.tail() - (location - axis.tail());

	basis1.move_to(location);
	basis2.move_to(location);
	basis3.move_to(location);

	basis1 *= -1;
	basis2 *= -1;
	basis3 *= -1;
      }

    else
      {
	location = location - 2*((location&axis) - axis.tail());
	Vector N=axis;
	N.move_to(location);

	basis1.move_to(location);
	basis2.move_to(location);
	basis3.move_to(location);

	basis1 -= 2*(basis1&N);
	basis2 -= 2*(basis2&N);
	basis3 -= 2*(basis3&N);
      }

  } // end of Basis::reflect(Vector)

  Point Basis::coords(const Point arg) const
  {
    return Point(basis1|(arg-location),
		 basis2|(arg-location),
		 basis3|(arg-location));
  }

} // end of namespace
