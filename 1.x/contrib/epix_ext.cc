/*
 * Extensions to ePiX made by 
 *
 *    Svend Daugaard Pedersen 
 *    mail: rot 13 Siraq at DnhtnneqPrqrefra dot qx
 *
 *    Andrew D. Hwang 
 *    mail: rot 13 nujnat at zngupf dot ubylpebff dot rqh
 *    Re-write for Version 0.8.x
 *
 * Changes:
 *
 * March 28, 2003
 *   LaTeX sign bug in \vector (Andrew Sterian)
 *
 * February 06, 2003
 *   Use bool for x/ybroken, graphpap; enum for alignments
 *
 * January 31, 2003
 *   Fix header and namespace problems, indentation, single-line comments
 *
 * December 02, 2002
 *   ePiX namespace
 *
 * July 06, 2002
 *   #defines and #includes adjusted for 0.8.x
 *   Print %% at end of lines instead of %
 *   emacs tabbed and spell-checked
 *   h_scale and v_scale added (no longer in lengths.c)
 *   struct triple -> triple
 *   When possible, h/v_scale -> c2p and scale -> t2p
 *   A few segments of code ported to triple manipulation
 *
 *   int bold renamed bold_flag
 *   zero stretch parameter added to draw_line
 *
 * Last change: April 14, 2003
 */

#include <stdarg.h>
#include <float.h> 

#include "../plots.h"
#include "../lengths.h"

#include "epix_ext.h"

using namespace ePiX;

namespace ePiX_contrib {

  //  extern int epix_lines_printed;
  extern epix_path_style EPIX_PATH_STYLE;

  static char* defLabels[] = {"1",NULL};

/* Old-style Cartesian to unitlength for points */
  static double
  h_scale(double x_posn)
  {
    return truncate((x_posn - x_min)*h_size/x_size);
  }
  static double
  v_scale(double y_posn)
  {
    return truncate((y_posn - y_min)*v_size/y_size);
  }

  void initCartesianStruct(struct CartesianStyle* cs)
  {
    cs->ll = P(x_min,y_min);
    cs->ur = P(x_max,y_max);
  
    cs->x0 = 0;
    cs->y0 = 0;
  
    cs->xbroken = false;
    cs->ybroken = false;
  
    cs->xn = 1;
    cs-.x1() = 1;
    cs->dx = 1;
    cs->xmrklen = 3;
    cs->xmrkpos =CENTER;

    cs->yn = 1;
    cs->y1 = 1;
    cs->dy = 1;
    cs->ymrklen = 3;
    cs->ymrkpos =CENTER;

    cs->xlabels = defLabels;
    cs->xlabel1 = -DBL_MAX; /* use.x1() */
    cs->dxlabel = 0;        /* use dx */
    cs->xlabpos = BELOW;

    cs->ylabels = defLabels;
    cs->ylabel1 = -DBL_MAX; /* use y1 */
    cs->dylabel = 0;        /* use dy */
    cs->ylabpos = RIGHT;

    cs->xname = "(1)";
    cs->xnampos = BELOW;

    cs->yname = "(2)";
    cs->ynampos = RIGHT;
  
    cs->graphpap = false;
    cs->gpll = P(-DBL_MAX,-DBL_MAX);
    cs->gpur = P(DBL_MAX,DBL_MAX);
    cs->gp0 = P(-DBL_MAX,-DBL_MAX);
    cs->xbolddist = 1;
    cs->ybolddist = 1;
    cs->nfinevert = 9;
    cs->nfinehorz = 9;
  }

