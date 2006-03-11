/*
 * output.h -- ePiX file-formatting output routines
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: March 06, 2006
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

#include <iostream>
#include <string>

#include "globals.h"
#include "triples.h"
#include "lengths.h"

namespace ePiX {

  using std::cout;

  void unitlength(const char *units);

  // Basic Sectioning Functions

  // Begin/end picture
  void begin(void);
  void end(void);
  
  inline void raw_print(const pair arg)
  {
    cout << "(" << arg.x1() << "," << arg.x2() << ")";
  }


  //  std::ostream& operator<< (std::ostream& s, const P);
  //  std::ostream& operator<< (std::ostream& s, const path&);


  void print(const P);
  void print(const pair);
  void print(const P location, const P offset);
  void print_vector(const P);

  void print_alignment(epix_label_posn POSN);
  void print_marker(epix_mark_type MARK);

  // Functions that write small pieces of a LaTeX \picture environment
  // are inlined. Aside from delimiters (braces and dollar), each command
  // preserves the LaTeX grouping level.
  inline void lbrace(void) { cout << '{'; }
  inline void rbrace(void) { cout << '}'; }
  inline void dollar(void) { cout << '$'; }
  inline void newl(void)   { cout << '\n'; }

  // common \LaTeX commands

  // print arg in braces
  template<class T>void epix_grouping(T arg)
    { cout << '{' << arg << '}'; }
  // same, in math mode
  template<class T>void epix_math_grouping(T arg)
    { cout << "{$" << arg << "$}"; }

  // label txt, with font information
  template<class T>void epix_label_text(T arg)
    { 
      cout << '{';
      if ( epix::fontsize != "normalsize" )
	{
	  cout << '\\' << epix::fontsize << " ";
	}

      if ( epix::fontface != "default" )
	cout << "\\text" << epix::fontface << '{';

      cout << arg;

      if ( epix::fontface != "default" )
	cout << '}';

      cout << '}'; 
    }

  inline void epix_neg(void)      { cout << "\\phantom{-}"; }

  inline void epix_put(void)      { cout << "\\put"; }
  inline void epix_multiput(void) { cout << "\\multiput"; }
  inline void epix_makebox(void)  { cout << "\\makebox(0,0)"; }
  inline void epix_whiten(void)   { cout << "\\whiten"; }

  void start_path(void);
  inline void end_path(void) {}

  // string-valued functions for path::draw()
  inline std::string start_path_string(void)
    {
      if (epix::using_pstricks)
	return "\n\\psline";

      else
	return "\n\\path";
    }
  inline std::string end_path_string(void) 
    {
      return "";
    }

  inline void psset(std::string arg) 
    { 
      if (epix::using_pstricks)
	cout << "\n\\psset{" << arg << "}";
    }

  inline void fill_color(std::string color="white") 
    { 
      if (epix::using_pstricks)
	{
	  epix::fillcolor=color; 
	  cout << "\n\\psset{fillcolor=" << epix::fillcolor << "}";
	}
    }

  inline void epix_rotatebox()
    {
      cout << "\\rotatebox";
      epix_grouping(epix::get_labelangle());
    }

  inline void epix_colorbox()
    {
      cout << "\\colorbox";
      if (epix::using_pstricks)
	epix_grouping(epix::fillcolor);
      else
	epix_grouping("white");
    }

  inline void epix_rule(const double x, const double y)
    {
      cout << "\\rule{" << x << "pt}{" << y << "pt}";
    }

  /*
  inline void epix_rule(const double x,  const double y,
			const double dx, const double dy)
    {
      cout << "{\\kern " << dx << "pt \\rule[" << dy << "pt]{"
	   << x << "pt}{" << y << "pt}}";
    }
  */

  void epix_circle(const double r_pt); // radius in pt
  void epix_disk(const double r_pt);

  inline void epix_newline(void)            { cout << "\n%%"; }
  inline void epix_newline(std::string msg) { cout << "\n%% " << msg; }

  inline void end_stanza(void)
  {
    epix_newline("---");
  }

  inline void end() { cout << "\n\\end{picture}\n"; }


  // Unified comment/warning/error functions
  inline void epix_warning(std::string msg)
  {
    std::cerr << "ePiX WARNING: " << msg << "\n";
  } 

  inline void epix_error(std::string msg)
  {
    std::cerr << "ePiX ERROR: " << msg << "\n";
    exit(1);
  } 


  //  void epix_comment(char *);
  void end_stanza(void);
  void line_break(int, int);

  // Misc Styles and output formatting
  // Output styles
  void pen(std::string);
  void pen(double);

  inline void bold(void) { cout << "\n\\thicklines"; }
  inline void plain(void)  { cout << "\n\\thinlines"; }

  // ePiX provides rgb/cmyk colors via pstcol. The following are equivalent:
  //    red();
  //    rgb(1,0,0);
  //
  // cmyk(0,1,1,0); is visually equivalent but not the same.
  //
  // Densities outside the unit interval [0,1] are truncated, e.g., 
  //    rgb(2, -1, 0.3) = rgb(1, 0, 0.3).

  // red-green-blue
  void rgb(double r, double g, double b); 
  // cyan-magenta-yellow-black
  void cmyk(double c, double m, double y, double k);

  // primary colors
  inline void red(double d=1)   { rgb(d,0,0); }
  inline void green(double d=1) { rgb(0,d,0); }
  inline void blue(double d=1)  { rgb(0,0,d); }
  inline void white(double d=1) { rgb(d,d,d); }
  inline void black(double d=1) { rgb(1-d,1-d,1-d); }

  inline void cyan(double d=1)     { cmyk(d,0,0,0); }
  inline void magenta(double d=1)  { cmyk(0,d,0,0); }
  inline void yellow(double d=1)   { cmyk(0,0,d,0); }

} /* end of namespace */

#endif /* EPIX_OUTPUT */
