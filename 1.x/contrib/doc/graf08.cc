#include "epix.h"
#include "epix_ext.h"
using namespace ePiX;
using namespace ePiX_contrib;

int main()
{
  CartesianCoord cs(-2,4,-1,5);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.grid();
  cs.draw();

  end();

  return 0;
}

