#include "epix.h"

int main()
{
  SingleLogCoord cs(0,10,1,10);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.logStyle(6);
  cs.grid();
  cs.draw();

  end();

  return 0;
}
