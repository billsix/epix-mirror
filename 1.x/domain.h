/* 
 * domain.h -- ePiX mesh and domain classes
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc13
 * Last Change: July 12, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef EPIX_DOMAIN
#define EPIX_DOMAIN

#include <vector>

#include "globals.h"
#include "triples.h"
#include "functions.h"

namespace ePiX {

  class mesh {
  private:
    int mesh1;
    int mesh2;
    int mesh3;

  public:
    mesh(int n1=1, int n2=1, int n3=1)
      {
	mesh1 = (int)max(1, fabs(n1));
	mesh2 = (int)max(1, fabs(n2));
	mesh3 = (int)max(1, fabs(n3));
      }

    int n1(void) { return mesh1; }
    int n2(void) { return mesh2; }
    int n3(void) { return mesh3; }

  }; // end of class mesh

  typedef mesh Net; // compatibility

  // A domain is a gridded coordinate box with specified corners
  // and coarse/fine meshes, used for plotting. The "coarse" step
  // sizes are step1, etc., and the "fine" step sizes are dx1, etc.
  // Steps may be negative.
  //
  // A domain may be "sliced" by setting one of its coordinates to
  // a specified constant.
  //
  // Data members are public for convenience; each member ensures
  // its own well-constructedness.
  //
  class domain {
  public:
    P corner1;
    P corner2;
    mesh coarse;
    mesh fine;

    domain(P arg1=P(1,1,1), P arg2=P(0,0,0), 
	   mesh c=mesh(1,1,1), mesh f=mesh(1,1,1)) 
      : corner1(arg1), corner2(arg2)
      {
	int c1=c.n1(), c2=c.n2(), c3=c.n3();
	int f1=f.n1(), f2=f.n2(), f3=f.n3();

	if (corner1.x1() == corner2.x1()) { c1 = f1 = 1; }
	if (corner1.x2() == corner2.x2()) { c2 = f2 = 1; }
	if (corner1.x3() == corner2.x3()) { c3 = f3 = 1; }

	coarse = mesh(c1,c2,c3);
	fine   = mesh(f1,f2,f3);
      }

    domain(double t_min, double t_max, int num_pts)
      : corner1(t_min), corner2(t_max), coarse(num_pts), fine(num_pts) {}

    // big and small steps
    double step1(void) { return (corner2.x1() - corner1.x1())/coarse.n1(); }
    double step2(void) { return (corner2.x2() - corner1.x2())/coarse.n2(); }
    double step3(void) { return (corner2.x3() - corner1.x3())/coarse.n3(); }

    double dx1(void) { return (corner2.x1() - corner1.x1())/fine.n1(); }
    double dx2(void) { return (corner2.x2() - corner1.x2())/fine.n2(); }
    double dx3(void) { return (corner2.x3() - corner1.x3())/fine.n3(); }

    // resizing attempts to preserve real resolution
    domain& resize1(double a1, double b1);
    domain& resize2(double a2, double b2);
    domain& resize3(double a3, double b3);

    // coordinate slices
    // "snip_to" is defined in globals.h and performs the "obvious"
    // truncation: snip_to(x, a, b) = x, min(a,b), or max(a,b)
    domain slice1(double a1)
      {
	a1 = snip_to(a1, corner1.x1(), corner2.x1());

	return domain(P(a1,  corner1.x2(), corner1.x3()),
		      P(a1,  corner2.x2(), corner2.x3()),
		      mesh(1, coarse.n2(),  coarse.n3()),
		      mesh(1,   fine.n2(),    fine.n3()));
      }

    domain slice2(double a2)
      {
	a2 = snip_to(a2, corner1.x2(), corner2.x2());

	return domain(P(corner1.x1(),  a2, corner1.x3()),
		      P(corner2.x1(),  a2, corner2.x3()),
		      mesh(coarse.n1(), 1,  coarse.n3()),
		      mesh(  fine.n1(), 1,    fine.n3()));
      }

    domain slice3(double a3)
      {
	a3 = snip_to(a3, corner1.x3(), corner2.x3());

	return domain(P(corner1.x1(),  corner1.x2(), a3),
		      P(corner2.x1(),  corner2.x2(), a3),
		      mesh(coarse.n1(), coarse.n2(),  1),
		      mesh(  fine.n1(),   fine.n2(),  1));
      }

    std::vector<domain> slices1(void);
    std::vector<domain> slices2(void);
    std::vector<domain> slices3(void);

  }; // end of class domain

} // end of namespace

#endif /* EPIX_DOMAIN */
