/*
 * output.h -- ePiX file-formatting output macros
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

#ifndef EPIX_OUTPUT
#define EPIX_OUTPUT

#include <string>

namespace ePiX {

  class P;
  class pair;

  void unitlength(const char *units);

  // Basic Sectioning Functions

  // Begin/end picture
  void begin();
  void end();

  // print truncated pair/triple coordinates  
  void raw_print(const pair&);

  void print(const P&);
  void print(const pair&);
  void print(const P& location, const P& offset);
  void print_vector(const P&);

  void print_alignment(epix_label_posn POSN);
  void print_marker(epix_mark_type MARK);

  // Functions that write small pieces of a LaTeX \picture environment
  void lbrace(); // {
  void rbrace(); // }
  void dollar(); // $
  void newl();   // \n

  // common \LaTeX commands

  // print arg in braces
  void epix_grouping(const double);
  void epix_grouping(const std::string&);

  // same, in math mode
  void epix_math_grouping(const double);
  void epix_math_grouping(const std::string&);

  // label txt, with font information
  void epix_label_text(const std::string& arg);

  void epix_neg(); // phantom{-}
  void epix_put();
  void epix_multiput();
  void epix_makebox();
  void epix_whiten();

  void start_path();
  void end_path();

  // string-valued functions for path::draw()
  std::string start_path_string();
  std::string end_path_string();

  void psset(const std::string&);
  void fill_color(const std::string& color="white");

  void epix_rotatebox();
  void epix_colorbox();

  void epix_rule(const double x, const double y);

  void epix_circle(const double r_pt); // radius in pt
  void epix_disk(const double r_pt);

  void epix_newline(); // \n%%
  void epix_newline(const std::string&);

  void end_stanza();
  void end();
  void line_break(int, int);

  // Misc Styles and output formatting
  // Output styles
  void pen(const std::string&);
  void pen(double);

  void bold();
  void plain();
} // end of namespace

#endif /* EPIX_OUTPUT */
