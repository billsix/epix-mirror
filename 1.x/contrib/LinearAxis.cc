/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * Implementation of LinearAxis
 *
 * Last change February 7, 2004
 *
 * June 26, 2004 (ADH)
 *
 *   h/v_size -> h/v_size()
 */

#include <cstring>
#include <cfloat>

#include "epix_ext.h"


using namespace ePiX;

namespace ePiX_contrib
{
  // Default values define
  #define DEF_MARKLENGTH    3
  #define DEF_MARKPOSITION  CENTER
  #define DEF_LABELS        {"1",NULL}
  #define DEF_LABELPOS      NEGATIVE
  #define DEF_LIN_XNAME     "(1)"
  #define DEF_LIN_YNAME     "(2)"
  #define DEF_NUMFINE       9

  static char*  defLabels[] = DEF_LABELS;

  LinAxis::LinAxis()
  {
    initLinAxis();
  }

  HorizLinAxis::HorizLinAxis()
  {
    orientation = HORIZONTAL;
  }

  void HorizLinAxis::draw(triple endpoint,double length)
  {
    if( name == NULL )
      name = DEF_LIN_XNAME;
    LinAxis::draw(endpoint,length);
  }

  VertLinAxis::VertLinAxis()
  {
    orientation = VERTICAL;
  }

  void VertLinAxis::draw(triple endpoint,double length)
  {
    if( name == NULL )
      name = DEF_LIN_YNAME;
    LinAxis::draw(endpoint,length);
  }

  void LinAxis::initLinAxis()
  {
    arrow = true;
    broken = false;
    crossPoint = -DBL_MAX;

    nummrk = 1;
    mrk1 = 1;
    distmrk = 1;
    mrklen = DEF_MARKLENGTH;
    mrkpos =DEF_MARKPOSITION;

    labels = defLabels;
    label1 = -DBL_MAX;  // use mrk1
    distlab = 0.0;      // use distmrk
    labpos = DEF_LABELPOS;

    firstline = -DBL_MAX;
    lastline = DBL_MAX;
    boldpos = -DBL_MAX;
    bolddist = 1;
    bolddist = 1;
    numfine = DEF_NUMFINE;
    midline = -1;
  }

