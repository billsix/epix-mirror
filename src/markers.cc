/*
 * markers.cc -- ePiX axes, grids, markers, and labels
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.19
 * Last Change: September 17, 2007
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

#include "constants.h"

#include "triples.h"
#include "length.h"

#include "functions.h"

#include "path.h"
#include "curves.h"
#include "screen.h"
#include "picture.h"

#include "label_data.h"
#include "markers.h"

namespace ePiX {
using enum epix_label_posn;
using enum epix_mark_type;

static void __epix_label(const P& base, const P& offset, const std::string& lbl,
                         epix_label_posn POSN, bool masked) {
  label_data val(base, offset, lbl);

  if (masked) {
    // get mask color
    Color my_mask(the_label_style().mask_color());

    if (my_mask.is_unset()) my_mask = White();

    val.mask_color(my_mask);
  }

  else  // !masked
    val.mask_color(Neutral());

  val.align_to(POSN).draw();
}

// label -- put POSN-aligned string <label_text>
// at Cartesian position <base> translated by <offset> true points.
void label(const P& base, const P& offset, const std::string& lbl,
           epix_label_posn POSN) {
  __epix_label(base, offset, lbl, POSN, the_label_style().is_masked());
}

// mask color comes from global style, falls back to White()
void masklabel(const P& base, const P& offset, const std::string& lbl,
               epix_label_posn POSN) {
  __epix_label(base, offset, lbl, POSN, true);
}

void label(const P& base, const P& offset, const std::string& lbl) {
  __epix_label(base, offset, lbl, none, the_label_style().is_masked());
}

void masklabel(const P& base, const P& offset, const std::string& lbl) {
  __epix_label(base, offset, lbl, none, true);
}

// centered labels
void label(const P& base, const std::string& lbl) {
  __epix_label(base, P(0, 0), lbl, c, the_label_style().is_masked());
}

void masklabel(const P& base, const std::string& lbl) {
  __epix_label(base, P(0, 0), lbl, c, true);
}

// Marker aliases
void marker(const P& arg, epix_mark_type TYPE) { label_data(arg, TYPE).draw(); }

// Markers with labels
void circ(const P& posn, const P& offset, const std::string& label_text,
          epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, CIRC);
  mk.draw();
}

void ring(const P& posn, const P& offset, const std::string& label_text,
          epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, RING);
  mk.draw();
}

void spot(const P& posn, const P& offset, const std::string& label_text,
          epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, SPOT);
  mk.draw();
}

void dot(const P& posn, const P& offset, const std::string& label_text,
         epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, DOT);
  mk.draw();
}

void ddot(const P& posn, const P& offset, const std::string& label_text,
          epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, DDOT);
  mk.draw();
}

void box(const P& posn, const P& offset, const std::string& label_text,
         epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, BOX);
  mk.draw();
}

void bbox(const P& posn, const P& offset, const std::string& label_text,
          epix_label_posn POSN) {
  if (label_text != "") label(posn, offset, label_text, POSN);
  label_data mk(posn, BBOX);
  mk.draw();
}

// Axis ticks (h_tick = tall, thin rectangle, for a *horizontal* axis)
void h_axis_tick(const P& arg, epix_label_posn POSN) {
  label_data mk(arg, HTICK, POSN);
  mk.draw();
}

void v_axis_tick(const P& arg, epix_label_posn POSN) {
  label_data mk(arg, VTICK, POSN);
  mk.draw();
}

//// Curve-like elements
// arrow with label
void arrow(const P& tail, const P& head, const P& offset,
           const std::string& label_text, epix_label_posn POSN, double scale) {
  arrow(tail, head, scale);
  label(tail, offset, label_text, POSN);
}

// fixed-size elements
void right_angle(const P& loc, P leg1, P leg2, double scale) {
  const double norm1(norm(leg1));
  const double norm2(norm(leg2));

  if (norm1 < EPIX_EPSILON || norm2 < EPIX_EPSILON) return;

  // else
  const double mult(pt_to_screen(scale));
  leg1 *= mult / norm1;
  leg2 *= mult / norm2;

  path bd;
  bd.pt(loc + leg1).pt(loc + leg1 + leg2).pt(loc + leg2);
  bd.draw();
}

// the *acute* angle
void arc_measure(const P& loc, P leg1, P leg2, const P& offset,
                 const std::string& text, epix_label_posn align, double scale) {
  const double norm1(norm(leg1));
  const double norm2(norm(leg2));

  if (norm1 < EPIX_EPSILON || norm2 < EPIX_EPSILON) return;

  // else normalize
  leg1 *= 1.0 / norm1;
  leg2 *= 1.0 / norm2;

  // check for parllelity
  P perp(leg1 * leg2);
  const double norm3(norm(perp));

  if (norm3 < EPIX_EPSILON) return;

  // else get small angle
  const double th(Acos(leg1 | leg2));

  perp *= 1.0 / norm3;
  leg1 *= pt_to_screen(scale);

  perp *= leg1;  // perp to leg1 in plane of legs

  ellipse(loc, leg1, perp, 0, th);

  if (text != "")
    label(loc + Cos(0.5 * th) * leg1 + Sin(0.5 * th) * perp, offset, text,
          align);
}

void arc_measure(const P& loc, P leg1, P leg2, double scale) {
  arc_measure(loc, leg1, leg2, P(0, 0), "", none, scale);
}

void axis_break(const P& tail, const P& head, double scale) {
  const P midpt(0.5 * (head + tail));

  P dir((head - tail) % E_3);  // project to (x1,x2)-plane

  if (norm(dir) < EPIX_EPSILON) return;

  // else
  dir *= pt_to_screen(0.25 * scale) / norm(dir);  // zag is scale pt long

  path bd;
  bd.pt(tail)
      .pt(midpt - 2 * dir)
      .pt(midpt - dir + 1.5 * J(dir))
      .pt(midpt + dir - 1.5 * J(dir))
      .pt(midpt + 2 * dir)
      .pt(head);

  bd.draw();
}

void h_error_bar(const P& loc, double err, epix_mark_type mk, double ht) {
  const P dy(0, pt_to_screen(0.5 * ht));
  const P bot(loc - P(err, 0));
  const P top(loc + P(err, 0));

  line(bot, top);
  line(bot - dy, bot + dy);
  line(top - dy, top + dy);
  marker(loc, mk);
}

void v_error_bar(const P& loc, double err, epix_mark_type mk, double wd) {
  const P dx(pt_to_screen(0.5 * wd), 0);
  const P bot(loc - P(0, err));
  const P top(loc + P(0, err));

  line(bot, top);
  line(bot - dx, bot + dx);
  line(top - dx, top + dx);
  marker(loc, mk);
}
}  // namespace ePiX
