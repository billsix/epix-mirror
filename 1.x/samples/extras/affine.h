/*
 * affine.h -- facilities for drawing affine transformations
 *
 * Last Change: October 22, 2006
 */

namespace ePiX {

  class P;

  enum gray_level {NONE, WHITE, GRAY10, GRAY20, GRAY30, GRAY40, GRAY60, BLACK};

  // x -> Ax+b; x, A1, A2, b
  P transf(const P&, const P& A1=P(1,0), const P& A2=P(0,1),
	   const P& b=P(0,0));

  // Image of the "standard F" under a plane affine transformation
  void std_F(const P& A1=P(1,0), const P& A2=P(0,1), const P& b=P(0,0),
	     gray_level back=WHITE, gray_level fore=BLACK);
} // end of namespace
