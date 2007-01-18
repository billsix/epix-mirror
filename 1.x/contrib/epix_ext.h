//
// Header file for ePiX_ext
//
// version 2
//
// Svend Daugaard Pedersen
//
// Last Change: February 14, 2004
// 
// May 24, 2006 (ADH) -- Declare virtual destructors for base classes
// Oct 17, 2006 (ADH) -- Remove "adplot" functions
// Nov 03, 2006 (ADH) Move h_scale, v_scale to Hatch.cc
//
//   h/v_size -> h/v_size()
//

#ifndef EPIX_EXT
#define EPIX_EXT

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "epix.h"

namespace ePiX_contrib
{
  using namespace ePiX;

  // old "screen position" functions needed by several files
  double h_scale(double x_posn);
  double v_scale(double y_posn);

  extern char* normalThickness;
  extern char* normalAxisThickness;
  extern char* boldAxisThickness;
  extern char* boldGridThickness;
  extern char* mediumGridThickness;
  extern char* fineGridThickness;
  extern char* crossHatchThickness;

  enum Orientation { HORIZONTAL = 0, VERTICAL };
  enum Position    { NEGATIVE = -1, CENTER, POSITIVE };

  //
  // Hatched polygon
  //
  void hatch_polygon(double angle,double dist,int corners,triple* points);
  void hatch_polygon(double angle,double dist,int corners,triple& points, ...);

  //
  //  Hatched polygon area (no polygon edges are drawn)
  //
  void hatch(double angle,double dist,int corners,triple* points);
  void hatch(double angle,double dist,int corners,triple& points, ...);

  //
  // Hatched area between two graphs
  //
  extern void hatch_area(double angle,double dist,double f(double),double g(double),double a, double b, int n);

  //
  // Structure defining general coordinate axis
  //
  //
  class Axis
    {
    public:
  
      Axis();

      virtual void draw(triple endpoint,double length) {;}
    
      Orientation orientation; // set to either HORIZONTAL or VERTICAL
      bool        drawaxis;    // draw axis, default: TRUE
      bool        boldaxis;    // draw axis bold, default: FALSE
      bool        arrow;       // make axis with arrow, default: TRUE
      bool        grid;        // make gridlines, default: FALSE

      // Grid lines (perpendicular to axis)
      double linestart; // start of line, default from ll
      double lineend;   // end of line, default from ur
  
      // Name of  axis
      char*    name;    // name of axis, default: "(1)"/"(2)"
      Position nampos;  // position (POSITIVE,CENTER,NEGATIVE), default: NEGATIVE

      // Tick marks and labels
      double   mrklen;  // length of marks (measured in pt), default: 3pt
      Position mrkpos;  // position (POSITIVE,CENTER,NEGATIVE), default: CENTER
      Position labpos;  // label position (POSITIVE,NEGATIVE), default: NEGATIVE

    protected:
      virtual ~Axis(void) { }
      void lineThickness(char* thicknessStr);
     
      char* currentThickness;
    };

  //
  // Structure defining linear coordinate axis
  //
  //
  class LinAxis:public Axis
    {
    public:
  
      LinAxis();
     
      void draw(triple endpoint,double length);
    
      bool   broken;     // broken axis, default: FALSE
      double crossPoint; // cross point for axis in coordinate system
  
      // Tick marks on axis
      int      nummrk;  // number of division marks on axis, default: 1
      double   mrk1 ;   // position of first division mark, default: 1
      double   distmrk; // dist between marks, default: 1 (ignored if nummrk<2)

      // Labels on axis
      char** labels;    // labels on axis, default: {"1",NULL}
      double label1;    // where to place first label,
      //   default: 1 if default label is used, otherwise mrk1
      double distlab;   // distance between labels, default: distmrk

      // Grid lines (perpendicular to axis)
      double firstline; // position of first grid line, default from coord. ll
      double lastline;  // position of last grid line, default from coord. ur
      double boldpos;   // position of bold line, default: axis crossing point
      double bolddist;  // distance between bold lines, default: 1
      int    numfine;   // no of fine lines between bold, default: 9
      int    midline;   // line to be half bold, default: 
      //   5 if default numfine, otherwise 0 (no half line)
     
    protected:
      void initLinAxis();
    };

  class HorizLinAxis:public LinAxis
    {
    public:
      HorizLinAxis();
     
      void draw(triple endpoint,double length);
    };

  class VertLinAxis:public LinAxis
    {
    public:
      VertLinAxis();
     
      void draw(triple endpoint,double length);
    };


  //
  //  Structures defining logarithmic axis
  //
  //
  class Mark
    {
    public:
      double position; // position of main mark
      char*  label;    // label to print at main mark
      int    numsub;   // number of submarks until next main mark
    };