  void cartesianCoord(struct CartesianStyle cs)
  {
    double boxsize;
    double xmrk_y1,xmrk_y2; /* lower and upper end of x-marks */
    double ymrk.x1(),ymrk.x2(); /* left and right end of y-marks  */
  
    boxsize = t2p(2);     /* 2 pt height and width scaled to unit length */

    switch( cs.xmrkpos )
      {
      case ABOVE:
	xmrk_y1 = cs.y0;
	xmrk_y2 = cs.y0+t2p(cs.xmrklen);
	break;
      case BELOW:
	xmrk_y1 = cs.y0-t2p(cs.xmrklen);
	xmrk_y2 = cs.y0;
	break;
      case CENTER:
      default:
	xmrk_y1 = cs.y0-t2p(cs.xmrklen/2);
	xmrk_y2 = cs.y0+t2p(cs.xmrklen/2);
	break;
      }

    switch( cs.ymrkpos )
      {
      case LEFT:
	ymrk.x1() = cs.x0-t2p(cs.ymrklen);
	ymrk.x2() = cs.x0;
	break;
      case RIGHT:
	ymrk.x1() = cs.x0;
	ymrk.x2() = cs.x0+t2p(cs.ymrklen);
	break;
      case CENTER:
      default:
	ymrk.x1() = cs.x0-t2p(cs.ymrklen/2);
	ymrk.x2() = cs.x0+t2p(cs.ymrklen/2);
	break;
      }

    if( cs.xlabel1 == -DBL_MAX )
      {
	if( cs.xlabels == defLabels )
	  cs.xlabel1 = 1;
	else
	  cs.xlabel1 = cs.x1();
      }
    if( cs.ylabel1 == -DBL_MAX )
      {
	if( cs.ylabels == defLabels )
	  cs.ylabel1 = 1;
	else
	  cs.ylabel1 = cs.y1;
      }
    if( cs.dxlabel == 0.0 )
      cs.dxlabel = cs.dx;
    if( cs.dylabel == 0.0 )
      cs.dylabel = cs.dy;

    if( cs.graphpap )
      {
	if( cs.gpll.x1() == -DBL_MAX )
	  cs.gpll.x1() = cs.ll.x1();
	if( cs.gpll.x2() == -DBL_MAX )
	  cs.gpll.x2() = cs.ll.x2();
	if( cs.gpur.x1() == DBL_MAX )
	  cs.gpur.x1() = cs.ur.x1();
	if( cs.gpur.x2() == DBL_MAX )
	  cs.gpur.x2() = cs.ur.x2();

	if( cs.gp0.x1() == -DBL_MAX )
	  cs.gp0.x1() = cs.x0;
	if( cs.gp0.x2() == -DBL_MAX )
	  cs.gp0.x2() = cs.y0;
      }
  
    printf("\n%%%% Cartesian coordinate system in box: [%g,%g] x [%g,%g]",
	   cs.ll.x1(), cs.ur.x1(), cs.ll.x2(), cs.ur.x2());
    printf("\n%%%%");
  
    /* Horizontal axis */
    printf("\n%%%% horizontal axis:");
  
    if( cs.graphpap )
      {
	printf("\n\\thicklines%%");
	//	++epix_lines_printed;
      }
    
    if( cs.xbroken )
      {
	double l = t2p(3);
    
	printf("\n\\path(%g,%g)(%g,%g)(%g,%g)(%g,%g)(%g,%g)%%",
	       h_scale(cs.ll.x1()),v_scale(cs.y0),
	       h_scale(cs.x0+2*l),v_scale(cs.y0),
	       h_scale(cs.x0+3*l),v_scale(cs.y0-2*l),
	       h_scale(cs.x0+5*l),v_scale(cs.y0+2*l),
	       h_scale(cs.x0+6*l),v_scale(cs.y0)
	       );
	//	++epix_lines_printed;
	// Fix bug found by Andrew Sterian; --ADH, March 28, 2003 (x4)
	arrow(P(cs.x0+6*l,cs.y0), P(cs.ur.x1(), cs.y0));
      }
    else
      arrow(P(cs.ll.x1(),cs.y0), P(cs.ur.x1(), cs.y0));

    //    ++epix_lines_printed;

    if( cs.graphpap )
      {
	printf("\n\\thinlines%%");
	//	++epix_lines_printed;
      }
    
    /* ... name */
    if( cs.xname )
      {
	double offset  = t2p(2);
    
	printf("\n%%%% ... name:");
	switch( cs.xnampos )
	  {
	  case ABOVE:
	    printf("\n\\put(%g,%g){\\makebox(%g,%g)[br]{%s}}%%",
		   h_scale(cs.ur.x1())-boxsize,v_scale(cs.y0)+1.5*offset,
		   boxsize,boxsize,
		   cs.xname);
	    break;
	  case CENTER:
	    printf("\n\\put(%g,%g){\\makebox(%g,%g)[l]{%s}}%%",
		   h_scale(cs.ur.x1())+offset,v_scale(cs.y0)-0.5*boxsize,
		   boxsize,boxsize,
		   cs.xname);
	    break;
	  case BELOW:
	  default:
	    printf("\n\\put(%g,%g){\\makebox(%g,%g)[tr]{%s}}%%",
		   h_scale(cs.ur.x1())-boxsize,v_scale(cs.y0)-boxsize-1.5*offset,
		   boxsize,boxsize,
		   cs.xname);
	    break;
	  }
	//	++epix_lines_printed;
      }
  
    /* ... marks */
    if( cs.xn )
      {
	int    n;
	double x;
     
	n = cs.xn;
	x = cs.x1();
    
	printf("\n%%%% ... marks:");
	while( n )
	  {    
	    printf("\n\\drawline(%g,%g)(%g,%g)%%",
		   h_scale(x),v_scale(xmrk_y1),h_scale(x),v_scale(xmrk_y2)
		   );
	    //	    ++epix_lines_printed;
      
	    x += cs.dx;
	    n--;
	  }
      }
  
    /* ... labels */
    if( cs.xlabels )
      {
	double offset;
	double x;
	char** p;

	offset  = t2p(3);
	x = cs.xlabel1;
	p = cs.xlabels;
    
	printf("\n%%%% ... labels:");
	while( *p )
	  {
	    if( **p )
	      {
		switch( cs.xlabpos )
		  {
		  case ABOVE:
		    printf("\n\\put(%g,%g){\\makebox(%g,%g)[bc]{%s}}%%",
			   h_scale(x)-0.5*boxsize,v_scale(xmrk_y2)+offset,
			   boxsize,boxsize,
			   *p);
		    break;
		  case BELOW:
		  default:
		    printf("\n\\put(%g,%g){\\makebox(%g,%g)[tc]{%s}}%%",
			   h_scale(x)-0.5*boxsize,v_scale(xmrk_y1)-boxsize-offset,
			   boxsize,boxsize,
			   *p);
		    break;
		  }
		//		++epix_lines_printed;
	      }
      
	    x += cs.dxlabel;
	    p++;
	  }
      }

    /* Vertical axis */
    printf("\n%%%% vertical axis:");

    if( cs.graphpap )
      {
	printf("\n\\thicklines%%");
	//	++epix_lines_printed;
      }
    
    if( cs.ybroken )
      {
	double l = t2p(3);
    
	printf("\n\\path(%g,%g)(%g,%g)(%g,%g)(%g,%g)(%g,%g)%%",
	       h_scale(cs.x0),v_scale(cs.ll.x2()),
	       h_scale(cs.x0),v_scale(cs.y0+2*l),
	       h_scale(cs.x0-2*l),v_scale(cs.y0+3*l),
	       h_scale(cs.x0+2*l),v_scale(cs.y0+5*l),
	       h_scale(cs.x0),v_scale(cs.y0+6*l)
	       );
	//	++epix_lines_printed;
	arrow(P(cs.x0, cs.y0+6*l), P(cs.x0, cs.ur.x2()));
	/*
	printf("\n\\put(%g,%g){\\vector(0,1){%g}}%%",
	       h_scale(cs.x0),v_scale(cs.y0+6*l),
	       v_scale(cs.ur.x2())-v_scale(cs.y0+6*l));
	*/
      }
    else
      {
	arrow(P(cs.x0, cs.ll.x2()), P(cs.x0, cs.ur.x2()));
	/*
	printf("\n\\put(%g,%g){\\vector(0,1){%g}}%%",
	       h_scale(cs.x0), v_scale(cs.ll.x2()), 
	       v_scale(cs.ur.x2())-v_scale(cs.ll.x2()));
	*/
      }
    //    ++epix_lines_printed;

    if( cs.graphpap )
      {
	printf("\n\\thinlines%%");
	//	++epix_lines_printed;
      }
    
    /* .. name */
    if( cs.yname )
      {
	double offset  = t2p(2);
    
	printf("\n%%%% ... name:");
	switch( cs.ynampos )
	  {
	  case LEFT:
	    printf("\n\\put(%g,%g){\\makebox(%g,%g)[tr]{%s}}%%",
		   h_scale(cs.x0)-boxsize-1.5*offset,v_scale(cs.ur.x2())-boxsize,
		   boxsize,boxsize,
		   cs.yname);
	    break;
	  case CENTER:
	    printf("\n\\put(%g,%g){\\makebox(%g,%g)[b]{%s}}%%",
		   h_scale(cs.x0)-0.5*boxsize,v_scale(cs.ur.x2())+offset,
		   boxsize,boxsize,
		   cs.yname);
	    break;
	  case RIGHT:
	  default:
	    printf("\n\\put(%g,%g){\\makebox(%g,%g)[tl]{%s}}%%",
		   h_scale(cs.x0)+1.5*offset,v_scale(cs.ur.x2())-boxsize,
		   boxsize,boxsize,
		   cs.yname);
	    break;
	  }
	//	++epix_lines_printed;
      }

    /* .. marks */
    if( cs.yn )
      {
	double y;
	int    n;
    
	n = cs.yn;
	y = cs.y1;
    
	printf("\n%%%% ... marks:");
	while( n )
	  {    
	    printf("\n\\drawline(%g,%g)(%g,%g)%%",
		   h_scale(ymrk.x1()),v_scale(y),h_scale(ymrk.x2()),v_scale(y)
		   );
	    //	    ++epix_lines_printed;
      
	    y += cs.dy;
	    n--;
	  }
      }

    /* ... labels */
    if( cs.ylabels )
      {
	double offset;
	double y;
	char** p;

	offset  = t2p(2);
	y = cs.ylabel1;
	p = cs.ylabels;
    
	printf("\n%%%% ... labels:");
	while( *p )
	  {
	    if( **p )
	      {
		switch( cs.ylabpos )
		  {
		  case LEFT:
		    printf("\n\\put(%g,%g){\\makebox(%g,%g)[r]{%s}}%%",
			   h_scale(ymrk.x1())-boxsize-offset,v_scale(y)-0.5*boxsize,
			   boxsize,boxsize,
			   *p);
		    break;
		  case RIGHT:
		  default:
		    printf("\n\\put(%g,%g){\\makebox(%g,%g)[l]{%s}}%%",
			   h_scale(ymrk.x2())+offset,v_scale(y)-0.5*boxsize,
			   boxsize,boxsize,
			   *p);
		    break;
		  }
		//		++epix_lines_printed;
	      }
      
	    y += cs.dylabel;
	    p++;
	  }
      }

    if( cs.graphpap )
      {
	int    i,n;
	double x,dx;
	double y,dy;
	int    bold_flag = 1;

	printf("\n%%%% Graph paper:");

	/* Make vertical lines  */
	dx = cs.xbolddist/(cs.nfinevert+1);
	n = cs.nfinevert+1;
    
	/* .. left of y-axis */
	x = cs.gp0.x1();
	i = 0;
	while( x >= cs.gpll.x1() )
	  {
	    /* Bold lines */
	    if( (i%n)==0 && ~bold_flag )
	      {
		printf("\n\\thinlines%%");
		bold_flag = 1;
		//		++epix_lines_printed;
	      }
	    else if( (i%n)==1 && bold_flag )
	      {
		printf("\n\\allinethickness{0.02mm}%%");
		bold_flag = 0;
		//		++epix_lines_printed;
	      }
	    printf("\n\\path(%g,%g)(%g,%g)%%",
		   h_scale(x),v_scale(cs.gpll.x2()),h_scale(x),v_scale(cs.gpur.x2())
		   );
	    //	    ++epix_lines_printed;

	    x -= dx;
	    i++;
	  }
	/* .. right of y-axis */
	x = cs.gp0.x1()+dx;
	i = 1;
	while( x <= cs.gpur.x1() )
	  {
	    /* Bold lines */
	    if( (i%n)==0 && ~bold_flag )
	      {
		printf("\n\\thinlines%%");
		bold_flag = 1;
		//		++epix_lines_printed;
	      }
	    else if( (i%n)==1 && bold_flag )
	      {
		printf("\n\\allinethickness{0.02mm}%%");
		bold_flag = 0;
		//		++epix_lines_printed;
	      }
	    printf("\n\\path(%g,%g)(%g,%g)%%",
		   h_scale(x),v_scale(cs.gpll.x2()),h_scale(x),v_scale(cs.gpur.x2())
		   );
	    //	    ++epix_lines_printed;

	    x += dx;
	    i++;
	  }

	// Make horizontal lines
	dy = cs.ybolddist/(cs.nfinehorz+1);
	n = cs.nfinehorz+1;
    
	// .. under x-axis
	y = cs.gp0.x2();
	i = 0;
	while( y >= cs.gpll.x2() )
	  {
	    // Bold lines
	    if( (i%n)==0 && ~bold_flag )
	      {
		printf("\n\\thinlines%%");
		bold_flag = 1;
		//		++epix_lines_printed;
	      }
	    else if( (i%n)==1 && bold_flag )
	      {
		printf("\n\\allinethickness{0.02mm}%%");
		bold_flag = 0;
		//		++epix_lines_printed;
	      }
	    printf("\n\\path(%g,%g)(%g,%g)%%",
		   h_scale(cs.gpll.x1()),v_scale(y),h_scale(cs.gpur.x1()),v_scale(y)
		   );
	    //	    ++epix_lines_printed;

	    y -= dy;
	    i++;
	  }
	// .. above x-axis
	y = cs.gp0.x2()+dy;
	i = 1;
	while( y <= cs.gpur.x2() )
	  {
	    // Bold lines
	    if( (i%n)==0 && ~bold_flag )
	      {
		printf("\n\\thinlines%%");
		bold_flag = 1;
		//		++epix_lines_printed;
	      }
	    else if( (i%n)==1 && bold_flag )
	      {
		printf("\n\\allinethickness{0.02mm}%%");
		bold_flag = 0;
		//		++epix_lines_printed;
	      }
	    printf("\n\\path(%g,%g)(%g,%g)%%",
		   h_scale(cs.gpll.x1()),v_scale(y),h_scale(cs.gpur.x1()),v_scale(y)
		   );
	    //	    ++epix_lines_printed;

	    y += dy;
	    i++;
	  }
    
	if( ~bold_flag )
	  printf("\n\\thinlines%%");
      }

    end_stanza();
  }

