/* 
 * affine.cc -- ePiX::affine class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.21
 * Last Change: September 23, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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

#include "errors.h"
#include "constants.h"

#include "pairs.h"
#include "triples.h"
#include "functions.h"

#include "affine.h"

namespace ePiX {

  // rotate/reflect about (0,0)
  static pair __epix_rotate(double Cs, double Sn, const pair& arg)
  {
    double x(arg.x1()), y(arg.x2());
    return pair(Cs*x - Sn*y, Sn*x + Cs*y);
  }

  static pair __epix_reflect(double Cs, double Sn, const pair& arg)
  {
    double x(arg.x1()), y(arg.x2());
    return pair(Cs*x + Sn*y, Sn*x - Cs*y);
  }

  // identity
  affine::affine()
    : m_00(0,0), m_10(1,0), m_01(0,1) { }

  // images of (1,0), (0,1), (0,0)
  affine::affine(const pair& pr1, const pair& pr2, const pair& loc)
    : m_00(loc), m_10(pr1), m_01(pr2) { }

  affine::affine(const P& pr1, const P& pr2, const P& loc)
    : m_00(loc.x1(), loc.x2()),
      m_10(pr1.x1(), pr1.x2()),
      m_01(pr2.x1(), pr2.x2()) { }

  affine&   affine::shift(const pair& arg)
  {
    m_00 += arg;
    m_10 += arg;
    m_01 += arg;

    return *this;
  }

  affine&   affine::shift(const P& arg)
  {
    return shift(pair(arg.x1(), arg.x2()));
  }

  affine&  affine::rotate(double theta, const pair& ctr)
  {
    const double Cs(Cos(theta));
    const double Sn(Sin(theta));

    // shift
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    // rotate about origin and shift back
    m_00 = __epix_rotate(Cs, Sn, m_00) + ctr;
    m_10 = __epix_rotate(Cs, Sn, m_10) + ctr;
    m_01 = __epix_rotate(Cs, Sn, m_01) + ctr;

    return *this;
  }

  affine&  affine::rotate(double theta, const P& ctr)
  {
    return rotate(theta, pair(ctr.x1(), ctr.x2()));
  }

  affine& affine::reflect(double theta, const pair& ctr)
  {
    const double Cs(Cos(2*theta));
    const double Sn(Sin(2*theta));

    // shift
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    // reflect about origin and shift back
    m_00 = __epix_reflect(Cs, Sn, m_00) + ctr;
    m_10 = __epix_reflect(Cs, Sn, m_10) + ctr;
    m_01 = __epix_reflect(Cs, Sn, m_01) + ctr;

    return *this;
  }

  affine&  affine::reflect(double theta, const P& ctr)
  {
    return reflect(theta, pair(ctr.x1(), ctr.x2()));
  }

  affine& affine::h_scale(double sc, const pair& ctr)
  {
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    const pair scale(sc, 1);

    m_00 = (m_00 & scale) + ctr;
    m_10 = (m_10 & scale) + ctr;
    m_01 = (m_01 & scale) + ctr;

    return *this;
  }

  affine&  affine::h_scale(double sc, const P& ctr)
  {
    return h_scale(sc, pair(ctr.x1(), ctr.x2()));
  }

  affine& affine::v_scale(double sc, const pair& ctr)
  {
    // shift
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    const pair scale(1, sc);

    m_00 = (m_00 & scale) + ctr;
    m_10 = (m_10 & scale) + ctr;
    m_01 = (m_01 & scale) + ctr;

    return *this;
  }

  affine&  affine::v_scale(double sc, const P& ctr)
  {
    return v_scale(sc, pair(ctr.x1(), ctr.x2()));
  }

  affine&   affine::scale(double sc, const pair& ctr)
  {
    // shift
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    const pair scale(sc, sc);

    m_00 = (m_00 & scale) + ctr;
    m_10 = (m_10 & scale) + ctr;
    m_01 = (m_01 & scale) + ctr;

    return *this;
  }

  affine&  affine::scale(double sc, const P& ctr)
  {
    return scale(sc, pair(ctr.x1(), ctr.x2()));
  }


  affine&  affine::h_shear(double sc, const pair& ctr)
  {
    // shift
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    m_00 += pair(sc*m_00.x2(), 0) + ctr;
    m_10 += pair(sc*m_10.x2(), 0) + ctr;
    m_01 += pair(sc*m_01.x2(), 0) + ctr;

    return *this;
  }

  affine&  affine::h_shear(double sc, const P& ctr)
  {
    return h_shear(sc, pair(ctr.x1(), ctr.x2()));
  }

  affine&  affine::v_shear(double sc, const pair& ctr)
  {
    // shift
    m_00 -= ctr;
    m_10 -= ctr;
    m_01 -= ctr;

    m_00 += pair(0, sc*m_00.x1()) + ctr;
    m_10 += pair(0, sc*m_10.x1()) + ctr;
    m_01 += pair(0, sc*m_01.x1()) + ctr;

    return *this;
  }

  affine&  affine::v_shear(double sc, const P& ctr)
  {
    return v_shear(sc, pair(ctr.x1(), ctr.x2()));
  }

  affine& affine::postcomp(const affine& af)
  {
    m_00 = af(m_00);
    m_10 = af(m_10);
    m_01 = af(m_01);

    return *this;
  }

  affine& affine::invert()
  {
    m_10 -= m_00;
    m_01 -= m_00;

    const double denom(m_10.x1()*m_01.x2() - m_10.x2()*m_01.x1());

    if (fabs(denom) < EPIX_EPSILON)
      {
	// restore
	m_10 += m_00;
	m_01 += m_00;
	epix_warning("affine not invertible, no action");
	return *this;
      }

    // else compute inverse entries
    const double a11( m_01.x2()/denom);
    const double a12(-m_01.x1()/denom);
    const double a21(-m_10.x2()/denom);
    const double a22( m_10.x1()/denom);

    pair tmp_00(-a11*m_00.x1() + a12*m_00.x2(),
		 a21*m_00.x1() - a22*m_00.x2());

    m_00 = tmp_00;
    m_10 = m_00 + pair(a11, a21);
    m_01 = m_00 + pair(a12, a22);

    return *this;
  }


  // evaluation
  pair affine::operator() (const pair& arg) const
  {
    return m_00 + arg.x1()*(m_10 - m_00) + arg.x2()*(m_01 - m_00);
  }

  pair affine::operator() (const P& arg) const
  {
    return m_00 + arg.x1()*(m_10 - m_00) + arg.x2()*(m_01 - m_00);
  }

  // pre-composition
  affine affine::operator() (const affine& af) const
  {
    return affine(this->operator()(af.m_10),
		  this->operator()(af.m_01),
		  this->operator()(af.m_00));
  }

  bool affine::reverses_orientation() const
  {
    const pair col1(m_10 - m_00);
    const pair col2(m_01 - m_00);

    return (col1.x1()*col2.x2() - col1.x2()*col2.x1() < -EPIX_EPSILON);
  }
} // end of namespace
