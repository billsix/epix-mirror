/* 
 * globals.h -- Top-level header file
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version EPIX_VERSION_NUMBER
 * Last Change: January 07, 2005
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
 *  This file provides the epix class to manage otherwise global 
 *  variables:
 *
 *  - dash_length (2 -- 200 true points; parameter same as dot_sep)
 *  - dash_fill   (0.05 -- 0.95, fraction of path filled when dashed)
 *  - dot_size    (0.5  --  256 true pt)
 *  - dot_sep
 *
 *  - gray_depth  (0=white -- 1=black)
 *  - angle_units (radians, degrees, revolutions)
 *  - path_style  (SOLID, DASHED, DOTTED)
 *
 *  - clipping
 *  - cropping
 *  - fill_paths
 */

#ifndef EPIX_GLOBALS
#define EPIX_GLOBALS

#include <string>
#include <cmath>

#ifdef EPIX_COMPILE_OUTPUT
namespace ePiX {
  std::string epix_version="EPIX_VERSION_NUMBER"; 
}
#endif

namespace ePiX {

  enum epix_path_style  {SOLID, DASHED, DOTTED};
  //  enum epix_output_type {EEPIC}; // METAPOST, SVG...

  // for backward compatibility, not in class epix below
  extern double x_min, x_max, x_size, y_min, y_max, y_size;
  extern double tix;

  inline double min(double a, double b) { return a<b ? a : b; }
  inline double max(double a, double b) { return a<b ? b : a; }

  inline double snip_to(double var, const double arg1, const double arg2)  
    {
      if      (var < min(arg1,arg2)) var = min(arg1,arg2);
      else if (var > max(arg1,arg2)) var = max(arg1,arg2);
      return var;
    }

  // global "style" constants
  class epix {
  public:
    static bool clipping;
    static bool cropping;
    static bool fill_paths;

    static bool using_pstricks;
    static std::string fillcolor; // no effect unless using_pstricks

    static std::string fontsize; // valid LaTeX font size
    static std::string fontface; // valid LaTeX font face

    // functions for setting style parameters
    friend void dash_fill(double t=0.5)    { dashfill = snip_to(t,0.05,0.95); }
    friend void dash_length(double len=12) { separation = snip_to(len,2,200); }

    friend void dot_sep(double len=12)     { separation = snip_to(len,2,200); }
    friend void dot_size(double diam=3)    { dotsize = snip_to(diam,0.5,256); }

    friend void gray(double depth=0.3) { gray_depth = snip_to(depth, 0, 1); }

    friend void radians(void)         { angle_units = 1.0; }
    friend void degrees(void)         { angle_units = M_PI/180; }
    friend void revolutions(void)     { angle_units = 2*M_PI; }

    // radians per angle unit
    friend double angle(const double t) { return t*angle_units; }

    // store label rotation angle internally in degrees
    friend void label_angle(double t); // defined in functions.cc

    // set state variable
    friend void solid(void)  { PATH_STYLE = SOLID; }
    friend void dashed(double t=0) 
      { 
	PATH_STYLE = DASHED; 
	if ( t != 0) dash_fill(t); // by default, leave
      }
    friend void dotted(double t=0) 
      { 
	PATH_STYLE = DOTTED; 
	if (t != 0) dot_size(t);
      }

    friend void clip(bool arg = true)   { clipping = arg; }
    friend void crop(bool arg = true)   { cropping = arg; }
    friend void fill(bool arg = true)   { fill_paths = arg; }
    friend void use_pstricks(bool arg = true)   { using_pstricks = arg; }

    // set font size and face
    friend void font_size(std::string arg="normalsize") { fontsize = arg; }
    friend void font_face(std::string arg="default")    { fontface = arg; }

    friend void begin(void); // defined in output.cc

