#include "epix.h"

Mark mrk[] =
 {
  {1,NULL,9},
  {2,"\\scriptsize 2",9},
  {3,"\\scriptsize 3",9},
  {4,"\\scriptsize 4",9},
  {5,"\\scriptsize 5",4},
  {6,"\\scriptsize 6",4},
  {7,"\\scriptsize 7",4},
  {8,"\\scriptsize 8",4},
  {9,"\\scriptsize 9",4},
  {10,NULL,0}
 };

int main()
{
  SingleLogCoord cs(0,10,1,100);

  picture(P(12,24));
  unitlength("5mm");

  begin();

  cs.logStyle(mrk);
  cs.labelAttr("");
  cs.draw();

  end();

  return 0;
}
