/* 
 *  dataplot.h -- ePiX::P data plotting and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.2
 * Last Change: December 31, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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


#ifndef EPIX_DATAPLOT
#define EPIX_DATAPLOT

#include <vector>
#include "globals.h"
#include "functions.h"

namespace ePiX {

  typedef std::vector<std::vector<double> > FILEDATA;

  // read columns of file into FILEDATA
  void read(const char*, FILEDATA&);

  // plot specified columns
  void plot(const FILEDATA& data_columns, epix_mark_type TYPE,
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	    P f(double, double, double) = xyz);

  void plot(const char* filename, epix_mark_type TYPE, int columns, 
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	    P f(double, double, double) = xyz);

  inline void data_plot(const char* filename, epix_mark_type TYPE)
    {
      epix_warning("data_plot deprecated; please use plot");
      plot(filename, TYPE, 2, 1, 2, 0);
    }

  // functions for data analysis
  double avg(FILEDATA data, unsigned int col1);
  double var(FILEDATA data, unsigned int col1);
  double covar(FILEDATA data, unsigned int col1, unsigned int col2);

  void regression(FILEDATA data, unsigned int col1=1, unsigned int col2=2);

  // Histogram plot of designated column, in the specified interval with
  // equal-length data subdivisions. By default the total area is unity;
  // v_scale adjusts height.
  void histogram(const FILEDATA& data_columns, unsigned int column, 
		 double low=x_min, double high=x_max, int subdivs=(int)x_size,
		 double v_scale=1);

  void histogram(const char* filename, int columns, unsigned int column, 
		 double low=x_min, double high=x_max, int subdivs=(int)x_size,
		 double v_scale=1);

} /* end of namespace */ 


#endif /* EPIX_DATAPLOT */
