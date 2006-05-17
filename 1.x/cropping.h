/* 
 * cropping.h -- ePiX classes and functions for cropping paths and objects
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: May 14, 2006
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

#ifndef EPIX_CROPPING
#define EPIX_CROPPING

#include "triples.h"
#include "pairs.h"
#include "path.h"

namespace ePiX {

  // pre-declare friends for gcc-4.1
  // friends of crop_mask
  P seek_path_end(const P& in, const P& out);
  void bounding_box(const P&, const P&);
  void crop_box(const P&, const P&);
  void crop_box(void); // (re)set crop box to bounding box

  void picture(const double, const double);
  void picture(const P&);
  void offset(const P& arg);
  void offset(const double hoff, const double voff);

  double h_size(void);
  double v_size(void);
  double h_offset(void);
  double v_offset(void);

  // friends of enclosure
  void clip_box(const P& arg1, const P& arg2);
  P seek_path_end(const P& in, const P& out);
  bool is_in_bounds(const P& arg);
  void clip_to(const P& arg);
  void clip_box(const P& arg); 

  double clip1_min(void);
  double clip2_min(void);
  double clip3_min(void);

  double clip1_max(void);
  double clip2_max(void);
  double clip3_max(void);
  // end of friends


  class crop_mask {
  public:
    static crop_mask Bounding_Box;
    static crop_mask Crop_Box;
    static crop_mask Picture;
    static pair Offset;

    crop_mask(const pair arg1=pair(1,1), const pair arg2=pair(0,0));

    double x_sz(void) const { return x1_sz; }
    double y_sz(void) const { return x2_sz; }

    pair bl(void)   const { return pair(x1_min, x2_min); }
    pair tr(void)   const { return pair(x1_max, x2_max); }
    //    pair size(void) const { return pair(x1_sz,  x2_sz); }

    // translate by pair
    crop_mask& operator+= (const pair arg);

    bool is_onscreen(const P& arg) const;

    // Given two points (one projecting to the crop_mask, one not),
    // return the point on the segment that projects to the border.
    P seek_crop(const P&, const P&) const;
    friend P seek_path_end(const P& in, const P& out);

    // functions in global scope for compatibility
    friend void bounding_box(const P&, const P&);

    friend void crop_box(const P&, const P&);
    friend void crop_box(void); // (re)set crop box to bounding box

    friend void picture(const double, const double);
    friend void picture(const P&);

    friend void offset(const P& arg) { crop_mask::Offset = pair(arg); }
    friend void offset(const double hoff, const double voff) 
      { crop_mask::Offset = pair(hoff, voff); }

    friend double h_size(void) { return crop_mask::Picture.x1_sz; }
    friend double v_size(void) { return crop_mask::Picture.x2_sz; }

    friend double h_offset(void) { return crop_mask::Offset.x1(); }
    friend double v_offset(void) { return crop_mask::Offset.x2(); }

  private:
    double x1_min, x1_max, x1_sz;
    double x2_min, x2_max, x2_sz;

  }; // end of class crop_mask


  class enclosure {
    friend class path;
  private:
    double x1_min, x2_min, x3_min;
    double x1_max, x2_max, x3_max;

    static enclosure Clip_Box;

  public:
    enclosure(const P& arg1, const P& arg2);
    //    bool is_cropped(const P arg);

    // functions in global scope for compatibility
    friend void clip_box(const P& arg1, const P& arg2);

    bool is_clipped1(const P& arg) const
      {
	double x1 = arg.x1();
	return !( (x1_min <= x1) && (x1 <= x1_max) );
      }

    bool is_clipped2(const P& arg) const
      {
	double x2 = arg.x2();
	return !( (x2_min <= x2) && (x2 <= x2_max) );
      }

    bool is_clipped3(const P& arg) const 
      {
	double x3 = arg.x3();
	return !( (x3_min <= x3) && (x3 <= x3_max) );
      }

    bool is_inside(const P& arg) const
      {
	return !(is_clipped1(arg) || is_clipped2(arg) || is_clipped3(arg));
      }

    // Given two points, one in the enclosure, the other not, return the
    // point on the boundary.
    P seek_clip(const P& in, const P& out) const;
    friend P seek_path_end(const P& in, const P& out);

    friend bool is_in_bounds(const P& arg)
      {
	return enclosure::Clip_Box.is_inside(arg);
      }

    friend void clip_to(const P& arg) 
      { 
	enclosure::Clip_Box = enclosure(P(0,0,0), arg); 
      }
    friend void clip_box(const P& arg) 
      { 
	enclosure::Clip_Box = enclosure(arg, -arg); 
      }

    friend double clip1_min(void) { return enclosure::Clip_Box.x1_min; }
    friend double clip2_min(void) { return enclosure::Clip_Box.x2_min; }
    friend double clip3_min(void) { return enclosure::Clip_Box.x3_min; }

    friend double clip1_max(void) { return enclosure::Clip_Box.x1_max; }
    friend double clip2_max(void) { return enclosure::Clip_Box.x2_max; }
    friend double clip3_max(void) { return enclosure::Clip_Box.x3_max; }

  }; // end of class enclosure

  inline bool is_visible(const P& arg)
    {
      bool visible = true;
      if (epix::clipping && !is_in_bounds(arg))
	visible = false;

      if (epix::cropping && !crop_mask::Crop_Box.is_onscreen(arg))
	visible = false;

      return visible;
    }

} /* end of namespace */

#endif /* EPIX_CROPPING */
