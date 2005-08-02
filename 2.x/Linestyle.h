/*
 * Linestyle.h -- ePiX3d line styles
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 0.8.11rc6
 *
 * Last Change: June 10, 2004
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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
 * Unlike in earlier versions of ePiX, in which eepic.sty's dashed/dotted
 * capabilities were used, dashed and dotted lines are now rendered using
 * paths and dots, either one segment per dash or one dot per path point.
 * The "stretch" is a positive integer with the following significance:
 *   - for a dashed path, the number of points between consecutive dashes
 *     (default is 2: *---*   *---*   *---*)
 *   - for a dotted path, the number of points between consecutive dots
 *     (default is one)
 *
 * A dashed path also has a "dashlength", the number of segments in a dash,
 * e.g., stretch=3, dashlength=2: *---*---*   *---*---*   *---*---*
 * In particular, dashes can conform to the path they trace. However,
 * dashed and dotted paths tend to be expensive in terms of file size.
 */

#ifndef EPIX3d_LINESTYLE
#define EPIX3d_LINESTYLE

namespace ePiX3d {

  // plain = 0.4pt, bold = 0.8pt
  const double PLAIN=0.4;
  const double BOLD=0.8;

  enum epix3d_path_style {SOLID, DASHED, DOTTED};
  
  class Line_Style {

  private:
    double linewidth;
    int stretch;
    int dashlength;
    epix3d_path_style style;

  public:
    Line_Style() : linewidth(PLAIN), stretch(1), dashlength(1), style(SOLID) {}

    // set/get members
    void set_path_style(epix3d_path_style arg) { style = arg; }
    epix3d_path_style path_style(void) { return style; }

    void set_path_width(const double width) { linewidth=width; }
    double path_width(void) const { return linewidth; }

    /*
    void bold(void)     { current_style.set_path_width(BOLD); }
    void bbold(void)    { current_style.set_path_width(BBOLD); }
    void endbold(void)  { current_style.set_path_width(PLAIN); }
    void end_bold(void) { current_style.set_path_width(PLAIN); }
    void plain(void)    { current_style.set_path_width(PLAIN); }

    // set path style
    void solid(void) { current_style.set_path_style(SOLID); }
    void dashed(void){ current_style.set_path_style(DASHED); }
    void dotted(void){ current_style.set_path_style(DOTTED); }
    */

  }; /* end of class epix_style */

} /* end of namespace */

#endif /* EPIX3d_LINESTYLE */
