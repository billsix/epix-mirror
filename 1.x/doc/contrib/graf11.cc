#include "epix.h"

int main()
{
  SingleLogCoord sc(0,8,0.1,5);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  sc.logStyle(3);
  sc.draw();

  end();

  return 0;
}
