/*
 * surface.cc -- Shaded surface plotting
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 1.0.24
 * Last Change: February 27, 2007
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
#include "globals.h"
#include "errors.h"

#include "triples.h"
#include "frame.h"

#include "camera.h"

#include "domain.h"
#include "curves.h"
#include "output.h"
#include "surface.h"

namespace ePiX {

  typedef std::list<domain>::const_iterator dolci;

  // utility classes
  class facet {
  public:
    // create quasi-planar region bounded by path segments in f(R)
    facet(P f(double, double),
	  const double u0, const double v0, const double du, const double dv,
	  const unsigned int N1, const unsigned int N2);

    facet(P f(double, double, double),
	  const double u0, const double v0, const double w0,
	  const double du, const double dv, const double dw,
	  const unsigned int N1, const unsigned int N2);

    // for surfaces of rotation
    facet(double f(double), double g(double),
	  const double u0, const double v0, const double du, const double dv,
	  const unsigned int N1, const unsigned int N2,
	  const frame& coords=frame());

    facet* clone() const;

    double how_far() const;
 
    bool front_facing() const;

    void draw(int cull);

  private:
    P pt1, pt2, pt3, pt4, center, direction;
    double distance;
    path bd;
  };


  class by_distance {
  public:
    bool operator() (const facet&, const facet&);
    bool operator() (const facet*, const facet*);
  };


  //// class functions ////
  // for f(double, double)
  facet::facet(P f(double, double),
	       const double u0, const double v0,
	       const double du, const double dv,
	       const unsigned int N1, const unsigned int N2)
    : pt1(f(u0,         v0)),
      pt2(f(u0 + N1*du, v0)),
      pt3(f(u0 + N1*du, v0 + N2*dv)),
      pt4(f(u0,         v0 + N2*dv)),
      center(0.25*(pt1 + pt2 + pt3 + pt4)),
      direction(center-camera.get_viewpt()),
      distance(norm(direction))
  {
    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i*du, v0));

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + N1*du, v0 + j*dv));

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + (N1-i)*du, v0 + N2*dv));

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0, v0 + (N2-j)*dv));

    bd.close().fill();
  }


  // facet constructor for f(double, double, double)
  facet::facet(P f(double, double, double),
	       const double u0, const double v0, const double w0,
	       const double du, const double dv, const double dw,
	       const unsigned int N1, const unsigned int N2)
    : pt1(f(u0, v0, w0))
  {
    if (du == 0) // (y,z)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0, v0 + i*dv, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0 + N1*dv, w0 + j*dw));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0, v0 + (N1-i)*dv, w0 + N2*dw));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0, w0 + (N2-j)*dw));

	// use corners to approximate distance to camera
	pt2 = f(u0, v0 + N1*dv, w0);
	pt3 = f(u0, v0 + N1*dv, w0 + N2*dw);
	pt4 = f(u0, v0,         w0 + N2*dw);
      }

    else if (dv == 0) // (x,z)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + i*du, v0, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0 + N1*du, v0, w0 + j*dw));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + (N1-i)*du, v0, w0 + N2*dw));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0, w0 + (N2-j)*dw));

	// use corners to approximate distance to camera
	pt2 = f(u0 + N1*du, v0, w0);
	pt3 = f(u0 + N1*du, v0, w0 + N2*dw);
	pt4 = f(u0,         v0, w0 + N2*dw);
      }

    else if (dw == 0) // (x,y)
      {
	// bottom edge
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + i*du, v0, w0));

	// right edge
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0 + N1*du, v0 + j*dv, w0));

	// top edge (backward)
	for (unsigned int i=0; i<N1; ++i)
	  bd.pt(f(u0 + (N1-i)*du, v0 + N2*dv, w0));

	// left edge (downward)
	for (unsigned int j=0; j<N2; ++j)
	  bd.pt(f(u0, v0 + (N2-j)*dv, w0));

	// use corners to approximate distance to camera
	pt2 = f(u0 + N1*du, v0,         w0);
	pt3 = f(u0 + N1*du, v0 + N2*dv, w0);
	pt4 = f(u0,         v0 + N2*dv, w0);
      }

    else
      epix_error("Bad call to facet constructor"); // exits

    bd.close().fill();

    center = 0.25*(pt1 + pt2 + pt3 + pt4);

    direction = center-camera.get_viewpt();
    distance = norm(direction);
  } // end of facet(f(double, double, double), ...)


  // for surface of rotation
  facet::facet(double f(double), double g(double),
	       const double u0, const double v0,
	       const double du, const double dv,
	       const unsigned int N1, const unsigned int N2,
	       const frame& coords)
    : pt1(f(u0)*coords.sea() +
	  g(u0)*Cos(v0)*coords.sky() +
	  g(u0)*Sin(v0)*coords.eye()),
      pt2(f(u0 + N1*du)*coords.sea() +
	  g(u0 + N1*du)*Cos(v0)*coords.sky() +
	  g(u0 + N1*du)*Sin(v0)*coords.eye()),
      pt3(f(u0 + N1*du)*coords.sea() +
	  g(u0 + N1*du)*Cos(v0 + N2*dv)*coords.sky() +
	  g(u0 + N1*du)*Sin(v0 + N2*dv)*coords.eye()),
      pt4(f(u0)*coords.sea() +
	  g(u0)*Cos(v0 + N2*dv)*coords.sky() +
	  g(u0)*Sin(v0 + N2*dv)*coords.eye()),
      center(0.25*(pt1 + pt2 + pt3 + pt4)),
      direction(center-camera.get_viewpt()),
      distance(norm(direction))
  {
    // bottom edge
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + i*du)*coords.sea() +
	    g(u0 + i*du)*Cos(v0)*coords.sky() +
	    g(u0 + i*du)*Sin(v0)*coords.eye());

    // right edge
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0 + N1*du)*coords.sea() +
	    g(u0 + N1*du)*Cos(v0 + j*dv)*coords.sky() +
	    g(u0 + N1*du)*Sin(v0 + j*dv)*coords.eye());

    // top edge (backward)
    for (unsigned int i=0; i<N1; ++i)
      bd.pt(f(u0 + (N1-i)*du)*coords.sea() +
	    g(u0 + (N1-i)*du)*Cos(v0 + N2*dv)*coords.sky() +
	    g(u0 + (N1-i)*du)*Sin(v0 + N2*dv)*coords.eye());

    // left edge (downward)
    for (unsigned int j=0; j<N2; ++j)
      bd.pt(f(u0)*coords.sea() +
	    g(u0)*Cos(v0 + (N2-j)*dv)*coords.sky() +
	    g(u0)*Sin(v0 + (N2-j)*dv)*coords.eye());

    bd.close().fill();
  }

  facet* facet::clone() const
  {
    return new facet(*this);
  }

  double facet::how_far() const { return distance; }

  bool facet::front_facing() const
  {
    P N((pt1-center)*(pt2-center));
    if (norm(N) < EPIX_EPSILON)
      N = (pt1-center)*(pt3-center);

    if (norm(N) < EPIX_EPSILON)
      N = (pt1-center)*(pt4-center);

    return ((camera.get_viewpt()-center)|N) > -EPIX_EPSILON;
  }

  void facet::draw(int cull)
  {
    if ((cull ==  1 && front_facing()) ||
        (cull == -1 && !front_facing()))
      return;

    // else
    bool my_fill(epix::fill_paths); // get fill state
    if (my_fill) // calculate cosine^2 of normal angle
      {
	P normal((pt2 - pt1)*(pt4 - pt1));

	if (norm(normal) < EPIX_EPSILON)
	  normal = (pt2 - pt1)*(pt3 - pt1);

	normal *= recip(norm(normal));

	// Magic formula (simulated ambient lighting)
	gray(0.75*(1-pow(normal|(recip(distance)*direction), 2)));
      }

    fill();
    bd.draw();
    fill(my_fill); // restore state
  }


  bool by_distance::operator() (const facet& arg1, const facet& arg2)
  {
    return arg1.how_far() > arg2.how_far();
  }

  bool by_distance::operator() (const facet* arg1, const facet* arg2)
  {
    return arg1->how_far() > arg2->how_far();
  }


  scenery::scenery() : m_cull(0) { }

  scenery::scenery(P F(double, double), const domain& R)
    : m_cull(0)
  {
    // construct sorted list of facet*s
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2.x1() - R.corner1.x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine.n2()/R.coarse.n2());
	const unsigned int N2(R.fine.n3()/R.coarse.n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse.n2(); ++i)
	  for (int j=0; j<R.coarse.n3(); ++j)
	    m_data.push_back(new facet(F, 
				       R.corner1.x2() + i*R.step2(),
				       R.corner1.x3() + j*R.step3(),
				       du, dv, N1, N2));
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2.x2() - R.corner1.x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine.n1()/R.coarse.n1());
	const unsigned int N2(R.fine.n3()/R.coarse.n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse.n1(); ++i)
	  for (int j=0; j<R.coarse.n3(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1.x1() + i*R.step1(),
				       R.corner1.x3() + j*R.step3(),
				       du, dv, N1, N2));
      }

    else if (fabs(R.corner2.x3() - R.corner1.x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine.n1()/R.coarse.n1());
	const unsigned int N2(R.fine.n2()/R.coarse.n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse.n1(); ++i)
	  for (int j=0; j<R.coarse.n2(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1.x1() + i*R.step1(),
				       R.corner1.x2() + j*R.step2(),
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


  scenery::scenery(P F(double, double, double), const domain& R)
    : m_cull(0)
  {
    if (R.dim() != 2)
      {
	epix_warning("scenery() requires a 2-dimensional domain");
	return;
      }
    // else may assume dim(R)=2

    // exactly one test of first three should succeed
    if (fabs(R.corner2.x1() - R.corner1.x1()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine.n2()/R.coarse.n2());
	const unsigned int N2(R.fine.n3()/R.coarse.n3());

	const double du(R.dx2());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse.n2(); ++i)
	  for (int j=0; j<R.coarse.n3(); ++j)
	    m_data.push_back(new facet(F, 
				       R.corner1.x1(),
				       R.corner1.x2() + i*R.step2(),
				       R.corner1.x3() + j*R.step3(),
				       0, du, dv, N1, N2));
      }

    // if dx2() == 0, use (u,v) = (x,z)
    else if (fabs(R.corner2.x2() - R.corner1.x2()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine.n1()/R.coarse.n1());
	const unsigned int N2(R.fine.n3()/R.coarse.n3());

	const double du(R.dx1());
	const double dv(R.dx3());

	for (int i=0; i<R.coarse.n1(); ++i)
	  for (int j=0; j<R.coarse.n3(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1.x1() + i*R.step1(),
				       R.corner1.x2(),
				       R.corner1.x3() + j*R.step3(),
				       du, 0, dv, N1, N2));
      }

    else if (fabs(R.corner2.x3() - R.corner1.x3()) <= EPIX_EPSILON)
      {
	const unsigned int N1(R.fine.n1()/R.coarse.n1());
	const unsigned int N2(R.fine.n2()/R.coarse.n2());

	const double du(R.dx1());
	const double dv(R.dx2());

	for (int i=0; i<R.coarse.n1(); ++i)
	  for (int j=0; j<R.coarse.n2(); ++j)
	    m_data.push_back(new facet(F,
				       R.corner1.x1() + i*R.step1(),
				       R.corner1.x2() + j*R.step2(),
				       R.corner1.x3(),
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


  scenery::scenery(const scenery& sc)
    : m_cull(0)
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
  }

  scenery& scenery::add(P F(double, double, double), const domain& R)
  {
    scenery tmp(F, R);
    (*this).m_data.merge(tmp.m_data, by_distance());
  }

  scenery& scenery::add(P F(double, double), const domain_list& DL)
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	
  }

  scenery& scenery::add(P F(double, double, double), const domain_list& DL)
  {
    for (dolci p=DL.m_list.begin(); p!=DL.m_list.end(); ++p)
      {
	scenery tmp(F, *p);
	(*this).m_data.merge(tmp.m_data, by_distance());
      }	
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

  void scenery::draw(int cull)
  {
    // sort(m_data.begin(), m_data.end(), by_distance());

    for (std::list<facet*>::iterator p=m_data.begin(); p!=m_data.end(); ++p)
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
		domain(P(min_x, 0), P(max_x, 2*M_PI/angle(1)),
		       mesh(i_max, j_max), mesh(i_max, j_max)), cull);
  }

  void surface_rev(double f(double),
		   double min_x, double max_x, int i_max, int j_max, int cull)
  {
    surface_rev(id, f,
		domain(P(min_x, 0), P(max_x, 2*M_PI/angle(1)),
		       mesh(i_max, j_max), mesh(i_max, j_max)), cull);
  }

  void surface_rev(double f(double), double g(double), 
		   const domain& R, int cull, const frame& coords)
  {
    if (R.dx3() > 0)
      epix_warning("surface_rev() ignores domain's third coordinate");

    std::list<facet*> mesh;

    const unsigned int N1(R.fine.n1()/R.coarse.n1());
    const unsigned int N2(R.fine.n2()/R.coarse.n2());
    const double du(R.dx1());
    const double dv(R.dx2());

    for (int i=0; i<R.coarse.n1(); ++i)
      for (int j=0; j<R.coarse.n2(); ++j)
	mesh.push_back(new facet(f, g,
				 R.corner1.x1() + i*R.step1(),
				 R.corner1.x2() + j*R.step2(),
				 du, dv, N1, N2, coords));

    mesh.sort(by_distance());
    // sort(mesh.begin(), mesh.end(), by_distance());

    for (std::list<facet*>::iterator p=mesh.begin(); p!=mesh.end(); ++p)
      {
	(*p)->draw(cull);
	delete *p;
      }
  }
} // end of namespace

