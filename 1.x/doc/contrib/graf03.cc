#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

static char* xlabels[] = {"-2","-1","","1","2","3",NULL};
static char* ylabels[] = {"-1","","1","2","3","4",NULL};

int main()
{
  CartesianCoord cs(-3,5,-2,6);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.xMarks(-2,6);
  cs.yMarks(-1,6);
  cs.xLabels(xlabels,-2);
  cs.yLabels(ylabels,-1);
  cs.draw();

  end();

  return 0;
}
