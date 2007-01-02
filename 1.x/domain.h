/* 
 * domain.h -- ePiX mesh and domain classes
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

#ifndef EPIX_DOMAIN
#define EPIX_DOMAIN

#include <list>

#include "globals.h"
#include "triples.h"
#include "functions.h"

namespace ePiX {

  class mesh {
  public:
    mesh(int n1, int n2, int n3=1);
    mesh(int n); // mesh(n, n, n);
    mesh();      // mesh(1, 1, 1);

    int n1() const { return n_1; }
    int n2() const { return n_2; }
    int n3() const { return n_3; }

  private:
    int n_1;
    int n_2;
    int n_3;
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
	   mesh c=mesh(1,1,1), mesh f=mesh(1,1,1));

    // 1-dim'l domain
    domain(double t_min, double t_max, int num_pts)
      : corner1(t_min), corner2(t_max), coarse(num_pts), fine(num_pts) { }

    int dim() const; // dimension

    // big and small steps
    double step1() const;
    double step2() const;
    double step3() const;

    double dx1() const;
    double dx2() const;
    double dx3() const;

    // resizing attempts to preserve real resolution
    domain resize1(double a1, double b1) const;
    domain resize2(double a2, double b2) const;
    domain resize3(double a3, double b3) const;

    // coordinate slices
    domain slice1(double a1) const;
    domain slice2(double a2) const;
    domain slice3(double a3) const;

    // "n=0": use number from coarse mesh
    std::list<domain> slices1(const unsigned int n=0) const;
    std::list<domain> slices2(const unsigned int n=0) const;
    std::list<domain> slices3(const unsigned int n=0) const;
  }; // end of class domain

  class domain_list {
  public:
    domain_list(std::list<domain>); // implicitly converts "slices" output
    domain_list& add(const domain&);
    domain_list& add(const domain_list&);

    std::list<domain> m_list; // public: we're a simple wrapper
  }; // end of class domain_list
} // end of namespace

#endif /* EPIX_DOMAIN */