  class LogAxis:public Axis
    {
    public:
  
      LogAxis();
     
      void draw(triple endpoint,double length);
    
      double arrowextra;// extra axis length if arrow (in pt), default: 0
      int    stdstyle;  // standard style number, 0 = custom style, default: 4

      // Tick marks and labels
      Mark*    labelmrk;// position and label information of main marks
      double   firstmrk;// position of first mark, default from coord-sys ll
      double   lastmrk; // position of last mark, default from coord-sys ur
      char*    labattr; // attribute used to print labels, default "\\scriptsize"

    protected:
      void initLogAxis();
    };

  class HorizLogAxis:public LogAxis
    {
    public:
      HorizLogAxis();
     
      void draw(triple endpoint,double length);
    };

  class VertLogAxis:public LogAxis
    {
    public:
      VertLogAxis();
     
      void draw(triple endpoint,double length);
    };

  //
  // Coordinate system structures
  //
  //
  class Coord
    {
    public:
         
      Coord();
      
      virtual void bounds(double xmin,double xmax,double ymin,double ymax) {;}
      virtual void draw();


      void showAxis(bool b) {xAxis->drawaxis=b;yAxis->drawaxis=b;}
      void showAxis(bool bx,bool by) {xAxis->drawaxis=bx;yAxis->drawaxis=by;}
      void showArrow(bool b)
	{xAxis->arrow=b;yAxis->arrow=b;}
      void showArrow(bool xb, bool yb)
	{xAxis->arrow=xb;yAxis->arrow=yb;}
      void xName(Position pos)
	{xAxis->nampos=pos;}
      void xName(char* name,Position pos)
	{xAxis->name=name;xAxis->nampos=pos;}
      void yName(Position pos)
	{yAxis->nampos=pos;}
      void yName(char* name,Position pos)
	{yAxis->name=name;yAxis->nampos=pos;}
      void labelPos(Position pos)
	{xAxis->labpos=yAxis->labpos=pos;}
      void labelPos(Position xpos,Position ypos)
	{xAxis->labpos=xpos;yAxis->labpos=ypos;}
      void markLayout(double length,Position pos=CENTER)
	{xAxis->mrklen=yAxis->mrklen=length;xAxis->mrkpos=yAxis->mrkpos=pos;}
      void markLayout(double lx,Position px,double ly,Position py)
	{xAxis->mrklen=lx;yAxis->mrklen=ly;xAxis->mrkpos=px;yAxis->mrkpos=py;}
      void boldAxis(bool b=true)
	{xAxis->boldaxis=b;yAxis->boldaxis=b;}
      void boldAxis(bool bx,bool by)
	{xAxis->boldaxis=bx;yAxis->boldaxis=by;}

      // Plotting routines
      // virtual void adplot(double f(double), double, double, int) {;}
      virtual void plot(double f(double), double, double, int) {;}

      // LaTeX labels
      virtual void label(triple base,triple offset,char *label_text) {;}
      virtual void label(triple base,char *label_text) {;}
      virtual void label(triple base,triple offset,char *label_text,epix_label_posn) {;}
          
      // Mark
      virtual void marker(triple arg,epix_mark_type mark) {;}

      triple ll;          // lower left corner  (default is (x_min,y_min))
      triple ur;          // upper right corner (default is (x_max,y_max))

      triple crossPoint;  // axis crossing point, 
      // default: (0,0) for cartesian
      //          (ll.x1(),ll.x2()) for single log
      //          (ll.x1(),ll.x2()) for double log

    protected:
      virtual ~Coord(void) { }
      Axis* xAxis;
      Axis* yAxis;
    };

  class CartesianCoord:public Coord
    {
    public:

      CartesianCoord();
      CartesianCoord(double xmin,double xmax,double ymin,double ymax);
          
      void bounds(double xmin,double xmax,double ymin,double ymax)
	{bounding_box(P(xmin,ymin), P(xmax,ymax));}

      void draw();

      void axisCross(triple point)
	{crossPoint=point;}
      void axisBounds(triple upperRight)
	{ur=upperRight;}
      void axisBounds(triple lowerLeft,triple upperRight)
	{ll=lowerLeft;ur=upperRight;}
      void xLabels(char** lab)
	{x.labels=lab;}
      void xLabels(char** lab,double first)
	{x.labels=lab;x.label1=first;}
      void xLabels(char** lab,double first,double dist)
	{x.labels=lab;x.label1=first;x.distlab=dist;}
      void yLabels(char** lab)
	{y.labels=lab;}
      void yLabels(char** lab,double first)
	{y.labels=lab;y.label1=first;}
      void yLabels(char** lab,double first,double dist)
	{y.labels=lab;y.label1=first;y.distlab=dist;}

