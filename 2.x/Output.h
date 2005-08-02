/* 
 *  Output.h -- epix2:: raw output functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 29, 2005
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

/*
 *   This file provides:
 *
 */


#ifndef EPIX2_OUTPUT
#define EPIX2_OUTPUT

#include <iostream>
#include <string>

#include "Constants.h"
#include "Pair.h"
#include "Point.h"
#include "Color.h"

namespace ePiX2 {

  class Output_State {

  public:

    Output_State(void);

    static Output_State output_state;

    // update and return T/F
    friend bool linecolor_isnt(const Color&);
    friend bool fillcolor_isnt(const Color&);
    friend bool fillstyle_isnt(const std::string&);

    static void reset(void) { Output_State::output_state = Output_State(); }

  private:
    std::string linecolor;
    std::string fillcolor;
    std::string fillstyle;

  }; // end of class Output_State

  inline double trunc(double t)
  {
    double tmp=t;
    if (fabs(tmp) < EPIX2_EPSILON)
      tmp=0;

    return tmp;
  } // end of trunc

  inline void print_line(const std::string message)
    {
      std::cout << std::endl << message;
    }
  
  inline void comment(const std::string message)
    {
      std::cout << std::endl << "%%  " << message;
    }
  
  inline void raw_print(const Pair arg)
    {
      std::cout << "(" << trunc(arg.x1()) << "," << trunc(arg.x2()) << ")";
    }

  inline void raw_print(const Point arg)
    {
      std::cout << "(" 
		<< trunc(arg.x1()) << "," 
		<< trunc(arg.x2()) << "," 
		<< trunc(arg.x3()) << ")";
    }

  /*
  void print(const P);
  void print(const pair);
  void print(const P location, const P offset);
  void print_vector(const P);

  void print_alignment(epix_label_posn POSN);
  void print_marker(epix_mark_type MARK);

  // Functions that write small pieces of a LaTeX \picture environment
  // are inlined. Aside from delimiters (braces and dollar), each command
  // preserves the LaTeX grouping level.
  inline void lbrace(void) { std::cout << '{'; }
  inline void rbrace(void) { std::cout << '}'; }
  inline void dollar(void) { std::cout << '$'; }
  inline void newl(void)   { std::cout << '\n'; }

  // common \LaTeX commands

  // print arg in braces
  template<class T>void epix_grouping(T arg)
    { std::cout << '{' << arg << '}'; }
  // same, in math mode
  template<class T>void epix_math_grouping(T arg)
    { std::cout << "{$" << arg << "$}"; }

  // label txt, with font information
  template<class T>void epix_label_text(T arg)
    { 
      std::cout << '{';
      if ( epix::fontsize != "normalsize" )
	{
	  std::cout << '\\' << epix::fontsize << " ";
	}

      if ( epix::fontface != "default" )
	std::cout << "\\text" << epix::fontface << '{';

      std::cout << arg;

      if ( epix::fontface != "default" )
	std::cout << '}';

      std::cout << '}'; 
    }

  inline void epix_neg(void)      { std::cout << "\\phantom{-}"; }

  inline void epix_put(void)      { std::cout << "\\put"; }
  inline void epix_multiput(void) { std::cout << "\\multiput"; }
  inline void epix_makebox(void)  { std::cout << "\\makebox(0,0)"; }
  inline void epix_whiten(void)   { std::cout << "\\whiten"; }

  inline void start_path(void)
    {
      newl();
      if (epix::using_pstricks)
	std::cout << "\\psline";

      else
	std::cout << "\\path";
    }
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
	std::cout << "\n\\psset{" << arg << "}";
    }

  inline void fill_color(std::string color="white") 
    { 
      if (epix::using_pstricks)
	{
	  epix::fillcolor=color; 
	  std::cout << "\n\\psset{fillcolor=" << epix::fillcolor << "}";
	}
    }

  inline void epix_rotatebox()
    {
      std::cout << "\\rotatebox";
      epix_grouping(epix::get_labelangle());
    }

  inline void epix_colorbox()
    {
      std::cout << "\\colorbox";
      if (epix::using_pstricks)
	epix_grouping(epix::fillcolor);
      else
	epix_grouping("white");
    }

  inline void epix_rule(const double x, const double y)
    {
      std::cout << "\\rule{" << x << "pt}{" << y << "pt}";
    }
  */

  /*
  inline void epix_rule(const double x,  const double y,
			const double dx, const double dy)
    {
      std::cout << "{\\kern " << dx << "pt \\rule[" << dy << "pt]{"
	   << x << "pt}{" << y << "pt}}";
    }
  */

  /*
  inline void epix_circle(const double r_pt) // radius in pt
    {
      lbrace();
      std::cout << "\\circle";
      epix_grouping(t2p(r_pt)); // convert radius to picture units
      rbrace();
    }
  inline void epix_disk(const double r_pt)
    {
      lbrace();
      std::cout << "\\circle*";
      epix_grouping(t2p(r_pt));
      rbrace();
    }

  inline void epix_newline(void)            { std::cout << "\n%%"; }
  inline void epix_newline(std::string msg) { std::cout << "\n%% " << msg; }

  inline void end_stanza(void)
  {
    epix_newline("---");
  }

  inline void end() { std::cout << "\n\\end{picture}\n"; }


  // Unified comment/warning/error functions
  inline void epix_warning(std::string msg)
  {
    std::clog << "ePiX WARNING: " << msg << "\n"; // buffered output
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

  inline void bold(void) { std::cout << "\n\\thicklines"; }
  inline void plain(void)  { std::cout << "\n\\thinlines"; }
  */

} /* end of namespace */

#endif /* EPIX2_OUTPUT */
