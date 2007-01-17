#include "epix.h"
#include "epix_ext.h"
using namespace ePiX;
using namespace ePiX_contrib;

static char* ylabels[] =
  {"22","23","24","25","26",NULL};

int main()
{
  CartesianCoord cs(-1,7,19,27);

  picture(P(12, 12));
  unitlength("5mm");

  begin();

  cs.axisCross(P(0,20));
  cs.xMarks(1,6);
  cs.yMarks(22,5);
  cs.yLabels(ylabels);
  cs.yName(POSITIVE);
  cs.yBroken();
  cs.draw();

  end();

  return 0;
}



