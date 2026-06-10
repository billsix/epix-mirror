// asan_smoke.cc -- exercises the libepix surface the Python bindings cover, so
// `make asan` (AddressSanitizer) catches latent memory bugs in that surface as
// the bound API grows. Mirror new bindings here when you add them.
//
// DEV-ONLY (paired with libasan in the image + the `asan` Makefile target).
// REMOVE in the final cleanup phase -- see the python-bindings task doc.
#include "epix.h"
using namespace ePiX;

static P para(double t)         { return P(t, t * t); }
static double sq(double t)      { return t * t; }   // double-valued, for Deriv/Integral
static P saddle(double u, double v) { return P(u, v, u * u - v * v); }

int main() {
  picture(P(-2, -2), P(2, 2), "3x3in");
  begin();

  // state + 2-D drawing
  pen(Blue()); fill(Red()); bold(); bold(Red()); plain(); plain(Black());
  nofill(); font_size("Huge");
  h_axis(4); v_axis(4); grid(2, 2);

  // axis class + factory functions (chained methods + draw)
  axis Ax(P(xmin(), 0), P(xmax(), 0), 4, P(0, 0), epix_label_posn::b);
  Ax.unmark(0).draw();
  bottom_axis(4, P(0, -4)).trig().draw_labels();
  left_axis(4, P(-4, 0)).draw_labels();

  // legend (chained) + derivative/integral plots
  legend L;
  L.item_border(0).border(Red(), 0.4);
  plot(Deriv(sq), -1.0, 1.0, 20);
  plot(Integral(sq, 0), -1.0, 1.0, 20);
  L.path_item("d"); L.draw(P(0, 0), P(0, 0), epix_label_posn::br);
  line(P(-1, -1), P(1, 1));
  plot(para, -1.0, 1.0, 50);
  label(P(0, 0), "x");
  label(P(0, 1), P(0, 0), "top", epix_label_posn::t);

  // markers + shapes + enums
  marker(P(1, 1), epix_mark_type::BOX);
  dot(P(0.5, 0.5)); box(P(-0.5, -0.5));
  arrow(P(0, 0), P(1, 0), P(0, 0), "", epix_label_posn::none);
  circle(P(0, 0), 1.0, E_3);
  rect(P(-1.5, -1.5), P(-1, -1));
  triangle(P(0, 0), P(1, 0), P(0, 1));
  ellipse(P(0, 0), P(1, 0), P(0, 0.5));
  dart(P(0, 0), P(0.5, 0.5));

  // styles + clipping
  solid(); dashed(); dotted(); line_style("-");
  clip_box(P(-2, -2), P(2, 2)); clip(true); set_crop(true); crop();

  // 3-D surface
  viewpoint(3, 2, 1.5);
  domain R(P(-1, -1), P(1, 1), mesh(4, 4), mesh(12, 12));
  surface(saddle, R);

  // camera + sphere + lat/long + hidden/visible plotting
  camera.at(P(1, 2.5, 3));
  sphere();
  latitude(30, 0, 360); longitude(60, 0, 360);
  backplot_N(sq, sq, -90, 90, 20);
  frontplot_N(sq, sq, -90, 90, 20);

  // border / pen(Color,str) / region grid / envelope / Line / masklabel
  border(); pen(Black(0.3), "0.1pt");
  grid(P(-2, 0), P(2, 4), 4, 4);
  envelope(para, -1.0, 1.0, 10);
  Line(P(-1, 0), P(1, 0));
  masklabel(P(0, 0), P(0, 1), "m", epix_label_posn::t);

  // more state setters, shapes, axis labels
  black(0.5); blue(0.5); green(0.5); rgb(0.1, 0.2, 0.3);
  arrow_inset(0.1); arrow_width(2);
  arc(P(0, 0), 1, 0, 1); polar_grid(1, 2, 8);
  h_axis_labels(4, P(0, -4), epix_label_posn::b);
  // function-pointer plots / fields (trampolines)
  polarplot(sq, 0, 1, 10);
  tan_field(para, 0, 1, 10);
  dart_field(saddle, P(-1, -1), P(1, 1), 3, 3);
  ode_plot(saddle, P(0.1, 0.1), 2, 20);

  // coordinate ctors + plot over a 3-D domain + animation/tix
  tix() = 0.5;
  line(polar(1, 0.25), sph(2, 0.1, 0.2));
  ellipse(cyl(1, 0.2, 0), P(0.5, 0.5));   // 2-arg ellipse
  label_angle(0.1); red(1.2);
  plot(xyz, domain(P(-1, -1, -1), P(1, 1, 1), mesh(1, 1, 1)));

  print_eepic("/tmp/asan_smoke.eepic");
  return 0;
}
