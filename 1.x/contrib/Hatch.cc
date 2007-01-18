/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * Implementation of hatching routines
 *
 * Last change February 7, 2004
 *
 * June 26, 2004 (ADH)
 *   - triple -> P 
 *   - draw_line -> line
 *   - draw_plot -> plot
 */


/***********************************************************************
*
*  Hatched polygon and area
*
************************************************************************/

#include <cstring>
#include <cfloat>

#include "epix_ext.h"

using namespace ePiX;

namespace ePiX_contrib
{
  // declared in epix_ext.h
  double h_scale(double x_posn)
  {
    double t((x_posn - x_min)*h_size()/x_size);
    return (fabs(t) < EPIX_EPSILON) ? 0 : t;
  }
  double v_scale(double y_posn)
  {
    double t((y_posn - y_min)*v_size()/y_size);
    return (fabs(t) < EPIX_EPSILON) ? 0 : t;
  }

  static void lineThickness(char* thicknessStr);

  struct Line
  {
    double a;
    double b;
    double c;
  };

  struct Polygon
  {
    int     n;           /* number of edges */
    int     orientation; /*                 */
    P* point;       /* the corners     */
  };

  struct IntsecPoint
  {
    IntsecPoint* next;

    int    edge;
    P point;
  };

  /*
  *  Support routines
  *
  */

  /*
  *  Calculate determinant
  */
  static double det(struct P P1, struct P P2)
  {
    return P1.x1()*P2.x2()-P1.x2()*P2.x1();
  }

  /*
  *  Calculate normalised difference
  */
  static struct P norm_diff(struct P P1, struct P P2)
  {
    P temp = P1 - P2;
    return (1.0/norm(temp))*temp;
  }

/*
*  Set orientation of polygon
*/
static void orientation(struct Polygon* p)
{
  struct P a,b;
  double v = 0;
  int i;

  b = norm_diff(p->point[0],p->point[p->n-1]);
  for( i = 0 ; i < p->n ; i++ )
  {
    a = b;
    b = norm_diff(p->point[(i+1) % p->n],p->point[i]);
    if( det(a,b) < 0 )
      v -= std::acos(a.x1()*b.x1()+a.x2()*b.x2());
    else
      v += std::acos(a.x1()*b.x1()+a.x2()*b.x2());
  }

  p->orientation = (v<0?-1:1);
}

/*
*  Find all intersections between hatch line and polygon edges
*
*  return as linked list of IntsecPoint (ordered)
*
*/
static struct IntsecPoint* intersections(struct Line l, struct Polygon polygon)
{
  int i;
  struct IntsecPoint* start = NULL;

  for( i = 0 ; i < polygon.n ; i++ )
  {
    /*
     *  An edge between points P(p1,p2) and Q(q1,q2) is described as
     *
     *   .x1() = p1 + t*(q1-p1)
     *   .x2() = p2 + t*(q2-p2)
     */

     double p1,p2,q1,q2;
     double k;
     int j = (i+1) % polygon.n;

     p1 = polygon.point[i].x1();
     p2 = polygon.point[i].x2();
     q1 = polygon.point[j].x1();
     q2 = polygon.point[j].x2();

     if( (k = l.a*(q1-p1)+l.b*(q2-p2)) )
     {
       /* Lines not parallel */

       double t = -(l.c+l.a*p1+l.b*p2)/k;

       if( t>=0 && t < 1 ) /* Intersection between end points of edge? */
       {
         /* Yes -- store coordinates and edge number */

         struct IntsecPoint* ip;

         if( (ip = (struct IntsecPoint*)malloc(sizeof(struct IntsecPoint))) )
         {
           ip->next = NULL;
           ip->edge = i;
	   /*
           ip->point.x1() = polygon.point[i].x1()+t*(polygon.point[j].x1()-polygon.point[i].x1());
           ip->point.x2() = polygon.point[i].x2()+t*(polygon.point[j].x2()-polygon.point[i].x2());
	   */
           ip->point = polygon.point[i]+t*(polygon.point[j]-polygon.point[i]);

           /* Link into list */
           {
             struct IntsecPoint **next;

             next = &start;
             while( *next && (det(ip->point,P(l.a,l.b)) < det((*next)->point,P(l.a,l.b))) )
               next = &((*next)->next);
             ip->next = *next;
             *next = ip;
           }
         }
       }
     }

  }

