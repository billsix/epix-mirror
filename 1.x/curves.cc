/* 
 * curves.cc -- Ellipses, circular arcs, splines
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.3
 * Last Change: January 05, 2005
 *
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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
#include "triples.h"
#include "domain.h"
#include "cropping.h"

#include "functions.h"
#include "pairs.h"

#include "camera.h"
#include "lengths.h"
#include "output.h"

#include "path.h"
#include "objects.h"
#include "curves.h"

namespace ePiX {

  extern epix_camera camera;

  // half-turn in current units
  static double epix_pi = angle(M_PI);

  // Simple geometric objects

  // Lines take a stretch factor, roughly in percent
  void line(const P tail, const P head, const double expand)
  {
    path data = path(tail, head, expand);
    data.draw();
  }

  void line(const P tail, const P head, const double expand,
	    int num_pts)
  {
    path data = path(tail, head, expand, num_pts);
    data.draw();
  }

  // Line(p1, p2) -- draw uncropped portion of long line through p1, p2
  void Line(P arg1, P arg2) //, const int num_pts)
  {
    pair screen_dir = camera(arg2)-camera(arg1);
    if (norm(screen_dir) < EPIX_EPSILON)
      ; // draw nothing

    else
      {
	P dir = arg2 - arg1;
	dir *= 1/norm(dir);

	const double BIG=EPIX_INFTY;
	const double SMALL = 0.05;   // hardwired constant
	// move endpoints far away
	P tail = arg1 - BIG*dir;
	P head = arg2 + BIG*dir;

	// zi > 0 iff point is in front of camera
	double z1 = -(tail - camera.get_viewpt())|camera.eye();
	double z2 = -(head - camera.get_viewpt())|camera.eye();

	if (z1 < SMALL) // if behind camera, move to front
	  {
	    tail += ((z1 - SMALL)/(dir|camera.eye())) * dir;
	    z1 = SMALL;
	  }

	if (z2 < SMALL)
	  {
	    head += ((z2 - SMALL)/(dir|camera.eye())) * dir;
	    z2 = SMALL;
	  }

	if (crop_mask::Crop_Box.is_onscreen(arg1))
	  {
	    if (!crop_mask::Crop_Box.is_onscreen(tail))
	      tail = seek_path_end(arg1, tail);

	    if (!crop_mask::Crop_Box.is_onscreen(head))
	      head = seek_path_end(arg1, head);
	  }

	else if (crop_mask::Crop_Box.is_onscreen(arg2)) // arg1 cropped
	  {
	    if (!crop_mask::Crop_Box.is_onscreen(tail))
	      tail = seek_path_end(arg2, tail);

	    if (!crop_mask::Crop_Box.is_onscreen(head))
	      head = seek_path_end(arg2, head);
	  }

	else // arg1, arg2 both cropped, do nothing for now
	  ;

	// screen distance from (expanded) tail to head, in true pt
	double dist = norm(p2t(c2s(camera(head)-camera(tail))));

	// one vertex every dashlength true pt
	int num_pts = (int) ceil(dist/epix::get_dashlength());
	//	std::cerr << "Line with " << num_pts << " points\n";

	// get crop state
	bool state = epix::cropping;
	crop(true);

	line(tail, head, 0, num_pts);
	crop(state); // restore crop state
      } // Line not parallel to eye

  } // end of Line


  // point-slope form
  void
  Line(P tail, double slope)
  {
    Line(tail, tail+P(1, slope, 0));
  }



  void triangle(const P arg1, const P arg2, const P arg3)
  {
    if (epix::path_style() == SOLID)
      {
	path data = polygon(3, &arg1, &arg2, &arg3);
	data.set_fill(epix::fill_paths);
	data.draw();
      }
    else // dashed/dotted -- not filled
      {
	line(arg1, arg2);
	line(arg2, arg3);
	line(arg3, arg1);
      }
  }

  void quad(const P arg1, const P arg2, 
	    const P arg3, const P arg4)
  {
    if (epix::path_style() == SOLID)
      {
        path data = polygon(4, &arg1, &arg2, &arg3, &arg4);
        data.set_fill(epix::fill_paths);
        data.draw();
      }
    else // dashed/dotted
      {
	line(arg1, arg2);
	line(arg2, arg3);
	line(arg3, arg4);
	line(arg4, arg1);
      }
  }



  // Draw coordinate rectangle with opposite corners as given. Arguments
  // must lie is a plane parallel to a coordinate plane, but not on a 
  // line parallel to a coordinate axis.

  void rect(const P arg1, const P arg2, bool solid)
  {
    P diagonal = arg2 - arg1;
    P jump;
    int perp_count = 0;

    // count coordinate axes perp to diagonal and flag normal
    if (fabs(diagonal|E_1) < EPIX_EPSILON)
      {
	++perp_count;
	jump = E_2&(diagonal);
      }
    if (fabs(diagonal|E_2) < EPIX_EPSILON)
      {
	++perp_count;
	jump = E_3&(diagonal);
      }
    if (fabs(diagonal|E_3) < EPIX_EPSILON)
      {
	++perp_count;
	jump = E_1&(diagonal);
      }

    // no warning if rectangle is degenerate

    P temp1 = arg1+jump;
    P temp2 = arg2-jump;

    quad(arg1, temp1, arg2, temp2);
  } // end rect


  void swatch(const P arg1, const P arg2)
  {
    rect(arg1, arg2, true);
  }


  void ellipse(const P center, const P axis1, const P axis2,  
	       const double t_min, const double t_max, int num_pts)
  {
    path data=path(center, axis1, axis2, t_min, t_max, num_pts);
    data.draw();
  }  



  // Arrows joining two specified <P>s. An arrow consists of a head
  // and a shaft; the head is a quadrilateral 2*width true points wide,
  // width*ratio true points high (in perpendicular), and with an undercamber
  // as specified in globals.h. The shaft is long enough to join the <tail>
  // to <base> (see below). If the projection of the arrow is so short that
  // the <tail> would be inside the arrowhead (with head at <tip3>), the 
  // shaft is not drawn. 
  //
  // Vertices of arrow:
  //
  //                     <tip1>
  // <tail> ====shaft====  <base>    <tip3> (aka <head>)
  //                     <tip2>
  //
  // Some of the points defining an arrowhead must be computed from true 
  // coords, since the aspect ratio of a figure is not generally unity. 
  // The algorithm is:
  //
  // (1) Find Cartesian direction vector from <tail> to <head>, and
  //    convert it to picture coords. Find the cosine of the angle between
  //    camera.eye and the Cartesian direction vector; call this cos_theta.
  //
  // (2) If the true length of the printed arrow is less than the height of 
  //    a tilted arrowhead, put <stem> at <tail> and do not draw the shaft.
  //    Otherwise, put <tip3> at <head> and calculate <stem> so that
  //    <tip3 - stem> projects to a vector of true length width*ratio*sin_theta
  //    in the screen plane. Place <base> at <stem> plus camber.
  //
  // (3) Find a vector in page coordinates that is perpendicular to the
  //    <direction> and is 2 true pt long. Lift to object space, using the
  //    {sea, sky, eye} coordinates of the camera, and find <tip1>, <tip2>.
  //
  // (4) Now we're ready to draw. For simplicity, use PostScript layering.
  // (a) If the arrow points away from the camera, draw segments from <tip3>
  //    to <tip1> and <tip2>, then draw a filled white ellipse (the base of
  //    the arrowhead), then draw the shaft up to <base>.
  // (b) If the arrow points toward the camera, draw the shaft, then draw a
  //    black-filled circle through <tip1> and <tip2>, then white fill the
  //    region enclosed by the appropriate semi-circle and the polygon
  //    <tip1>, <tip3>, <tip2>. (These circles are perpendicular to the
  //    shaft in object space, and are usually rendered as ellipses.)
  //
  // As with other polygon objects, draw_arrowhead and draw_shaft print
  // only lists of coordinate pairs, not line styles, etc. draw_arrow is the
  // user analogue of draw_rect (e.g.); it draws the requisite paths and
  // nothing else. This division of labor  allows the pieces to be used in
  // other functions, e.g., plots of tangent vectors to a parametrized curve.

  void arrow(const P tail, const P head, double scale)
  {
    // may assume head-tail is not the zero vector in object/screen
    double ratio = epix::get_arrow_ratio();
    double width = epix::get_arrow_width()*fabs(scale);
    double dens  = epix::get_arrow_fill();

    P tip1, tip2, tip3, base, stem;
    P object_dir = head - tail;
    double cos_theta = (object_dir|camera.eye())/norm(object_dir);
    double sin_theta = sqrt(1 - cos_theta*cos_theta);
    double arrowhead_height = sin_theta*width*ratio; // in true pt
    double camber = arrowhead_height*epix::get_arrow_camber();

    pair screen_dir = camera(head) - camera(tail);
    pair picture_dir = c2s(screen_dir);
    double true_sep = true_length(picture_dir); // in true pt

    P object_unit = (1.0/true_sep)*object_dir; // projects to 1 true pt

    if (arrowhead_height  < true_sep) // <tail> outside arrowhead
      {
	tip3 = head;
	stem = tip3 - arrowhead_height*object_unit;
      }

    else // <tail> inside arrowhead
      {
	stem = tail;
	tip3 = stem + arrowhead_height*object_unit;
      }

    base = stem + camber*object_unit;

    // compute page normals in object coordinates
    // 1 true pt, in picture coords
    pair picture_perp = (1.0/true_sep)*J(picture_dir); 
    pair screen_perp  = s2c(picture_perp);

    // object, projects to 1 true pt normal
    P object_perp = ((screen_perp.x1())*camera.sea()) +
      ((screen_perp.x2())*camera.sky());

    tip1 = stem + width*object_perp;
    tip2 = stem - width*object_perp;

    line(tail, base);
    epix_path_style TEMP = epix::path_style(); // save path style
    solid();
    if (dens != 0)
      std::cout << "\n\\special{sh " << dens << "}%";

    polygon(4, &base, &tip1, &tip3, &tip2).draw();
    // restore path style
    if (TEMP == DASHED)
      dashed();
    else if (TEMP == DOTTED)
      dotted();
  }


  // Standard half-ellipse functions
  void ellipse_left (const P center, const P radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, 
	    -0.5*epix_pi, 0.5*epix_pi);
  }

  void ellipse_right (const P center, const P radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, 
	    0.5*epix_pi, 1.5*epix_pi);
  }

  void ellipse_top (const P center, const P radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, 0, epix_pi);
  }

  void ellipse_bottom (const P center, const P radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, -epix_pi, 0);
  }


  // circular arcs parallel to (x,y)-plane

  // Angle subtended by arrowhead at Cartesian distance r
  static double subtended(double r, double scale)
  {
    return scale*epix::get_arrow_width()*epix::get_arrow_ratio()/arc_scale(r);
  }

  void arc_arrow(const P center, const double r, 
		 const double start, const double finish, const double scale)
  {
    bool fill_state=epix::fill_paths;

    double temp = subtended(r, scale);
    if (fabs(start - finish) >= temp) // long enough arc
      {
	temp *= sgn(finish-start);
	fill(false);
	ellipse(center, r*E_1, r*E_2, start, finish-temp); // arc
	fill(fill_state);
	arrow(center+polar(r, finish - temp), 
	      center+polar(r, finish - temp + EPIX_EPSILON*sgn(finish-start)),
	      scale);
      }
  }


  // quadratic spline
  void spline(const P p1, const P p2, const P p3, int num_pts)
  {
    path data(p1, p2, p3, num_pts);
    data.draw();
  }

  // spline arrowhead
  static void sparrowhead(P tail, P head, double scale=1)
  {
    double ratio = epix::get_arrow_ratio();
    double width = epix::get_arrow_width()*fabs(scale);

    P object_dir = head - tail;

    double cos_theta = (object_dir|camera.eye())/norm(object_dir);
    double sin_theta = sqrt(1 - cos_theta*cos_theta);
    double ht = sin_theta*width*ratio; // in true pt

    pair screen_dir = camera(head) - camera(tail);
    pair picture_dir = c2s(screen_dir);
    double true_sep = true_length(picture_dir); // in true pt

    P object_unit = (1.0/true_sep)*object_dir; // projects to 1 true pt

    arrow(head-ht*object_unit, head, scale);
  }

  void arrow(const P p1, const P p2, const P p3, double scale)
  {
    path data(p1, p2, p3, EPIX_NUM_PTS);
    data.draw();
    // hack to align arrowhead when spline is highly curved
    double wt=0.95; // Hardwired constant 0.95
    sparrowhead(wt*p2+(1-wt)*p1, p3, scale);
  }

  // cubic spline
  void spline(const P p1, const P p2, 
	      const P p3, const P p4, int num_pts)
  {
    path data(p1, p2, p3, p4, num_pts);
    data.draw();
  }

  void arrow(const P p1, const P p2, const P p3, const P p4, double scale)
  {
    path data(p1, p2, p3, p4, EPIX_NUM_PTS);
    data.draw();
    double wt=0.95; // Hardwired constant 0.95
    sparrowhead(wt*p3+(1-wt)*p2, p4, scale);
  }


  // polygon specified by number of points and list of vertices
  // draw_polygon prints data points only (for pstricks...)
  // polygon is closed, polyline is not
  /*
  void draw_polygon(int num_pts, ...)
  {
    va_list ap;
    va_start(ap, num_pts);
  
    P* curr;
    P* p1 = va_arg(ap, P*);

    newl();
    print(*p1);
    
    for (int i=1; i <= num_pts-1; ++i) {
      curr = va_arg(ap, P*);
      print(*curr);

      line_break(i, num_pts);
    }

    va_end(ap);

    print(*p1); // close path
  }
  */


  // n1 x n2 Cartesian grid, where coarse = (n1, n2)
  void grid(const P arg1, const P arg2, mesh coarse, mesh fine)
  {
    P diagonal = arg2 - arg1;
    P jump1, jump2; // sides of grid

    int perp_count = 0;

    int N1 = coarse.n1();
    int N2 = coarse.n2();

    // count coordinate axes diagonal is perp to and flag normal
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

    if (perp_count != 1)
      epix_warning("Ignoring degenerate coordinate grid");

    else
      {
	// grid line spacing
	P grid_step1 = (1.0/N1)*jump1;
	P grid_step2 = (1.0/N2)*jump2;

	for (int i=0; i <= N1; ++i)
	  line(arg1+i*grid_step1, arg1+i*grid_step1+jump2, 0, fine.n2());

	for (int j=0; j <= N2; ++j)
	  line(arg1+j*grid_step2, arg1+j*grid_step2+jump1, 0, fine.n1());

	end_stanza();
      }
  }

  // Grids that fill bounding_box with default camera
  void grid(const P arg1, const P arg2, int n1, int n2)
  {
    mesh temp(n1,n2);
    if (!epix::cropping && ! epix::clipping)
      grid(arg1, arg2, temp, mesh(1,1));
    else
      grid(arg1, arg2, temp, temp);
  }


  // polar grid with specified radius, mesh (rings and sectors), and resolution
  void polar_grid(double radius, mesh coarse, mesh fine)
  {
    for (int i=1; i <= coarse.n1(); ++i) 
      ellipse(P(0,0,0), 
	      (i*radius/coarse.n1())*E_1, (i*radius/coarse.n1())*E_2, 
	      0, epix::full_turn(), fine.n2());

    for (int j=0; j < coarse.n2(); ++j)
      line(P(0,0,0), polar(radius, j*(epix::full_turn())/coarse.n2()), 
	   0, 2*fine.n1());

    end_stanza();
  }

  void polar_grid(double radius, int n1, int n2)
  {
    polar_grid(radius, mesh(n1,n2), mesh(n1,EPIX_NUM_PTS));
  }

  // fractal generation
  //
  // The basic recursion unit is a piecewise-linear path whose segments
  // are parallel to spokes on a wheel, labelled modulo <spokes>.
  // Recursively up to <depth>, each segment is replaced by a copy of the
  // recursion unit, scaled and rotated in order to join p to q.
  //
  // Kludge: pre_seed[0] = spokes, pre_seed[1] = length of seed;
  //
  // Sample data for _/\_ standard Koch snowflake:
  // const int seed[] = {6, 4, 0, 1, -1, 0};

  static P jump(int spokes, int length, const std::vector<int>& seed)
  {
    P sum = P(0,0);

    for (int i=0; i< length; ++i)
      sum += cis(seed.at(i)*(epix::full_turn())/spokes);

    return sum;
  }

  void fractal(triple p, triple q, int depth, const int *pre_seed)
  {
    int spokes = pre_seed[0];
    int seed_length = pre_seed[1];
    std::vector<int> seed(seed_length);

    // extract seed from pre_seed
     for (int i=0; i<seed_length; ++i)
      seed.at(i) = pre_seed[i+2];

    // Unit-length steps in <seed> sequence add up to <scale>
    P scale = jump(spokes, seed_length, seed);
    const double norm_scale = norm(scale);

    // Number of points in final fractal
    int length = 1+(int)pow(seed_length, depth); 
    std::vector<int> dir(length);     // stepping information
    std::vector<vertex> data(length); // vertices

    // dir[] starts out [0, 1, -1, 0, ..., 0] (seed_length^depth entries)
    // then take repeated "Kronecker sum" with seed = [0, 1, -1, 0]

    for(int i=0; i<seed_length; ++i)
      dir.at(i) = seed.at(i);

    for(int i=1; i<depth; ++i) // recursively fill dir array
      for(int j=0; j < pow(seed_length,i); ++j)
	for(int k=seed_length-1; 0 < k; --k)
	  dir.at(k*(int)pow(seed_length,i) + j) = dir.at(j) + seed.at(k);

    triple curr = p;
    double radius = pow(norm_scale, -depth);
    pair temp;     // pair increment between successive points

    for(int i=0; i<length; ++i)
      {
	data.at(i) = vertex(curr);
	// complex arithmetic...
	temp = pair(polar(radius, 
			  dir.at(i)*(epix::full_turn())/spokes))*pair(q-p);
	temp /= pair(scale); // homothety to join p to q

	curr += P(temp.x1(), temp.x2());
      }

    path fractal(data, false);
    fractal.draw();
    end_stanza();
  }

} /* end of namespace */
