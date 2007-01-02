/* 
 * domain.cc -- ePiX domain slicing operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.1
 * Last Change: January 01, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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

#include <list>
#include <iostream>

#include "globals.h"
#include "triples.h"
#include "functions.h"

#include "domain.h"

namespace ePiX {

  typedef std::list<domain>::const_iterator dolci;

  mesh::mesh(int n1, int n2, int n3)
    : n_1((int)max(1, fabs(n1))),
      n_2((int)max(1, fabs(n2))),
      n_3((int)max(1, fabs(n3))) { }

  mesh::mesh(int n)
    : n_1((int)max(1, fabs(n))),
      n_2((int)max(1, fabs(n))),
      n_3((int)max(1, fabs(n))) { }

  mesh::mesh() : n_1(1), n_2(1), n_3(1) { }


  domain::domain(P arg1, P arg2, mesh c, mesh f) 
    : corner1(arg1), corner2(arg2)
  {
    int c1(c.n1()), c2(c.n2()), c3(c.n3());
    int f1(f.n1()), f2(f.n2()), f3(f.n3());

    if (corner1.x1() == corner2.x1()) { c1 = f1 = 1; }
    if (corner1.x2() == corner2.x2()) { c2 = f2 = 1; }
    if (corner1.x3() == corner2.x3()) { c3 = f3 = 1; }

    coarse = mesh(c1,c2,c3);
    fine   = mesh(f1,f2,f3);
  }


  int domain::dim() const
  {
    int D(0);
    if (fabs(corner2.x1() - corner1.x1()) > EPIX_EPSILON)
      ++D;
    if (fabs(corner2.x2() - corner1.x2()) > EPIX_EPSILON)
      ++D;
    if (fabs(corner2.x3() - corner1.x3()) > EPIX_EPSILON)
      ++D;

    return D;
  }

  double domain::step1() const
  {
    return (corner2.x1() - corner1.x1())/coarse.n1();
  }

  double domain::step2() const
  {
    return (corner2.x2() - corner1.x2())/coarse.n2();
  }

  double domain::step3() const
  {
    return (corner2.x3() - corner1.x3())/coarse.n3();
  }

  double domain::dx1() const
  {
    return (corner2.x1() - corner1.x1())/fine.n1();
  }

  double domain::dx2() const
  {
    return (corner2.x2() - corner1.x2())/fine.n2();
  }

  double domain::dx3() const
  {
    return (corner2.x3() - corner1.x3())/fine.n3();
  }


  // resizing attempts to preserve real resolution
  domain domain::resize1(double a1, double b1) const
  {
    P new_corner1(a1, corner1.x2(), corner1.x3());
    P new_corner2(b1, corner2.x2(), corner2.x3());

    double ratio((b1-a1)/(corner2.x1()-corner1.x1()));
    mesh new_c((int)floor(ratio*coarse.n1()), coarse.n2(),  coarse.n3());
    mesh new_f((int)floor(ratio*fine.n1()),   fine.n2(),    fine.n3());

    if (step1() == 0)
      {
	new_c = mesh(1, coarse.n2(),  coarse.n3());
	new_f = mesh(1,   fine.n2(),    fine.n3());
      }

    return domain(new_corner1, new_corner2, new_c, new_f);
  }

  domain domain::resize2(double a2, double b2) const
  {
    P new_corner1(corner1.x1(),  a2, corner1.x3());
    P new_corner2(corner2.x1(),  b2, corner2.x3());

    double ratio((b2-a2)/(corner2.x2()-corner1.x2()));
    mesh new_c(coarse.n1(), (int)floor(ratio*coarse.n2()),  coarse.n3());
    mesh new_f(  fine.n1(), (int)floor(ratio*fine.n2()),    fine.n3());

    if (step2() == 0)
      {
	new_c = mesh(coarse.n1(), 1,  coarse.n3());
	new_f = mesh(  fine.n1(), 1,    fine.n3());
      }

    return domain(new_corner1, new_corner2, new_c, new_f);
  }

  domain domain::resize3(double a3, double b3) const
  {
    P new_corner1(corner1.x1(),  corner1.x2(), a3);
    P new_corner2(corner2.x1(),  corner2.x2(), b3);

    double ratio((b3-a3)/(corner2.x3()-corner1.x3()));
    mesh new_c(coarse.n1(), coarse.n2(), (int)floor(ratio*coarse.n3()));
    mesh new_f(  fine.n1(),   fine.n2(), (int)floor(ratio*fine.n3()));

    if (step3() == 0)
      {
	new_c = mesh(coarse.n1(), coarse.n2(),  1);
	new_f = mesh(  fine.n1(),   fine.n2(),  1);
      }

    return domain(new_corner1, new_corner2, new_c, new_f);
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
  std::list<domain> domain::slices1(const unsigned int n) const
  {
    unsigned int N(coarse.n1());
    double du(step1());

    if (n > 0)
      {
	du *= N*1.0/n;
	N = n;
      }

    std::list<domain> val;

    for (unsigned int i=0; i <= N; ++i)
      val.push_back(slice1(corner1.x1() + i*du));

    return val;
  }

  std::list<domain> domain::slices2(const unsigned int n) const
  {
    unsigned int N(coarse.n2());
    double du(step2());

    if (n > 0)
      {
	du *= N*1.0/n;
	N = n;
      }

    std::list<domain> val;

    for (unsigned int i=0; i <= N; ++i)
      val.push_back(slice2(corner1.x2() + i*du));

    return val;
  }

  std::list<domain> domain::slices3(const unsigned int n) const
  {
    unsigned int N(coarse.n3());
    double du(step3());

    if (n > 0)
      {
	du *= N*1.0/n;
	N = n;
      }

    std::list<domain> val;

    for (unsigned int i=0; i <= N; ++i)
      val.push_back(slice3(corner1.x3() + i*du));

    return val;
  }

  domain_list::domain_list(std::list<domain> arg)
    : m_list(arg) { }

  domain_list& domain_list::add(const domain& arg)
  {
    m_list.push_back(arg);
    return *this;
  }

  domain_list& domain_list::add(const domain_list& arg)
  {
    // Less efficient than m_list.splice(m_list.end(), arg), but preserves arg
    for (dolci p=arg.m_list.begin(); p != arg.m_list.end(); ++p)
      m_list.push_back(*p);

    return *this;
  }
} // end of namespace