  void LinAxis::draw(triple endpoint,double length)
  {
    double from,to;
    double boxsize;
    double mrk_start,mrk_end; // ending points of marks
    double mrk_len;           // length of mark

    boxsize = t2p(2);         // 2 pt height and width scaled to unit length

    if( orientation == HORIZONTAL )
      from = endpoint.x1();
    else
      from = endpoint.x2();
    to = from+length;

    if( crossPoint == -DBL_MAX )
      crossPoint = from;

    if( orientation == HORIZONTAL )
    {
      mrk_start = mrk_end = endpoint.x2();
      mrk_len = t2p(mrklen)*y_size/v_size();
    }
    else
    {
      mrk_start = mrk_end = endpoint.x1();
      mrk_len = t2p(mrklen)*x_size/h_size();
    }

    switch( mrkpos )
    {
      case POSITIVE:
        mrk_end += mrk_len;
        break;
      case NEGATIVE:
        mrk_start -= mrk_len;
        break;
      case CENTER:
      default:
        mrk_start -= mrk_len/2;
        mrk_end += mrk_len/2;
        break;
    }

    if( label1 == -DBL_MAX )
    {
      if( labels == defLabels )
        label1 = 1;
      else
        label1 = mrk1;
    }

    if( distlab == 0.0 )
      distlab = distmrk;

    if( boldaxis )
      lineThickness(boldAxisThickness);
    else
      lineThickness(normalAxisThickness);

    if( midline < 0 )
    {
      if( numfine == 9 )
        midline = 5;
      else
        midline = 0;
    }

    if( drawaxis )
    {
      // Draw the axis
      if( orientation == HORIZONTAL )
      {
        double x1;

        if( broken )
        {
          double l = t2p(3);

          printf("\n\\path(%g,%g)(%g,%g)(%g,%g)(%g,%g)(%g,%g)%%",
                         h_scale(from),v_scale(endpoint.x2()),
                         h_scale(crossPoint+2*l),v_scale(endpoint.x2()),
                         h_scale(crossPoint+3*l),v_scale(endpoint.x2()-2*l),
                         h_scale(crossPoint+5*l),v_scale(endpoint.x2()+2*l),
                         h_scale(crossPoint+6*l),v_scale(endpoint.x2())
                );

         x1 = crossPoint+6*l;
        }
        else
        {
         x1 = from;
        }

        if( arrow )
          printf("\n\\put(%g,%g){\\vector(1,0){%g}}%%",
              h_scale(x1),v_scale(endpoint.x2()),h_scale(to)-h_scale(x1));
        else
          printf("\n\\put(%g,%g){\\line(1,0){%g}}%%",
              h_scale(x1),v_scale(endpoint.x2()),h_scale(to)-h_scale(x1));
      }
      else // VERTICAL
      {
        double x2;

        if( broken )
        {
          double l = t2p(3);

          printf("\n\\path(%g,%g)(%g,%g)(%g,%g)(%g,%g)(%g,%g)%%",
                         h_scale(endpoint.x1()),v_scale(from),
                         h_scale(endpoint.x1()),v_scale(crossPoint+2*l),
                         h_scale(endpoint.x1()-2*l),v_scale(crossPoint+3*l),
                         h_scale(endpoint.x1()+2*l),v_scale(crossPoint+5*l),
                         h_scale(endpoint.x1()),v_scale(crossPoint+6*l)
                );

         x2 = crossPoint+6*l;
        }
        else
        {
         x2 = from;
        }

        if( arrow )
          printf("\n\\put(%g,%g){\\vector(0,1){%g}}%%",
                h_scale(endpoint.x1()),v_scale(x2),v_scale(to)-v_scale(x2));
        else
          printf("\n\\put(%g,%g){\\vector(0,1){%g}}%%",
                h_scale(endpoint.x1()),v_scale(x2),v_scale(to)-v_scale(x2));
      }
    }

    // ... name
    if( name )
    {
      double offset  = t2p(2);

      printf("\n%%%% ... name:");

      if( orientation == HORIZONTAL )
      {
        switch( nampos )
        {
          case POSITIVE:
            printf("\n\\put(%g,%g){\\makebox(%g,%g)[br]{%s}}%%",
                   h_scale(to)-boxsize,v_scale(endpoint.x2())+1.5*offset,
                   boxsize,boxsize,
                   name);
            break;
          case CENTER:
            printf("\n\\put(%g,%g){\\makebox(%g,%g)[l]{%s}}%%",
                   h_scale(to)+offset,v_scale(endpoint.x2())-0.5*boxsize,
                   boxsize,boxsize,
                   name);
            break;
          case NEGATIVE:
          default:
            printf("\n\\put(%g,%g){\\makebox(%g,%g)[tr]{%s}}%%",
                   h_scale(to)-boxsize,v_scale(endpoint.x2())-boxsize-1.5*offset,
                   boxsize,boxsize,
                   name);
            break;
        }
      }
      else // VERTICAL
      {
        switch( nampos )
        {
          case POSITIVE:
            printf("\n\\put(%g,%g){\\makebox(%g,%g)[tl]{%s}}%%",
                   h_scale(endpoint.x1())+1.5*offset,v_scale(to)-boxsize,
                   boxsize,boxsize,
                   name);
            break;
          case CENTER:
            printf("\n\\put(%g,%g){\\makebox(%g,%g)[b]{%s}}%%",
                   h_scale(endpoint.x1())-0.5*boxsize,v_scale(to)+offset,
                   boxsize,boxsize,
                   name);
            break;
          case NEGATIVE:
          default:
            printf("\n\\put(%g,%g){\\makebox(%g,%g)[tr]{%s}}%%",
                   h_scale(endpoint.x1())-boxsize-1.5*offset,v_scale(to)-boxsize,
                   boxsize,boxsize,
                   name);
            break;
        }
      }
    }

    // ... marks
    if( nummrk )
    {
      int    n;
      double t;

      lineThickness(boldGridThickness);

      n = nummrk;
      t = mrk1;

      printf("\n%%%% ... marks:");
      while( n )
      {
        if( orientation == HORIZONTAL )
        {
          printf("\n\\drawline(%g,%g)(%g,%g)%%",
                 h_scale(t),v_scale(mrk_start),h_scale(t),v_scale(mrk_end)
                );
        }
        else // VERTICAL
        {
          printf("\n\\drawline(%g,%g)(%g,%g)%%",
                 h_scale(mrk_start),v_scale(t),h_scale(mrk_end),v_scale(t)
                );
        }

        t += distmrk;
        n--;
      }
    }

    // ... labels
    if( labels )
    {
      double offset;
      double t;
      char** p;

      offset  = t2p(3);
      t = label1;
      p = labels;

      printf("\n%%%% ... labels:");
      while( *p )
      {
        if( **p )
        {
          switch( labpos )
          {
            case POSITIVE:
              if( orientation == HORIZONTAL )
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[bc]{%s}}%%",
                       h_scale(t)-0.5*boxsize,v_scale(mrk_end)+offset,
                       boxsize,boxsize,
                       *p);
              }
              else
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[l]{%s}}%%",
                       h_scale(mrk_end)+offset,v_scale(t)-0.5*boxsize,
                       boxsize,boxsize,
                       *p);
              }
              break;
            case NEGATIVE:
            default:
              if( orientation == HORIZONTAL )
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[tc]{%s}}%%",
                       h_scale(t)-0.5*boxsize,v_scale(mrk_start)-boxsize-offset,
                       boxsize,boxsize,
                       *p);
              }
              else
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[r]{%s}}%%",
                       h_scale(mrk_start)-boxsize-offset,v_scale(t)-0.5*boxsize,
                       boxsize,boxsize,
                       *p);
              }
              break;
          }
        }

        t += distlab;
        p++;
      }
    }

    if( grid )
    {
      int    i,n;
      double t,dt;

      printf("\n%%%% Grid lines:");

      n = numfine+1;
      dt = bolddist/(double)n;

      // negative part
      t = boldpos;
      i = 0;
      while( t >= firstline )
      {
        // Bold lines
        if( (i%n)==0 )
          lineThickness(boldGridThickness);
        else if( (i%n)==(n-midline) )
          lineThickness(mediumGridThickness);
        else
          lineThickness(fineGridThickness);

        if( orientation == HORIZONTAL )
        {
          printf("\n\\path(%g,%g)(%g,%g)%%",
                 h_scale(t),v_scale(linestart),h_scale(t),v_scale(lineend)
                );
        }
        else
        {
          printf("\n\\path(%g,%g)(%g,%g)%%",
                 h_scale(linestart),v_scale(t),h_scale(lineend),v_scale(t)
                );
        }

        t -= dt;
        i++;
      }
      // .. positive part
      t = boldpos+dt;
      i = 1;
      while( t <= lastline )
      {
        // Bold lines
        if( (i%n)==0 )
          lineThickness(boldGridThickness);
        else if( (i%n)==midline )
          lineThickness(mediumGridThickness);
        else
          lineThickness(fineGridThickness);

        if( orientation == HORIZONTAL )
        {
          printf("\n\\path(%g,%g)(%g,%g)%%",
                 h_scale(t),v_scale(linestart),h_scale(t),v_scale(lineend)
                );
        }
        else
        {
          printf("\n\\path(%g,%g)(%g,%g)%%",
                 h_scale(linestart),v_scale(t),h_scale(lineend),v_scale(t)
                );
        }
        t += dt;
        i++;
      }

      lineThickness(normalThickness);
    }

    end_stanza();
  }

} // end of namespace
