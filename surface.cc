/*
 * surface.cc -- Shaded surface plotting
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.5
 * Last Change: June 18, 2008
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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

#include "constants.h"
#include "errors.h"

#include "triples.h"
#include "frame.h"

#include "map.h"

#include "camera.h"
#include "Color.h"

#include "pen_data.h"
#include "state.h"

#include "domain.h"

#include "facet.h"
#include "surface.h"

namespace ePiX {

  typedef std::list<domain>::const_iterator dolci;

  scenery::scenery() : m_cull(0) { }

  scenery::scenery(P F(double, double), const domain& R) : m_cull(0)
  {
    // construct sorted list of facet*s
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2_x1() - R.corner1_x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n2()/R.coarse_n2());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n2(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    m_data.push_back(new facet(F, 
				       R.corner1_x2() + i*R.step2(),
				       R.corner1_x3() + j*R.step3(),
				       du, dv, N1, N2));
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2_x2() - R.corner1_x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1_x1() + i*R.step1(),
				       R.corner1_x3() + j*R.step3(),
				       du, dv, N1, N2));
      }

    else if (fabs(R.corner2_x3() - R.corner1_x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n2()/R.coarse_n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n2(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1_x1() + i*R.step1(),
				       R.corner1_x2() + j*R.step2(),
				       du, dv, N1, N2));
      }

    else
      {
	epix_warning("scenery() internal error!");
	// clean up
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	return;
      }

    m_data.sort(by_distance());
    // sort(m_data.begin(), m_data.end(), by_distance());
  } // end of scenery(P F(double, double), const domain& R)


  scenery::scenery(P F(double, double, double), const domain& R) : m_cull(0)
  {
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2_x1() - R.corner1_x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n2()/R.coarse_n2());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n2(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    m_data.push_back(new facet(F, 
				       R.corner1_x1(),
				       R.corner1_x2() + i*R.step2(),
				       R.corner1_x3() + j*R.step3(),
				       0, du, dv, N1, N2));
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2_x2() - R.corner1_x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1_x1() + i*R.step1(),
				       R.corner1_x2(),
				       R.corner1_x3() + j*R.step3(),
				       du, 0, dv, N1, N2));
      }

    else if (fabs(R.corner2_x3() - R.corner1_x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n2()/R.coarse_n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n2(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1_x1() + i*R.step1(),
				       R.corner1_x2() + j*R.step2(),
				       R.corner1_x3(),
				       du, dv, 0, N1, N2));
      }

    else
      {
	epix_warning("scenery() internal error!");
	// clean up
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	return;
      }

    m_data.sort(by_distance());
    // sort(m_data.begin(), m_data.end(), by_distance());
  } // end of scenery(P F(double, double, double), const domain& R)


  //// Color-dependent constructors ////
  // position-valued color
  scenery::scenery(P F(double, double), const domain& R,
		   P color(double, double, double)) : m_cull(0)
  {
    // construct sorted list of facet*s
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2_x1() - R.corner1_x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n2()/R.coarse_n2());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n2(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      // use mesh center to approximate facet's location
	      P posn(F(R.corner1_x2() + (i+0.5)*R.step2(),
		       R.corner1_x3() + (j+0.5)*R.step3()));

	      P col(color(posn.x1(), posn.x2(), posn.x3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x2() + i*R.step2(),
					 R.corner1_x3() + j*R.step3(),
					 du, dv, N1, N2,
					 RGB(col.x1(),col.x2(),col.x3())));
	    }
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2_x2() - R.corner1_x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      // use mesh center to approximate facet's location
	      P posn(F(R.corner1_x1() + (i+0.5)*R.step1(),
		       R.corner1_x3() + (j+0.5)*R.step3()));

	      P col(color(posn.x1(), posn.x2(), posn.x3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x3() + j*R.step3(),
					 du, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else if (fabs(R.corner2_x3() - R.corner1_x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n2()/R.coarse_n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n2(); ++j)
	    {
	      // use mesh center to approximate facet's location
	      P posn(F(R.corner1_x1() + (i+0.5)*R.step1(),
		       R.corner1_x2() + (j+0.5)*R.step2()));

	      P col(color(posn.x1(), posn.x2(), posn.x3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x2() + j*R.step2(),
					 du, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else
      {
	epix_warning("scenery() internal error!");
	// clean up
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	return;
      }

    m_data.sort(by_distance());
    // sort(m_data.begin(), m_data.end(), by_distance());
  } // end of scenery(P F(double, double), const domain& R)


  scenery::scenery(P F(double, double, double), const domain& R,
		   P color(double, double, double)) : m_cull(0)
  {
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2_x1() - R.corner1_x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n2()/R.coarse_n2());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n2(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      // use mesh center to approximate facet's location
	      P posn(F(R.corner1_x1(),
		       R.corner1_x2() + (i+0.5)*R.step2(),
		       R.corner1_x3() + (j+0.5)*R.step3()));

	      P col(color(posn.x1(), posn.x2(), posn.x3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x1(),
					 R.corner1_x2() + i*R.step2(),
					 R.corner1_x3() + j*R.step3(),
					 0, du, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2_x2() - R.corner1_x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      P posn(F(R.corner1_x1() + (i+0.5)*R.step1(),
		       R.corner1_x2(),
		       R.corner1_x3() + (j+0.5)*R.step3()));

	      P col(color(posn.x1(), posn.x2(), posn.x3()));

	      m_data.push_back(new facet(F,
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x2(),
					 R.corner1_x3() + j*R.step3(),
					 du, 0, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else if (fabs(R.corner2_x3() - R.corner1_x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n2()/R.coarse_n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n2(); ++j)
	    {
	      P posn(F(R.corner1_x1() + (i+0.5)*R.step1(),
		       R.corner1_x2() + (j+0.5)*R.step2(),
		       R.corner1_x3()));

	      P col(color(posn.x1(), posn.x2(), posn.x3()));

	      m_data.push_back(new facet(F,
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x2() + j*R.step2(),
					 R.corner1_x3(),
					 du, dv, 0, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else
      {
	epix_warning("scenery() internal error!");
	// clean up
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	return;
      }

    m_data.sort(by_distance());
    // sort(m_data.begin(), m_data.end(), by_distance());
  } // end of scenery(P F(double, double, double), const domain& R)


  //// domain-valued color ////
  scenery::scenery(P F(double, double), const domain& R,
		   P color(double, double)) : m_cull(0)
  {
    // construct sorted list of facet*s
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2_x1() - R.corner1_x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n2()/R.coarse_n2());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n2(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      P col(color(R.corner1_x2() + (i+0.5)*R.step2(),
			  R.corner1_x3() + (j+0.5)*R.step3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x2() + i*R.step2(),
					 R.corner1_x3() + j*R.step3(),
					 du, dv, N1, N2,
					 RGB(col.x1(),col.x2(),col.x3())));
	    }
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2_x2() - R.corner1_x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      // use mesh center to approximate facet's location
	      P col(color(R.corner1_x1() + (i+0.5)*R.step1(),
			  R.corner1_x3() + (j+0.5)*R.step3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x3() + j*R.step3(),
					 du, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else if (fabs(R.corner2_x3() - R.corner1_x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n2()/R.coarse_n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n2(); ++j)
	    {
	      // use mesh center to approximate facet's location
	      P col(color(R.corner1_x1() + (i+0.5)*R.step1(),
			  R.corner1_x2() + (j+0.5)*R.step2()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x2() + j*R.step2(),
					 du, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else
      {
	epix_warning("scenery() internal error!");
	// clean up
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	return;
      }

    m_data.sort(by_distance());
    // sort(m_data.begin(), m_data.end(), by_distance());
  } // end of scenery(P F(double, double), const domain& R)


  scenery::scenery(P F(double, double, double), const domain& R,
		   P color(double, double)) : m_cull(0)
  {
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2_x1() - R.corner1_x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n2()/R.coarse_n2());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n2(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      // Color indep of x1
	      P col(color(R.corner1_x2() + (i+0.5)*R.step2(),
			  R.corner1_x3() + (j+0.5)*R.step3()));

	      m_data.push_back(new facet(F, 
					 R.corner1_x1(),
					 R.corner1_x2() + i*R.step2(),
					 R.corner1_x3() + j*R.step3(),
					 0, du, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2_x2() - R.corner1_x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n3()/R.coarse_n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n3(); ++j)
	    {
	      P col(color(R.corner1_x1() + (i+0.5)*R.step1(),
			  R.corner1_x3() + (j+0.5)*R.step3()));

	      m_data.push_back(new facet(F,
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x2(),
					 R.corner1_x3() + j*R.step3(),
					 du, 0, dv, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else if (fabs(R.corner2_x3() - R.corner1_x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine_n1()/R.coarse_n1());
	const unsigned int N2(R.fine_n2()/R.coarse_n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse_n1(); ++i)
	  for (int j=0; j<R.coarse_n2(); ++j)
	    {
	      P col(color(R.corner1_x1() + (i+0.5)*R.step1(),
			  R.corner1_x2() + (j+0.5)*R.step2()));

	      m_data.push_back(new facet(F,
					 R.corner1_x1() + i*R.step1(),
					 R.corner1_x2() + j*R.step2(),
					 R.corner1_x3(),
					 du, dv, 0, N1, N2,
					 RGB(col.x1(), col.x2(), col.x3())));
	    }
      }

    else
      {
	epix_warning("scenery() internal error!");
	// clean up
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	return;
      }

    m_data.sort(by_distance());
    // sort(m_data.begin(), m_data.end(), by_distance());
  } // end of scenery(P F(double, double, double), const domain& R)


  scenery::scenery(const scenery& sc)
    : m_cull(sc.m_cull)
  {
    for (std::list<facet*>::const_iterator p=sc.m_data.begin();
	 p!=sc.m_data.end(); ++p)
      m_data.push_back((*p)->clone());
  }

  scenery& scenery::operator= (const scenery& sc)
  {
    if (this != &sc)
      {
	std::list<facet*> tmp;
	// get facets from sc
	for (std::list<facet*>::const_iterator p=sc.m_data.begin();
	     p!=sc.m_data.end(); ++p)
	  tmp.push_back((*p)->clone());

	// free memory
	for (std::list<facet*>::iterator p=m_data.begin();
	     p!=m_data.end(); ++p)
	  delete *p;

	swap(m_data, tmp);

	m_cull=sc.m_cull;
      }

    return *this;
  }

  scenery::~scenery()
  {
    for (std::list<facet*>::iterator p=m_data.begin(); p!=m_data.end(); ++p)
      delete *p;
  }


  scenery& scenery::add(P F(double, double), const domain& R)
  {
    scenery tmp(F, R);
    (*this).m_data.merge(tmp.m_data, by_distance());

    return *this;
  }

  scenery& scenery::add(P F(double, double, double), const domain& R)
  {
    scenery tmp(F, R);
    (*this).m_data.merge(tmp.m_data, by_distance());

    return *this;
  }

  scenery& scenery::add(P F(double, double), const domain_list& DL)
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	

    return *this;
  }

  scenery& scenery::add(P F(double, double, double), const domain_list& DL)
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	

    return *this;
  }


  //// Color-dependent versions ////
  // position-dependent color
  scenery& scenery::add(P F(double, double), const domain& R,
			P color(double, double, double))
  {
    scenery tmp(F, R, color);
    (*this).m_data.merge(tmp.m_data, by_distance());

    return *this;
  }

  scenery& scenery::add(P F(double, double, double), const domain& R,
			P color(double, double, double))
  {
    scenery tmp(F, R, color);
    (*this).m_data.merge(tmp.m_data, by_distance());

    return *this;
  }

  scenery& scenery::add(P F(double, double), const domain_list& DL,
			P color(double, double, double))
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p, color);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	

    return *this;
  }

  scenery& scenery::add(P F(double, double, double), const domain_list& DL,
			P color(double, double, double))
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p, color);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	

    return *this;
  }


  //domain-dependent color
  scenery& scenery::add(P F(double, double), const domain& R,
			P color(double, double))
  {
    scenery tmp(F, R, color);
    (*this).m_data.merge(tmp.m_data, by_distance());

    return *this;
  }

  scenery& scenery::add(P F(double, double, double), const domain& R,
			P color(double, double))
  {
    scenery tmp(F, R, color);
    (*this).m_data.merge(tmp.m_data, by_distance());

    return *this;
  }

  scenery& scenery::add(P F(double, double), const domain_list& DL,
			P color(double, double))
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p, color);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	

    return *this;
  }

  scenery& scenery::add(P F(double, double, double), const domain_list& DL,
			P color(double, double))
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p, color);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	

    return *this;
  }


  scenery& scenery::cull(int c)
  {
    if (c == 0)
      m_cull=0;
    else if (c > 0)
      m_cull=1;
    else
      m_cull=-1;
    return *this;
  }

  void scenery::draw(int cull) const
  {
    for (std::list<facet*>::const_iterator p=m_data.begin();
	 p!=m_data.end(); ++p)
      (*p)->draw(cull);
  }


  //// User-accessible functions ////
  void surface(P F(double, double), const domain& R, int cull)
  {
    scenery surf(F, R);
    surf.draw(cull);
  }


  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R, int cull)
  {
    scenery surf(F, R);
    surf.draw(cull);
  }

  void surface(P F(double, double, double), const domain_list& R, int cull)
  {
    scenery tmp;

    for (dolci p= R.m_list.begin(); p!=R.m_list.end(); ++p)
      tmp.add(F, *p);

    tmp.draw(cull);
  }


  void surface_rev(double f(double), double g(double), 
		   double min_x, double max_x, int i_max, int j_max, int cull)
  {
    surface_rev(f, g,
		domain(P(min_x, 0), P(max_x, full_turn()),
		       mesh(i_max, j_max), mesh(i_max, j_max)), frame(), cull);
  }

  void surface_rev(double f(double),
		   double min_x, double max_x, int i_max, int j_max, int cull)
  {
    surface_rev(id, f,
		domain(P(min_x, 0), P(max_x, full_turn()),
		       mesh(i_max, j_max), mesh(i_max, j_max)), frame(), cull);
  }

  void surface_rev(double f(double), double g(double), 
		   const domain& R, const frame& coords, int cull)
  {
    if (R.dx3() > 0)
      epix_warning("surface_rev() ignores domain's third coordinate");

    std::list<facet*> mesh;

    const unsigned int N1(R.fine_n1()/R.coarse_n1());
    const unsigned int N2(R.fine_n2()/R.coarse_n2());
    const double du(R.dx1());
    const double dv(R.dx2());

    for (int i=0; i<R.coarse_n1(); ++i)
      for (int j=0; j<R.coarse_n2(); ++j)
	mesh.push_back(new facet(f, g,
				 R.corner1_x1() + i*R.step1(),
				 R.corner1_x2() + j*R.step2(),
				 du, dv, N1, N2, coords));

    mesh.sort(by_distance());
    // sort(mesh.begin(), mesh.end(), by_distance());

    for (std::list<facet*>::iterator p=mesh.begin(); p!=mesh.end(); ++p)
      {
	(*p)->draw(cull);
	delete *p;
      }
  }

  //// Color-dependent versions ////
  // location-dependent color
  void surface(P F(double, double), const domain& R,
	       P color(double, double, double), int cull)
  {
    scenery surf(F, R, color);
    surf.draw(cull);
  }


  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R,
	       P color(double, double, double), int cull)
  {
    scenery surf(F, R, color);
    surf.draw(cull);
  }

  void surface(P F(double, double, double), const domain_list& R,
	       P color(double, double, double), int cull)
  {
    scenery tmp;

    for (dolci p= R.m_list.begin(); p!=R.m_list.end(); ++p)
      tmp.add(F, *p, color);

    tmp.draw(cull);
  }


  void surface_rev(double f(double), double g(double), 
		   double min_x, double max_x, int i_max, int j_max,
		   P color(double, double, double), int cull)
  {
    surface_rev(f, g,
		domain(P(min_x, 0), P(max_x, full_turn()),
		       mesh(i_max, j_max), mesh(i_max, j_max)), color,
		frame(), cull);
  }

  void surface_rev(double f(double),
		   double min_x, double max_x, int i_max, int j_max,
		   P color(double, double, double), int cull)
  {
    surface_rev(id, f,
		domain(P(min_x, 0), P(max_x, full_turn()),
		       mesh(i_max, j_max), mesh(i_max, j_max)), color,
		frame(), cull);
  }

  void surface_rev(double f(double), double g(double), 
		   const domain& R, P color(double, double, double),
		   const frame& coords, int cull)
  {
    if (R.dx3() > 0)
      epix_warning("surface_rev() ignores domain's third coordinate");

    std::list<facet*> mesh;

    const unsigned int N1(R.fine_n1()/R.coarse_n1());
    const unsigned int N2(R.fine_n2()/R.coarse_n2());
    const double du(R.dx1());
    const double dv(R.dx2());

    const double u0(R.corner1_x1());
    const double v0(R.corner1_x2());
    const double DU(R.step1());
    const double DV(R.step2());

    for (int i=0; i<R.coarse_n1(); ++i)
      for (int j=0; j<R.coarse_n2(); ++j)
	{
	  // mesh center approximates facet's location
	  P posn(f(u0 + (i+0.5)*DU)*coords.sea() +
		 g(u0 + (i+0.5)*DU)*Cos(v0 + (j+0.5)*DV)*coords.sky() +
		 g(u0 + (i+0.5)*DU)*Sin(v0 + (j+0.5)*DV)*coords.eye());

	  P col(color(posn.x1(), posn.x2(), posn.x3()));

	  mesh.push_back(new facet(f, g,
				   R.corner1_x1() + i*R.step1(),
				   R.corner1_x2() + j*R.step2(),
				   du, dv, N1, N2, 
				   RGB(col.x1(), col.x2(), col.x3()), coords));
	}

    mesh.sort(by_distance());
    // sort(mesh.begin(), mesh.end(), by_distance());

    for (std::list<facet*>::iterator p=mesh.begin(); p!=mesh.end(); ++p)
      {
	(*p)->draw(cull);
	delete *p;
      }
  }

  // domain-dependent color
  void surface(P F(double, double), const domain& R,
	       P color(double, double), int cull)
  {
    scenery surf(F, R, color);
    surf.draw(cull);
  }


  // for slices of maps R^3 -> R^3
  void surface(P F(double, double, double), const domain& R,
	       P color(double, double), int cull)
  {
    scenery surf(F, R, color);
    surf.draw(cull);
  }

  void surface(P F(double, double, double), const domain_list& R,
	       P color(double, double), int cull)
  {
    scenery tmp;

    for (dolci p= R.m_list.begin(); p!=R.m_list.end(); ++p)
      tmp.add(F, *p, color);

    tmp.draw(cull);
  }


  void surface_rev(double f(double), double g(double), 
		   double min_x, double max_x, int i_max, int j_max,
		   P color(double, double), int cull)
  {
    surface_rev(f, g,
		domain(P(min_x, 0), P(max_x, full_turn()),
		       mesh(i_max, j_max), mesh(i_max, j_max)), color,
		frame(), cull);
  }

  void surface_rev(double f(double),
		   double min_x, double max_x, int i_max, int j_max,
		   P color(double, double), int cull)
  {
    surface_rev(id, f,
		domain(P(min_x, 0), P(max_x, full_turn()),
		       mesh(i_max, j_max), mesh(i_max, j_max)), color,
		frame(), cull);
  }

  void surface_rev(double f(double), double g(double), 
		   const domain& R, P color(double, double),
		   const frame& coords, int cull)
  {
    if (R.dx3() > 0)
      epix_warning("surface_rev() ignores domain's third coordinate");

    std::list<facet*> mesh;

    const unsigned int N1(R.fine_n1()/R.coarse_n1());
    const unsigned int N2(R.fine_n2()/R.coarse_n2());
    const double du(R.dx1());
    const double dv(R.dx2());

    const double u0(R.corner1_x1());
    const double v0(R.corner1_x2());
    const double DU(R.step1());
    const double DV(R.step2());

    for (int i=0; i<R.coarse_n1(); ++i)
      for (int j=0; j<R.coarse_n2(); ++j)
	{
	  // mesh center approximates facet's location
	  P posn(f(u0 + (i+0.5)*DU)*coords.sea() +
		 g(u0 + (i+0.5)*DU)*Cos(v0 + (j+0.5)*DV)*coords.sky() +
		 g(u0 + (i+0.5)*DU)*Sin(v0 + (j+0.5)*DV)*coords.eye());

	  P col(color(u0 + (i+0.5)*DU, v0 + (j+0.5)*DV));

	  mesh.push_back(new facet(f, g,
				   R.corner1_x1() + i*R.step1(),
				   R.corner1_x2() + j*R.step2(),
				   du, dv, N1, N2, 
				   RGB(col.x1(), col.x2(), col.x3()), coords));
	}

    mesh.sort(by_distance());
    // sort(mesh.begin(), mesh.end(), by_distance());

    for (std::list<facet*>::iterator p=mesh.begin(); p!=mesh.end(); ++p)
      {
	(*p)->draw(cull);
	delete *p;
      }
  }
} // end of namespace