  return start;
}

static void doHatching(double angle, double dist, struct Polygon polygon)
{
  /*
  *  Each hatch line is described by a line of the form .x1()+.x2()+c=0
  *  with
  *         a = sin(v)
  *         b = -cos(v)
  *         c = -a*r1 -b*r2
  *
  *  where v = angle*pi/180 and (r1,r2) is a point on the line.
  */

  struct Line l;
  struct P startHatch, endHatch;
  int i;

  startHatch = polygon.point[0];
  endHatch = polygon.point[0];
  l.a = std::sin(angle/180*M_PI);
  l.b = -std::cos(angle/180*M_PI);

  /*
  *  Find corner of first and last hatch line
  */
  for( i = 0 ; i < polygon.n ; i++ )
  {
    if( l.a*polygon.point[i].x1()+l.b*polygon.point[i].x2() < l.a*startHatch.x1()+l.b*startHatch.x2() )
      startHatch = polygon.point[i];
    if( l.a*polygon.point[i].x1()+l.b*polygon.point[i].x2() > l.a*endHatch.x1()+l.b*endHatch.x2() )
      endHatch = polygon.point[i];
  }

  l.c = -l.a*startHatch.x1()-l.b*startHatch.x2() - dist;
  while( l.a*endHatch.x1()+l.b*endHatch.x2()+l.c > 0 )
  {
    struct IntsecPoint* ip;

    if( (ip = intersections(l,polygon)) )
    {
      struct IntsecPoint* point = ip;

      while( point && point->next )
      {
        int i = point->edge;
        int j = (i+1) % polygon.n;

        if( polygon.orientation*det(polygon.point[j]-polygon.point[i],point->next->point-point->point) > 0 )
        {
          printf("\n\\path(%g,%g)(%g,%g)%%",
                 h_scale(point->point.x1()),v_scale(point->point.x2()),
                 h_scale(point->next->point.x1()),v_scale(point->next->point.x2())
                );
        }

        point = point->next;
      }

      /* free list */
      while( ip )
      {
        struct IntsecPoint* next = ip->next;

        free(ip);
        ip = next;
      }
    }

    l.c = l.c - dist;
  }
}

  void hatch_polygon(double angle, double dist, int corners, P& startPoint, ...)
  {
    hatch_polygon(angle,dist,corners,&startPoint);
  }

  void hatch_polygon(double angle, double dist, int corners, P* points)
  {
    Polygon p;
    int i;

    printf("\n%%%% Hatched polygon with %d corners:",corners);
    printf("\n%%");
    printf("\n%% Polygon:");

    p.n     = corners;
    p.point = points;
    orientation(&p);

    /*
    *  Draw polygon edges
    */
    start_path();
    for( i = 0 ; i < corners ; i++ )
    {
      printf("(%g,%g)",h_scale(p.point[i].x1()), v_scale(p.point[i].x2()));
      if( ((i+1) % 4) == 0 )
      {
        printf("%%\n");
      }
    }
    printf("(%g,%g)%%",h_scale(p.point[0].x1()), v_scale(p.point[0].x2()));

    printf("\n%%");
    printf("\n%% Hatching:");

    lineThickness(crossHatchThickness);

    doHatching(angle,dist,p);

    lineThickness(normalThickness);

    end_stanza();
  }

  void hatch(double angle,double dist,int corners,P& startPoint, ...)
  {
    hatch(angle,dist,corners,&startPoint);
  }

  void hatch(double angle,double dist,int corners,P* points)
  {
    Polygon p;

    printf("\n%%%% Hatced polygon area with %d corners:",corners);

    p.n     = corners;
    p.point = points;
    orientation(&p);

    printf("\n%%");
    printf("\n%% Hatching:");

    lineThickness(crossHatchThickness);

    doHatching(angle,dist,p);

    lineThickness(normalThickness);

    end_stanza();
  }


  void hatch_area(double angle,double dist,double f(double),double g(double),double a, double b, int n)
  {
    Polygon polygon;

    printf("\n%%%% Hatched area between two graphs");
    printf("\n%%");
    printf("\n%% Graphs etc.:");
    printf("\n%%");

    polygon.n = n+1;

    solid();
    plot(f,a,b,n);
    if( g )
    {

      plot(g,a,b,n);
      dashed();
      line(P(a,g(a)),P(a,f(a)),1,0);
      line(P(b,g(b)),P(b,f(b)),1,0);
      solid();
      polygon.n += (n+1);

      if( f((a+b)/2) > g((a+b)/2) )
        polygon.orientation = -1;
      else
        polygon.orientation = 1;
    }
    else
    {

      dashed();
      line(P(a,0),P(a,f(a)),1,0);
      line(P(b,0),P(b,f(b)),1,0);
      solid();
      polygon.n += 2;

      if( f((a+b)/2) > 0 )
        polygon.orientation = -1;
      else
        polygon.orientation = 1;
    }

    if( (polygon.point = (struct P *)calloc(polygon.n,sizeof(struct P))) )
    {
      struct P* point;
      double x,dx;
      int i;

      dx = (b-a)/n;
      x = a;
      point = polygon.point;

      for( i = 0 ; i < n ; i++ )
      {
        *(point++) = P(x,f(x));
        x += dx;
      }
      *(point++) = P(b,f(b));

      x = b;
      if( g )
      {
        for( i = 0 ; i < n ; i++ )
        {
          *(point++) = P(x,g(x));
          x -= dx;
        }
        *point = P(a,g(a));
      }
      else
      {
        *(point++) = P(b,0);

        *point = P(a,0);
      }

      printf("\n%%");
      printf("\n%% Hatching:");

      lineThickness(crossHatchThickness);

      doHatching(angle,dist,polygon);

      lineThickness(normalThickness);

      end_stanza();

      free(polygon.point);
    }

}

  static char* currentThickness = "";

  static void lineThickness(char* thicknessStr)
  {
    if( strcmp(thicknessStr,currentThickness) )
    {
      printf("\n%s%%",thicknessStr);
      currentThickness = thicknessStr;
    }
  }
} // end of namespace
