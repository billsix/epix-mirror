/* 
 * Line.cc -- ePiX::Line class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc7
 * Last Change: April 18, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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

#include <iostream>

#include "exceptions.h"
#include "Line.h"
#include "../globals.h"
#include "../triples.h"
#include "../camera.h"
#include "../lengths.h"
#include "../output.h"
#include "../objects.h"

using namespace ePiX;

namespace ePiX {

  // intersection
  triple operator * (Line L1, Line L2)
  {
    triple p1 = L1.X0();
    triple p2 = L2.X0();

    triple dir1 = L1.direction();
    triple dir2 = L2.direction();
    triple normal = dir1*dir2; // cross product
    double normal_length = raw_len(normal);

    if (fabs( (dir1*dir2) | (p2 - p1) ) != 0)
      throw join_error(NON_COPLANAR);

    else if (L1 || L2)
      throw join_error(PARALLEL);

    else
      {
	triple unit_normal = (1.0/normal_length)*normal;
	triple dir3 = dir2*unit_normal;

	double s = (dir3|(p2-p1))/(dir3|dir1);
	return p1 + s*dir1;
      }
  }

  // parallel
  bool operator || (const Line& L1, const Line& L2)
    {
      return (norm((L1.direction)*(L2.direction)) == 0);
    }

  // (in)equality
  bool operator == (const Line& L1, const Line& L2)
    {
      return (( (L1.direction) || (L2.direction) ) &&
	      norm(((L1.X0) - (L2.X0))*(L2.direction)) == 0);
    }

  bool operator != (const Line& L1, const Line& L2)
    {
      return !(( (L1.direction) || (L2.direction) ) &&
	       norm(((L1.X0) - (L2.X0))*(L2.direction)) == 0);
    }

  /*
  bool contains(const triple& arg)
    { 
      return (*this) || (Line(arg, this->X0));
    }
  */

  /*
   * Line(p1, p2).draw() -- Portion of line (p1,p2) that lies in clip_box
   *
   * Algorithm: Consider the line parametrized by p1 + t*(p2-p1),
   * and find the times at which this line crosses the planes bounding
   * the clip_box (if applicable). Take the maximum minimum time (t_min)
   * and the minimum maximum time (t_max). If t_min < t_max, the param
   * line passes through the clip_box, otherwise not.
   */

  static inline double min(double a, double b)
  {
    if (a < b) return a;
    else return b;
  }
  static inline double max(double a, double b)
  {
    if (a > b) return a;
    else return b;
  }

  void Line::draw(int n)
  {
    triple arg1 = this->X0;
    triple arg2 = arg1 + this->direction;

    // entry, exit times
    double t_min, t_max;
    double t1_min, t1_max, t2_min, t2_max, t3_min, t3_max;
    double temp1, temp2;
    triple dir = arg2 - arg1;

    if (dir.x1 != 0)
      {
	temp1 = (clip1_max - arg1.x1)/dir.x1;
	temp2 = (clip1_min - arg1.x1)/dir.x1;

	t1_min = min(temp1, temp2);
	t1_max = max(temp1, temp2);
      }
    else
      {
	// is Line between bounding planes?
	if (clip1_min <= arg1.x1 && arg1.x1 <= clip1_max)
	  {
	    t1_min = -EPIX_INFTY;
	    t1_max =  EPIX_INFTY;
	  }
	else // Line is empty
	  return;
      }

    if (dir.x2 != 0)
      {
	temp1 = (clip2_max - arg1.x2)/dir.x2;
	temp2 = (clip2_min - arg1.x2)/dir.x2;

	t2_min = min(temp1, temp2);
	t2_max = max(temp1, temp2);
      }
    else
      {
	if (clip2_min <= arg1.x2 && arg1.x2 <= clip2_max)
	  {
	    t2_min = -EPIX_INFTY;
	    t2_max =  EPIX_INFTY;
	  }
	else // Line is empty
	  return;
      }

    if (dir.x3 != 0)
      {
	temp1 = (clip3_max - arg1.x3)/dir.x3;
	temp2 = (clip3_min - arg1.x3)/dir.x3;

	t3_min = min(temp1, temp2);
	t3_max = max(temp1, temp2);
      }
    else
      {
	if (clip3_min <= arg1.x3 && arg1.x3 <= clip3_max)
	  {
	    t3_min = -EPIX_INFTY;
	    t3_max =  EPIX_INFTY;
	  }
	else // Line is empty
	  return;
      }

    t_min = max(t1_min, t2_min);
    t_min = max(t_min, t3_min);
    
    t_max = min(t1_max, t2_max);
    t_max = min(t_max, t3_max);
    
    if (t_min < t_max) // Line is non-empty
      {
	triple tail = arg1 + t_min*dir;
	triple head = arg1 + t_max*dir;

	if (EPIX_PATH_STYLE != SOLID)
	  {
	    double true_dist = p2t(raw_len(c2s(view(head)-view(tail))));
	    n = (int) ceil(true_dist/(12*epix_stretch_factor));
	  }
	epix_comment("Line");
	draw_line(tail, head, n, 0);
	end_stanza();
      }

    //    else // do nothing
    //      ;
  }

} /* end of namespace */
