/***
 ***  Mapping.h -- epix2::Mapping class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 16, 2005
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
 *** This file provides:
 ***   The Mapping class, which represents functions as Point -> Point maps.
 ***   A Mapping is built from one of five types of user-defined function:
 ***   - double -> double or Point (map_1_1 or map_1_3)
 ***   - (double, double) -> double or Point (map_2_1 or map_2_3)
 ***   - Point -> Point (map_3_3).
 ***
 ***   Mappings can be composed with syntax f(g).
 ***/

#ifndef EPIX2_MAPPING
#define EPIX2_MAPPING

#include "Point.h"

namespace ePiX2 {

  /* * * Mapping.h * * */

  class Mapping {

    // map_n_m represents a mapping from R^n to R^m
    enum epix2_map_type { map_1_1, map_2_1, map_1_3, map_2_3, map_3_3 };

  public:

    Mapping(double f(double))
      {
	flag=map_1_1;
	f11=f;
	inner = &Mapping::Identity;
      }

    Mapping(double f(double, double))
      {
	flag=map_2_1;
	f21=f;
	inner = &Mapping::Identity;
      }

    Mapping(Point f(double))
      {
	flag=map_1_3;
	f13=f;
	inner = &Mapping::Identity;
      }

    Mapping(Point f(double, double))
      {
	flag=map_2_3;
	f23=f;
	inner = &Mapping::Identity;
      }

    Mapping(Point f(Point))
      {
	flag=map_3_3;
	f33=f;
	inner = &Mapping::Identity;
      }

    // evaluation
    Point operator() (const Point&);

    // composition
    Mapping operator() (Mapping& inner);

    static Mapping Identity;

  private:

    epix2_map_type flag;

    double (*f11)(double);
    double (*f21)(double, double);
    Point (*f13)(double);
    Point (*f23)(double, double);
    Point (*f33)(Point);

    Mapping* inner; // precompose with this when evaluating

  }; // end of class Mapping

} /* end of namespace */


#endif /* EPIX2_HEADER */
