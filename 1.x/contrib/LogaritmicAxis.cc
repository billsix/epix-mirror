/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: Svend@DaugaardPedersen.dk
 *
 * Implementation of LogarithmicAxis
 *
 * Last change February 7, 2004
 *
 * June 26, 2004 (ADH)
 *
 *    h/v_size -> h/v_size()
 */

#include <cstring>
#include <cfloat>

#include "epix_ext.h"

#define smallValue (1000*DBL_EPSILON)

using namespace ePiX;

namespace ePiX_contrib
{
  // Default values define
  #define DEF_MARKLENGTH    3
  #define DEF_MARKPOSITION  CENTER
  #define DEF_LABELPOS      NEGATIVE
  #define DEF_LOG_XNAME     NULL
  #define DEF_LOG_YNAME     NULL
  #define DEF_NUMFINE       9
  #define DEF_STDSTYLE      4
  #define DEF_LABELATTR     "\\scriptsize"

  LogAxis::LogAxis()
  {
    initLogAxis();
  }

  HorizLogAxis::HorizLogAxis()
  {
    orientation = HORIZONTAL;
  }

  void HorizLogAxis::draw(triple endpoint,double length)
  {
    if( name == NULL )
      name = DEF_LOG_XNAME;
    LogAxis::draw(endpoint,length);
  }

  VertLogAxis::VertLogAxis()
  {
    orientation = VERTICAL;
  }

  void VertLogAxis::draw(triple endpoint,double length)
  {
    if( name == NULL )
      name = DEF_LOG_YNAME;
    LogAxis::draw(endpoint,length);
  }

  void LogAxis::initLogAxis()
  {

    arrow = false;

    arrowextra = 0;

    stdstyle = DEF_STDSTYLE;

    labelmrk = (Mark*)(-1L);
    firstmrk = -DBL_MAX;
    lastmrk = DBL_MAX;
    mrklen = DEF_MARKLENGTH;
    mrkpos =DEF_MARKPOSITION;
    labpos = DEF_LABELPOS;
    labattr = NULL;          // Force DEF_LABELATTR to be used
  }

  //
  // Styles of logarithmic axis
  //
  static Mark mrk1[] =
  {
    {1,NULL,0},
    {10,NULL,0}
  };

  static Mark mrk2[] =
  {
    {1,NULL,9},
    {10,NULL,0}
  };

  static Mark mrk3[] =
  {
    {1,NULL,1},
    {2,"2",0},
    {3,"3",1},
    {5,"5",4},
    {10,NULL,0}
  };

  static Mark mrk4[] =
  {
    {1,NULL,3},
    {2,"2",1},
    {3,"3",1},
    {5,"5",4},
    {10,NULL,0}
  };

  static Mark mrk5[] =
  {
    {1,NULL,3},
    {2,"2",1},
    {3,"3",1},
    {4,"4",1},
    {5,"5",0},
    {6,"6",0},
    {7,"7",0},
    {8,"8",0},
    {9,"9",0},
    {10,NULL}
  };

  static Mark mrk6[] =
  {
    {1,NULL,9},
    {2,"2",9},
    {3,"3",9},
    {4,"4",9},
    {5,"5",4},
    {6,"6",4},
    {7,"7",4},
    {8,"8",4},
    {9,"9",4},
    {10,NULL,0}
  };

