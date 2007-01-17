#include "epix.h"
#include "epix_ext.h"
using namespace ePiX;
using namespace ePiX_contrib;

static char* linLabels[] =
  {"\\scriptsize 0","\\scriptsize 0.1","\\scriptsize 0.2",
   "\\scriptsize 0.3","\\scriptsize 0.4","\\scriptsize 0.5",
   "\\scriptsize 0.6","\\scriptsize 0.7","\\scriptsize 0.8",
   "\\scriptsize 0.9","\\scriptsize 1.0",NULL
  };

int main()
{
  HorizLinAxis linAs;
  HorizLogAxis logAs;

  bounding_box(P(0,0), P(1,1.17));
  picture(P(10,1.5));
  unitlength("1cm");

  /* Start picture */
  begin();

  linAs.nummrk = 11;
  linAs.distmrk = 0.1;
  linAs.mrk1 = 0;
  linAs.mrkpos = POSITIVE;
  linAs.labels = linLabels;
  linAs.labpos = POSITIVE;
  linAs.name = "$\\log(x)$";
  linAs.nampos = POSITIVE;
  linAs.draw(P(0,0.75),1.17);

  logAs.drawaxis = false;
  logAs.arrow = true;
  logAs.arrowextra = 5;
  logAs.stdstyle = 6;
  logAs.mrkpos = NEGATIVE;
  logAs.name = "$x$";
  logAs.draw(P(0,0.75),1);

  /* End picture */
  end();

  return 0;
}
