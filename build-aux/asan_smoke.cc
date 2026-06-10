// asan_smoke.cc -- exercises the libepix surface the Python bindings cover, so
// `make asan` (AddressSanitizer) catches latent memory bugs in that surface as
// the bound API grows. Mirror new bindings here when you add them.
//
// DEV-ONLY (paired with libasan in the image + the `asan` Makefile target).
// REMOVE in the final cleanup phase -- see the python-bindings task doc.
#include "epix.h"
using namespace ePiX;

static P para(double t) { return P(t, t * t); }
static double sq(double t) {
  return t * t;
}  // double-valued, for Deriv/Integral
static P saddle(double u, double v) { return P(u, v, u * u - v * v); }
static P color3(double x, double y, double z) { return P(x, 0.5, z); }
static P color2(double u, double v) { return P(1, 1, 1); }

int main() {
  picture(P(-2, -2), P(2, 2), "3x3in");
  begin();

  // state + 2-D drawing
  pen(Blue());
  fill(Red());
  bold();
  bold(Red());
  plain();
  plain(Black());
  nofill();
  font_size("Huge");
  h_axis(4);
  v_axis(4);
  grid(2, 2);

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
  L.path_item("d");
  L.draw(P(0, 0), P(0, 0), epix_label_posn::br);
  line(P(-1, -1), P(1, 1));
  plot(para, -1.0, 1.0, 50);
  label(P(0, 0), "x");
  label(P(0, 1), P(0, 0), "top", epix_label_posn::t);

  // markers + shapes + enums
  marker(P(1, 1), epix_mark_type::BOX);
  dot(P(0.5, 0.5));
  box(P(-0.5, -0.5));
  arrow(P(0, 0), P(1, 0), P(0, 0), "", epix_label_posn::none);
  circle(P(0, 0), 1.0, E_3);
  rect(P(-1.5, -1.5), P(-1, -1));
  triangle(P(0, 0), P(1, 0), P(0, 1));
  ellipse(P(0, 0), P(1, 0), P(0, 0.5));
  dart(P(0, 0), P(0.5, 0.5));

  // styles + clipping
  solid();
  dashed();
  dotted();
  line_style("-");
  clip_box(P(-2, -2), P(2, 2));
  clip(true);
  set_crop(true);
  crop();

  // 3-D surface
  viewpoint(3, 2, 1.5);
  domain R(P(-1, -1), P(1, 1), mesh(4, 4), mesh(12, 12));
  surface(saddle, R);

  // camera + sphere + lat/long + hidden/visible plotting
  camera.at(P(1, 2.5, 3));
  sphere();
  latitude(30, 0, 360);
  longitude(60, 0, 360);
  backplot_N(sq, sq, -90, 90, 20);
  frontplot_N(sq, sq, -90, 90, 20);

  // border / pen(Color,str) / region grid / envelope / Line / masklabel
  border();
  pen(Black(0.3), "0.1pt");
  grid(P(-2, 0), P(2, 4), 4, 4);
  envelope(para, -1.0, 1.0, 10);
  Line(P(-1, 0), P(1, 0));
  masklabel(P(0, 0), P(0, 1), "m", epix_label_posn::t);

  // more state setters, shapes, axis labels
  black(0.5);
  blue(0.5);
  green(0.5);
  rgb(0.1, 0.2, 0.3);
  arrow_inset(0.1);
  arrow_width(2);
  arc(P(0, 0), 1, 0, 1);
  polar_grid(1, 2, 8);
  h_axis_labels(4, P(0, -4), epix_label_posn::b);
  // function-pointer plots / fields (trampolines)
  polarplot(sq, 0, 1, 10);
  tan_field(para, 0, 1, 10);
  dart_field(saddle, P(-1, -1), P(1, 1), 3, 3);
  ode_plot(saddle, P(0.1, 0.1), 2, 20);

  // coordinate ctors + plot over a 3-D domain + animation/tix
  tix() = 0.5;
  line(polar(1, 0.25), sph(2, 0.1, 0.2));
  ellipse(cyl(1, 0.2, 0), P(0.5, 0.5));  // 2-arg ellipse
  label_angle(0.1);
  red(1.2);
  plot(xyz, domain(P(-1, -1, -1), P(1, 1, 1), mesh(1, 1, 1)));

  // batch 2: pair / Plane / riemann_sum / domain ops / markers / misc
  {
    pair a(1, 2);
    pair b = a * pair(0, 1);
    (void)b;
  }
  {
    Plane pl(P(-1, -1, -1), P(1, 1, 1));
    pl.shift(P(0.1, 0.1, 0.1));
    pl.draw();
  }
  white(0.5);
  dot_size(2);
  ddot(P(0.4, 0.4));
  ddot(P(0.4, 0.4), P(0, 1), "d", epix_label_posn::t);
  riemann_sum(sq, -1, 1, 8, epix_integral_type::UPPER);
  plot(saddle, R.resize1(0, 0.5).slice2(0));
  line(cis(0.5), cb(0.3) * E_3);

  // batch 3: screen / Segment / border(Color,str) / dot(4-arg)
  {
    screen scr(P(-1, -1), P(1, 1));
    activate(scr);
    border(Red(0.6), "1pt");
    circle(P(0, 0), 0.5);
    inset(P(0, 0), P(0.5, 0.5));
    deactivate(scr);
  }
  {
    Segment s1(P(-1, 0), P(1, 1)), s2(P(-1, 1), P(1, 0));
    P x = s1 * s2;
    s1.draw();
    (void)x;
  }
  dot(P(0.2, 0.2), P(0, 1), "d", epix_label_posn::t);

  // batch 4: clipping by face / label_color / base / backing / surface+color
  label_color(White());
  base(Green(0.6), "1.5pt");
  backing(Black());
  clip_face(P(0, 0, 0), -E_3);
  surface(saddle, domain(P(-1, -1), P(1, 1), mesh(4, 4)), color3, 0);
  surface(saddle, domain(P(-1, -1), P(1, 1), mesh(4, 4)), color2,
          0);  // 2-var color
  clip_restore();

  // batch 5: 3-D fields F(x,y,z)
  ode_plot(color3, P(1, 0, 0), 0, 2, 20);
  dart_field(color3, domain(P(-1, -1, -1), P(1, 1, 1), mesh(3, 3, 2)), 0.5);

  // batch 7: magenta/Neutral/clip_box(P)
  magenta(0.5);
  clip_box(P(2, 2, 2));
  {
    Color c = Neutral();
    pen(c);
  }

  // batch 6: Deriv functor + scenery object
  {
    P d = Deriv(sq)(P(0.5));
    (void)d;
  }
  {
    scenery sc(saddle, domain(P(-1, -1), P(1, 1), mesh(3, 3)));
    sc.add(saddle, domain(P(-1, -1), P(0, 0), mesh(2, 2)));  // multi-surface
    sc.draw(-1);
  }
  camera.filter(RGB_Neutral());
  gray(0.5);
  ring(P(0.5, 0.5));
  clip_slice(P(0, 0, 0), E_3, 0.1);
  clip_restore();
  {
    P e = camera.eye();
    (void)e;
  }
  axis_break(P(1, 0), P(1.1, 0));
  shadeplot(sq, -1, 1, 10);
  {
    double r = recip(2.0);
    (void)r;
  }

  // batch 9: path / affine / screen transforms + misc markers/functions
  {
    path p(P(0, 0), E_1, E_2, 0, 1);
    p += path(P(0, 0), P(0.5, 0.5));
    p.pt(P(0.3, 0.3)).close().fill().draw();
  }
  {
    affine af;
    af.reflect(0.1).shift(P(0.1, 0));
  }
  J(P(1, 0));
  flow(saddle, P(0.1, 0.1), 1, 10);
  circ(P(0.2, 0.2));
  spot(P(0.3, 0.3));
  right_angle(P(0, 0), E_1, E_2);
  arc_arrow(P(0, 0), 0.5, 0, 1);
  {
    Segment s(P(-1, 0), P(1, 0));
    P m = s.midpoint();
    (void)m;
  }
  circle(P(0, 0), P(1, 0), P(0, 1));
  aarrow(P(0, 0), P(1, 1), 0.5);
  tan_line(sq, 0.5);
  masklabel(P(0.5, 0.5), "m");
  slope_field(saddle, P(-1, -1), P(1, 1), 4, 4);
  {
    double iv = Integral(sq, 0).eval(1.0);
    (void)iv;
  }
  {
    screen sc(P(-1, -1), P(1, 1));
    sc.scale(0.9).rotate(0.1);
    sc.set_crop();
  }

  // batch 10: arrow_fill / v_error_bar / h_axis_masklabels / screen corners
  arrow_fill(true);
  v_error_bar(P(0.5, 0.5), 0.1, epix_mark_type::CIRC, 4);
  h_axis_masklabels(P(-1, 0), P(1, 0), 4, P(0, 2), epix_label_posn::c);
  {
    screen sc2(P(-1, -1), P(1, 1));
    P c = sc2.br();
    (void)c;
  }

  print_eepic("/tmp/asan_smoke.eepic");
  return 0;
}