    // set arrowhead parameters
    friend void arrow_width(double w=3) { arrowwidth=fabs(0.5*w); }
    friend void arrow_ratio(double r=5.5) { arrowratio=fabs(r); }
    friend void arrow_camber(double arg=0)
      { arrowcamber=snip_to(arg,0,1); }
    friend void arrow_fill(double dens=0) { arrowfill=snip_to(dens,0,1);}
    static double get_arrow_width(void)  { return arrowwidth; }
    static double get_arrow_ratio(void)  { return arrowratio; }
    static double get_arrow_camber(void) { return arrowcamber; }
    static double get_arrow_fill(void)   { return arrowfill; }

    // for internal use
    static double get_dashfill(void)   { return dashfill; }
    static double get_dashlength(void) { return separation; }

    static double get_gray(void)       { return gray_depth; }
    static double get_dotsize(void)    { return dotsize; }
    static double full_turn(void)      { return 2*M_PI/angle_units; }
    static double get_labelangle(void) { return labelangle; } // in degrees
    static epix_path_style path_style(void) { return PATH_STYLE; }

  private:
    static double dashfill;    // 0.05 --   0.95
    static double separation;  // 2    -- 200 (true pt)
    static double gray_depth;  // 0    --   1
    static double dotsize;     // 0.5  --  10

    static double angle_units;
    static double labelangle;  // rotate labels this amount in degrees

    // arrowhead parameters
    static double arrowwidth; // Half-width of arrowheads in pt
    static double arrowratio; // 2*Length/width ratio of arrowheads
    static double arrowcamber;// Base indentation (fraction of width*ratio)
    static double arrowfill;  // Fill density for arrowheads

    //    static epix_output_type OUTPUT_TYPE;
    //    static std::string font;

    static epix_path_style PATH_STYLE;
    static int begin_count; // number of times begin() has been called

  }; // end of class epix

  // Constants not meant to be modifiable except at compile time

  // Precision-related constants:
  // ePiX's idea of too small/large
  const double EPIX_EPSILON=0.0001;  // 10^-4
  const double EPIX_INFTY=100000.0;  // 10^5

  // In calculus plotting, the interval between adjacent points is too large
  // for accuracy. Divide each such interval into this many subintervals
  const int EPIX_ITERATIONS=200;


  // Output-formatting constants:
  const int EPIX_NUM_PTS=80;     // # of points in ellipses, splines
  const int EPIX_FILE_WIDTH=70;  // Width of output file
  const int EPIX_PATH_LENGTH=60; // Number of points per path segment

  // Enumeration types:
  enum epix_mark_type {PATH, CIRC, SPOT, RING, DOT, DDOT, PLUS, OPLUS, 
		       TIMES, OTIMES, DIAMOND, UP, DOWN, BOX, BBOX, 
		       H_TICK, V_TICK, TEXT};

  enum epix_label_posn {none, c, r, tr, rt, t, tl, lt, l, bl, lb, b, br, rb};

  enum epix_tick_type {TICK_NULL, H_AXIS, V_AXIS};

  enum epix_field_type {SLOPE, DART, VECTOR};

  enum epix_integral_type {LEFT, RIGHT, UPPER, LOWER, TRAP, MIDPT};

  // Deprecated constants

  const int EPIX_PAIRS_PER_LINE=4;/* Number of pairs per line in output file */


  // <pic_unit> is one of the following valid LaTeX length units: 
  // bp, cm, in, mm, pc, pt (default), or sp.
  // <pic_size> is a double that (with pic_unit) specifies unitlength
  extern double pic_size; // e.g., 1
  extern char *pic_unit;  // e.g., "pt" 


  // Shape error types...
  enum constructor_error_type {MALFORMED, MULTIPLICITY, COLLINEAR_PTS};
  enum join_error_type {TANGENT, PARALLEL, COINCIDENT, NON_COPLANAR,
			SEPARATED, CONCENTRIC};

  // and handlers
  struct constructor_error {
    constructor_error_type type;
    constructor_error(constructor_error_type x) { type = x; }
  };

  struct join_error {
    join_error_type type;
    join_error(join_error_type x) { type = x; }
  };

} /* end of namespace */

#endif /* EPIX_GLOBALS */
