/* 
 * domain.h -- ePiX mesh and domain classes
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.1.6
 * Last Change: July 04, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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

#ifndef EPIX_DOMAIN
#define EPIX_DOMAIN

#include <list>

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
    domain(const P& arg1, const P& arg2, const mesh& c, const mesh& f);
    domain(const P& arg1, const P& arg2, const mesh& c);

    // 1-dim'l domain
    domain(double t_min, double t_max, int num_pts);

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

    // state  functions for internal use
    P corner1() const;
    P corner2() const;

    double corner1_x1() const;
    double corner1_x2() const;
    double corner1_x3() const;

    double corner2_x1() const;
    double corner2_x2() const;
    double corner2_x3() const;

    int coarse_n1() const;
    int coarse_n2() const;
    int coarse_n3() const;

    int fine_n1() const;
    int fine_n2() const;
    int fine_n3() const;

  private:
    P m_corner1;
    P m_corner2;
    mesh m_coarse;
    mesh m_fine;
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
