/* 
 * label_style.cc -- ePiX's current state for text and markers
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <string>

#include "enums.h"
#include "constants.h"
#include "functions.h" // for snip_to

#include "angle_units.h"

#include "Color.h"
#include "camera.h"
#include "length.h"

#include "label_style.h"

namespace ePiX {

  // Users can only construct the default style, since label angles
  // are specified in current units but stored/returned in degrees.
  label_state::label_state()
    : the_text_color(Black()), the_mask_color(Neutral()),
      the_padding(length(0)), the_label_border(Xfine()),
      the_alignment(none), the_fontsize("nsz"),
      the_fontface("rm"),
      the_angle(0) { }

  // set
  void label_state::text_color(const Color& col)
  {
    the_text_color = col;
  }

  void label_state::mask_color(const Color& col)
  {
    the_mask_color = col;
  }

  void label_state::label_padding(const length& len)
  {
    the_padding=len;
  }

  void label_state::label_border(const pen_data& p)
  {
    the_label_border = p;
  }

  void label_state::align_to(const epix_label_posn a)
  {
    the_alignment = a;
  }

  void label_state::fontsize(const std::string& fs)
  {
    the_fontsize = fs;
  }

  void label_state::fontface(const std::string& ff)
  {
    the_fontface = ff;
  }

  // store label rotation angle internally in degrees
  void label_state::label_angle(double th)
  {
    the_angle = the_angle_style().to_degrees(th);
  }

  // get
  Color label_state::text_color() const
  {
    return the_text_color;
  }

  Color label_state::mask_color() const
  {
    return the_mask_color;
  }

  bool label_state::is_masked() const
  {
    return !the_mask_color.is_unset();
  }

  length label_state::label_padding() const
  {
    return the_padding;
  }

  pen_data label_state::label_border() const
  {
    return the_label_border;
  }

  epix_label_posn label_state::align_to() const
  {
    return the_alignment;
  }

  // e.g. "scriptsize"
  std::string label_state::fontsize() const
  {
    return the_fontsize;
  }

  std::string label_state::fontface() const
  {
    return the_fontface;
  }

  double label_state::label_angle() const
  {
    return the_angle;
  }

  // map us through a camera filter
  label_state label_state::seen_through(const Camera& mycam) const
  {
    return label_state(mycam(the_text_color), mycam(the_mask_color),
		       the_padding, the_label_border.seen_through(mycam),
		       the_alignment, the_fontsize, the_fontface,
		       the_angle);
  }

  // private constructor, for seen_through
  label_state::label_state(const Color& text, const Color& mask,
                           const length& pad, const pen_data& border,
                           const epix_label_posn align,
                           const std::string& fontsize,
                           const std::string& fontface,
                           double th)
    : the_text_color(text), the_mask_color(mask),
      the_padding(pad), the_label_border(border),
      the_alignment(align), the_fontsize(fontsize),
      the_fontface(fontface),
      the_angle(th) { }


  label_state& the_label_style()
  {
    static label_state* the_label_state(new label_state());
    return *the_label_state;
  }
} // end of namespace
