/* 
 * geometry.cc -- spherical and hyperbolic geometry
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.20
 * Last Change: December 5, 2021
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2021
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
#include <vector>

#include "constants.h"
#include "errors.h"
#include "functions.h"

#include "triples.h"
#include "frame.h"

#include "state.h"

#include "camera.h"

#include "path.h"

#include "sphere.h"
#include "curves.h"

#include "geometry.h"

namespace ePiX {

  //// Static utility functions ////

  // Flag for type of projection to the sphere
  enum sphere_proj_type {RADIAL, STEREO_N, STEREO_S};

  // point constructor in geographic coords on specified sphere/frame
  P sphere_posn(double lat, double lngtd, const Sphere& S, const frame& coords)
  {
    double rad(S.radius());

    return S.center() + 
      rad*Cos(lat)*(Cos(lngtd)*coords.sea() + Sin(lngtd)*coords.sky()) +
      rad*Sin(lat)*coords.eye();
  }

  // latitudes and longitudes
  void draw_latitude(double lat, double lngtd_min, double lngtd_max,
		     bool front, const Sphere& S, const frame& coords)
  {
    P center(S.center() + (S.radius()*Sin(lat)*coords.eye()));
    double radius(S.radius()*Cos(lat));

    path temp(center, radius*coords.sea(), radius*coords.sky(),
	      lngtd_min, lngtd_max);

    temp.clip_to(S, cam().viewpt(), front);
    temp.draw();
  }

  // draw portion of longitude line
  void draw_longitude(double lngtd, double lat_min, double lat_max,
		      bool front, const Sphere& S, const frame& coords)
  {
    P center(S.center());
    double radius(S.radius());

    path temp(center,
	      radius*(Cos(lngtd)*coords.sea()+Sin(lngtd)*coords.sky() ),
	      radius*coords.eye(), lat_min, lat_max);

    temp.clip_to(S, cam().viewpt(), front);
    temp.draw();
  }


  // Spherical geometry
  P proj_to_sphere(const P& arg, const Sphere& S, sphere_proj_type TYPE)
  {
    P O(S.center());
    double rad(S.radius());
    P loc(arg - O); // location relative to O

    if (TYPE == RADIAL)
      return O + (rad/norm(loc))*loc;

    else if (TYPE == STEREO_N)
      {
	P temp(loc%E_3);
	double rho(temp|temp);
	return O + (rad/(rho+1))*P(2*temp.x1(), 2*temp.x2(), rho-1);
      }

    else if (TYPE == STEREO_S)
      {
	P temp(loc%E_3);
	double rho(temp|temp);
	return O + (rad/(rho+1))*P(2*temp.x1(), 2*temp.x2(), 1-rho);
      }

    else // Return center on erroneous projection type
      return O;
  }

  void draw_sphereplot(double f1(double),double f2(double),double f3(double), 
		       double t_min, double t_max, int num_pts, bool front, 
		       sphere_proj_type TYPE, const Sphere& S)
  {
    std::vector<P> data(num_pts+1);
    double t(t_min);
    const double dt((t_max - t_min)/num_pts);

    for (int i=0; i <= num_pts; ++i, t += dt)
      data.at(i) = proj_to_sphere(P(f1(t), f2(t), f3(t)), S, TYPE);

  path temp(data, false, false);
  temp.clip_to(S, cam().viewpt(), front);
  temp.draw();
  } // end of draw_sphereplot

  void draw_sphereplot(P Phi(double), double t_min, double t_max, 
		       int num_pts, bool front, sphere_proj_type TYPE,
		       const Sphere& S)
  {
    std::vector<P> data(num_pts+1);
    double t(t_min);
    const double dt((t_max - t_min)/num_pts);

    for (int i=0; i <= num_pts; ++i, t += dt)
      data.at(i) = proj_to_sphere(Phi(t), S, TYPE);

  path temp(data, false, false);
  temp.clip_to(S, cam().viewpt(), front);
  temp.draw();
  } // end of draw_sphereplot


  // segment mapping class
  class seg {
  public:
    seg(const P& tail, const P& head)
      : m_tail(tail), m_head(head) { }

    // so we can be plotted
    P operator() (double t) const
    {
      return m_tail + t*(m_head - m_tail);
    }

  private:
    P m_tail;
    P m_head;
  }; // end of class seg


  // assumes seg contains the actual (scaled, translated) endpoints
  void draw_sphere_arc(const seg& sgmt, double t_min, double t_max, 
		       bool front, sphere_proj_type TYPE, const Sphere& S)
  {
    P tail(sgmt(t_min)), head(sgmt(t_max));
    double cos_theta(((head-S.center())|(tail-S.center()))/pow(S.radius(),2));
    if (1-cos_theta < EPIX_EPSILON) // endpoints equal
      return; // draw nothing

    else if (1+cos_theta < EPIX_EPSILON) // endpoints antipodal
      {
	epix_warning("Spherical arc joins antipodes, no output");
	return;
      }
    // else

    int num_pts((int) ceil(EPIX_NUM_PTS*Acos(cos_theta)/full_turn()));
    if (num_pts < 2)
      num_pts=2;

    std::vector<P> data(num_pts+1);
    double t(t_min);
    const double dt((t_max - t_min)/num_pts);

    P O(S.center());
    double rad(S.radius());

    for (int i=0; i <= num_pts; ++i, t += dt)
      {
	P loc(sgmt(t) - O); // location relative to O
	data.at(i) = S.center() + rad*recip(norm(loc))*loc;
      }

    path temp(data, false, false);
    temp.clip_to(S, cam().viewpt(), front);
    temp.draw();
  } // end of draw_sphere_arc


  //// "Geography" and spherical plotting ////
  void latitude(double lat, double lngtd_min, double lngtd_max,
		const Sphere& S, const frame& coords)
  {
    draw_latitude(lat, lngtd_min, lngtd_max, true, S, coords);
  }


  void longitude(double lngtd, double lat_min, double lat_max,
		 const Sphere& S, const frame& coords)
  {
    draw_longitude(lngtd, lat_min, lat_max, true, S, coords);
  }

  void back_latitude(double lat, double lngtd_min, double lngtd_max,
		     const Sphere& S, const frame& coords)
  {
    draw_latitude(lat, lngtd_min, lngtd_max, false, S, coords);
  }

  void back_longitude(double lngtd, double lat_min, double lat_max,
		      const Sphere& S, const frame& coords)
  {
    draw_longitude(lngtd, lat_min, lat_max, false, S, coords);
  }


  // spherical plotting
  void frontplot_N(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const Sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, true, STEREO_N, S);
  }

  void backplot_N(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts,
		  const Sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, false, STEREO_N, S);
  }

  void frontplot_S(double f1(double), double f2(double),
		   double t_min, double t_max, int num_pts, 
		   const Sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, true, STEREO_S, S);
  }

  void backplot_S(double f1(double), double f2(double),
		  double t_min, double t_max, int num_pts, 
		  const Sphere& S)
  {
    draw_sphereplot(f1, f2, zero, t_min, t_max, num_pts, false, STEREO_S, S);
  }


  // Radial projection from center
  void frontplot_R(P phi(double), double t_min, double t_max, 
		   int num_pts, const Sphere& S)
  {
    draw_sphereplot(phi, t_min, t_max, num_pts, true, RADIAL, S);
  }

  void backplot_R(P phi(double), double t_min, double t_max, 
		  int num_pts, const Sphere& S)
  {
    draw_sphereplot(phi, t_min, t_max, num_pts, false, RADIAL, S);
  }


  //// Spherical geometry -- arcs and polyhedra ////
  // arc of great circle between non-antipodal points
  void arc(const P& tail, const P& head, const bool front, const Sphere& S)
  {
    draw_sphere_arc(seg(tail, head), 0, 1, front, RADIAL, S);
  }


  // user-space functions
  void front_arc(const P& p1, const P& p2, const Sphere& S)
  {
    const P ctr(S.center());
    const double rad(S.radius());

    arc(ctr+(rad/norm(p1))*p1, ctr+(rad/norm(p2))*p2, true, S);
  }

  void  back_arc(const P& p1, const P& p2, const Sphere& S)
  {
    const P ctr(S.center());
    const double rad(S.radius());

    arc(ctr+(rad/norm(p1))*p1, ctr+(rad/norm(p2))*p2, false, S);
  }

  // join p1 to -p1 through p2
  void front_arc2(const P& p1, const P& p2, const Sphere& S)
  {
    front_arc(p1,  p2, S);
    front_arc(p2, -p1, S);
  }

  void  back_arc2(const P& p1, const P& p2, const Sphere& S)
  {
    back_arc(p1,  p2, S);
    back_arc(p2, -p1, S);
  }

  void front_line(const P& p1, const P& p2, const Sphere& S)
  {
    front_arc( p1,  p2, S);
    front_arc( p2, -p1, S);
    front_arc(-p1, -p2, S);
    front_arc(-p2,  p1, S);
  }


  void back_line(const P& p1, const P& p2, const Sphere& S)
  {
    back_arc( p1,  p2, S);
    back_arc( p2, -p1, S);
    back_arc(-p1, -p2, S);
    back_arc(-p2,  p1, S);
  }


  void front_triangle(const P& p1, const P& p2, const P& p3, const Sphere& S)
  {
    front_arc(p1, p2, S);
    front_arc(p2, p3, S);
    front_arc(p3, p1, S);
  }

  void  back_triangle(const P& p1, const P& p2, const P& p3, const Sphere& S)
  {
    back_arc(p1, p2, S);
    back_arc(p2, p3, S);
    back_arc(p3, p1, S);
  }

  // local to this file
  void front_dual(const P& p1, const P& p2, const P& p3, const Sphere& S)
  {
    const P ctr(0.3333*(p1+p2+p3));

    front_arc(ctr, 0.5*(p1+p2), S);
    front_arc(ctr, 0.5*(p2+p3), S);
    front_arc(ctr, 0.5*(p3+p1), S);
  }

  void  back_dual(const P& p1, const P& p2, const P& p3, const Sphere& S)
  {
    const P ctr(0.3333*(p1+p2+p3));

    back_arc(ctr, 0.5*(p1+p2), S);
    back_arc(ctr, 0.5*(p2+p3), S);
    back_arc(ctr, 0.5*(p3+p1), S);
  }


  // spherical polyhedra
  void front_tetra(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());

    const P ppp( f1+f2+f3);
    const P pnn( f1-f2-f3);
    const P npn(-f1+f2-f3);
    const P nnp(-f1-f2+f3);

    front_triangle(ppp,pnn,npn,S);
    front_triangle(ppp,npn,nnp,S);
    front_triangle(ppp,nnp,pnn,S);
    front_triangle(nnp,pnn,npn,S);
  }
  void  back_tetra(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());

    const P ppp( f1+f2+f3);
    const P pnn( f1-f2-f3);
    const P npn(-f1+f2-f3);
    const P nnp(-f1-f2+f3);

    back_triangle(ppp,pnn,npn,S);
    back_triangle(ppp,npn,nnp,S);
    back_triangle(ppp,nnp,pnn,S);
    back_triangle(nnp,pnn,npn,S);
  }

  void front_cube(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());

    const P ppp( f1+f2+f3);
    const P npp(-f1+f2+f3);
    const P nnp(-f1-f2+f3);
    const P pnp( f1-f2+f3);

    const P ppn( f1+f2-f3);
    const P npn(-f1+f2-f3);
    const P nnn(-f1-f2-f3);
    const P pnn( f1-f2-f3);

    front_arc(ppp,npp,S);
    front_arc(npp,nnp,S);
    front_arc(nnp,pnp,S);
    front_arc(pnp,ppp,S);

    front_arc(ppn,npn,S);
    front_arc(npn,nnn,S);
    front_arc(nnn,pnn,S);
    front_arc(pnn,ppn,S);

    front_arc(ppp,ppn,S);
    front_arc(npp,npn,S);
    front_arc(nnp,nnn,S);
    front_arc(pnp,pnn,S);
  }
  void  back_cube(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());

    const P ppp( f1+f2+f3);
    const P npp(-f1+f2+f3);
    const P nnp(-f1-f2+f3);
    const P pnp( f1-f2+f3);

    const P ppn( f1+f2-f3);
    const P npn(-f1+f2-f3);
    const P nnn(-f1-f2-f3);
    const P pnn( f1-f2-f3);

    back_arc(ppp,npp,S);
    back_arc(npp,nnp,S);
    back_arc(nnp,pnp,S);
    back_arc(pnp,ppp,S);

    back_arc(ppn,npn,S);
    back_arc(npn,nnn,S);
    back_arc(nnn,pnn,S);
    back_arc(pnn,ppn,S);

    back_arc(ppp,ppn,S);
    back_arc(npp,npn,S);
    back_arc(nnp,nnn,S);
    back_arc(pnp,pnn,S);
  }

  void front_octa(const Sphere& S, const frame& coords)
  {
    const P p1(coords.sea());
    const P p2(coords.sky());
    const P p3(coords.eye());

    const P m1(-coords.sea());
    const P m2(-coords.sky());
    const P m3(-coords.eye());

    // draw "even parity" triangles only
    front_triangle(p1,p2,p3,S);
    front_triangle(m1,m2,p3,S);

    front_triangle(m1,p2,m3,S);
    front_triangle(p1,m2,m3,S);
  }
  void  back_octa(const Sphere& S, const frame& coords)
  {
    const P p1(coords.sea());
    const P p2(coords.sky());
    const P p3(coords.eye());

    const P m1(-coords.sea());
    const P m2(-coords.sky());
    const P m3(-coords.eye());

    // draw "even parity" triangles only
    back_triangle(p1,p2,p3,S);
    back_triangle(m1,m2,p3,S);

    back_triangle(m1,p2,m3,S);
    back_triangle(p1,m2,m3,S);
  }


  void front_dodeca(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());
    const double gam(0.5*(1+sqrt(5)));

    const P pop( gam*f1 + f3);
    const P pom( gam*f1 - f3);
    const P mom(-gam*f1 - f3);
    const P mop(-gam*f1 + f3);

    const P ppo( f1 + gam*f2);
    const P pmo( f1 - gam*f2);
    const P mmo(-f1 - gam*f2);
    const P mpo(-f1 + gam*f2);

    const P opp( f2 + gam*f3);
    const P opm( f2 - gam*f3);
    const P omm(-f2 - gam*f3);
    const P omp(-f2 + gam*f3);

    // faces surrounding pop
    front_dual(pop, ppo, opp, S);
    front_dual(pop, opp, omp, S);
    front_dual(pop, omp, pmo, S);
    front_dual(pop, pmo, pom, S);
    front_dual(pop, pom, ppo, S);

    // respective reflections about link of pop
    front_dual(opp, ppo, mpo, S);
    front_dual(omp, opp, mop, S);
    front_dual(pmo, omp, mmo, S);
    front_dual(pom, pmo, omm, S);
    front_dual(ppo, pom, opm, S);

    // and their antipodes
    front_dual(mom, omm, mmo, S);
    front_dual(mom, opm, omm, S);
    front_dual(mom, mpo, opm, S);
    front_dual(mom, mop, mpo, S);
    front_dual(mom, mmo, mop, S);

    front_dual(omm, pmo, mmo, S);
    front_dual(opm, pom, omm, S);
    front_dual(mpo, ppo, opm, S);
    front_dual(mop, opp, mpo, S);
    front_dual(mmo, omp, mop, S);
  }

  void  back_dodeca(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());
    const double gam(0.5*(1+sqrt(5)));

    const P pop( gam*f1 + f3);
    const P pom( gam*f1 - f3);
    const P mom(-gam*f1 - f3);
    const P mop(-gam*f1 + f3);

    const P ppo( f1 + gam*f2);
    const P pmo( f1 - gam*f2);
    const P mmo(-f1 - gam*f2);
    const P mpo(-f1 + gam*f2);

    const P opp( f2 + gam*f3);
    const P opm( f2 - gam*f3);
    const P omm(-f2 - gam*f3);
    const P omp(-f2 + gam*f3);

    // faces surrounding pop
    back_dual(pop, ppo, opp, S);
    back_dual(pop, opp, omp, S);
    back_dual(pop, omp, pmo, S);
    back_dual(pop, pmo, pom, S);
    back_dual(pop, pom, ppo, S);

    // respective reflections about link of pop
    back_dual(opp, ppo, mpo, S);
    back_dual(omp, opp, mop, S);
    back_dual(pmo, omp, mmo, S);
    back_dual(pom, pmo, omm, S);
    back_dual(ppo, pom, opm, S);

    // and their antipodes
    back_dual(mom, omm, mmo, S);
    back_dual(mom, opm, omm, S);
    back_dual(mom, mpo, opm, S);
    back_dual(mom, mop, mpo, S);
    back_dual(mom, mmo, mop, S);

    back_dual(omm, pmo, mmo, S);
    back_dual(opm, pom, omm, S);
    back_dual(mpo, ppo, opm, S);
    back_dual(mop, opp, mpo, S);
    back_dual(mmo, omp, mop, S);
  }


  void front_icosa(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());
    const double gam(0.5*(1+sqrt(5)));

    const P pop( gam*f1 + f3);
    const P pom( gam*f1 - f3);
    const P mom(-gam*f1 - f3);
    const P mop(-gam*f1 + f3);

    const P ppo( f1 + gam*f2);
    const P pmo( f1 - gam*f2);
    const P mmo(-f1 - gam*f2);
    const P mpo(-f1 + gam*f2);

    const P opp( f2 + gam*f3);
    const P opm( f2 - gam*f3);
    const P omm(-f2 - gam*f3);
    const P omp(-f2 + gam*f3);

    // faces surrounding pop
    front_triangle(pop, ppo, opp, S);
    front_triangle(pop, opp, omp, S);
    front_triangle(pop, omp, pmo, S);
    front_triangle(pop, pmo, pom, S);
    front_triangle(pop, pom, ppo, S);

    // respective reflections about link of pop
    front_triangle(opp, ppo, mpo, S);
    front_triangle(omp, opp, mop, S);
    front_triangle(pmo, omp, mmo, S);
    front_triangle(pom, pmo, omm, S);
    front_triangle(ppo, pom, opm, S);

    // and their antipodes
    front_triangle(mom, omm, mmo, S);
    front_triangle(mom, opm, omm, S);
    front_triangle(mom, mpo, opm, S);
    front_triangle(mom, mop, mpo, S);
    front_triangle(mom, mmo, mop, S);

    front_triangle(omm, pmo, mmo, S);
    front_triangle(opm, pom, omm, S);
    front_triangle(mpo, ppo, opm, S);
    front_triangle(mop, opp, mpo, S);
    front_triangle(mmo, omp, mop, S);
  }

  void  back_icosa(const Sphere& S, const frame& coords)
  {
    const P f1(coords.sea());
    const P f2(coords.sky());
    const P f3(coords.eye());
    const double gam(0.5*(1+sqrt(5)));

    const P pop( gam*f1 + f3);
    const P pom( gam*f1 - f3);
    const P mom(-gam*f1 - f3);
    const P mop(-gam*f1 + f3);

    const P ppo( f1 + gam*f2);
    const P pmo( f1 - gam*f2);
    const P mmo(-f1 - gam*f2);
    const P mpo(-f1 + gam*f2);

    const P opp( f2 + gam*f3);
    const P opm( f2 - gam*f3);
    const P omm(-f2 - gam*f3);
    const P omp(-f2 + gam*f3);

    // faces surrounding pop
    back_triangle(pop, ppo, opp, S);
    back_triangle(pop, opp, omp, S);
    back_triangle(pop, omp, pmo, S);
    back_triangle(pop, pmo, pom, S);
    back_triangle(pop, pom, ppo, S);

    // respective reflections about link of pop
    back_triangle(opp, ppo, mpo, S);
    back_triangle(omp, opp, mop, S);
    back_triangle(pmo, omp, mmo, S);
    back_triangle(pom, pmo, omm, S);
    back_triangle(ppo, pom, opm, S);

    // and their antipodes
    back_triangle(mom, omm, mmo, S);
    back_triangle(mom, opm, omm, S);
    back_triangle(mom, mpo, opm, S);
    back_triangle(mom, mop, mpo, S);
    back_triangle(mom, mmo, mop, S);

    back_triangle(omm, pmo, mmo, S);
    back_triangle(opm, pom, omm, S);
    back_triangle(mpo, ppo, opm, S);
    back_triangle(mop, opp, mpo, S);
    back_triangle(mmo, omp, mop, S);
  }



  // Hyperbolic lines in upper half space
  // For compatibility with 2-D geometry, the boundary is the (x1,x3)-plane

  void hyperbolic_line(const P& tail, const P& head)
  {
    if ( (tail.x2() < 0) || (head.x2() < 0) )
      epix_warning("Endpoint not in upper half-space");

    P sh_tail(tail%E_2); // shadow of tail
    P sh_head(head%E_2);
    double ht_tail(tail|E_2), ht_head(head|E_2);
    double dist(norm(sh_head - sh_tail)); // dist btw projections to boundary

    if (dist < EPIX_EPSILON)
      line(tail, head);

    else
      {
	// use similar triangles to find center; get basis; draw arc
	double diff((ht_head - ht_tail)*(ht_head + ht_tail)/dist);
	double frac(0.5*(diff + dist));
	P center((1-frac/dist)*sh_tail + (frac/dist)*sh_head);
	P e1(tail - center);
	double rad(norm(e1));

	P e2(E_2%e1);
	e2 *= rad/norm(e2);
	double theta(Acos(((head-center)|e1)/(rad*rad)));

	ellipse(center, e1, e2, 0, theta);
      }
  } // end of hyperbolic_line


  // Lines in Poincare disk model.
  //
  // Consider the "positive" portion of the standard hyperboloid of two 
  // sheets: x^2 + y^2 + 1 = z^2, z>0, and consider copies of the unit
  // disk in the planes z=0 (D0) and z=1 (D1). The Klein model of the disk
  // is gotten by stereographic projection from the origin to D1, while the
  // Poincare model is gotten by stereographic projection from (0,0,-1) to
  // D0. Appropriate compositions of these projection maps are hyperbolic
  // isometries. The algorithm for drawing lines in the disk model is to
  // find the images of the endpoints in the Klein model, draw the line
  // between them, and map this line back to the Poincare model. Because
  // the isometry is "square-root-like" in the radial direction at the
  // unit circle, the points on the Klein line are spaced quadratically
  // close together at the endpoints of the segment (the variable "s") so
  // their images will be roughly equally-spaced in the Poincare model.
  // There is no visual harm if one or both endpoints are far from the
  // circle, and the result is acceptable if both points are on or near the
  // circle. The number of points to draw is determined both by the true
  // distance between the endpoints and by how close they are to the circle.

  P poincare_klein(P pt)
  {
    return (2.0/(1+(pt|pt)))*pt;
  }

  P klein_poincare(P pt)
  {
    return (1.0/(1+sqrt(fabs(1 - (pt|pt)))))*pt;
  }

  P p_line(const P& tail, const P& head, double t)
  {
    double s(0.5*(1+std::cos(M_PI*t))); // s in [0,1]

    P current((s*poincare_klein(tail)) + ((1-s)*poincare_klein(head)));
    return klein_poincare(current);
  }

  void disk_line(const P& tail, const P& head)
  {
    const int N(EPIX_NUM_PTS);
    std::vector<P> data(N+1);

    double t(0);

    for (int i=0; i <= N; ++i, t += 1.0/N)
      data.at(i) = p_line(tail, head, t);

    path temp(data, false, false);
    temp.draw();
  }

} // end of namespace
