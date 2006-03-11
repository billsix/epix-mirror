/* 
 * domain.cc -- ePiX domain slicing operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: March 05, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

#include <vector>

#include "globals.h"
#include "triples.h"
#include "functions.h"

#include "domain.h"

namespace ePiX {

  mesh::mesh(int n1, int n2, int n3)
  {
    mesh1 = (int)max(1, fabs(n1));
    mesh2 = (int)max(1, fabs(n2));
    mesh3 = (int)max(1, fabs(n3));
  }


  domain::domain(P arg1, P arg2, mesh c, mesh f) 
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


  double domain::step1(void) const 
  { 
    return (corner2.x1() - corner1.x1())/coarse.n1();
  }

  double domain::step2(void) const
  { 
    return (corner2.x2() - corner1.x2())/coarse.n2();
  }

  double domain::step3(void) const
  {
    return (corner2.x3() - corner1.x3())/coarse.n3();
  }

  double domain::dx1(void) const
  {
    return (corner2.x1() - corner1.x1())/fine.n1();
  }

  double domain::dx2(void) const
  {
    return (corner2.x2() - corner1.x2())/fine.n2();
  }

  double domain::dx3(void) const
  {
    return (corner2.x3() - corner1.x3())/fine.n3();
  }


  // resizing attempts to preserve real resolution
  domain& domain::resize1(double a1, double b1)
  {
    double ratio = (b1-a1)/(corner2.x1()-corner1.x1());
    corner1 = P(a1,  corner1.x2(), corner1.x3());
    corner2 = P(b1,  corner2.x2(), corner2.x3());

    if (step1() == 0)
      {
	coarse  = mesh(1, coarse.n2(),  coarse.n3());
	fine    = mesh(1,   fine.n2(),    fine.n3());
      }
    else
      {
	int c = (int)floor(ratio*coarse.n1());
	int f = (int)floor(ratio*fine.n1());

	coarse  = mesh(c, coarse.n2(),  coarse.n3());
	fine    = mesh(f,   fine.n2(),    fine.n3());
      }
    return *this;
  }

  domain& domain::resize2(double a2, double b2)
  {
    double ratio = (b2-a2)/(corner2.x2()-corner1.x2());
    corner1 = P(corner1.x1(),  a2, corner1.x3());
    corner2 = P(corner2.x1(),  b2, corner2.x3());
    if (step2() == 0)
      {
	coarse = mesh(coarse.n1(), 1,  coarse.n3());
	fine   = mesh(  fine.n1(), 1,    fine.n3());
      }
    else
      {
	int c = (int)floor(ratio*coarse.n2());
	int f = (int)floor(ratio*fine.n2());

	coarse = mesh(coarse.n1(), c,  coarse.n3());
	fine   = mesh(  fine.n1(), f,    fine.n3());
      }
    return *this;
  }

  domain& domain::resize3(double a3, double b3)
  {
    double ratio = (b3-a3)/(corner2.x3()-corner1.x3());
    corner1 = P(corner1.x1(),  corner1.x2(), a3);
    corner2 = P(corner2.x1(),  corner2.x2(), b3);
    if (step3() == 0)
      {
	coarse = mesh(coarse.n1(), coarse.n2(),  1);
	fine   = mesh(  fine.n1(),   fine.n2(),  1);
      }
    else
      {
	int c = (int)floor(ratio*coarse.n3());
	int f = (int)floor(ratio*fine.n3());

	coarse = mesh(coarse.n1(), coarse.n2(),  c);
	fine   = mesh(  fine.n1(),   fine.n2(),  f);
      }
    return *this;
  }


  // "snip_to" is defined in globals.h and performs the "obvious"
  // truncation: snip_to(x, a, b) = x, min(a,b), or max(a,b)

  // one slice
  domain domain::slice1(double a1) const
  {
    a1 = snip_to(a1, corner1.x1(), corner2.x1());

    return domain(P(a1,  corner1.x2(), corner1.x3()),
		  P(a1,  corner2.x2(), corner2.x3()),
		  mesh(1, coarse.n2(),  coarse.n3()),
		  mesh(1,   fine.n2(),    fine.n3()));
  }

  domain domain::slice2(double a2) const
  {
    a2 = snip_to(a2, corner1.x2(), corner2.x2());

    return domain(P(corner1.x1(),  a2, corner1.x3()),
		  P(corner2.x1(),  a2, corner2.x3()),
		  mesh(coarse.n1(), 1,  coarse.n3()),
		  mesh(  fine.n1(), 1,    fine.n3()));
  }

  domain domain::slice3(double a3) const
  {
    a3 = snip_to(a3, corner1.x3(), corner2.x3());

    return domain(P(corner1.x1(),  corner1.x2(), a3),
		  P(corner2.x1(),  corner2.x2(), a3),
		  mesh(coarse.n1(), coarse.n2(),  1),
		  mesh(  fine.n1(),   fine.n2(),  1));
  }


  // coordinate slices
  std::vector<domain> domain::slices1(void) const
  {
    std::vector<domain> temp(1+coarse.n1());

    for (int i=0; i <= coarse.n1(); ++i)
      temp.at(i) = slice1(corner1.x1() + i*step1());

    return temp;
  }

  std::vector<domain> domain::slices2(void) const
  {
    std::vector<domain> temp(1+coarse.n2());

    for (int j=0; j <= coarse.n2(); ++j)
      temp.at(j) = slice2(corner1.x2() + j*step2());

    return temp;
  }

  std::vector<domain> domain::slices3(void) const
  {
    std::vector<domain> temp(1+coarse.n3());

    for (int k=0; k <= coarse.n3(); ++k)
      temp.at(k) = slice3(corner1.x3() + k*step3());

    return temp;
  }

} // end of namespace
