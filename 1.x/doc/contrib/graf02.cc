#include "epix.h"

int main()
{
  ePiX_contrib::CartesianCoord cs(-3,5,-2,6);

  picture(P(12, 12));
  unitlength("5mm");

  begin();

  cs.xMarks(-2,6);
  cs.yMarks(-1,6);
  cs.draw();

  end();

  return 0;
}
