/* 
 * affine.cc -- affine transformations
 *
 * Last Change: October 22, 2006
 */
#include "epix.h"
#include "affine.h"

namespace ePiX {

  // 2x2 matrix multiplication [a1 a2][in] = [out]
  P transf(const P& in, const P& A1, const P& A2, const P& b)
  {
    return P(A1.x1()*in.x1() + A2.x1()*in.x2() + b.x1(),
	     A1.x2()*in.x1() + A2.x2()*in.x2() + b.x2());
  }

  double density(const gray_level arg)
  {
    if (arg == BLACK)
      return 1.0;

    else if (arg == WHITE || arg == NONE)
      return 0.0;

    else if (arg == GRAY10)
      return 0.1;

    else if (arg == GRAY20)
      return 0.2;

    else if (arg == GRAY30)
      return 0.3;

    else if (arg == GRAY40)
      return 0.4;

    else if (arg == GRAY60)
      return 0.6;

    else // UNKNOWN
      return -1.0;
  }

  // Image of the "standard F" under an affine transformation
  void std_F (const P& A1, const P& A2, const P& b,
	      gray_level back, gray_level fore)
  {
    const double r(1.0/6.0);

    path F;

    F.pt(transf(P(r,   0.75*r), A1, A2, b));
    F.pt(transf(P(2*r, 0.75*r), A1, A2, b));
    F.pt(transf(P(2*r, 2.25*r), A1, A2, b));
    F.pt(transf(P(4*r, 2.25*r), A1, A2, b));
    F.pt(transf(P(4*r, 3.25*r), A1, A2, b));
    F.pt(transf(P(2*r, 3.25*r), A1, A2, b));
    F.pt(transf(P(2*r, 4.25*r), A1, A2, b));
    F.pt(transf(P(5*r, 4.25*r), A1, A2, b));
    F.pt(transf(P(5*r, 5.25*r), A1, A2, b));
    F.pt(transf(P(  r, 5.25*r), A1, A2, b));

    F.close();
    if (fore != NONE)
      F.fill();

    // get state
    const double tmp_gray(epix::get_gray());
    const bool tmp_fill(epix::fill_paths);

    if (back != NONE)
      {
	fill();
	gray(density(back));
      }

    // Bounding parallelogram
    ePiX::quad(transf(P(0,0), A1, A2, b),
	       transf(P(1,0), A1, A2, b),
	       transf(P(1,1), A1, A2, b),
	       transf(P(0,1), A1, A2, b));

    if (fore != NONE)
      {
	fill();
	gray(density(fore));
      }

    F.draw();

    // restore state
    gray(tmp_gray);
    fill(tmp_fill);
  }
} // end of namespace