      void xMarks(double first,int num,double dist=1.0)
	{x.mrk1=first,x.nummrk=num;x.distmrk=dist;}
      void yMarks(double first,int num,double dist=1.0)
	{y.mrk1=first,y.nummrk=num;y.distmrk=dist;}

      void xBroken()
	{x.broken=true;}
      void yBroken()
	{y.broken=true;}

      void grid()
	{x.grid=y.boldaxis=true; y.grid=x.boldaxis=true;}
      void grid(double xmin,double xmax,double ymin,double ymax)
	{
	  x.grid=y.boldaxis=true; y.grid=x.boldaxis=true;
	  x.firstline=y.linestart=xmin;x.lastline=y.lineend=xmax;
	  y.firstline=x.linestart=ymin;y.lastline=x.lineend=ymax;
	}
      void gridPosition(triple boldCross)
	{x.boldpos=boldCross.x1();y.boldpos=boldCross.x2();}
      void gridDensity(double dist,int num=9,int mid=0)
	{x.bolddist=y.bolddist=dist;x.numfine=y.numfine=num;x.midline=y.midline=mid;}
      void gridDensity(double xd,int xn,int xm,double yd,int yn,int ym)
	{x.bolddist=xd;y.bolddist=yd;x.numfine=xn;y.numfine=yn;x.midline=xm;y.midline=ym;}


      // Plotting routines
      // void adplot(double f(double),double a,double b,int n)
      // {ePiX::adplot(f,a,b,n);}
      void plot(double f(double),double a,double b,int n)
	{ePiX::plot(f,a,b,n);}
     
      // Hatched area 
      void hatchArea(double angle,double dist,double f(double),double a,double b, int n)
	{hatchArea(angle,dist,f,NULL,a,b,n);}
      void hatchArea(double angle,double dist,double f(double),double g(double),double a, double b, int n)
	{hatch_area(angle,dist,f,g,a,b,n);}

      // LaTeX labels
      void label(triple base,triple offset,char *label_text)
	{ePiX::label(base,offset,label_text);}
      void label(triple base,char *label_text)
	{ePiX::label(base,label_text);}
      void label(triple base,triple offset,char *label_text,epix_label_posn pos)
	{ePiX::label(base,offset,label_text,pos);}
  
      // Mark
      void marker(triple arg,epix_mark_type mark)
	{ePiX::marker(arg,mark);}

      HorizLinAxis x;
      VertLinAxis  y;
    };

  class SingleLogCoord:public Coord
    {
    public:
         
      SingleLogCoord();
      SingleLogCoord(double xmin,double xmax,double ymin,double ymax);
          
      void bounds(double xmin,double xmax,double ymin,double ymax);
      void draw();

      void axisCross(triple point)
	{crossPoint=P(point.x1(),log10(point.x2()));}
      void axisBounds(triple upperRight)
	{ur=P(upperRight.x1(),log10(upperRight.x2()));}
      void axisBounds(triple lowerLeft,triple upperRight)
	{ll=P(lowerLeft.x1(),log10(lowerLeft.x2()));
	ur=P(upperRight.x1(),log10(upperRight.x2()));}
      void xLabels(char** lab)
	{x.labels=lab;}
      void xLabels(char** lab,double first)
	{x.labels=lab;x.label1=first;}
      void xLabels(char** lab,double first,double dist)
	{x.labels=lab;x.label1=first;x.distlab=dist;}

      void showArrow(bool b)
	{x.arrow=b;}
      void showArrow(double extra)
	{y.arrow=true;y.arrowextra=extra;}
      void showArrow(bool b,double extra)
	{x.arrow=b;y.arrow=true;y.arrowextra=extra;}
      void xMarks(double first,int num,double dist=1.0)
	{x.mrk1=first,x.nummrk=num;x.distmrk=dist;}
      void yMarks(double first,double last)
	{y.firstmrk=first;y.lastmrk=last;}
      void xBroken() {x.broken=true;}
     
      void logStyle(int style)
	{y.stdstyle=style;}
      void logStyle(Mark* mrk)
	{y.stdstyle=0;y.labelmrk=mrk;}
      void labelAttr(char* attr)
	{y.labattr=attr;}

      void grid()
	{x.grid=y.boldaxis=true; y.grid=x.boldaxis=true;}
      void grid(double xmin,double xmax)
	{ 
	  x.grid=y.boldaxis=true; y.grid=x.boldaxis=true;
	  x.firstline=y.linestart=xmin;x.lastline=y.lineend=xmax;
	}
      void gridPosition(double crossX)
	{x.boldpos=crossX;}
      void gridDensity(double dist,int num=9,int mid=0)
	{x.bolddist=dist;x.numfine=num;x.midline=mid;}

