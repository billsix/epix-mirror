/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * Implementation of SingleLogCoord
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
  SingleLogCoord::SingleLogCoord()
  {
    xAxis = &x;
    yAxis = &y;

    bounds(-1,10,1.0,100);
  }

  SingleLogCoord::SingleLogCoord(double xmin,double xmax,double ymin,double ymax)
  {
    xAxis = &x;
    yAxis = &y;

    bounds(xmin,xmax,ymin,ymax);
  }

  void SingleLogCoord::bounds(double xmin,double xmax,double ymin,double ymax)
  {
    if( ymax <=0 )
      ymax = 100;
    if( (ymin <= 0) || (ymin>=ymax) )
      ymin = ymax/100.0;

    bounding_box(P(xmin,log10(ymin)), P(xmax,log10(ymax)));
  }

  void SingleLogCoord::draw()
  {
  	 if( (ll.x1()==-DBL_MAX) || (ll.x2()==-DBL_MAX) )
  	   ll = P(x_min,y_min);
  	 if( (ur.x1()==DBL_MAX) || (ur.x2()==DBL_MAX) )
  	   ur = P(x_max,y_max);

    if( crossPoint.x1() == DBL_MAX )
      crossPoint = P(ll.x1(),ll.x2());

    // Horizontal axis
    printf("\n%%%% horizontal linear axis:");
    if( x.grid )
    {
      if( x.firstline == -DBL_MAX )
        x.firstline = ll.x1();
      if( x.lastline == DBL_MAX )
        x.lastline = ur.x1();
      if( x.linestart == -DBL_MAX )
        x.linestart = ll.x2();
      if( x.lineend == DBL_MAX )
        x.lineend = ur.x2();
      if( x.boldpos == -DBL_MAX )
        x.boldpos = crossPoint.x1();
    }

    // Vertical axis
    printf("\n%%%% vertical logarithmic axis:");

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

  static double singleLogFunc(double x)
  {
    return log10(func(x));
  }

  /*
  void SingleLogCoord::adplot(double f(double), double a, double b, int n)
  {
    func = f;
    ePiX::adplot(singleLogFunc,a,b,n);
  }
  */

  void SingleLogCoord::plot(double f(double), double a, double b, int n)
  {
    func = f;
    ePiX::plot(singleLogFunc,a,b,n);
  }

} // end of namespace