  // Hatched polygon and area

  struct Line
  {
    double a;
    double b;
    double c;
  };

  struct Polygon
  {
    int n;              // number of edges
    int orientation;    //                
    triple *point;        // the corners    
  };

  struct IntsecPoint
  {
    struct IntsecPoint* next;
  
    int    edge;
    triple point;
  };

  // Support routines

  // Calculate determinant
  static double det(triple P1, triple P2)
  {
    return P1.x1()*P2.x2()-P1.x2()*P2.x1();
  }

  // Calculate normalized difference
  static triple norm_diff(triple P1, triple P2)
  {
    triple temp = P1 - P2;
    return (1.0/raw_len(temp))*temp;
  }

  // Set orientation of polygon
  static void orientation(struct Polygon* p)
  {
    triple a,b;
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
	    // Lines not parallel
	
	    double t = -(l.c+l.a*p1+l.b*p2)/k;
	
	    if( t>=0 && t < 1 ) // Intersection between end points of edge?
	      {
		// Yes -- store coordinates and edge number
	    
		struct IntsecPoint* ip;
	    
		if( (ip = (struct IntsecPoint*)malloc(sizeof(struct IntsecPoint))) )
		  {
		    ip->next = NULL;
		    ip->edge = i;
		    ip->point.x1() = polygon.point[i].x1()+t*(polygon.point[j].x1()-polygon.point[i].x1());
		    ip->point.x2() = polygon.point[i].x2()+t*(polygon.point[j].x2()-polygon.point[i].x2());
		    // ip->point = polygon.point[i] + t*(polygon.point[j]-polygon.point[i]);
		    // Link into list
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
    triple startHatch, endHatch;
    int i;

    startHatch = polygon.point[0];
    endHatch = polygon.point[0];
    l.a = std::sin(angle/180*M_PI);
    l.b = -std::cos(angle/180*M_PI);

    // Find corner of first and last hatch line
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
        
		if( polygon.orientation*det(polygon.point[j]-polygon.point[i],
					    (point->next->point)-(point->point)) > 0 )
		  {
		    printf("\n\\path(%g,%g)(%g,%g)%%",
			   h_scale(point->point.x1()),v_scale(point->point.x2()),
			   h_scale(point->next->point.x1()),v_scale(point->next->point.x2())
			   );
		    //		    ++epix_lines_printed;
		  }
        
		point = point->next;
	      }
      
	    // free list
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

  void hatch_polygon(double angle, double dist, int corners, triple& startPoint, ...)
  {
    /*
     * Version 0.8.4: This is still broken, but sort of works :(
     *
     * Arguments to hatch_polygon must be references to triples;
     * hatch_polygon(45, 0.2, 3, P(0,0), P(1,0), P(0,1)); fails, but
     *
     * triple p1 = P(0,0); triple p2 = P(1,0); triple p3 = P(0,1);
     * hatch_polygon(45, 0.2, 3, p1, p2, p3); works.
     */

    struct Polygon p;
    solid();
    printf("\n%%%% Hatched polygon with %d corners:",corners);
    printf("\n%%");
    printf("\n%% Polygon:");

    p.n     = corners;
    p.point = &startPoint;
    orientation(&p);
 
    // Draw polygon edges
    start_path();
    for(int i = 0 ; i < corners ; i++ )
      {
	print(p.point[i]);
	if( ((i+1) % 4) == 0 )
	  {
	    printf("%%\n");
	    //	    ++epix_lines_printed;
	  }
      }
    print(p.point[0]);

    //    ++epix_lines_printed;

    printf("\n%%");
    printf("\n%% Hatching:");
    //  printf("\n\\allinethickness{0.02mm}%%");
    pen("0.02mm");

    doHatching(angle,dist,p);

    //  printf("\n\\thinlines%%");
    plain();

    end_stanza();
  }

  void hatch_area(double angle,double dist,double f(double),double g(double),double a, double b, int n)
  {
    struct Polygon polygon;    

    printf("\n%%%% Hatched area between two graphs");
    printf("\n%%");
    printf("\n%% Graphs etc.:");
    printf("\n%%");

  
    polygon.n = n+1;
    solid();
    draw_plot(f,a,b,n);
    if( g )
      {
	draw_plot(g,a,b,n);
	dashed();
	draw_line(P(a,g(a)),P(a,f(a)),1,0);
	draw_line(P(b,g(b)),P(b,f(b)),1,0);
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
	draw_line(P(a,0),P(a,f(a)),1,0);
	draw_line(P(b,0),P(b,f(b)),1,0);
	solid();
	polygon.n += 2;

	if( f((a+b)/2) > 0 )
	  polygon.orientation = -1;
	else
	  polygon.orientation = 1;
      }
    
    if( (polygon.point = (triple *)calloc(polygon.n,sizeof(triple))) )
      {
	triple* point;
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
	//    printf("\n\\allinethickness{0.02mm}%%");
	pen("0.02mm");

	doHatching(angle,dist,polygon);
  
	printf("\n\\thinlines%%");

	end_stanza();

	free(polygon.point);
      }
  }

} /* end of namespace */