      // Plotting routines
      // void adplot(double f(double), double, double, int);
      void plot(double f(double), double, double, int);

      // Labels
      void label(triple base,triple offset,char *label_text)
	{ePiX::label(P(base.x1(),log10(base.x2())),offset,label_text);}
      void label(triple base,char *label_text)
	{ePiX::label(P(base.x1(),log10(base.x2())),label_text);}
      void label(triple base,triple offset,char *label_text,epix_label_posn pos)
	{ePiX::label(P(base.x1(),log10(base.x2())),offset,label_text,pos);}

      // Marker
      void marker(triple arg,epix_mark_type mark)
	{ePiX::marker(P(arg.x1(),log10(arg.x2())),mark);}

      HorizLinAxis x;
      VertLogAxis  y;
    };

  class DoubleLogCoord:public Coord
    {
    public:
         
      DoubleLogCoord();
      DoubleLogCoord(double xmin,double xmax,double ymin,double ymax);
          
      void bounds(double xmin,double xmax,double ymin,double ymax);
      void draw();

      void axisCross(triple point)
	{crossPoint=P(log10(point.x1()),log10(point.x2()));}
      void axisBounds(triple upperRight)
	{ur=P(log10(upperRight.x1()),log10(upperRight.x2()));}
      void axisBounds(triple lowerLeft,triple upperRight)
	{ll=P(log10(lowerLeft.x1()),log10(lowerLeft.x2()));
	ur=P(log10(upperRight.x1()),log10(upperRight.x2()));}
      void showArrow(double extra)
	{x.arrow=true;x.arrowextra=extra;y.arrow=true;y.arrowextra=extra;}
      void showArrow(bool bx,double extrax,bool by,double extray)
	{x.arrow=bx;x.arrowextra=extrax;y.arrow=by;y.arrowextra=extray;}
      void xMarks(double first,double last)
	{x.firstmrk=first;x.lastmrk=last;}
      void yMarks(double first,double last)
	{y.firstmrk=first;y.lastmrk=last;}
     
      void logStyle(int style)
	{x.stdstyle=y.stdstyle=style;}
      void logStyle(Mark* mrk)
	{x.stdstyle=y.stdstyle=0;x.labelmrk=y.labelmrk=mrk;}
      void logStyle(int stylex,int styley)
	{x.stdstyle=stylex;y.stdstyle=styley;}
      void logStyle(Mark* mrkx,Mark* mrky)
	{x.stdstyle=y.stdstyle=0;x.labelmrk=mrkx;y.labelmrk=mrky;}
      void logStyle(int stylex,Mark* mrky)
	{x.stdstyle=stylex;y.stdstyle=0;y.labelmrk=mrky;}
      void logStyle(Mark* mrkx,int styley)
	{x.stdstyle=0;y.stdstyle=styley;x.labelmrk=mrkx;}

      void labelAttr(char* attr)
	{x.labattr=y.labattr=attr;}
      void labelAttr(char* attrx,char* attry)
	{x.labattr=attrx;y.labattr=attry;}

      void grid()
	{x.grid=y.boldaxis=true; y.grid=x.boldaxis=true;}

      // Plotting routines
      // void adplot(double f(double), double, double, int);
      void plot(double f(double), double, double, int);

      // Labels
      void label(triple base,triple offset,char *label_text)
	{ePiX::label(P(log10(base.x1()),log10(base.x2())),offset,label_text);}
      void label(triple base,char *label_text)
	{ePiX::label(P(log10(base.x1()),log10(base.x2())),label_text);}
      void label(triple base,triple offset,char *label_text,epix_label_posn pos)
	{ePiX::label(P(log10(base.x1()),log10(base.x2())),offset,label_text,pos);}
    
      // Marker
      void marker(triple arg,epix_mark_type mark)
	{ePiX::marker(P(log10(arg.x1()),log10(arg.x2())),mark);}

      HorizLogAxis x;
      VertLogAxis  y;
    };

  //
  // General picture command
  //
  // Example of use (of var-args version):
  //
  //   picCommand("\\spline",5,P.x1(),y1),P.x2(),y2),P(x3,y3),P(x4,y4),P(x5,y5));
  //   picCommand("\\dottedline{2}",5,P.x1(),y1),P.x2(),y2),P(x3,y3),P(x4,y4),P(x5,y5));
  //
  extern void picCommand(char* cmd,int numPnts,triple* poins);
  extern void picCommand(char* cmd,int numPnts,triple& point,...);

} // end of ePiX_contrib namespace

#endif // EPIX_EXT
