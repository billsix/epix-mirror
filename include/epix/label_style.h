/*
 * label_style.h -- ePiX's current state for text and markers
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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

/*
 * This file implements label_state:
 *
 *  [] Text and mask color (Color x2)
 *  [] Alignment (epix_label_posn)
 *  [] Font face and size (strings)
 *  [] Label angle (double, in degrees)
 *
 * The class in this file implements backend for user-level global
 * functions defined in state.h, which modify the_label_style().
 *
 * This header is not part of epix.h.
 */
#ifndef EPIX_LABEL_STATE
#define EPIX_LABEL_STATE

#include <string>

#include "enums.h"

#include "Color.h"
#include "length.h"

#include "pen_data.h"

namespace ePiX {

class Camera;

class label_state {
 public:
  // Clients can only construct the default state: black, unmasked,
  // unbordered, basepoint-aligned, unrotated normalsize Roman text.
  label_state();

  // set
  void text_color(const Color& col);
  void mask_color(const Color& col);

  void label_padding(const length& len);
  void label_border(const pen_data& p);

  void align_to(const epix_label_posn a);

  void fontsize(const std::string& fs);
  void fontface(const std::string& f);

  // store label rotation angle internally in degrees
  void label_angle(double th);

  // get
  Color text_color() const;
  Color mask_color() const;

  bool is_masked() const;

  length label_padding() const;
  pen_data label_border() const;

  epix_label_posn align_to() const;

  // e.g. "scriptsize"
  std::string fontsize() const;
  std::string fontface() const;

  // return angle in degrees
  double label_angle() const;

  // map us through a camera filter
  label_state seen_through(const Camera& cam) const;

 private:
  Color the_text_color;
  Color the_mask_color;

  length the_padding;
  pen_data the_label_border;

  epix_label_posn the_alignment;

  std::string the_fontsize;
  std::string the_fontface;

  double the_angle;  // stored in degrees

  // text and background color, padding, border style,
  // alignment, font size and face, angle (degrees)
  label_state(const Color& text, const Color& mask, const length& pad,
              const pen_data& border, const epix_label_posn align,
              const std::string& fontsize, const std::string& fontface,
              double th);
};  // end of label_state

label_state& the_label_style();

}  // namespace ePiX

#endif /* EPIX_LABEL_STATE */
