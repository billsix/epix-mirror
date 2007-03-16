/* 
 *  dataplot.h -- ePiX::P data plotting and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: February 11, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
#ifndef EPIX_DATAPLOT
#define EPIX_DATAPLOT

#include <vector>
#include <list>
#include "globals.h"
#include "functions.h"

namespace ePiX {

  class d_bin;

  /*
  // wrapper for bool-valued function of P
  class selection {
  public:
    selection();
    selection(bool F(P));

    bool operator() (const P&) const;

  private:
    bool (*m_selector)(P);

  }; // end of class selection

  selection& all();
  */

  // safer, more featureful replacement for "FILEDATA"
  class data_file {
  public:
    explicit data_file(unsigned int n=2);
    explicit data_file(const char*, const char* delim = " ",
    	const char* commt = "%"); // get size from first line of file

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
	std::string delimiter() { return m_delim; }
	void comment(const char* commt) { m_commt = commt; }
	std::string comment() { return m_commt; }

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


  class data_bins {
  public:
    data_bins(const double lo, const double hi, unsigned int n=1);

    data_bins(const data_bins&);
    data_bins& operator= (const data_bins&);
    ~data_bins();

    data_bins&  cut(const double); // add a cut
    data_bins& read(const std::vector<double>&);

    unsigned int pop() const; // current population

    void histogram(const double scale=1) const; // draw rectangles
    void plot(const double scale=1) const;  // smooth interpolation

  private:
    double m_lo_val;
    double m_hi_val;

    unsigned int m_lo_ct; // low count
    unsigned int m_hi_ct; // high count
    unsigned int m_pop; // current population

    bool m_cuts_locked; // true once we read data

    std::list<double> m_cuts;
    std::vector<d_bin*> m_bins;

    // bookkeeping
    void initialize();         // convert cuts to bins and lock
    void insert(const double); // add data point
  }; // end of class data_bins


  //// Global functions that use file_data, data_bins ////
  void plot(const char* filename, epix_mark_type TYPE,
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	    P f(double, double, double) = xyz);

  void plot(const char* filename, epix_mark_type TYPE,
	    P f(double, double, double),
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0);

  // pass 3rd arg by value
  void histogram(const char* filename, unsigned int col, data_bins);


  ///////////////////////////////////////////////////
  //// OBSOLETE FUNCTION CALLS -- may be removed //// 
  ///////////////////////////////////////////////////
  typedef std::vector<std::vector<double> > FILEDATA;

  // read columns of file into FILEDATA
  void read(const char*, FILEDATA&);

  // plot specified columns
  // Primarily for raw data
  void plot(const FILEDATA& data_columns, epix_mark_type TYPE,
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	    P f(double, double, double) = xyz);

  void plot(const char* filename, epix_mark_type TYPE, int columns, 
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0,
	    P f(double, double, double) = xyz);


  // Primarily for log plots
  void plot(const char* filename, epix_mark_type TYPE, int columns, 
	    P f(double, double, double),
	    unsigned int col1=1, unsigned int col2=2, unsigned int col3=0);


  // deprecated placeholder
  void data_plot(const char* filename, epix_mark_type TYPE);

  // functions for data analysis
  double avg(FILEDATA data, unsigned int col1);
  double var(FILEDATA data, unsigned int col1);
  double covar(FILEDATA data, unsigned int col1, unsigned int col2);

  void regression(FILEDATA data, unsigned int col1=1, unsigned int col2=2);

  // Histogram plot of designated column, in the specified interval with
  // equal-length data subdivisions. By default the total area is unity;
  // v_scale adjusts height.
  void histogram(const FILEDATA& data_columns, unsigned int column, 
		 int subdivs=(int)x_size, 
		 double low=x_min, double high=x_max, double v_scale=1);

  void histogram(const char* filename, int columns, unsigned int column, 
		 int subdivs=(int)x_size, 
		 double low=x_min, double high=x_max, double v_scale=1);

} // end of namespace

#endif /* EPIX_DATAPLOT */
