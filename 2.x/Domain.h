/***
 ***  Domain.h -- epix2::Domain and slice functions
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 13, 2005
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
 *** ***
 *** ***
 *** A Domain is a gridded coordinate box with specified corners
 *** and coarse/fine meshes, used for plotting. The "coarse" step
 *** sizes are step1, etc., and the "fine" step sizes are dx1, etc.
 *** Steps may be negative.
 ***
 *** A Domain may be "sliced" by setting one of its coordinates to
 *** a specified constant.
 ***
 *** Data members are public for convenience; each member ensures
 *** its own well-constructedness.
 ***/

#ifndef EPIX2_DOMAIN
#define EPIX2_DOMAIN

#include <vector>

#include "Point.h"

namespace ePiX2 {

  /* * * Domain.h * * */

  class Mesh {

  public:
    Mesh(int n1=1, int n2=1, int n3=1)
      {
	mesh1 = (int)max(1, abs(n1));
	mesh2 = (int)max(1, abs(n2));
	mesh3 = (int)max(1, abs(n3));
      }

    int n1(void) const { return mesh1; }
    int n2(void) const { return mesh2; }
    int n3(void) const { return mesh3; }

  private:
    int mesh1;
    int mesh2;
    int mesh3;

  }; // end of class Mesh


  struct Domain {

    Point corner1;
    Point corner2;
    Mesh coarse;
    Mesh fine;

    Domain(const Point& arg1=Point(1,1,1), const Point& arg2=Origin, 
	   const Mesh& c=Mesh(1,1,1), const Mesh& f=Mesh(1,1,1));

    Domain(double t_min, double t_max, int num_pts)
      : corner1(t_min), corner2(t_max), coarse(num_pts), fine(num_pts) { }

    // big and small steps
    double step1(void) const;
    double step2(void) const;
    double step3(void) const;

    double dx1(void) const;
    double dx2(void) const;
    double dx3(void) const;

    // resizing attempts to preserve real resolution
    Domain& resize1(double a1, double b1);
    Domain& resize2(double a2, double b2);
    Domain& resize3(double a3, double b3);

    // coordinate slices
    Domain slice1(double a1) const;
    Domain slice2(double a2) const;
    Domain slice3(double a3) const;

    std::vector<Domain> slices1(void) const;
    std::vector<Domain> slices2(void) const;
    std::vector<Domain> slices3(void) const;

  }; // end of struct Domain

} /* end of namespace */

#endif /* EPIX2_DOMAIN */
