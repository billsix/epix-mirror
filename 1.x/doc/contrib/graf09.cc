#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

static char* ylabels[] = {"10",NULL};

int main()
{
  CartesianCoord cs(-2,4,-10,50);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.yMarks(10,1);
  cs.yLabels(ylabels);
  cs.grid();
  cs.gridDensity(1,9,5,10,9,0);
  cs.draw();

  end();

  return 0;
}
