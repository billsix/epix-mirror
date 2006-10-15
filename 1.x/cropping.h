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

#ifndef EPIX_CROPPING
#define EPIX_CROPPING

#include "pairs.h"

namespace ePiX {

  class P;
  class path;

  // pre-declare friends for gcc-4.1
  // friends of crop_mask
  P seek_path_end(const P& in, const P& out);
  void bounding_box(const P&, const P&);
  void crop_box(const P&, const P&);
  void crop_box(); // (re)set crop box to bounding box

  void picture(const double, const double);
  void picture(const P&);
  void offset(const P& arg);
  void offset(const double hoff, const double voff);

  double h_size();
  double v_size();
  double h_offset();
  double v_offset();

  // friends of enclosure
  void clip_box(const P& arg1, const P& arg2);
  P seek_path_end(const P& in, const P& out);
  bool is_in_bounds(const P& arg);
  void clip_to(const P& arg);
  void clip_box(const P& arg); 

  double clip1_min();
  double clip2_min();
  double clip3_min();

  double clip1_max();
  double clip2_max();
  double clip3_max();
  // end of friends


  class crop_mask {
  public:
    static crop_mask Bounding_Box;
    static crop_mask Crop_Box;
    static crop_mask Picture;
    static pair Offset;

    crop_mask(const pair arg1=pair(1,1), const pair arg2=pair(0,0));

    double x_sz() const;
    double y_sz() const;

    pair bl() const;
    pair tr() const;
    //    pair size() const { return pair(x1_sz,  x2_sz); }

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
    friend void crop_box(); // (re)set crop box to bounding box

    friend void picture(const double, const double);
    friend void picture(const P&);

    friend void offset(const P& arg);
    friend void offset(const double hoff, const double voff);

    friend double h_size();
    friend double v_size();

    friend double h_offset();
    friend double v_offset();

  private:
    double x1_min, x1_max, x1_sz;
    double x2_min, x2_max, x2_sz;

  }; // end of class crop_mask


  class enclosure {
    friend class path;
  public:
    enclosure(const P& arg1, const P& arg2);
    //    bool is_cropped(const P arg);

    bool is_clipped1(const P&) const;
    bool is_clipped2(const P&) const;
    bool is_clipped3(const P&) const;
    bool is_inside(const P&) const;

    // Given two points, one in the enclosure, the other not, return the
    // point on the boundary.
    P seek_clip(const P& in, const P& out) const;

    // in global scope for compatibility
    friend void clip_box(const P& arg1, const P& arg2);

    friend P seek_path_end(const P& in, const P& out);

    friend bool is_in_bounds(const P&);
    friend void clip_to(const P&);
    friend void clip_box(const P&);

    friend double clip1_min();
    friend double clip2_min();
    friend double clip3_min();

    friend double clip1_max();
    friend double clip2_max();
    friend double clip3_max();

  private:
    double x1_min, x2_min, x3_min;
    double x1_max, x2_max, x3_max;

    static enclosure Clip_Box;

  }; // end of class enclosure

  bool is_visible(const P&);

} // end of namespace

#endif /* EPIX_CROPPING */
