/*
 * PS.cc -- ePiX2::PS (PostScript) output class
 *
 * This file is part of ePiX, a program for creating high-quality
 * figures in LaTeX
 *
 * Version 2.0pre
 *
 * Last Change: February 12, 2006
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
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

#include <string>
#include <sstream>
#include <list>
#include <set>

#include "Constants.h"
#include "Functions.h"
#include "Styles.h"
#include "PS.h"

namespace ePiX2 {

  std::string PS::print_box(const Pair loc, std::string msg,
			    const Color& text, const Color& fill,
			    const Color& edge,
			    const Length& pad, const Length& bd,
			    epix2_align_type align,
			    epix2_font_type font, double angle) const
  {
    using std::endl;
    std::stringstream obuf;

    switch(font)
      {
      case ROMAN:
	obuf << "/Times-Roman findfont" << endl;
	break;

      case ITALIC:
	obuf << "/Times-Italic findfont" << endl;
	break;

      case BFACE:
	obuf << "/Times-Bold findfont" << endl;
	break;

      case TTYPE:
	obuf << "/Courier findfont" << endl;
	break;
      }

    obuf << "0.2 scalefont" << endl << "setfont" << endl
	 << print(loc) << " moveto" << endl
	 << print(text) << endl
	 << "(" << msg << ") show" << endl;

    return obuf.str();
  } // end of PS::print_box()


  // location; marker; text, fill, edge colors; padding, border width;
  std::string PS::print_mark(const Pair loc, epix2_mark_type mark,
			     const Color& text, const Color& fill,
			     const Color& edge,
			     const Length& pad, const Length& bd) const
  {
    std::stringstream obuf;
    obuf << "\\rput" << print(loc) << "{";

    if (fill != Color::Unset) // draw border if masking
      {
	obuf << "\\psframebox*[framesep=" << print(pad);

	if (m_fill != fill)
	  obuf << ",fillcolor=" << print(fill);

	if (m_line != edge)
	  obuf << ",linecolor=" << print(edge);

	if (m_pen != bd)
	  obuf << ",linewidth=" << print(bd);

	obuf << "]";
      }

    else // not masking
      ;

    // set mark color
    if (text != Black(1))
      obuf << "\\begin{" << print(text) << "}";

    obuf << print(mark);

    if (text != Black(1))
      obuf << "\\end{" << print(text) << "}";

    obuf << "}" << std::endl;

    return obuf.str();
  } // end of PS::print_mark()


  // default line color and width, fill color, closed/filled flags, vertices
  std::string PS::print_sil(const Color& line, const Length& pen,
			    const Color& fill,
			    bool filled, bool cropped,
			    const std::list<Pair>& vertices) const
  {
    using std::endl;

    std::stringstream obuf;
    obuf << "newpath" << endl;

    std::list<Pair>::const_iterator p=vertices.begin();

    for (p=vertices.begin(); p!=vertices.end(); ++p)
      {
	obuf << print(*p) ;
	if (p == vertices.begin())
	  obuf << " moveto" << endl;
	else
	  obuf << " lineto" << endl;
      }

    obuf << "closepath" << endl;

    if (filled)
      {
	obuf << "gsave" << endl << print(fill) << endl 
	     << "fill"  << endl
	     << "grestore" << endl;
      }

    // TO DO: if cropped, only draw uncropped edges
    obuf << print(line) << endl << "stroke" << endl << endl;

    return obuf.str();
  } // end of PS::print_sil()


  std::string PS::print_outline(const Color& lc, const Length& lw,
				const Color& bc, const Length& bw,
				const std::list<Pair>& vertices) const
  {
    using std::endl;

    std::stringstream obuf;
    obuf << "newpath" << endl;

    std::list<Pair>::const_iterator p=vertices.begin();

    for (p=vertices.begin(); p!=vertices.end(); ++p)
      {
	obuf << print(*p) ;
	if (p == vertices.begin())
	  obuf << " moveto" << endl;
	else
	  obuf << " lineto" << endl;
      }


    if ((bc != lc) && (lw < bw)) // TO DO: set line width
      {
	obuf << "gsave" << endl << print(bc) << endl 
	     << "stroke"  << endl
	     << "grestore" << endl;
      }

    obuf << print(lc) << endl << "stroke" << endl << endl;

    return obuf.str();
  }

  std::string PS::pic_header(const Pair sw, const Pair ne, 
			     const Length& len,
			     const std::set<Color>& pallet) const
  {
    using std::endl;

    std::stringstream obuf;
    Length len_in_pt = len;
    len_in_pt *= recip(len_in_pt.magnitude()); // one unit
    len_in_pt.to("pt");
    double scale = len_in_pt.magnitude();

    obuf << "%!" << endl << "%%Creator: ePiX-2.0pre" << endl
	 << "gsave" << endl
	 << "0 setlinecap" << endl << "1 setlinejoin" << endl
	 << scale << " " << scale << " scale" << endl
      //	 << recip(scale) << " setlinewidth" << endl << endl;
	 << 0.001 << " setlinewidth" << endl << endl;

    return obuf.str();
  }

  std::string PS::pic_footer(void) const
  {
    std::stringstream obuf;
    obuf << "grestore" << std::endl << "showpage" << std::endl
	 << "%%EOF" << std::endl;

    return obuf.str();
  }

  std::string PS::print_color(std::string model, std::string name,
			      const double d1, const double d2,
			      const double d3) const
  {
    std::stringstream obuf;
    obuf << d1 << " " << d2 << " " << d3;
    if (model == "cmy")
      obuf << " " << min(min(d1,d2),d3);

    obuf << " set" << model;
    if (model == "cmy")
      obuf << "k";

    obuf << "color";

    return obuf.str();
  }

  std::string PS::print_color(std::string model, std::string name,
			      const double d1, const double d2,
			      const double d3, const double d4) const
  {
    std::stringstream obuf;
    obuf << d1 << " " << d2 << " " << d3 << " " << d4
	 << " set" << model << "color";

    return obuf.str();
  }


  std::string PS::pic_caption(std::string caption) const
  {
    std::stringstream obuf;
    obuf << "\\caption{" << caption << "}" << std::endl;
    return obuf.str();
  }

  std::string PS::pic_xref(std::string xref) const
  {
    std::stringstream obuf;
    obuf << "\\label{" << xref << "}" << std::endl;
    return obuf.str();
  }

  static std::string epix2_rule(const double x, const double y)
  {
    std::stringstream buf;
    buf << "\\rule{" << x << "pt}{" << y << "pt}";

    return buf.str();
  }

  std::string PS::print(const epix2_align_type arg) const
  {
    std::string val;
    switch(arg) 
      {
      case B:
	val="B";
	break;
      case Br:
	val="Br";
	break;
      case Bl:
	val="Bl";
	break;
      case c:
	val="c";
	break;
      case r:
	val="r";
	break;
      case tr:
      case rt:
	val="tr";
	break;
      case t:
	val="t";
	break;
      case tl:
      case lt:
	val="tl";
	break;
      case l:
	val="l";
	break;
      case bl:
      case lb:
	val="bl";
	break;
      case b:
	val="b";
	break;
      case br:
      case rb:
	val="br";
	break;
      }

    return val;
  } // end of PS::print(epix2_align_type)

  std::string PS::print(epix2_mark_type mark) const
  {
    std::string val="\\$";

    switch(mark)
      {
      case MK_NONE:
	val = ""; // N.B. reassign, not append
	break;
      case MK_CIRC:
	val += "\\circle";
	break;
      case MK_SPOT:
	val += "\\circle*";
	break;
      case MK_RING:
	val += "\\circle";
	break;
      case MK_DOT:
	val += "\\circle*";
	break;
      case MK_DDOT:
	val += "\\circle*";
	break;
      case MK_PLUS:
	val += "+";
	break;
      case MK_OPLUS:
	val += "\\oplus";
	break;
      case MK_TIMES:
	val += "\\times";
	break;
      case MK_OTIMES:
	val += "\\otimes";
	break;
      case MK_DIAMOND:
	val += "\\diamond";
	break;
      case MK_UP:
	val += "\\bigtriangleup";
	break;
      case MK_DOWN:
	val += "\\bigtriangledown";
	break;
      case MK_BOX:
	val += epix2_rule(2, 2); // Magic numbers
	break;
      case MK_BBOX:
	val += epix2_rule(1.5, 1.5);
	break;
      case MK_HTICK:
	val += epix2_rule(4, 0.5);
	break;
      case MK_VTICK:
	val += epix2_rule(0.5, 4);
	break;
      }

    if (mark != MK_NONE)
      val +="$";
    return val;
  } // end of PS::print(epix2_mark_type)


  std::string PS::print(epix2_font_type f) const
  {
    std::string val = "\\";

    switch(f)
      {
      case ROMAN:
	val = ""; // N.B. reassign
	break;
      case ITALIC:
	val += "textit";
	break;
      case BFACE:
	val += "textbf";
	break;
      case TTYPE:
	val += "texttt";
	break;
      }

    return val;
  } // end of PS::print(epix2_font_type)

  static double trunc(double t)
  {
    double tmp=t;
    if (fabs(tmp) < EPIX2_EPSILON)
      tmp=0;

    return tmp;
  } // end of trunc

  std::string PS::print(const Pair& arg) const
  {
    std::stringstream obuf;
    obuf << trunc(arg.x1()) << " " << trunc(arg.x2());
    return obuf.str();
  }

  std::string PS::print(const Color& col) const
  {
    return col.print_to(*this);
  }

  std::string PS::print(const Length& len) const
  {
    std::stringstream o;
    o << len.magnitude() << len.units();
    return o.str();
  }

} /* end of namespace */
