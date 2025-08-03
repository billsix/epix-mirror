/* 
 * std_F.cc -- The std_F class definitions
 *
 * Last Change: September 23, 2007
 */
// Include only the headers we need; burdensome, but generally more
// efficient for larger projects with many dependencies.
#ifdef MINIMAL
#include "epix/affine.h"
#include "epix/Color.h"
#include "epix/curves.h"
#include "epix/pairs.h"
#include "epix/path.h"
#include "epix/state.h"
#include "epix/triples.h"

#else
#include "epix.h"
#endif

#include "epix/paint_style.h"
#include "std_F.h"

namespace ePiX {

  // The constructor initializes the class data, ensuring objects are
  // well-formed. For efficiency, we use "initialization" instead of
  // assignment (the "=" operator); this way, data members get their
  // actual values immediately. The constructor body is empty, since
  // this class requires no further initialization.

  // All class member function definitions must be qualified with the
  // class name, here "std_F::"
  std_F::std_F()
    : m_loc(0,0), m_e1(1,0), m_e2(0,1),
      m_fore(Black()), m_back(White()),
      m_edge(Black()), m_edge_width(0.4) { }

  // Apply the affine map af; return a reference to this object so
  // calls can be daisy-chained. Note the efficiency of storing only
  // the bare minimum data to specify an affine map rather than
  // holding and manipulating the shape of an F.
  std_F& std_F::map_by(const affine& af)
  {
    m_loc = af(m_loc); // affines map locations, so this works
    m_e1 = af(m_e1);
    m_e2 = af(m_e2);

    return *this;
  }

  // Set the background and foreground colors. Arguments' names chosen
  // to indicate their purpose.
  std_F& std_F::backing(const Color& back)
  {
    m_back = back;
    return *this;
  }

  std_F& std_F::fill(const Color& fore)
  {
    m_fore = fore;
    return *this;
  }

  // Set outline parameters.
  std_F& std_F::border(const Color& edge, double wid)
  {
    m_edge = edge;
    m_edge_width = wid;
    return *this;
  }

  // Draw in the active screen. Most of the "real work" is done here.
  void std_F::draw() const
  {
    // Get global drawing state, so we can restore before returning.
    // Not particularly elegant, but it's what we have to work with.
    Color old_fill(the_paint_style().fill_color());
    bool old_flag(the_paint_style().fill_flag());

    Color old_line(the_paint_style().line_color());
    length old_line_width(the_paint_style().line_width());

    // now we can draw
    const double r(1.0/6.0);

    path F;

    // pr converts coords in [0,1] x [0,1] to our coords
    F .pt(pr(r,   0.75*r)).pt(pr(2*r, 0.75*r)).pt(pr(2*r, 2.25*r))
      .pt(pr(4*r, 2.25*r)).pt(pr(4*r, 3.25*r)).pt(pr(2*r, 3.25*r))
      .pt(pr(2*r, 4.25*r)).pt(pr(5*r, 4.25*r)).pt(pr(5*r, 5.25*r))
      .pt(pr(  r, 5.25*r));

    F.close().fill();

    // Set global drawing state. We have a member function named fill,
    // so the call must be explicitly qualified with "ePiX::".
    ePiX::fill(m_back);
    ePiX::pen(m_edge, m_edge_width);

    // Bounding parallelogram
    ePiX::quad(pr(0,0), pr(1,0), pr(1,1), pr(0,1));

    ePiX::fill(m_fore);
    F.draw();
    // Restore global fill state.
    the_paint_style().fill_color(old_fill);
    the_paint_style().fill_flag(old_flag);

    the_paint_style().line_color(old_line);
    the_paint_style().line_width(old_line_width);
  }

  // private; convert (x,y) to a location usable in path construction
  // "Privacy" is enforced by the compiler from reading the header;
  // the definition requires no special syntax.
  P std_F::pr(double x, double y) const
  {
    pair loc((1 - x -y)*m_loc + x*m_e1 + y*m_e2);
    return P(loc.x1(), loc.x2());
  }
} // end of namespace
