/* 
 * plots.cc: Plots of user-defined functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.1
 * Last Change: December 26, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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

#include <math.h>

#include "globals.h"
#include "functions.h"
#include "triples.h"
#include "domain.h"

#include "path.h"
#include "cropping.h"

#include "camera.h"
#include "objects.h"
#include "output.h"
#include "curves.h"
#include "plots.h"

namespace ePiX {

  extern epix_camera camera;

  // tuples of real-valued plot arguments
  void tan_line(double f1(double t), double f2(double t), double t0)
  {
    P dir = P(deriv(f1, t0), deriv(f2, t0));
    Line(P(f1(t0), f2(t0)), P(f1(t0), f2(t0))+dir);
  }	      
  void tan_line(double f(double t), double t0)
  {
    P dir = P(1, deriv(f, t0));
    Line(P(t0, f(t0)), P(t0, f(t0))+dir);
  }
  void envelope(double f1(double t), double f2(double t), 
		double t_min, double t_max, int num_pts)
  {
    const double step = (t_max - t_min)/num_pts;
    double t=t_min;
    P dir;

    for (int i=0; i <= num_pts; ++i, t += step)
      {
	dir = P(deriv(f1, t), deriv(f2, t));
	Line(P(f1(t), f2(t)), P(f1(t), f2(t)) + dir);
      }
  }	      
  void envelope(double f(double t), double t_min, double t_max, int num_pts)
  {
    const double step = (t_max - t_min)/num_pts;
    double t=t_min;
    P dir;

    for (int i=0; i <= num_pts; ++i, t += step)
      {
	dir = P(1, deriv(f, t));
	Line(P(t, f(t)), P(t, f(t))+dir);
      }
  }	      

  // P-valued plot argument
  void tan_line(P f(double t), double t0)
  {
    P dir = deriv(f, t0);
    Line(f(t0), f(t0)+dir);
  }
  void envelope(P f(double t), double t_min, double t_max, int num_pts)
  {
    const double step = (t_max - t_min)/num_pts;
    double t=t_min;
    P dir;

    for (int i=0; i <= num_pts; ++i, t += step)
      {
	dir = deriv(f, t);
	Line(f(t), f(t) + dir);
      }
  }	      

  // Tangent field along parametrized path
  void tan_field(double f1(double), double f2(double), double t_min,
		 double t_max, double num_pts)
  {
    double t=t_min;
    const double step=(t_max - t_min)/num_pts;

    for (int i=0; i <= num_pts; ++i, t += step)
      arrow(P(f1(t), f2(t)), P(f1(t), f2(t)) +
	    step*P(deriv(f1, t), deriv(f2, t)));

    end_stanza();
  }

  // P-valued plot argument
  void tan_field(P f(double), double t_min, double t_max, double num_pts)
  {
    double t=t_min;
    const double step=(t_max - t_min)/num_pts;

    for (int i=0; i <= num_pts; ++i, t += step)
      arrow(f(t), f(t) + step*deriv(f, t)); // wrong scale?

    end_stanza();
  }

  // vector, dart, and slope fields
  static void field_element(P tail, P head, epix_field_type TYPE)
  {
    switch(TYPE) {

    case VECTOR:
      arrow(tail, head);
      break;

    case DART:
      dart(tail, head); // assuming caller has adjusted length
      break;

    case SLOPE:
      line(tail, head, 0, 1);
      break;

    default:
      ; // do nothing

    } // end of switch(TYPE)

  } // end of field_element


  void draw_field(P F(double, double, double), 
		  P p, P q, int n1, int n2, 
		  epix_field_type TYPE = VECTOR)
  {
    P diagonal = q-p;
    int perp_count = 0;
    P jump1, jump2;

    if (fabs(diagonal|E_1) < EPIX_EPSILON)
      {
        ++perp_count;
        jump1 = E_2&diagonal;
        jump2 = E_3&diagonal;

      }
    if (fabs(diagonal|E_2) < EPIX_EPSILON)
      {
        ++perp_count;
        jump1 = E_3&diagonal;
        jump2 = E_1&diagonal;
      }
    if (fabs(diagonal|E_3) < EPIX_EPSILON)
      {
        ++perp_count;
        jump1 = E_1&diagonal;
        jump2 = E_2&diagonal;
      }

    if (perp_count == 0)
      {
	epix_warning("Projecting corners of vector field plot");
        jump1 = E_1&diagonal;
        jump2 = E_2&diagonal;
	p = p%E_3; // project corners to (x1, x2)-plane
	q = q%E_3;
      }

    // grid line spacing
    jump1 *= 1.0/n1;
    jump2 *= 1.0/n2;

    const double d1 = norm(jump1);
    const double d2 = norm(jump2);

    P base = p;
    P vect;

    for (int i=0; i <= n1; ++i)
      for (int j=0; j <= n2; ++j)
	{
	  base = p + (i*jump1 + j*jump2);
	  vect = F(base.x1(), base.x2(), base.x3());

	  if (norm(camera(base+vect) - camera(base)) < EPIX_EPSILON)
	    marker(base, BBOX);

	  else if (TYPE == VECTOR)
	    field_element(base, base + vect, VECTOR);

	  else
	    {
	      double object_len = norm(c2s(camera(base+vect)-camera(base)));
	      pair dX = c2s(pair(d1, d2));
	      // Hardwired constant 0.4: Segment 80% of shorter grid length
	      double minimum = (dX.x1() > dX.x2()) ? 0.4*dX.x2() : 0.4*dX.x1();
	      if ( object_len > EPIX_EPSILON) // not projected to zero
		vect *= minimum/object_len;

	      field_element(base - vect, base + vect, TYPE);
	    }
	}
    end_stanza();
  } // end of draw_field F(x, y, z)


  void draw_field(P F(double, double), 
		  P p, P q, int n1, int n2, 
		  epix_field_type TYPE = VECTOR)
  {
    P diagonal = q-p;
    P jump1 = E_1&diagonal;
    P jump2 = E_2&diagonal;
    p = p%E_3; // project corners to (x1, x2)-plane
    q = q%E_3;

    // grid line spacing
    jump1 *= 1.0/n1;
    jump2 *= 1.0/n2;

    const double d1 = norm(jump1);
    const double d2 = norm(jump2);

    P base = p;
    P vect;

    for (int i=0; i <= n1; ++i)
      for (int j=0; j <= n2; ++j)
	{
	  base = p + (i*jump1 + j*jump2);
	  vect = F(base.x1(), base.x2());

	  if (norm(camera(base+vect) - camera(base)) < EPIX_EPSILON)
	    marker(base, BBOX);

	  else if (TYPE == VECTOR)
	    field_element(base, base + vect, VECTOR);

	  else
	    {
	      double object_len = norm(c2s(camera(base+vect)-camera(base)));
	      pair dX = c2s(pair(d1, d2));
	      // Hardwired constant 0.4: Segment 80% of shorter grid length
	      double minimum = (dX.x1() > dX.x2()) ? 0.4*dX.x2() : 0.4*dX.x1();
	      if ( object_len > EPIX_EPSILON) // not projected to zero
		vect *= minimum/object_len;

	      field_element(base - vect, base + vect, TYPE);
	    }
	}
    end_stanza();
  } // end of draw_field F(x, y)


  void riemann_sum(double f(double), const double a, const double b,
		   unsigned int n, epix_integral_type TYPE)
  {
    double x=a, dx=(b-a)/n;

    for (unsigned int i=0; i < n; ++i)
      {
	x = a + i*dx;

	switch (TYPE) {

	case LEFT:

	  rect(P(x, f(x)), P(x+dx,0));
	  break;

	case RIGHT:

	  rect(P(x, 0), P(x+dx, f(x+dx)));
	  break;

	case UPPER:

	  rect(P(x, 0), P(x+dx, sup(f, x, x+dx)));
	  break;

	case LOWER:

	  rect(P(x, 0), P(x+dx, inf(f, x, x+dx)));
	  break;

	case TRAP:

	  quad(P(x, 0), P(x+dx, 0), P(x+dx, f(x+dx)), P(x, f(x)));
	  break;

	case MIDPT:
	  
	  rect(P(x, 0), P(x+dx, f(x+0.5*dx)));
	  break;

	default:
	  ;

	} // end of switch(TYPE)
      }
  } // end of riemann_sum()


  // Jay Belanger's shaded plot routines -- December 1, 2002
  // Re-implemented using paths, July 16, 2004
  void shadeplot(double f1(double), double f2(double), 
		 double t_min, double t_max, int num_pts)
  {
    // build contour
    path boundary = path(f1, t_min, t_max, num_pts);
    path graph2   = path(f2, t_min, t_max, num_pts);

    boundary -= graph2;

    boundary.close();
    boundary.set_fill();
    boundary.draw();
    end_stanza();
  }

  // Adaptive plotting; not working perfectly, likely to be removed
  void adplot(double f1(double), double f2(double), 
	      double t_min, double t_max, int num_pts)
  {
    int N = num_pts*EPIX_ITERATIONS;

    double t = t_min;
    double dt = (t_max - t_min)/N;

    std::vector<P> small_steps(N+1);
    std::vector<double> lengths(N+1);
    std::vector<vertex> data(num_pts+1);

    small_steps.at(0) = P(f1(t), f2(t));
    lengths.at(0) = 0;

    t += dt;

    for (int i=1; i <= N; ++i, t += dt)
      {
	small_steps.at(i) = P(f1(t), f2(t));
	lengths.at(i) 
	  = lengths.at(i-1) + norm(small_steps.at(i) - small_steps.at(i-1));
      }

    double step = lengths.at(N)/num_pts;

    int j=0;
    double next_length = 0;
    for (int i=0; i < num_pts; ++i, next_length += step)
      {
	// try to space elements of data at equal distance
	while (lengths.at(j) < next_length)
	  ++j;

	data.at(i) = vertex(small_steps.at(j));
      }
    data.at(num_pts) = vertex(small_steps.at(N));

    path temp = path(data, false);

    temp.draw();
    end_stanza();
  } // end of adplot(f1, f2)

} /* end of namespace */
