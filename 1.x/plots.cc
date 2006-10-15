/* 
 * plots.cc: Plotting functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.16
 * Last Change: October 14, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
#include "functions.h"

#include "lengths.h" // dart field must calculate page distances

#include "map.h"
#include "domain.h"

#include "path.h"
#include "cropping.h"

#include "camera.h"
#include "objects.h"
#include "output.h"
#include "curves.h"
#include "plot_algorithms.h"
#include "plots.h"

namespace ePiX {

  extern epix_camera camera;

  // f:R -> R^3
  void plot(P f(double), double t_min, double t_max, int num_pts)
  {
    plot_map_dom(path_map(f), domain(t_min, t_max, num_pts));
  }

  // f:R -> R x R x R
  void plot(double f1(double), double f2(double), double f3(double),
	    double t_min, double t_max, int num_pts)
  {
    plot_map_dom(column_1var(f1,f2,f3), domain(t_min, t_max, num_pts));
  }

  // f:R -> R
  void plot(double f(double), double t_min, double t_max, int num_pts)
  {
    plot(id, f, zero, t_min, t_max, num_pts); 
  }

  void plot(const Deriv& f, double t_min, double t_max, int num_pts)
  { 
    plot_map_dom(f, domain(t_min, t_max, num_pts));
  }

  void plot(const Integral& f, double t_min, double t_max, int num_pts)
  {
    plot_map_dom(f, domain(t_min, t_max, num_pts));
  }

  // f: R -> R x R
  void plot(double f1(double), double f2(double), 
	    double t_min, double t_max, int num_pts)
  {
    plot(f1, f2, zero, t_min, t_max, num_pts); 
  }

  void polarplot (double f(double), double t_min, double t_max, int num_pts)
  {
    plot_map_dom(column_1var(f, id, zero, cylindrical), 
		 domain(t_min, t_max, num_pts));
  }
  
  // surfaces
  // f:R^2 -> R x R x R
  void plot(double f1(double u1, double u2),
	    double f2(double u1, double u2),
	    double f3(double u1, double u2), 
	    const P& min, const P& max, const mesh& coarse, const mesh& fine)
  {
    plot_map_dom(column_2var(f1,f2,f3), domain(min, max, coarse, fine));
  }

  void plot(double f1(double u1, double u2),
	    double f2(double u1, double u2),
	    double f3(double u1, double u2), 
	    const domain& R)
  {
    plot_map_dom(column_2var(f1,f2,f3), R);
  }

  // f:R^2 -> R
  void plot(double f(double u1, double u2),
	    const P& p1, const P& p2, const mesh& coarse, const mesh& fine)
  {
    plot(proj1, proj2, f, p1, p2, coarse, fine);
  }

  void plot(double f(double u1, double u2), const domain& R)
  {
    plot_map_dom(column_2var(proj1, proj2, f), R);
  }

  // f:R^2 -> R^3
  void plot(P f(double u1, double u2),
	    const P& p1, const P& p2, const mesh& coarse, const mesh& fine)
  {
    plot_map_dom(surface_map(f), domain(p1, p2, coarse, fine));
  }

  void plot(P f(double u1, double u2), const domain& R)
  {
    plot_map_dom(surface_map(f), R);
  }


  // Derivatives and integrals
  void plot_deriv(double f(double t), double a, double b, int num_pts)
  {
    plot_function(D(f), a, b, num_pts);
  }

  void plot_int(double f(double), double a, double b, int num_pts)
  {
    plot_function(I(f, a), a, b, num_pts);
  }

  void plot_int(double f(double), double x0, double a, double b, int num_pts)
  {
    plot_function(I(f, x0), a, b, num_pts);
  }

  // tuples of real-valued plot arguments
  void tan_line(double f1(double t), double f2(double t), double t0)
  {
    Line(P(f1(t0), f2(t0)),
	 P(f1(t0), f2(t0)) + P(deriv(f1, t0), deriv(f2, t0)));
  }	      

  void tan_line(double f(double t), double t0)
  {
    Line(P(t0, f(t0)), P(t0, f(t0)) + P(1, deriv(f, t0)));
  }

  void envelope(double f1(double t), double f2(double t), 
		double t_min, double t_max, int num_pts)
  {
    const double step((t_max - t_min)/num_pts);
    double t(t_min);

    for (int i=0; i <= num_pts; ++i, t += step)
      Line(P(f1(t), f2(t)),
	   P(f1(t), f2(t)) + P(deriv(f1, t), deriv(f2, t)));
  }	      

  void envelope(double f(double t), double t_min, double t_max, int num_pts)
  {
    const double step((t_max - t_min)/num_pts);
    double t(t_min);

    for (int i=0; i <= num_pts; ++i, t += step)
      Line(P(t, f(t)), P(t, f(t)) + P(1, deriv(f, t)));
  }	      

  // P-valued plot argument
  void tan_line(P f(double t), double t0)
  {
    Line(f(t0), f(t0) + deriv(f, t0));
  }

  void envelope(P f(double t), double t_min, double t_max, int num_pts)
  {
    const double step((t_max - t_min)/num_pts);
    double t(t_min);

    for (int i=0; i <= num_pts; ++i, t += step)
      Line(f(t), f(t) + deriv(f, t));
  }	      

  // Tangent field along parametrized path
  void tan_field(double f1(double), double f2(double), double t_min,
		 double t_max, double num_pts)
  {
    const double step((t_max - t_min)/num_pts);
    double t(t_min);

    for (int i=0; i <= num_pts; ++i, t += step)
      arrow(P(f1(t), f2(t)), P(f1(t), f2(t)) +
	    step*P(deriv(f1, t), deriv(f2, t)));

    end_stanza();
  }

  // P-valued plot argument
  void tan_field(P f(double), double t_min, double t_max, double num_pts)
  {
    const double step((t_max - t_min)/num_pts);
    double t(t_min);

    for (int i=0; i <= num_pts; ++i, t += step)
      arrow(f(t), f(t) + step*deriv(f, t));

    end_stanza();
  }

  // Slope, dart, and vector fields

  // utility functions
  // vector, dart, and slope fields
  void field_element(const P& tail, const P& head, epix_field_type TYPE)
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

  void draw_field(P F(double, double, double), P p, P q, int n1, int n2, 
		  epix_field_type TYPE)
  {
    P diagonal(q-p);
    int perp_count(0);
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
	p %= E_3; // project corners to (x1, x2)-plane
	q %= E_3;
      }

    // grid line spacing
    jump1 *= 1.0/n1;
    jump2 *= 1.0/n2;

    const double d1(norm(jump1));
    const double d2(norm(jump2));

    for (int i=0; i <= n1; ++i)
      for (int j=0; j <= n2; ++j)
	{
	  P base(p + (i*jump1 + j*jump2));
	  P vect(F(base.x1(), base.x2(), base.x3()));

	  if (norm(camera(base+vect) - camera(base)) < EPIX_EPSILON)
	    marker(base, BBOX);

	  else if (TYPE == VECTOR)
	    field_element(base, base + vect, VECTOR);

	  else
	    {
	      double object_len(norm(c2s(camera(base+vect)-camera(base))));
	      pair dX(c2s(pair(d1, d2)));
	      // Hardwired constant 0.4: Segment 80% of shorter grid length
	      double minimum((dX.x1() > dX.x2()) ? 0.4*dX.x2() : 0.4*dX.x1());
	      if ( object_len > EPIX_EPSILON) // not projected to zero
		vect *= minimum/object_len;

	      field_element(base - vect, base + vect, TYPE);
	    }
	}
    end_stanza();
  } // end of draw_field F(x, y, z)


  void draw_field(P F(double, double), P p, P q, int n1, int n2, 
		  epix_field_type TYPE = VECTOR)
  {
    P diagonal(q-p);
    P jump1(E_1&diagonal);
    P jump2(E_2&diagonal);
    p %= E_3; // project corners to (x1, x2)-plane
    q %= E_3;

    // grid line spacing
    jump1 *= 1.0/n1;
    jump2 *= 1.0/n2;

    const double d1(norm(jump1));
    const double d2(norm(jump2));

    for (int i=0; i <= n1; ++i)
      for (int j=0; j <= n2; ++j)
	{
	  P base(p + (i*jump1 + j*jump2));
	  P vect(F(base.x1(), base.x2()));

	  if (norm(camera(base+vect) - camera(base)) < EPIX_EPSILON)
	    marker(base, BBOX);

	  else if (TYPE == VECTOR)
	    field_element(base, base + vect, VECTOR);

	  else
	    {
	      double object_len(norm(c2s(camera(base+vect)-camera(base))));
	      pair dX(c2s(pair(d1, d2)));
	      // Hardwired constant 0.4: Segment 80% of shorter grid length
	      double minimum((dX.x1() > dX.x2()) ? 0.4*dX.x2() : 0.4*dX.x1());
	      if ( object_len > EPIX_EPSILON) // not projected to zero
		vect *= minimum/object_len;

	      field_element(base - vect, base + vect, TYPE);
	    }
	}
    end_stanza();
  } // end of draw_field F(x, y)


  // planar fields
  void slope_field(P F(double, double),
		   const P& p, const P& q, int n1, int n2)
  {
    draw_field(F, p, q, n1, n2, SLOPE);
  }

  void dart_field(P F(double, double),
		  const P& p, const P& q, int n1, int n2)
  {
    draw_field(F, p, q, n1, n2, DART);
  }

  void vector_field(P F(double, double),
		    const P& p, const P& q, int n1, int n2)
  {
    draw_field(F, p, q, n1, n2, VECTOR);
  }

  // spatial fields
  void slope_field(P F(double, double, double),
		   const P& p, const P& q, int n1, int n2)
  {
    draw_field(F, p, q, n1, n2, SLOPE);
  }

  void dart_field(P F(double, double, double),
		  const P& p, const P& q, int n1, int n2)
  {
    draw_field(F, p, q, n1, n2, DART);
  }

  void vector_field(P F(double, double, double), 
		    const P& p, const P& q, int n1, int n2)
  {
    draw_field(F, p, q, n1, n2, VECTOR);
  }


  // Solutions of ODE systems
  // start at time 0
  void ode_plot (P F(double, double), const P& start, double t_max, int n)
  {
    euler_plot(surface_map(F), start, 0, t_max, n);
  }

  void ode_plot (P F(double, double, double), const P& start,
		 double t_max, int num_pts)
  {
    euler_plot(space_map(F), start, 0, t_max, num_pts);
  }

  // arbitrary start time
  void ode_plot (P F(double, double), const P& start, 
		 double t_min, double t_max, int num_pts)
  {
    euler_plot(surface_map(F), start, t_min, t_max, num_pts);
  }

  void ode_plot (P F(double, double, double), const P& start, 
		 double t_min, double t_max, int num_pts)
  {
    euler_plot(space_map(F), start, t_min, t_max, num_pts);
  }


  // flow x0 under field for specified time
  P flow (P F(double, double), const P& start, double t_max, int n)
  {
    return euler_flow(surface_map(F), start, t_max, n);
  }

  P flow (P F(double, double, double), const P& start, double t_max, int n)
  {
    return euler_flow(space_map(F), start, t_max, n);
  }


  void riemann_sum(double f(double), const double a, const double b,
		   unsigned int n, epix_integral_type TYPE)
  {
    const double dx((b-a)/n);

    for (unsigned int i=0; i < n; ++i)
      {
	double x(a + i*dx);

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


  // Jay Belanger's shaded plotting functions -- December 1, 2002
  // Re-implemented using paths, July 16, 2004
  void shadeplot(double f1(double), double f2(double), 
		 double t_min, double t_max, int num_pts)
  {
    // build contour
    path boundary(f1, t_min, t_max, num_pts);
    path graph2(f2, t_min, t_max, num_pts);

    boundary -= graph2;

    boundary.close();
    boundary.set_fill();
    boundary.draw();
    end_stanza();
  } // end of shadeplot

  void shadeplot(double f(double), double t_min, double t_max, int num_pts)
  {
    shadeplot(f, zero, t_min, t_max, num_pts);
  }

  void blackplot(double f1(double), double f2(double), 
		 double t_min, double t_max, int num_pts)
  {
    double temp_gray(epix::get_gray()); // get state
    gray(1);
    shadeplot(f1, f2, t_min, t_max, num_pts);
    gray(temp_gray); // restore
  }

  void blackplot(double f(double), double t_min, double t_max, int num_pts)
  {
    blackplot(f, zero, t_min, t_max, num_pts);
  }

  void whiteplot(double f1(double), double f2(double), 
		 double t_min, double t_max, int num_pts)
  {
    double temp_gray(epix::get_gray());
    gray(0);
    shadeplot(f1, f2, t_min, t_max, num_pts);
    gray(temp_gray);
  }

  void whiteplot(double f(double), double t_min, double t_max, int num_pts)
  {
    whiteplot(f, zero, t_min, t_max, num_pts);
  }


  // wiremesh surface plotting
  void clipplot(P Phi(double, double),
		const P& p1, const P& p2, const mesh& N, const mesh& num_pts)
  {
    bool temp_clip(epix::clipping); // get state
    clip();
    plot_map_dom(surface_map(Phi), domain(p1, p2, N, num_pts));
    clip(temp_clip); // restore
  }

  void cropplot(P Phi(double, double),
	    const P& p1, const P& p2, const mesh& N, const mesh& num_pts)
  {
    bool temp_crop(epix::cropping);
    crop();
    plot_map_dom(surface_map(Phi), domain(p1, p2, N, num_pts));
    crop(temp_crop);
  }
} // end of namespace
