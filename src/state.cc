/*
 * state.cc -- ePiX's current drawing state
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

#include "constants.h"
#include "errors.h"
#include "triples.h"
#include "Color.h"

#include "length.h"

#include "camera.h"

#include "angle_units.h"
#include "arrow_style.h"
#include "label_style.h"
#include "marker_style.h"
#include "paint_style.h"
#include "path_style.h"

#include "active_screen.h"
#include "screen.h"

#include "clipping.h"

#include "state.h"

namespace ePiX {

// set camera position
void viewpoint(const P& arg) { cam().at(arg); }

void viewpoint(double x1, double x2, double x3) { cam().at(P(x1, x2, x3)); }

// Set angular mode
void radians() { the_angle_style().set_radians_mode(); }

void degrees() { the_angle_style().set_degrees_mode(); }

void revolutions() { the_angle_style().set_revolutions_mode(); }

// number of current units in a full turn
double full_turn() { return the_angle_style().from_degrees(360.0); }

// clipping is always on
void clip(bool arg) {}

void clip_box() { the_clip_box() = clip_state(); }

void clip_box(const P& arg1, const P& arg2) {
  the_clip_box() = clip_state(arg1, arg2);
}

void clip_to(const P& arg) { the_clip_box() = clip_state(P(0, 0, 0), arg); }

void clip_box(const P& arg) { the_clip_box() = clip_state(-arg, arg); }

// add a clip face; perp points inward
void clip_face(const P& loc, const P& perp) {
  the_clip_box().add_face(loc, perp);
}

// add parallel clip planes
void clip_slice(const P& loc, const P& perp) {
  the_clip_box()
      .add_face(loc - 0.5 * EPIX_EPSILON * perp, perp)
      .add_face(loc + 0.5 * EPIX_EPSILON * perp, -perp);
}

void clip_slice(const P& perp, double thickness) {
  clip_slice(P(0, 0, 0), perp, thickness);
}

void clip_slice(const P& perp) { clip_slice(P(0, 0, 0), perp); }

void clip_slice(const P& loc, const P& perp, double thickness) {
  the_clip_box()
      .add_face(loc - 0.5 * thickness * perp, perp)
      .add_face(loc + 0.5 * thickness * perp, -perp);
}

// remove user-specified clip planes, preserve clip box settings
void clip_restore() { the_clip_box().clear_extras(); }

// Set crop state and select region
void set_crop(bool arg) { active_screen()->set_crop(arg); }

void crop() { active_screen()->crop(); }

void crop_rectangle(const P& sw, const P& ne) {
  active_screen()->crop_mask_rectangle(sw, ne).set_crop(true);
}

void crop_ellipse(const P& sw, const P& ne) {
  active_screen()->crop_mask_ellipse(sw, ne).set_crop(true);
}

void crop_diamond(const P& sw, const P& ne) {
  active_screen()->crop_mask_diamond(sw, ne).set_crop(true);
}

void crop_rectangle() { active_screen()->crop_mask_rectangle().set_crop(true); }

void crop_ellipse() { active_screen()->crop_mask_ellipse().set_crop(true); }

void crop_diamond() { active_screen()->crop_mask_diamond().set_crop(true); }

void crop_box(const P& arg1, const P& arg2) {
  active_screen()->crop_mask_rectangle(arg1, arg2).set_crop(true);
}

void crop_box()  // (re)set active crop box to active bounding box
{
  active_screen()->crop_mask_rectangle().set_crop(true);
}

// Set font size and face, label angle, mask color
void font_size(const std::string& arg) { the_label_style().fontsize(arg); }

void font_face(const std::string& arg) { the_label_style().fontface(arg); }

void label_color(const Color& col) { the_label_style().text_color(col); }

void label_angle(double th) { the_label_style().label_angle(th); }

void label_mask(const Color& arg) { the_label_style().mask_color(arg); }

void label_mask() { the_label_style().mask_color(White()); }

void label_pad(std::string len) {
  the_label_style().label_padding(length(len));
}

void label_border(const Color& col, double len) {
  the_label_style().label_border(pen_data(col, length(len)));
}

void label_border(const Color& col, std::string len) {
  the_label_style().label_border(pen_data(col, len));
}

// set color only
void label_border(const Color& col) {
  length len(the_label_style().label_border().width());
  the_label_style().label_border(pen_data(col, len));
}

// set width only
void label_border(double len) {
  Color col(the_label_style().label_border().color());
  the_label_style().label_border(pen_data(col, length(len)));
}

void label_border(std::string len) {
  Color col(the_label_style().label_border().color());
  the_label_style().label_border(pen_data(col, len));
}

void no_label_border() { label_border("0pt"); }

// Gray depth, path filling, [fill color]
void gray(double depth) { the_paint_style().fill_color(Black(depth)); }

// new function
void fill(const Color& col) {
  the_paint_style().fill_color(col);
  the_paint_style().fill_flag(!col.is_unset());
}

void fill(bool arg) {
  the_paint_style().fill_flag(arg);
  if (arg && the_paint_style().fill_color().is_unset())
    the_paint_style().fill_color(White());
}

void nofill() { the_paint_style().fill_flag(false); }

// Arrowhead parameters
void arrow_width(double w) { the_arrowhead_style().width(w); }

void arrow_ratio(double r) { the_arrowhead_style().ratio(r); }

void arrow_inset(double arg) { the_arrowhead_style().inset(arg); }

void arrow_fill(bool fill) { the_arrowhead_style().fill(fill); }

// Set (approximate) length of dashes, dot separation
void dash_size(double len) { the_path_style().separation(len); }

void dot_sep(double len) { dash_size(len); }

// Dot/box marker size
void dot_size(double diam) { the_mark_size().dot_size(diam); }

void tick_size(double len) { the_mark_size().tick_size(len); }

// Line style
void line_style(std::string style) { the_path_style() = path_state(style); }

void solid() { line_style("-"); }

void dashed() { line_style("-  -"); }

void dotted() { line_style(" . "); }

// Line width
void pen(const std::string& len) { the_paint_style().line_width(length(len)); }

void pen(double wd) { the_paint_style().line_width(length(wd)); }

void bbold() { the_paint_style().line_width(BBOLD_WIDTH); }

void bold() { the_paint_style().line_width(BOLD_WIDTH); }

void plain() { the_paint_style().line_width(PLAIN_WIDTH); }

// line width and color
void bbold(const Color& col) {
  the_paint_style().line_color(col);
  the_paint_style().line_width(BBOLD_WIDTH);
}

void bold(const Color& col) {
  the_paint_style().line_color(col);
  the_paint_style().line_width(BOLD_WIDTH);
}

void plain(const Color& col) {
  the_paint_style().line_color(col);
  the_paint_style().line_width(PLAIN_WIDTH);
}

void pen(const Color& col) { the_paint_style().line_color(col); }
void pen(const Color& col, double len) {
  the_paint_style().line_color(col);
  the_paint_style().line_width(length(len));
}
void pen(const Color& col, std::string len) {
  the_paint_style().line_color(col);
  the_paint_style().line_width(length(len));
}

// base width and color
void base(const Color& col) { the_paint_style().base_color(col); }
void base(const Color& col, double len) {
  the_paint_style().base_color(col);
  the_paint_style().base_width(length(len));
}
void base(const Color& col, std::string len) {
  the_paint_style().base_color(col);
  the_paint_style().base_width(length(len));
}

// Red-Blue-Green
void rgb(double r, double g, double b) {
  the_paint_style().line_color(RGB(r, g, b));
  the_paint_style().fill_color(RGB(r, g, b));
  the_label_style().text_color(RGB(r, g, b));
}

// Cyan-Magenta-Yellow-Black
void cmyk(double c, double m, double y, double k) {
  the_paint_style().line_color(CMYK(c, m, y, k));
  the_paint_style().fill_color(CMYK(c, m, y, k));
  the_label_style().text_color(CMYK(c, m, y, k));
}

void rgb(const P& arg) { rgb(arg.x1(), arg.x2(), arg.x3()); }

void cmyk(const P& arg) { cmyk(arg.x1(), arg.x2(), arg.x3(), 0); }

// primary colors
void red(double d) { rgb(d, 0, 0); }
void green(double d) { rgb(0, d, 0); }
void blue(double d) { rgb(0, 0, d); }
void white(double d) { rgb(d, d, d); }
void black(double d) { rgb(1 - d, 1 - d, 1 - d); }

void cyan(double d) { cmyk(d, 0, 0, 0); }
void magenta(double d) { cmyk(0, d, 0, 0); }
void yellow(double d) { cmyk(0, 0, d, 0); }
}  // namespace ePiX
