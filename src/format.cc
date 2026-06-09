/*
 * format.cc -- ePiX output format interface and label-writing functions
 *
 * This file is part of ePiX, a c++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <list>

#include "enums.h"
#include "constants.h"

#include "errors.h"
#include "utils.h"

#include "functions.h"

#include "marker_style.h"
#include "label_style.h"
#include "path_style.h"
#include "Color.h"

#include "pen_data.h"

#include "pairs.h"
#include "edge_data.h"

#include "length.h"

#include "picture.h" // circ may need to add White to palette
#include "picture_data.h"

#include "format.h"

namespace ePiX {
  // public, non-virtual
  // LaTeX label-generating commands written in terms of protected interface
  std::string format::print_mark(const pair& here, const pair& offset,
				 const epix_mark_type mk,
				 const marker_sizes& sz,
				 const label_state& style,
				 const std::string& len) const
  {
    // convert dotsize to len
    length true_diam(sz.dot_size());
    double diam(true_diam.to(len).magnitude());

    // set diameter
    if (mk == DOT || mk == CIRC)
      diam *= 0.75;
    else if (mk == DDOT)
      diam *= 0.5;

    // and print
    if (mk == SPOT || mk == DOT || mk == DDOT)
      return print_circle_marker(here + pt_to_len(offset, len), diam, true,
				 style.text_color(), len);

    else if (mk == CIRC) // Draw filled dot, then ring
      {
	Color bk(style.mask_color());
	// use White for unset mask
	if (bk.is_unset())
	  {
	    bk = White();
	    the_picture().add_to_palette(bk);
	  }

	return print_circle_marker(here + pt_to_len(offset, len), diam, true,
				   bk, len)
	  + print_circle_marker(here + pt_to_len(offset, len), diam, false,
				style.text_color(), len);
      }

    else if (mk == RING)
      return print_circle_marker(here + pt_to_len(offset, len), diam, false,
				 style.text_color(), len);

    else
      return put_box(here + pt_to_len(offset, len),
		     print_latex_label(style, print_latex_marker(mk, sz, len),
				       false)); // no mask or box
  }

  // public, non-virtual
  std::string format::print_text(const pair& here, const pair& offset,
				 const std::string& label,
				 const label_state& style,
				 const std::string& len) const
  {
    return put_box(here + pt_to_len(offset, len),
		   print_latex_label(style, label, true));
  }


  // public, non-virtual
  std::string format::print_line(const edge2d& e,
				 const pair& offset,
				 const pen_data& line,
				 const pen_data& base,
				 const path_state& style,
				 const std::string& len) const
  {
    if ((line.width() < length(EPIX_EPSILON))
	|| (line.color().is_unset())
	|| !e.is_seen() )
      return "";

    // else
    std::list<edge2d> edges;
    edges.push_back(e);
    return print_line(edges, offset, line, base, style, len);
  }


  // public, non-virtual (file header comment block)
  std::string format::file_comments(const std::string& units,
				    const length& hoffset,
				    const length& voffset,
				    double Xmin, double Xmax,
				    double Ymin, double Ymax,
				    double Hsize, double Vsize) const
  {
    std::ostringstream obuf;

    const std::string hoff_dir(length(0) < hoffset ? "right" : "left");
    const std::string voff_dir(length(0) < voffset ? "up" : "down");

    const double hoff(hoffset.magnitude());
    const double voff(voffset.magnitude());

    std::stringstream bd_box, act_sz, fg_off;
    bd_box << "  Cartesian bounding box: ["
	   << Xmin << "," << Xmax << "] x [" << Ymin << "," << Ymax<< "]";

    act_sz << "  Actual size: " << Hsize << " x " << Vsize << units;

    fg_off << "  Figure offset: "
	   << hoff_dir << " by " << hoff << units << ", "
	   << voff_dir << " by " << voff << units;

    obuf << print_comment("ePiX-" + epix_version())
	 << print_comment("")
	 << print_comment(bd_box.str())
	 << print_comment(act_sz.str())
	 << print_comment(fg_off.str())
	 << print_comment("")
	 << print_comment(usepackages())
	 << print_comment("");

    return obuf.str();
  } // end of file_comments()


  // public, virtual
  std::string format::pic_header(const pair& sizes,
				 const pair& offsets,
				 const std::set<Color>& palette,
				 const std::string& len) const
  {
    std::stringstream obuf;
    obuf << print_palette(palette)
	 << set_unitlength(len)
	 << start_picture(sizes, offsets);

    return obuf.str();
  }

  // public, virtual
  std::string format::pic_footer() const
  {
    return end_picture();
  }

  // public, non-virtual
  std::string format::print_color(const std::string& model,
				  const std::string& name,
				  const std::vector<double>& dens) const
  {
    if (dens.size() == 3)
      return print_color(model, name, dens.at(0), dens.at(1), dens.at(2));

    else if (dens.size() == 4)
      return print_color(model, name,
			 dens.at(0), dens.at(1), dens.at(2), dens.at(3));

    else if (dens.size() == 0) // Neutral()
      return "";

    else
      {
	epix_warning("format::print_color(): Internal error");
	return print_color("rgb", "rgb_000000", 0, 0, 0);
      }
  }

  // protected, non-virtual (one-pen version)
  std::string format::print_line(const std::list<edge2d>& edges,
				 const pair& offset, const pen_data& line,
				 const path_state& style,
				 const std::string& attribs,
				 const std::string& len) const
  {
    if ( (line.width() < length(EPIX_EPSILON))
	 || (line.color().is_unset())
	 || edges.begin() == edges.end() )
      return "";

    // else
    std::stringstream value, obuf;

    obuf << set_pen_state(line);

    if (style.is_solid())
      obuf << print_paths(edges, offset, attribs, len);

    else // not solid
      {
	// compute diameter of dots
	length lw(line.width());
	lw.to(len);

	// compute unit length in pt
	length units("1"+len);
	units.to("pt");
	double in_pt(units.magnitude());

	double pattern_size(style.separation()); // in pt

	// split each edge into pattern_size chunks if necessary
	for (std::list<edge2d>::const_iterator ep = edges.begin();
	     ep != edges.end(); ++ep)
	  {
	    double curr_len(((*ep).length())*in_pt);

	    // number of dash patterns *ep can hold
	    unsigned int N((unsigned int) ceil(curr_len/pattern_size));
	    if (N == 1)
	      obuf << print_edge((*ep), offset, style.breakpts(),
				 lw.magnitude(), line.color(), attribs, len);

	    if (N > 1)
	      {
		pair T((*ep).tail()), dir((1.0/N)*((*ep).head() - T));
		for (unsigned int i=0; i<N; ++i)
		  obuf << print_edge(edge2d(T+i*dir, T+(i+1)*dir,
					    (*ep).is_seen()),
				     offset, style.breakpts(),
				     lw.magnitude(), line.color(),
				     attribs, len);
	      }
	  }
      } // end of !solid

    return obuf.str();
  } // end of print_line(edges, offset, line, style, attribs, len)


  std::string format::print_line(const edge2d& e,
				 const pair& offset,
				 const pen_data& line,
				 const path_state& style,
				 const std::string& attribs,
				 const std::string& len) const
  {
    if ( (line.width() < length(EPIX_EPSILON)) ||
         (line.color().is_unset()) ||
         !e.is_seen() )
      return "";

    std::list<edge2d> edges;
    edges.push_back(e);
    return print_line(edges, offset, line, style, attribs, len);
  }


  // local to this file
  std::string __epix_rule(double x, double y)
  {
    std::stringstream buf;
    buf << "\\rule{" << x << "pt}{" << y << "pt}";

    return buf.str();
  }


  // protected, non-virtual
  std::string format::xdefinecolor(const std::string& model,
				   const std::string& name,
				   double d1, double d2, double d3) const
  {
    std::stringstream obuf;
    obuf << "\\xdefinecolor{" << name << "}";

    if (model == "cmy")
      {
	double bk(min(min(d1,d2),d3));

	obuf << "{cmyk}{"
	     << d1-bk << "," << d2-bk << "," << d3-bk << "," << bk << "}%"
	     << std::endl;
      }

    else
      obuf << "{" << model << "}{" << d1 << "," << d2 << "," << d3 << "}%"
	   << std::endl;

    return obuf.str();
  }

  // protected, non-virtual
  std::string format::xdefinecolor(const std::string& model,
				   const std::string& name,
				   double d1, double d2,
				   double d3, double d4) const
  {
    std::stringstream obuf;
    obuf << "\\xdefinecolor{" << name << "}{cmyk}{"
	 << d1 << "," << d2 << "," << d3 << "," << d4 << "}%"
	 << std::endl;

    return obuf.str();
  }

  // protected, virtual
  std::string format::print_palette(const std::set<Color>& palette) const
  {
    std::stringstream obuf;

    for (std::set<Color>::const_iterator cp=palette.begin();
	 cp!=palette.end(); ++cp)
      obuf << print_color((*cp).model(), (*cp).name(), (*cp).densities());

    return obuf.str();
  }

  // protected, virtual
  std::string format::print(const pair& arg) const
  {
    std::stringstream o;
    o << "(" << truncate(arg.x1()) << "," << truncate(arg.x2()) << ")";
    return o.str();
  }

  // protected, virtual
  std::string format::print(const length& len) const
  {
    return len.name();
  }

  // protected, virtual
  std::string format::print(const Color& col) const
  {
    return col.name();
  }

  // protected, non-virtual (print non-solid edge as segments and dots)
  std::string format::print_edge(const edge2d& edge, const pair& offset,
				 const std::vector<double> bkpts,
				 double diam, const Color& col,
				 const std::string& attribs,
				 const std::string& len) const
  {
    if (!edge.is_seen())
      return "";

    if (bkpts.size() % 2 != 0)
      {
        epix_warning("Internal error in path pattern");
        return "";
      }

    // else
    std::stringstream obuf;

    const pair T(edge.tail() + pt_to_len(offset, len));
    const pair dir(edge.head() - edge.tail());

    for (unsigned int i=0; i < bkpts.size(); i += 2)
      {
        double curr(bkpts.at(i)), next(bkpts.at(i+1));

        if (EPIX_EPSILON < next - curr) // dash
	  obuf << start_open_path(attribs)
	       << print(T + curr*dir)
	       << path_connector()
	       << print(T + next*dir)
	       << end_open_path(attribs);

        else // dot
	  obuf << print_circle_marker(T + curr*dir, diam, true, col, len);
      }

    return obuf.str();
  }


  // protected, non-virtual (solid path, handle (offset) data only)
  std::string format::print_paths(const std::list<edge2d>& edges,
				  const pair& offset,
				  const std::string& attribs,
				  const std::string& len) const
  {
    std::stringstream obuf;

    std::list<edge2d>::const_iterator ep(edges.begin());

    const pair shift(pt_to_len(offset, len));

    bool all_visible(true); // all edges visible?

    // Assume not for now
    const std::string pname(start_open_path(attribs));

    // print contiguous lists of visible edges
    while (ep != edges.end())
      {
	if ((*ep).is_seen()) // print contiguous visible path
	  {
	    obuf << pname
		 << print((*ep).tail() + shift)
		 << path_connector()
		 << print((*ep).head() + shift);
	    ++ep;

	    while (ep != edges.end() && (*ep).is_seen())
	      obuf << path_connector() << print((*ep++).head() + shift);

	    obuf << end_open_path(attribs);
	  }

	else // invisible edge
	  {
	    ++ep;
	    all_visible = false;
	  }
      }

    std::string value(obuf.str());

    --ep; // last edge

    // if we're a closed loop of visible edges, adjust open/close path
    if (all_visible && (*edges.begin()).tail() == (*ep).head())
      {
	value.replace(0,
		      start_open_path(attribs).size(),
		      start_closed_path(attribs));
    
	value.replace(value.size()-end_open_path(attribs).size(),
		      end_open_path(attribs).size(),
		      end_closed_path(attribs));
      }

    return break_lines(value, path_break());
  }


  //// Label and marker functions ////
  // protected, non-virtual (print LaTeX command for marker of specified size)
  std::string format::print_latex_marker(const epix_mark_type mark,
					 const marker_sizes& sz,
					 const std::string& len) const
  {
    std::stringstream obuf;

    obuf << "$";

    switch(mark)
      {
      case PLUS:
	obuf << "+";
	break;
      case OPLUS:
	obuf << "\\oplus";
	break;
      case TIMES:
	obuf << "\\times";
	break;
      case OTIMES:
	obuf << "\\otimes";
	break;
      case DIAMOND:
	obuf << "\\diamond";
	break;
      case UP:
	obuf << "\\bigtriangleup";
	break;
      case DOWN:
	obuf << "\\bigtriangledown";
	break;
      case BOX:
	obuf << __epix_rule(0.75*sz.dot_size(), 0.75*sz.dot_size());
	break;
      case BBOX:
	obuf << __epix_rule(0.5*sz.dot_size(), 0.5*sz.dot_size());
	break;
      case HTICK:
	obuf << __epix_rule(0.5, 2*sz.tick_size());
	break;
      case VTICK:
	obuf << __epix_rule(2*sz.tick_size(), 0.5);
	break;
	// should never see these cases
      case SPOT:
      case DOT:
      case DDOT:
      case CIRC:
      case RING:
	epix_warning("Mark type internal error"); // fall through
      case PATH: // fall through
      case TEXT:
      default:
	return ""; // N.B. return
      }

    obuf << "$";

    return obuf.str();
  }


  // public, non-virtual
  // handles font face, size, and color
  std::string format::print_latex_hbox(const label_state& style,
				       const std::string& label) const
  {
    std::stringstream text_buf;

    bool need_ff(style.fontface() != "rm");  // roman
    bool need_fs(style.fontsize() != "nsz"); // normalsize

    text_buf << "\\hbox{\\color{" << print(style.text_color()) << "}";

    if (need_ff)
      text_buf << "\\text" << style.fontface() << "{";

    if (need_fs)
      text_buf << "\\" << style.fontsize() << " ";

    text_buf << label;

    if (need_ff)
      text_buf << "}";

    text_buf << "}";

    return text_buf.str();
  }


  // protected, virtual
  std::string format::path_break() const
  {
    std::stringstream buf;
    buf << std::endl << "  ";

    return buf.str();
  }

  // inserted between points of a path
  std::string format::path_connector() const
  {
    return "";
  }

  // protected, non-virtual
  // handles rotation, masking, border, and alignment
  std::string format::print_latex_label(const label_state& style,
					const std::string& label,
					bool boxed) const
  {
    std::stringstream obuf;

    // need rotatebox?
    bool rotated(EPIX_EPSILON < fabs(style.label_angle()));

    // need colorbox?
    bool masked(boxed && style.is_masked());

    // need extra fcolorbox?
    bool padded(style.label_padding() != length(0));

    obuf << "\\makebox(0,0)"
	 << print(style.align_to()); // "none" -> ""

    obuf << "{";

    if (rotated)
      obuf << "\\rotatebox{" << style.label_angle() << "}{";

    if (masked)
      {
	pen_data bpen(style.label_border());

	// draw border?
	if (!bpen.color().is_unset() && length(EPIX_EPSILON) < bpen.width())
	  obuf << "\\fboxrule" << print(bpen.width()) << "%" << std::endl
	       << "\\fcolorbox{" << print(bpen.color()) << "}{"
	       << print(style.mask_color()) << "}";

	else
	  obuf << "\\colorbox{" << print(style.mask_color()) << "}";

	if (padded)
	  {
	    // cancel off the xcolor default
	    length extra_pad(style.label_padding() + length(-3));

	    obuf << "{\\fboxrule" << print(extra_pad) << "%" << std::endl
		 << "\\fcolorbox{" << print(style.mask_color()) << "}{"
		 << print(style.mask_color()) << "}";
	  }

	obuf << "{";
      }

    // finally, the payload
    obuf << print_latex_hbox(style, label);

    // close wrappers
    if (masked)
      {
	obuf << "}";

	if (padded)
	  obuf << "}";
      }

    if (rotated)
      obuf << "}";
  
    obuf << "}";

    return obuf.str();
  } // end of print_latex_label

  // private (convert an epix_label_posn to a string)
  std::string format::print(const epix_label_posn arg) const
  {
    std::string val;
    switch(arg) 
      {
      case none:
	return ""; // other cases return at bottom

      case c:
	val="c";
	break;
      case r:
	val="l"; // opposite of LaTeX
	break;
      case tr:
      case rt:
	val="bl";
	break;
      case t:
	val="b";
	break;
      case tl:
      case lt:
	val="br";
	break;
      case l:
	val="r";
	break;
      case bl:
      case lb:
	val="tr";
	break;
      case b:
	val="t";
	break;
      case br:
      case rb:
	val="tl";
	break;
      }

    return "[" + val + "]";
  } // end of format::print(align_type)


  // private (true pt offset to len)
  pair format::pt_to_len(pair offset, const std::string& len) const
  {
    return offset *= length("1pt").to(len).magnitude();
  }
} // end of namespace
