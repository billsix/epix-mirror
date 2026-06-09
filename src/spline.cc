/* 
 * spline.cc -- ePiX n_spline class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.9
 * Last Change: August 02, 2007
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

#include "path.h"
#include "spline.h"

namespace ePiX {
  // natural spline constructor
  n_spline::n_spline(const std::vector<P>& data, bool loop)
    : m_vertices(data)
  {
    if (loop)
      m_vertices.push_back(m_vertices.at(0));

    const unsigned int N(m_vertices.size()-1);

    // notation and algorithm from John H. Matthews and Kurtis K. Fink
    std::vector<double> tmp_h(N+1); // h_k = x_{k+1} - x_k
    std::vector<double> tmp_d(N+1); // d_k = (y_{k+1} - y_k)/h_k
    std::vector<double> tmp_u(N+1); // u_k = 6*(d_{k+1} - d_k)
    std::vector<double> tmp_y(N+1); // y_k

    // entries of tridiagonal coefficient matrix
    std::vector<double> tmp_A(N+1);   // subdiagonal of coefficient matrix
    std::vector<double> tmp_C(N+1);   // superdiagonal
    std::vector<double> tmp_D(N+1); // diagonal

    std::vector<double> tmp_m(N+1); // m_k = S"(x_k), 1 <= k <= N-1

    // initialize...
    for (unsigned int i = 0; i <= N; ++i)
      {
	P curr(m_vertices.at(i));

	tmp_y.at(i) = curr.x2();

	if (i <= N-1)
	  {
	    P next(m_vertices.at(i+1));

	    tmp_h.at(i) = (next.x1() - curr.x1());
	    tmp_d.at(i) = (next.x2() - curr.x2())/tmp_h.at(i);

	    if (0 < i)
	      {
		tmp_u.at(i) = 6*(tmp_d.at(i) - tmp_d.at(i-1));
		tmp_D.at(i) = 2*(tmp_h.at(i) + tmp_h.at(i-1));

		if (i <= N-2)
		  {
		    tmp_A.at(i) = tmp_h.at(i);
		    tmp_C.at(i) = tmp_h.at(i);
		  }
	      }
	  }
      }

    for (unsigned int i=2; i <= N; ++i)
      {
	const double ratio(tmp_A.at(i-1)/tmp_D.at(i-1));

	tmp_D.at(i) -= ratio*tmp_C.at(i-1);
	tmp_u.at(i) -= ratio*tmp_u.at(i-1);
      }

    tmp_m.at(N) = tmp_u.at(N-1)/tmp_D.at(N-1);

    for (unsigned int i=N-1; 0 < i; --i)
      tmp_m.at(i) = (tmp_u.at(i) - tmp_C.at(i)*tmp_m.at(i+1))/tmp_D.at(i);

    std::vector<double> tmp_k0(N);
    std::vector<double> tmp_k1(N);
    std::vector<double> tmp_k2(N);
    std::vector<double> tmp_k3(N);

    for (unsigned int i=0; i < N; ++i)
      {
	tmp_k0.at(i) = tmp_y.at(i);
	tmp_k1.at(i)
	  = tmp_d.at(i) - (tmp_h.at(i)/6)*(2*tmp_m.at(i) + tmp_m.at(i+1));

	tmp_k2.at(i) =  0.5*tmp_m.at(i);
	tmp_k3.at(i) = (tmp_m.at(i+1) - tmp_m.at(i))/(6*tmp_h.at(i));
      }

    swap(tmp_k0, m_k0);
    swap(tmp_k1, m_k1);
    swap(tmp_k2, m_k2);
    swap(tmp_k3, m_k3);
  }

  path n_spline::data(const unsigned int num_pts) const
  {
    path value;
    for (unsigned int i=0; i < m_vertices.size()-1; ++i)
      {
	const double Xmin(m_vertices.at(i).x1());
	const double dx((m_vertices.at(i+1).x1() - Xmin)/num_pts);

	for (unsigned int j=0; j<=num_pts; ++j)
	  {
	    const double w(j*dx);
	    const double x(Xmin + w);

	    // Horner's form
      double y((((m_k3.at(i)*w) + m_k2.at(i))*w + m_k1.at(i))*w + m_k0.at(i));

	    value.pt(P(x, y));
	  }
      }	    
    return value;
  }

} // end of namespace