  void LogAxis::draw(triple endpoint,double length)
  {
    double from,to;
    double boxsize;
    double mrk_start,mrk_end;   // ending points of long marks
    double mrk_start0,mrk_end0; // ending points of short marks
    double mrk_len;             // full length of mark/
    double arrow_extra;

    boxsize = t2p(2);   // 2 pt height and width scaled to unit length

    if( orientation == HORIZONTAL )
      from = endpoint.x1();
    else
      from = endpoint.x2();
    to = from+length;

    if( firstmrk == -DBL_MAX )
      firstmrk = from;
    if( lastmrk == DBL_MAX )
      lastmrk = to;

    if( labelmrk == (Mark*)(-1L) )
    {
      switch( stdstyle )
      {
        case 1:
          labelmrk = mrk1;
          break;
        case 2:
          labelmrk = mrk2;
          break;
        case 3:
          labelmrk = mrk3;
          break;
        case 4:
          labelmrk = mrk4;
          break;
        case 5:
          labelmrk = mrk5;
          break;
        case 6:
          labelmrk = mrk6;
          break;
        default:
          labelmrk = NULL;
      }
    }

    if( labattr == NULL )
      labattr = DEF_LABELATTR;

    if( orientation == HORIZONTAL )
    {
      mrk_start = mrk_end = endpoint.x2();
      mrk_start0 = mrk_end0 = endpoint.x2();
      mrk_len = t2p(mrklen)*y_size/v_size();

      arrow_extra = t2p(arrowextra)*y_size/v_size();
    }
    else
    {
      mrk_start = mrk_end = endpoint.x1();
      mrk_start0 = mrk_end0 = endpoint.x1();
      mrk_len = t2p(mrklen)*x_size/h_size();

      arrow_extra = t2p(arrowextra)*x_size/h_size();
    }

    if( arrow )
      to += arrow_extra;

    switch( mrkpos )
    {
      case POSITIVE:
        mrk_end += mrk_len;
        mrk_end0 += 0.5*mrk_len;
        break;
      case NEGATIVE:
        mrk_start -= mrk_len;
        mrk_start0 -= 0.5*mrk_len;
        break;
      case CENTER:
      default:
        mrk_start -= mrk_len/2;
        mrk_end += mrk_len/2;
        mrk_start0 -= 0.5*mrk_len/2;
        mrk_end0 += 0.5*mrk_len/2;
        break;
    }

    if( boldaxis )
      lineThickness(boldAxisThickness);
    else
      lineThickness(normalAxisThickness);

    if( drawaxis )
    {
      // Draw the axis
      if( orientation == HORIZONTAL )
      {
        if( arrow )
          printf("\n\\put(%g,%g){\\vector(1,0){%g}}%%",
                 h_scale(from),v_scale(endpoint.x2()),h_scale(to)-h_scale(from)
                );
        else
          printf("\n\\put(%g,%g){\\line(1,0){%g}}%%",
                 h_scale(from),v_scale(endpoint.x2()),h_scale(to)-h_scale(from)
                );
      }
      else // VERTICAL
      {
        if( arrow )
          printf("\n\\put(%g,%g){\\vector(0,1){%g}}%%",
                 h_scale(endpoint.x1()),v_scale(from),v_scale(to)-v_scale(from)
                );
        else
          printf("\n\\put(%g,%g){\\line(0,1){%g}}%%",
                 h_scale(endpoint.x1()),v_scale(from),v_scale(to)-v_scale(from)
                );
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

    // ... marks, labels and grid lines
    if( labelmrk )
    {
      Mark*  mp;
      double t,offset;
      int    dec;

      mp = labelmrk;
      dec = (int)(floor(firstmrk+smallValue));
      offset = t2p(3);

      t = log10(mp->position)+(double)dec;
      while( t <= lastmrk+smallValue )
      {
        if( t >= firstmrk-smallValue )
        {
          char label[128];

          if( mp->label )
            sprintf(label,"%s %s",labattr,mp->label);
          else if( fabs(mp->position-1.0)>smallValue ) // mp->position != 1.0
            label[0] = 0;
          else
          {
            switch( dec )
            {
              case -2:
                sprintf(label,"%s %s",labattr,"0.01");
                break;
              case -1:
                sprintf(label,"%s %s",labattr,"0.1");
                break;
              case 0:
                sprintf(label,"%s %s",labattr,"1.0");
                break;
              case 1:
                sprintf(label,"%s %s",labattr,"10");
                break;
              case 2:
                sprintf(label,"%s %s",labattr,"100");
                break;
              case 3:
                sprintf(label,"%s %s",labattr,"1000");
                break;
              default:
                sprintf(label,"%s $10^{%d}$",labattr,dec);
            }
          }

          // print the label
          switch( labpos )
          {
            case POSITIVE:
              if( orientation == HORIZONTAL )
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[bc]{%s}}%%",
                       h_scale(t)-0.5*boxsize,v_scale(mrk_end)+offset,
                       boxsize,boxsize,
                       label);
              }
              else
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[l]{%s}}%%",
                       h_scale(mrk_end)+offset,v_scale(t)-0.5*boxsize,
                       boxsize,boxsize,
                       label);
              }
              break;
            case NEGATIVE:
            default:
              if( orientation == HORIZONTAL )
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[tc]{%s}}%%",
                       h_scale(t)-0.5*boxsize,v_scale(mrk_start)-boxsize-offset,
                       boxsize,boxsize,
                       label);
              }
              else
              {
                printf("\n\\put(%g,%g){\\makebox(%g,%g)[r]{%s}}%%",
                       h_scale(mrk_start)-boxsize-offset,v_scale(t)-0.5*boxsize,
                       boxsize,boxsize,
                       label);
              }
              break;
          }

          // .. the main marks and bold grid lines
          lineThickness(boldGridThickness);
          if( orientation == HORIZONTAL )
          {
            printf("\n\\drawline(%g,%g)(%g,%g)%%",
                   h_scale(t),v_scale(mrk_start),h_scale(t),v_scale(mrk_end)
                  );

            if( grid )
            {
              printf("\n\\path(%g,%g)(%g,%g)%%",
                     h_scale(t),v_scale(linestart),h_scale(t),v_scale(lineend)
                    );
            }
          }
          else // VERTICAL
          {
            printf("\n\\drawline(%g,%g)(%g,%g)%%",
                   h_scale(mrk_start),v_scale(t),h_scale(mrk_end),v_scale(t)
                  );

            if( grid )
            {
              printf("\n\\path(%g,%g)(%g,%g)%%",
                     h_scale(linestart),v_scale(t),h_scale(lineend),v_scale(t)
                    );
            }
          }

          // ..and the submarks/fine lines
          if( mp->numsub )
          {
            int    i;
            double pos,dt;

            pos = mp->position;
            if( mp[1].position > mp[0].position )
              dt = (mp[1].position-mp[0].position)/((double)(mp->numsub+1));
            else
              dt = (10.0*mp[1].position-mp[0].position)/((double)(mp->numsub+1));

            if( grid )
            {
              lineThickness(fineGridThickness);

              for( i = 0 ; i < mp->numsub ; i++ )
              {
                pos += dt;
                t = log10(pos)+(double)dec;

                if( t <= lastmrk+smallValue )
                {
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
                }
              }
            }
            else
            {
              lineThickness(boldGridThickness);

              for( i = 0 ; i < mp->numsub ; i++ )
              {
                pos += dt;
                t = log10(pos)+(double)dec;

                if( t <= lastmrk+smallValue )
                {
                  if( orientation == HORIZONTAL )
                  {
                    printf("\n\\drawline(%g,%g)(%g,%g)%%",
                           h_scale(t),v_scale(mrk_start0),h_scale(t),v_scale(mrk_end0)
                          );
                  }
                  else // VERTICAL
                  {
                    printf("\n\\drawline(%g,%g)(%g,%g)%%",
                           h_scale(mrk_start0),v_scale(t),h_scale(mrk_end0),v_scale(t)
                          );
                  }
                }
              }
            }
          }
        }

        mp++;
        if( fabs(mp->position-10) < smallValue ) // ( mp->position == 10 )
        {
          mp = labelmrk;
          dec++;
        }
        else if( fabs(mp->position-1) < smallValue ) // ( mp->position == 1 )
        {
          dec++;
        }

        t = log10(mp->position)+(double)dec;
      }
    }

    lineThickness(normalThickness);

    end_stanza();
  }

} // end of namespace
