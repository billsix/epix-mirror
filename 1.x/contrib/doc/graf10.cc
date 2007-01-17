#include "epix.h"
#include "epix_ext.h"
using namespace ePiX;
using namespace ePiX_contrib;

int main()
{
  SingleLogCoord sc(0,8,1,100);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  sc.draw();

  end();

  return 0;
}
