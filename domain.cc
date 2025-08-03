/* 
 * domain.cc -- ePiX domain slicing operations
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.10
 * Last Change: August09, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <cmath>
#include <list>
#include <iostream>

#include "constants.h"
#include "triples.h"
#include "functions.h"

#include "domain.h"

namespace ePiX {

  typedef std::list<domain>::const_iterator dolci;
  const double EPS(EPIX_EPSILON);

  mesh::mesh(int n1, int n2, int n3)
    : n_1((int)max(1, fabs(n1))),
      n_2((int)max(1, fabs(n2))),
      n_3((int)max(1, fabs(n3))) { }

  mesh::mesh(int n)
    : n_1((int)max(1, fabs(n))),
      n_2((int)max(1, fabs(n))),
      n_3((int)max(1, fabs(n))) { }

  mesh::mesh() : n_1(1), n_2(1), n_3(1) { }


  domain::domain(const P& arg1, const P& arg2, const mesh& c, const mesh& f)
    : m_corner1(arg1), m_corner2(arg2)
  {
    int c1(c.n1()), c2(c.n2()), c3(c.n3());
    int f1(f.n1()), f2(f.n2()), f3(f.n3());

    if (m_corner1.x1() == m_corner2.x1()) { c1 = f1 = 1; }
    if (m_corner1.x2() == m_corner2.x2()) { c2 = f2 = 1; }
    if (m_corner1.x3() == m_corner2.x3()) { c3 = f3 = 1; }

    m_coarse = mesh(c1,c2,c3);
    m_fine   = mesh(f1,f2,f3);
  }

  domain::domain(const P& arg1, const P& arg2, const mesh& c)
    : m_corner1(arg1), m_corner2(arg2)
  {
    int c1(c.n1()), c2(c.n2()), c3(c.n3());

    if (m_corner1.x1() == m_corner2.x1()) { c1 = 1; }
    if (m_corner1.x2() == m_corner2.x2()) { c2 = 1; }
    if (m_corner1.x3() == m_corner2.x3()) { c3 = 1; }

    m_coarse = mesh(c1,c2,c3);
    m_fine   = mesh(c1,c2,c3);
  }

  // 1-dim'l domain
  domain::domain(double t_min, double t_max, int num_pts)
    : m_corner1(t_min), m_corner2(t_max), m_coarse(num_pts), m_fine(num_pts)
  { }


  int domain::dim() const
  {
    int D(0);
    if (fabs(m_corner2.x1() - m_corner1.x1()) > EPIX_EPSILON)
      ++D;
    if (fabs(m_corner2.x2() - m_corner1.x2()) > EPIX_EPSILON)
      ++D;
    if (fabs(m_corner2.x3() - m_corner1.x3()) > EPIX_EPSILON)
      ++D;

    return D;
  }

  double domain::step1() const
  {
    return (m_corner2.x1() - m_corner1.x1())/m_coarse.n1();
  }

  double domain::step2() const
  {
    return (m_corner2.x2() - m_corner1.x2())/m_coarse.n2();
  }

  double domain::step3() const
  {
    return (m_corner2.x3() - m_corner1.x3())/m_coarse.n3();
  }

  double domain::dx1() const
  {
    return (m_corner2.x1() - m_corner1.x1())/m_fine.n1();
  }

  double domain::dx2() const
  {
    return (m_corner2.x2() - m_corner1.x2())/m_fine.n2();
  }

  double domain::dx3() const
  {
    return (m_corner2.x3() - m_corner1.x3())/m_fine.n3();
  }


  // resizing attempts to preserve real resolution
  domain domain::resize1(double a1, double b1) const
  {
    P new_corner1(a1, m_corner1.x2(), m_corner1.x3());
    P new_corner2(b1, m_corner2.x2(), m_corner2.x3());

    // hack to avoid spurious round-down; should never cause round-up
    double ratio((b1-a1)/(m_corner2.x1()-m_corner1.x1()) + EPS);
    mesh new_c((int)floor(ratio*m_coarse.n1()), m_coarse.n2(),  m_coarse.n3());
    mesh new_f((int)floor(ratio*m_fine.n1()),   m_fine.n2(),    m_fine.n3());

    if (step1() == 0)
      {
	new_c = mesh(1, m_coarse.n2(),  m_coarse.n3());
	new_f = mesh(1,   m_fine.n2(),    m_fine.n3());
      }

    return domain(new_corner1, new_corner2, new_c, new_f);
  }

  domain domain::resize2(double a2, double b2) const
  {
    P new_corner1(m_corner1.x1(),  a2, m_corner1.x3());
    P new_corner2(m_corner2.x1(),  b2, m_corner2.x3());

    double ratio((b2-a2)/(m_corner2.x2()-m_corner1.x2()) + EPS);
    mesh new_c(m_coarse.n1(), (int)floor(ratio*m_coarse.n2()),  m_coarse.n3());
    mesh new_f(  m_fine.n1(), (int)floor(ratio*m_fine.n2()),    m_fine.n3());

    if (step2() == 0)
      {
	new_c = mesh(m_coarse.n1(), 1,  m_coarse.n3());
	new_f = mesh(  m_fine.n1(), 1,    m_fine.n3());
      }

    return domain(new_corner1, new_corner2, new_c, new_f);
  }

  domain domain::resize3(double a3, double b3) const
  {
    P new_corner1(m_corner1.x1(),  m_corner1.x2(), a3);
    P new_corner2(m_corner2.x1(),  m_corner2.x2(), b3);

    double ratio((b3-a3)/(m_corner2.x3()-m_corner1.x3()) + EPS);
    mesh new_c(m_coarse.n1(), m_coarse.n2(), (int)floor(ratio*m_coarse.n3()));
    mesh new_f(  m_fine.n1(),   m_fine.n2(), (int)floor(ratio*m_fine.n3()));

    if (step3() == 0)
      {
	new_c = mesh(m_coarse.n1(), m_coarse.n2(),  1);
	new_f = mesh(  m_fine.n1(),   m_fine.n2(),  1);
      }

    return domain(new_corner1, new_corner2, new_c, new_f);
  }


  // "snip_to" is defined in functions.h and performs the "obvious"
  // truncation: snip_to(x, a, b) = x, min(a,b), or max(a,b)

  // one slice
  domain domain::slice1(double a1) const
  {
    a1 = snip_to(a1, m_corner1.x1(), m_corner2.x1());

    return domain(P(a1,  m_corner1.x2(), m_corner1.x3()),
		  P(a1,  m_corner2.x2(), m_corner2.x3()),
		  mesh(1, m_coarse.n2(),  m_coarse.n3()),
		  mesh(1,   m_fine.n2(),    m_fine.n3()));
  }

  domain domain::slice2(double a2) const
  {
    a2 = snip_to(a2, m_corner1.x2(), m_corner2.x2());

    return domain(P(m_corner1.x1(),  a2, m_corner1.x3()),
		  P(m_corner2.x1(),  a2, m_corner2.x3()),
		  mesh(m_coarse.n1(), 1,  m_coarse.n3()),
		  mesh(  m_fine.n1(), 1,    m_fine.n3()));
  }

  domain domain::slice3(double a3) const
  {
    a3 = snip_to(a3, m_corner1.x3(), m_corner2.x3());

    return domain(P(m_corner1.x1(),  m_corner1.x2(), a3),
		  P(m_corner2.x1(),  m_corner2.x2(), a3),
		  mesh(m_coarse.n1(), m_coarse.n2(),  1),
		  mesh(  m_fine.n1(),   m_fine.n2(),  1));
  }


  // coordinate slices
  std::list<domain> domain::slices1(const unsigned int n) const
  {
    unsigned int N(m_coarse.n1());
    double du(step1());

    if (n > 0)
      {
	du *= N*1.0/n;
	N = n;
      }

    std::list<domain> val;

    for (unsigned int i=0; i <= N; ++i)
      val.push_back(slice1(m_corner1.x1() + i*du));

    return val;
  }

  std::list<domain> domain::slices2(const unsigned int n) const
  {
    unsigned int N(m_coarse.n2());
    double du(step2());

    if (n > 0)
      {
	du *= N*1.0/n;
	N = n;
      }

    std::list<domain> val;

    for (unsigned int i=0; i <= N; ++i)
      val.push_back(slice2(m_corner1.x2() + i*du));

    return val;
  }

  std::list<domain> domain::slices3(const unsigned int n) const
  {
    unsigned int N(m_coarse.n3());
    double du(step3());

    if (n > 0)
      {
	du *= N*1.0/n;
	N = n;
      }

    std::list<domain> val;

    for (unsigned int i=0; i <= N; ++i)
      val.push_back(slice3(m_corner1.x3() + i*du));

    return val;
  }

  P domain::corner1() const
  {
    return m_corner1;
  }
  P domain::corner2() const
  {
    return m_corner2;
  }

  double domain::corner1_x1() const
  {
    return m_corner1.x1();
  }
  double domain::corner1_x2() const
  {
    return m_corner1.x2();
  }
  double domain::corner1_x3() const
  {
    return m_corner1.x3();
  }

  double domain::corner2_x1() const
  {
    return m_corner2.x1();
  }
  double domain::corner2_x2() const
  {
    return m_corner2.x2();
  }
  double domain::corner2_x3() const
  {
    return m_corner2.x3();
  }


  int domain::coarse_n1() const
  {
    return m_coarse.n1();
  }
  int domain::coarse_n2() const
  {
    return m_coarse.n2();
  }
  int domain::coarse_n3() const
  {
    return m_coarse.n3();
  }

  int domain::fine_n1() const
  {
    return m_fine.n1();
  }
  int domain::fine_n2() const
  {
    return m_fine.n2();
  }
  int domain::fine_n3() const
  {
    return m_fine.n3();
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
