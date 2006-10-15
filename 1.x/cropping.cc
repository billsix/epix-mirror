/* 
 * cropping.h -- ePiX classes and functions for cropping paths and objects
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 10, 2006
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

#include <cmath>
#include "functions.h"
#include "errors.h"

#include "camera.h"
#include "cropping.h"

namespace ePiX {

  extern epix_camera camera;

  // define static members
  crop_mask crop_mask::Bounding_Box = crop_mask(); // defaults to unit square
  crop_mask crop_mask::Crop_Box     = crop_mask();
  crop_mask crop_mask::Picture      = crop_mask();
  pair crop_mask::Offset = pair(0,0);

  enclosure enclosure::Clip_Box =
    enclosure(P(-EPIX_INFTY,-EPIX_INFTY,-EPIX_INFTY), 
	      P( EPIX_INFTY, EPIX_INFTY, EPIX_INFTY));


  // input collinear Ps, check if b is between a and c
  bool between(const P& a, const P& b, const P& c)
  {
    if (((a-b) | (c-b)) < 0)
      return true;
    else 
      return false;
  }

  crop_mask::crop_mask(const pair arg1, const pair arg2)
    : x1_min(min(arg1.x1(), arg2.x1())), x1_max(max(arg1.x1(), arg2.x1())),
      x1_sz(x1_max - x1_min),
      x2_min(min(arg1.x2(), arg2.x2())), x2_max(max(arg1.x2(), arg2.x2())),
      x2_sz(x2_max - x2_min) { }

  double crop_mask::x_sz() const
  {
    return x1_sz;
  }
  double crop_mask::y_sz() const
  {
    return x2_sz;
  }

  pair crop_mask::bl() const
  {
    return pair(x1_min, x2_min);
  }
  pair crop_mask::tr() const
  {
    return pair(x1_max, x2_max);
  }

  // translate by pair
  crop_mask& crop_mask::operator+= (const pair arg)
  {
    x1_min += arg.x1();
    x1_max += arg.x1();

    x2_min += arg.x2();
    x2_max += arg.x2();

    return *this;
  }

  // not defined in header b/c of camera dependence
  bool crop_mask::is_onscreen(const P& arg) const
  {
    pair temp(camera(arg));
    double x(temp.x1()), y(temp.x2());
    return ( (x1_min <= x) && (x <= x1_max) &&
	     (x2_min <= y) && (y <= x2_max) );
  }

  // Find t in [0,1] so that t*in + (1-t)*out is on the Crop_Box boundary
  // by checking four cases. Assume "in" is in the Crop_Box and out isn't.
  P crop_mask::seek_crop(const P& in, const P& out) const
  {
    // find t such that (1-t)*in + t*out projects to boundary
    double t_min(0);
    double t_max(1);
    double mid(0.5);

    while (norm(camera((1-t_max)*in+t_max*out)  -
		camera((1-t_min)*in+t_min*out)) > EPIX_EPSILON)
      {
	// bisection method
	mid = 0.5*(t_max + t_min);
	if (is_onscreen((1-mid)*in + mid*out))
	  t_min = mid;

	else
	  t_max = mid;

      }
    return (1-mid)*in + mid*out;
  } // end of seek_crop


  //// Friends of crop_mask ////

  // Find endpoint of path segment by testing both clipping and cropping;
  // algorithm works because only two convex criteria determine whether
  // a point is invisible. Assume out is either clipped or cropped
  P seek_path_end(const P& in, const P& out)
  {
    crop_mask& cropper = crop_mask::Crop_Box;
    enclosure& clipper = enclosure::Clip_Box;

    // out not clipped, must be cropped
    if ((!epix::clipping) || clipper.is_inside(out))
      return cropper.seek_crop(in, out);

    // out not cropped, must be clipped
    else if ((!epix::cropping) || cropper.is_onscreen(out))
      return clipper.seek_clip(in, out);

    else // out is clipped and cropped
      {
	P clip_endpoint(clipper.seek_clip(in, out));
	P crop_endpoint(cropper.seek_crop(in, out));

	// out is both clipped and cropped
	if (between(in, crop_endpoint, clip_endpoint))
	  return crop_endpoint;

	else
	  return clip_endpoint;
      }
  } // end of seek_path_end


  // functions in global scope for compatibility
  void bounding_box(const P& arg1, const P& arg2)
  {
    crop_mask::Bounding_Box = crop_mask(pair(arg1), pair(arg2));
    if (crop_mask::Bounding_Box.x1_sz == 0)
      epix_error("Bounding box has width zero!");

    if (crop_mask::Bounding_Box.x2_sz == 0)
      epix_error("Bounding box has height zero!");
    
    // "proper" initialization of Crop_Box
    crop_mask::Crop_Box = crop_mask(pair(arg1), pair(arg2));
  }

  void crop_box(const P& arg1, const P& arg2)
  {
    crop_mask::Crop_Box = crop_mask(pair(arg1), pair(arg2));
  }

  void crop_box(void)
  {
    crop_mask::Crop_Box = crop_mask::Bounding_Box;
  }

  void picture(const double horiz, const double vert)
  {
    if (horiz == 0)
      epix_error("Picture has width zero!");

    if (vert == 0)
      epix_error("Picture has height zero!");

    crop_mask::Picture = crop_mask(pair(0,0), pair(horiz, vert));
  }

  void picture(const P& arg)
  {
    crop_mask::Picture = crop_mask(pair(0,0), pair(arg));
    if (crop_mask::Picture.x1_sz == 0)
      epix_error("Picture has width zero!");

    if (crop_mask::Picture.x2_sz == 0)
      epix_error("Picture has height zero!");
  }


  void offset(const P& arg)
  {
    crop_mask::Offset = pair(arg);
  }
  void offset(const double hoff, const double voff) 
  {
    crop_mask::Offset = pair(hoff, voff);
  }

  double h_size()
  {
    return crop_mask::Picture.x1_sz;
  }
  double v_size()
  {
    return crop_mask::Picture.x2_sz;
  }

  double h_offset()
  {
    return crop_mask::Offset.x1();
  }
  double v_offset()
  {
    return crop_mask::Offset.x2();
  }


  //// enclosure functions ////

  enclosure::enclosure(const P& arg1, const P& arg2)
    : x1_min(min(arg1.x1(), arg2.x1())), x1_max(max(arg1.x1(), arg2.x1())),
      x2_min(min(arg1.x2(), arg2.x2())), x2_max(max(arg1.x2(), arg2.x2())),
      x3_min(min(arg1.x3(), arg2.x3())), x3_max(max(arg1.x3(), arg2.x3())) { }


  bool enclosure::is_clipped1(const P& arg) const
  {
    double x1(arg.x1());
    return !( (x1_min <= x1) && (x1 <= x1_max) );
  }

  bool enclosure::is_clipped2(const P& arg) const
  {
    double x2(arg.x2());
    return !( (x2_min <= x2) && (x2 <= x2_max) );
  }

  bool enclosure::is_clipped3(const P& arg) const 
  {
    double x3(arg.x3());
    return !( (x3_min <= x3) && (x3 <= x3_max) );
  }

  bool enclosure::is_inside(const P& arg) const
  {
    return !(is_clipped1(arg) || is_clipped2(arg) || is_clipped3(arg));
  }

  // Find t in [0,1] so that t*in + (1-t)*out is on the Clip_Box boundary
  // by checking six cases. Assume "in" is in the Clip_Box and out isn't.
  P enclosure::seek_clip(const P& in, const P& out) const
  {
    double t=1;

    if (out.x1() != in.x1())
      {
	if (out.x1() < clip1_min())
	  t = min(t, (clip1_min() - in.x1())/(out.x1() - in.x1()));
	
	else if (clip1_max() < out.x1())
	  t = min(t, (clip1_max() - in.x1())/(out.x1() - in.x1()) );
      }

    if (out.x2() != in.x2())
      {
	if (out.x2() < clip2_min())
	  t = min(t, (clip2_min() - in.x2())/(out.x2() - in.x2()));

	else if (clip2_max() < out.x2())
	  t = min(t, (clip2_max() - in.x2())/(out.x2() - in.x2()) );
      }

    if (out.x3() != in.x3())
      {
	if (out.x3() < clip3_min())
	  t = min(t, (clip3_min() - in.x3())/(out.x3() - in.x3()));

	else if (clip3_max() < out.x3())
	  t = min(t, (clip3_max() - in.x3())/(out.x3() - in.x3()) );
      }

    return ((1-t)*in) + (t*out);
  } // end of seek_clip


  //// Friends of enclosure ////
  void clip_box(const P& arg1, const P& arg2)
  {
    enclosure::Clip_Box = enclosure(arg1, arg2);
  }


  bool is_in_bounds(const P& arg)
  {
    return enclosure::Clip_Box.is_inside(arg);
  }

  void clip_to(const P& arg) 
  { 
    enclosure::Clip_Box = enclosure(P(0,0,0), arg); 
  }
  void clip_box(const P& arg) 
  { 
    enclosure::Clip_Box = enclosure(arg, -arg); 
  }

  double clip1_min()
  {
    return enclosure::Clip_Box.x1_min;
  }
  double clip2_min()
  {
    return enclosure::Clip_Box.x2_min;
  }
  double clip3_min()
  {
    return enclosure::Clip_Box.x3_min;
  }

  double clip1_max()
  {
    return enclosure::Clip_Box.x1_max;
  }
  double clip2_max()
  {
    return enclosure::Clip_Box.x2_max;
  }
  double clip3_max()
  {
    return enclosure::Clip_Box.x3_max;
  }


  bool is_visible(const P& arg)
  {
    bool visible = true;
    if (epix::clipping && !is_in_bounds(arg))
      visible = false;

    if (epix::cropping && !crop_mask::Crop_Box.is_onscreen(arg))
      visible = false;

    return visible;
  }
} // end of namespace
