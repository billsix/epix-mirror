/* 
 * data_file.h -- ePiX::data_file class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.6
 * Last Change: July 04, 2007
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
#ifndef EPIX_DATA_FILE
#define EPIX_DATA_FILE

#include <vector>
#include <list>

#include "enums.h"

namespace ePiX {

  class data_bins;
  class data_mask;
  class interval;

  class data_file {
  public:
    explicit data_file(unsigned int n=2);

    // get size from first line of file;
    // provide two versions to avoid exposing default delim/commt strings
    data_file(const char* filename,
	      const std::string& delim, const std::string& commt);
    explicit data_file(const char* filename);

    // file made from components
    data_file(double f(double),
	      double t_min, double t_max, unsigned int num_pts);

    data_file(double f1(double), double f2(double), 
	      double t_min, double t_max, unsigned int num_pts);

    data_file(double f1(double), double f2(double), double f3(double),
	      double t_min, double t_max, unsigned int num_pts);

    // input
    data_file& read(const char*);

    // transform column(s)
    data_file& transform(double f(double), unsigned int col=0);

    // apply f to selected columns; components of image go back to columns
    data_file& transform(P f(double, double),
			 unsigned int col1=1, unsigned int col2=2);

    data_file& transform(P f(double, double, double),
			 unsigned int col1, unsigned int col2);

    data_file& transform(P f(double, double, double),
			 unsigned int col1,
			 unsigned int col2,
			 unsigned int col3);

    // remove rows where specified entry is masked or outside interval/range
    data_file& prune(const data_mask&, const unsigned int col);
    data_file& prune(const interval&, const unsigned int col);
    data_file& prune(double, double, const unsigned int col);

    // basic statistical operations on columns
    double dot(unsigned int col1, unsigned int col2) const; // dot product
    double avg(unsigned int col1) const;  // mean
    double var(unsigned int col1) const;  // |x|^2 - n*avg^2
    double covar(unsigned int col1, unsigned int col2) const;
    void   regression(unsigned int col1, unsigned int col2) const;


    // output functions
    // extract column
    std::vector<double> column(unsigned int) const;
    // apply f to values
    std::vector<double> column(double f(double), unsigned int) const;

    // set C++ output precision for write
    void precision(unsigned int n=6) const;

	// Setters and getters for delimiter and comment character
	void delimiter(const char* delim) { m_delim = delim; }
	std::string delimiter() const { return m_delim; }

	void comment(const char* commt) { m_commt = commt; }
	std::string comment() const { return m_commt; }

    // write raw data to file
    void write(const char* filename) const;

    // write selected columns formatted by string-valued function
    void write(const char* filename, std::string pt_out(double, double),
	       unsigned int col1=1, unsigned int col2=2) const;

    // LaTeX tabular environment
    void tabular(const char* filename,
		 const std::string& alignment,
		 const std::string& legend="") const;


    /*
    // (un)set cropping criterion for plotting
    void select(bool Fsel(P));
    void select(const selection&); // e.g. select(all());
    */

    // scatter plots; f applied to selected columns
    void plot(epix_mark_type TYPE,
	      unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	      P f(double, double, double) = xyz) const;

    void plot(epix_mark_type TYPE, P f(double, double, double),
	      unsigned int col1=1, unsigned int col2=2,
	      unsigned int col3=0) const;

  private:
	std::vector<double> tokenise(std::string line);
	unsigned int entries(const char* filename);

    mutable unsigned int m_precision;
    // selection m_select;

    std::vector<std::vector<double> > m_data;
    std::string m_delim;		// Field delimiter
    std::string m_commt;		// Comment character
  }; // end of class data_file


  //// Global functions that use file_data, data_bins ////
  void plot(const char* filename, epix_mark_type TYPE,
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	    P f(double, double, double) = xyz);

  void plot(const char* filename, epix_mark_type TYPE,
	    P f(double, double, double),
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0);

  // pass 3rd arg by value
  void histogram(const char* filename, unsigned int col, data_bins,
		 double scale=1);
  void bar_chart(const char* filename, unsigned int col, data_bins,
		 double scale=1);
} // end of namespace

#endif /* EPIX_DATA_FILE */
