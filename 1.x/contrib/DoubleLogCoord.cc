/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * Implementation of DoubleLogCoord
 *
 * Last change February 7, 2004
 *
 */

#include <cstring>
#include <cfloat>

#include "epix_ext.h"


using namespace ePiX;

namespace ePiX_contrib
{
  DoubleLogCoord::DoubleLogCoord()
  {
    xAxis = &x;
    yAxis = &y;

    bounds(1.0,10,1.0,100);
  }

  DoubleLogCoord::DoubleLogCoord(double xmin,double xmax,double ymin,double ymax)
  {
    xAxis = &x;
    yAxis = &y;

    bounds(xmin,xmax,ymin,ymax);
  }

  void DoubleLogCoord::bounds(double xmin,double xmax,double ymin,double ymax)
  {
    if( xmax <=0 )
      xmax = 100;
    if( (xmin <= 0) || (xmin>=xmax) )
      xmin = xmax/100.0;
    if( ymax <=0 )
      ymax = 100;
    if( (ymin <= 0) || (ymin>=ymax) )
      ymin = ymax/100.0;

    bounding_box(P(log10(xmin),log10(ymin)),P(log10(xmax),log10(ymax)));
  }

  void DoubleLogCoord::draw()
  {
  	 if( (ll.x1()==-DBL_MAX) || (ll.x2()==-DBL_MAX) )
  	   ll = P(x_min,y_min);
  	 if( (ur.x1()==DBL_MAX) || (ur.x2()==DBL_MAX) )
  	   ur = P(x_max,y_max);

    if( crossPoint.x1() == DBL_MAX )
      crossPoint = P(ll.x1(),ll.x2());

    // Horizontal axis
    printf("\n%%%% horizontal logarithmic axis:");

    if( x.firstmrk == -DBL_MAX )
      x.firstmrk = ll.x1();
    if( x.lastmrk == DBL_MAX )
      x.lastmrk = ur.x1();

    if( x.grid )
    {
      if( x.linestart == -DBL_MAX )
        x.linestart = ll.x2();
      if( x.lineend == DBL_MAX )
        x.lineend = ur.x2();
    }

    // Vertical axis
    printf("\n%%%% vertical logarithmic axis:");

    if( y.firstmrk == -DBL_MAX )
      y.firstmrk = ll.x2();
    if( y.lastmrk == DBL_MAX )
      y.lastmrk = ur.x2();

    if( y.grid )
    {
      if( y.linestart == -DBL_MAX )
        y.linestart = ll.x1();
      if( y.lineend == DBL_MAX )
        y.lineend = ur.x1();
    }

    Coord::draw();
  }

  //
  //  Graph plotting
  //
  static double (*func)(double);

  static double doubleLogFunc(double x)
  {
    return log10(func(pow(10,x)));
  }

  void DoubleLogCoord::adplot(double f(double), double a, double b, int n)
  {
    func = f;
    ::adplot(doubleLogFunc,a,b,n);
  }

  void DoubleLogCoord::plot(double f(double), double a, double b, int n)
  {
    func = f;
    ::plot(doubleLogFunc,a,b,n);
  }

} // end of namespace
