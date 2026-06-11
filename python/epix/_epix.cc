// _epix.cc -- nanobind bindings over libepix (Phase 2).
//
// ePiX uses a global-state drawing model: picture()/begin() set up, drawing
// functions mutate the active picture, and print_eepic()/end() emit it. We bind
// the free functions + value types directly; the `epix` package adds Figure /
// figure() ergonomics + inline render on top.
//
// Bound surface = the Phase-0 "core tier" (functions used by >=20 demos) plus
// the value types they need. Expand by tier (see the Phase-0 bind-list in
// tasks/.../python-bindings-and-notebooks.md). NB: keep ASan-checking as the
// surface grows -- libepix has latent uninitialized-pimpl bugs (screen was
// one).

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "epix.h"

namespace nb = nanobind;
using namespace ePiX;

// ---- plot() trampolines: ePiX plot takes raw C function pointers, so we route
// through a module-global Python callable (one plot at a time; fine for
// notebooks).
static nb::callable g_fn;
static P tramp_P(double t) { return nb::cast<P>(g_fn(t)); }
static double tramp_d(double t) { return nb::cast<double>(g_fn(t)); }
// two-variable trampoline for surface(P F(double,double), ...)
static nb::callable g_fn2;
static P tramp_P2(double u, double v) { return nb::cast<P>(g_fn2(u, v)); }
// two-variable *scalar* trampoline (reuses g_fn2) for the level-set/contour
// plot(double f(double,double), p1, p2, mesh, mesh).
static double tramp_d2(double u, double v) {
  return nb::cast<double>(g_fn2(u, v));
}
// two-function trampolines for back/frontplot_N(double f(double), double
// g(double), ...)
static nb::callable g_fa, g_fb;
static double tramp_a(double t) { return nb::cast<double>(g_fa(t)); }
static double tramp_b(double t) { return nb::cast<double>(g_fb(t)); }
// three-variable trampoline for plot(P f(double,double,double), domain)
static nb::callable g_fn3;
static P tramp_P3(double x, double y, double z) {
  return nb::cast<P>(g_fn3(x, y, z));
}

// dedicated trampoline for a 2-var color function (F uses g_fn2, so the color
// needs its own slot when both are 2-variable).
static nb::callable g_col;
static P tramp_col2(double u, double v) { return nb::cast<P>(g_col(u, v)); }

// Does this callable take two args (vs three)? Count parameters via
// inspect.signature rather than a trial call -- a trial call can't tell "wrong
// arity" from "the function raised at the probe point" (e.g. 1/0). These
// multi-variable callables are always user Python functions/lambdas, for which
// inspect.signature is reliable.
static bool callable_takes_two(nb::callable f) {
  try {
    nb::object sig = nb::module_::import_("inspect").attr("signature")(f);
    nb::object params = sig.attr("parameters");
    return nb::len(params) == 2;
  } catch (...) {
    return true;
  }
}

// scenery samples each surface (and captures the current fill state) at
// construction/add time, so we build the C++ scenery EAGERLY — set the
// trampoline, sample, clear — rather than deferring to draw(). That captures
// per-surface colors correctly and holds no Python callable (no GC cycle).
struct PyScenery {
  scenery sc;
  PyScenery(nb::callable f, const domain& R) { build(f, R, true); }
  PyScenery(nb::callable f, const domain& R, nb::callable color) {
    build_c(f, R, color, true);
  }
  PyScenery& add(nb::callable f, const domain& R) {
    build(f, R, false);
    return *this;
  }
  PyScenery& add_c(nb::callable f, const domain& R, nb::callable color) {
    build_c(f, R, color, false);
    return *this;
  }
  void build(nb::callable f, const domain& R, bool first) {
    if (callable_takes_two(f)) {
      g_fn2 = f;
      if (first)
        sc = scenery(tramp_P2, R);
      else
        sc.add(tramp_P2, R);
      g_fn2 = nb::callable();
    } else {
      g_fn3 = f;
      if (first)
        sc = scenery(tramp_P3, R);
      else
        sc.add(tramp_P3, R);
      g_fn3 = nb::callable();
    }
  }
  // f(u,v)->P over the domain, colored by color(u,v)->RGB or color(x,y,z)->RGB.
  void build_c(nb::callable f, const domain& R, nb::callable color,
               bool first) {
    g_fn2 = f;
    if (callable_takes_two(color)) {
      g_col = color;
      if (first)
        sc = scenery(tramp_P2, R, tramp_col2);
      else
        sc.add(tramp_P2, R, tramp_col2);
      g_col = nb::callable();
    } else {
      g_fn3 = color;
      if (first)
        sc = scenery(tramp_P2, R, tramp_P3);
      else
        sc.add(tramp_P2, R, tramp_P3);
      g_fn3 = nb::callable();
    }
    g_fn2 = nb::callable();
  }
};

NB_MODULE(_epix, m) {
  m.doc() =
      "Low-level nanobind bindings over libepix (use the `epix` package).";

  // ---- P (ordered triple / 3-vector) ----
  nb::class_<P>(m, "Point")
      .def(nb::init<double, double, double>(), nb::arg("x") = 0.0,
           nb::arg("y") = 0.0, nb::arg("z") = 0.0)
      .def("x1", &P::x1)
      .def("x2", &P::x2)
      .def("x3", &P::x3)
      .def("__add__", [](const P& a, const P& b) { return a + b; })
      .def("__sub__", [](const P& a, const P& b) { return a - b; })
      .def("__neg__", [](const P& a) { return -a; })
      .def("__mul__", [](const P& a, double c) { return c * a; })  // P * scalar
      .def("__rmul__",
           [](const P& a, double c) { return c * a; })  // scalar * P
      .def("__xor__",
           [](const P& a, const P& b) { return a * b; })  // cross: a ^ b
      .def("dot", [](const P& a, const P& b) { return a | b; })
      .def("__or__", [](const P& a, const P& b) { return a | b; })  // dot
      .def("__mod__", [](P a, const P& b) { return a % b; })  // orthogonalize
      .def("norm", [](const P& a) { return norm(a); })
      .def("__repr__", [](const P& p) {
        return "Point(" + std::to_string(p.x1()) + ", " +
               std::to_string(p.x2()) + ", " + std::to_string(p.x3()) + ")";
      });

  // ---- Color (opaque value type; constructed via the color functions below)
  // ----  `c * d` / `d * c` scale intensity (mirrors C++ `Color::operator*=`).
  nb::class_<Color>(m, "Color")
      .def("__mul__", [](const Color& c, double d) { return d * c; })
      .def("__rmul__", [](const Color& c, double d) { return d * c; })
      // blend toward `other` by t; returns the blended color (non-mutating from
      // Python's view -- operates on a copy, mirroring the .xp usage pattern).
      .def(
          "blend",
          [](Color c, const Color& other, double t) -> Color {
            c.blend(other, t);
            return c;
          },
          nb::arg("other"), nb::arg("t"))
      .def(
          "filter",
          [](const Color& c, const Color& other) -> Color {
            return c.filter(other);
          },
          nb::arg("other"));

  // ---- scoped enums (the C++ enum class types) ----
  nb::enum_<epix_mark_type>(m, "MarkType")
      .value("PATH", epix_mark_type::PATH)
      .value("CIRC", epix_mark_type::CIRC)
      .value("SPOT", epix_mark_type::SPOT)
      .value("RING", epix_mark_type::RING)
      .value("DOT", epix_mark_type::DOT)
      .value("DDOT", epix_mark_type::DDOT)
      .value("PLUS", epix_mark_type::PLUS)
      .value("OPLUS", epix_mark_type::OPLUS)
      .value("TIMES", epix_mark_type::TIMES)
      .value("OTIMES", epix_mark_type::OTIMES)
      .value("DIAMOND", epix_mark_type::DIAMOND)
      .value("UP", epix_mark_type::UP)
      .value("DOWN", epix_mark_type::DOWN)
      .value("BOX", epix_mark_type::BOX)
      .value("BBOX", epix_mark_type::BBOX)
      .value("HTICK", epix_mark_type::HTICK)
      .value("VTICK", epix_mark_type::VTICK)
      .value("TEXT", epix_mark_type::TEXT);

  nb::enum_<epix_label_posn>(m, "LabelPos")
      .value("none", epix_label_posn::none)
      .value("c", epix_label_posn::c)
      .value("r", epix_label_posn::r)
      .value("tr", epix_label_posn::tr)
      .value("rt", epix_label_posn::rt)
      .value("t", epix_label_posn::t)
      .value("tl", epix_label_posn::tl)
      .value("lt", epix_label_posn::lt)
      .value("l", epix_label_posn::l)
      .value("bl", epix_label_posn::bl)
      .value("lb", epix_label_posn::lb)
      .value("b", epix_label_posn::b)
      .value("br", epix_label_posn::br)
      .value("rb", epix_label_posn::rb);

  // color constructors (return Color)
  m.def("rgb", &RGB, nb::arg("r") = 0.0, nb::arg("g") = 0.0,
        nb::arg("b") = 0.0);
  for (auto&& [name, fn] : {std::pair{"black", &Black},
                            {"white", &White},
                            {"red", &Red},
                            {"green", &Green},
                            {"blue", &Blue},
                            {"yellow", &Yellow},
                            {"cyan", &Cyan},
                            {"magenta", &Magenta}})
    m.def(name, fn, nb::arg("intensity") = 1.0);

  // ---- picture setup / lifecycle ----
  m.def("picture",
        static_cast<void (*)(const P&, const P&, std::string)>(&picture),
        nb::arg("lower_left"), nb::arg("upper_right"), nb::arg("size"));
  m.def("begin", &begin);
  m.def("end_picture", &end_picture);
  m.def("print_eepic", &print_eepic, nb::arg("filename"));

  m.def("xmin", []() { return xmin(); });
  m.def("xmax", []() { return xmax(); });
  m.def("ymin", []() { return ymin(); });
  m.def("ymax", []() { return ymax(); });
  m.def("xsize", []() { return xsize(); });
  m.def("ysize", []() { return ysize(); });
  m.def("tix", []() { return tix(); });
  m.def(
      "set_tix", [](double t) { tix() = t; },
      nb::arg("t"));  // animation frame param

  // standard basis vectors (P constants)
  m.attr("E_1") = nb::cast(E_1);
  m.attr("E_2") = nb::cast(E_2);
  m.attr("E_3") = nb::cast(E_3);

  // ---- coordinate constructors (return P) ----
  m.def("xyz", &xyz, nb::arg("x"), nb::arg("y"), nb::arg("z") = 0.0);
  m.def("polar", &polar, nb::arg("radius"), nb::arg("theta"));
  m.def("sph", &sph, nb::arg("radius"), nb::arg("theta"), nb::arg("phi"));
  m.def("cyl", &cyl, nb::arg("radius"), nb::arg("theta"), nb::arg("z"));

  // ---- pen / fill / color state ----
  m.def("pen", static_cast<void (*)(const Color&)>(&pen), nb::arg("color"));
  m.def("pen", static_cast<void (*)(double)>(&pen), nb::arg("width"));
  m.def("pen", static_cast<void (*)(const Color&, double)>(&pen),
        nb::arg("color"), nb::arg("width"));
  m.def("pen", static_cast<void (*)(const Color&, std::string)>(&pen),
        nb::arg("color"),
        nb::arg("width"));  // width as a length string, e.g. "0.1pt"
  m.def("bold", static_cast<void (*)()>(&bold));
  m.def("bold", static_cast<void (*)(const Color&)>(&bold), nb::arg("color"));
  m.def("bbold", static_cast<void (*)()>(&bbold));
  m.def("bbold", static_cast<void (*)(const Color&)>(&bbold), nb::arg("color"));
  m.def("plain", static_cast<void (*)()>(&plain));
  m.def("plain", static_cast<void (*)(const Color&)>(&plain), nb::arg("color"));
  m.def("fill", static_cast<void (*)(const Color&)>(&fill), nb::arg("color"));
  m.def("fill", static_cast<void (*)(bool)>(&fill), nb::arg("on") = true);
  m.def("nofill", &nofill);
  m.def("font_size", &font_size, nb::arg("size") = "nsz");
  m.def("set_red", &red, nb::arg("intensity") = 1.0);  // set pen to a red tint
  m.def("label_angle", static_cast<void (*)(double)>(&label_angle),
        nb::arg("t") = 0.0);

  // ---- drawing ----
  m.def("label", static_cast<void (*)(const P&, const std::string&)>(&label),
        nb::arg("at"), nb::arg("text"));
  m.def("line", static_cast<void (*)(const P&, const P&, double)>(&line),
        nb::arg("tail"), nb::arg("head"), nb::arg("expand") = 0.0);
  m.def("infinite_line", static_cast<void (*)(const P&, const P&)>(&Line),
        nb::arg("tail"), nb::arg("head"));  // infinite line through two points
  m.def("infinite_line", static_cast<void (*)(const P&, double)>(&Line),
        nb::arg("pt"),
        nb::arg("slope"));  // infinite line through pt with given slope
  m.def("grid", static_cast<void (*)(unsigned int, unsigned int)>(&grid),
        nb::arg("nx") = 1, nb::arg("ny") = 1);
  m.def("grid",
        static_cast<void (*)(const P&, const P&, unsigned int, unsigned int)>(
            &grid),
        nb::arg("lower_left"), nb::arg("upper_right"), nb::arg("nx"),
        nb::arg("ny"));
  m.def("border", static_cast<void (*)()>(&border));
  m.def(
      "h_axis", [](unsigned int n) { h_axis(n, epix_label_posn::c); },
      nb::arg("n") = 10);
  m.def(
      "v_axis", [](unsigned int n) { v_axis(n, epix_label_posn::c); },
      nb::arg("n") = 10);

  // ---- plot: f may return P (parametric) or float (graph y=f(t)) ----
  m.def(
      "plot",
      [](nb::callable f, double t_min, double t_max, unsigned int n) {
        g_fn = f;
        nb::object probe = f(t_min);
        if (nb::isinstance<P>(probe))
          plot(tramp_P, t_min, t_max, n);
        else
          plot(tramp_d, t_min, t_max, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);

  // plot a function over a domain. Two forms share this Python signature; pick
  // by the callable's arity: f(u, v) -> P (parametric surface wireframe) vs
  // f(x, y, z) -> P (image of a 3-D box, e.g. a cube's edges).
  m.def(
      "plot",
      [](nb::callable f, const domain& R) {
        // Pick the form by probing the callable's arity (robust for nanobind
        // f(u,v) (surface wireframe) vs f(x,y,z) (3-D box image).
        if (callable_takes_two(f)) {
          g_fn2 = f;
          plot(tramp_P2, R);
          g_fn2 = nb::callable();
        } else {
          g_fn3 = f;
          plot(tramp_P3, R);
          g_fn3 = nb::callable();
        }
      },
      nb::arg("f"), nb::arg("domain"));

  // plot(F, domain_list) -- F over each domain in a list of slices (from
  // domain.slices2()/slices3()). 2-var vs 3-var by arity, like plot(F, domain).
  m.def(
      "plot",
      [](nb::callable f, const domain_list& dl) {
        if (callable_takes_two(f)) {
          g_fn2 = f;
          plot(tramp_P2, dl);
          g_fn2 = nb::callable();
        } else {
          g_fn3 = f;
          plot(tramp_P3, dl);
          g_fn3 = nb::callable();
        }
      },
      nb::arg("f"), nb::arg("domain"));

  // plot(f, p1, p2, coarse, fine) -- level-set/contour plot of the scalar field
  // f(x,y)->double over a rectangular region. (Only the double-valued form is
  // defined in libepix; the P-valued region overload is
  // declared-but-uncompiled, so it is intentionally not bound.)
  m.def(
      "plot",
      [](nb::callable f, const P& p1, const P& p2, const mesh& coarse,
         const mesh& fine) {
        g_fn2 = f;
        plot(tramp_d2, p1, p2, coarse, fine);
        g_fn2 = nb::callable();
      },
      nb::arg("f"), nb::arg("p1"), nb::arg("p2"), nb::arg("coarse"),
      nb::arg("fine"));

  // envelope of tangent lines to a parametric curve f(t) -> P
  m.def(
      "envelope",
      [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f;
        envelope(tramp_P, a, b, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);

  // aligned label overload: label(at, offset, text, align)
  m.def("label",
        static_cast<void (*)(const P&, const P&, const std::string&,
                             epix_label_posn)>(&label),
        nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));
  m.def("masklabel",
        static_cast<void (*)(const P&, const P&, const std::string&,
                             epix_label_posn)>(&masklabel),
        nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));

  // ---- markers ----
  m.def("marker", static_cast<void (*)(const P&, epix_mark_type)>(&marker),
        nb::arg("at"), nb::arg("kind"));
  m.def("dot", [](const P& p) { dot(p); }, nb::arg("at"));
  m.def(
      "dot", [](const P& p, const std::string& s) { dot(p, P(0, 0), s); },
      nb::arg("at"), nb::arg("text"));
  m.def("dot",
        static_cast<void (*)(const P&, const P&, const std::string&,
                             epix_label_posn)>(&dot),
        nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));
  m.def("box", [](const P& p) { box(p); }, nb::arg("at"));
  m.def(
      "arrow",
      [](const P& tail, const P& head) {
        arrow(tail, head, P(0, 0), "", epix_label_posn::none);
      },
      nb::arg("tail"), nb::arg("head"));
  // the plain scaled arrow (curves.h): tail -> head with arrowhead `scale`. No
  // default on `scale`, so a 2-arg arrow() still resolves to the labelled form
  // above (registration order) and existing ports are unaffected.
  m.def("arrow", static_cast<void (*)(const P&, const P&, double)>(&arrow),
        nb::arg("tail"), nb::arg("head"), nb::arg("scale"));

  // ---- shapes ----
  m.def("circle", static_cast<void (*)(const P&, double, const P&)>(&circle),
        nb::arg("center") = P(0, 0, 0), nb::arg("radius") = 1.0,
        nb::arg("normal") = E_3);
  m.def("rect", static_cast<void (*)(const P&, const P&)>(&rect),
        nb::arg("lower_left"), nb::arg("upper_right"));
  m.def("triangle", &triangle, nb::arg("a"), nb::arg("b"), nb::arg("c"));
  m.def("quad", &quad, nb::arg("a"), nb::arg("b"), nb::arg("c"), nb::arg("d"));
  // fractal: a Python list of ints is the seed (passed as the C int*).
  m.def(
      "fractal",
      [](const P& p, const P& q, int depth, const std::vector<int>& pre_seed) {
        fractal(p, q, depth, pre_seed.data());
      },
      nb::arg("p"), nb::arg("q"), nb::arg("depth"), nb::arg("pre_seed"));
  m.def("ellipse",
        static_cast<void (*)(const P&, const P&, const P&)>(&ellipse),
        nb::arg("center"), nb::arg("axis1"), nb::arg("axis2"));
  m.def("ellipse", static_cast<void (*)(const P&, const P&)>(&ellipse),
        nb::arg("center"), nb::arg("radius"));
  m.def("dart", &dart, nb::arg("tail"), nb::arg("head"));

  // ---- 3-D viewpoint + angle mode ----
  m.def("viewpoint", static_cast<void (*)(double, double, double)>(&viewpoint),
        nb::arg("x"), nb::arg("y"), nb::arg("z"));
  m.def("viewpoint", static_cast<void (*)(const P&)>(&viewpoint),
        nb::arg("at"));
  m.def("radians", &radians);
  m.def("degrees", &degrees);
  m.def("revolutions", &revolutions);

  // ---- line styles + clipping ----
  m.def("solid", &solid);
  m.def("dashed", &dashed);
  m.def("dotted", &dotted);
  m.def("line_style", &line_style, nb::arg("style") = "-");
  m.def("clip_box", static_cast<void (*)(const P&)>(&clip_box),
        nb::arg("corner"));
  m.def("clip_box", static_cast<void (*)(const P&, const P&)>(&clip_box),
        nb::arg("lower_left"), nb::arg("upper_right"));
  m.def("clip_box", static_cast<void (*)()>(&clip_box));
  m.def("clip", &clip, nb::arg("on") = true);
  m.def("set_crop", &set_crop, nb::arg("on") = true);
  m.def("crop", static_cast<void (*)()>(&crop));

  // ---- axis (annotated axis; methods chain, returning axis&) ----
  nb::class_<axis>(m, "Axis")
      .def(nb::init<const P&, const P&, unsigned int, const P&,
                    epix_label_posn>(),
           nb::arg("tail"), nb::arg("head"), nb::arg("n"),
           nb::arg("offset") = P(0, 0),
           nb::arg("align") = epix_label_posn::none)
      .def("draw", &axis::draw)
      .def("draw_labels", &axis::draw_labels)
      .def("draw_ticks", &axis::draw_ticks)
      .def("unmark", &axis::unmark, nb::arg("at") = 0.0,
           nb::rv_policy::reference)
      .def("frac", &axis::frac, nb::rv_policy::reference)
      .def("trig", &axis::trig, nb::rv_policy::reference)
      .def("sci", &axis::sci, nb::rv_policy::reference)
      .def("log", &axis::log, nb::arg("base") = 10, nb::rv_policy::reference)
      .def("align", &axis::align, nb::arg("position"), nb::rv_policy::reference)
      .def("align_labels", &axis::align_labels, nb::arg("position"),
           nb::rv_policy::reference)
      .def("subdivide", &axis::subdivide, nb::arg("n"),
           nb::rv_policy::reference)
      .def("precision", &axis::precision, nb::arg("digits") = 0,
           nb::rv_policy::reference)
      .def("tick_ratio", &axis::tick_ratio, nb::arg("r"),
           nb::rv_policy::reference);

  // axis factory free-functions (return an axis to chain on)
  m.def("top_axis", &top_axis, nb::arg("n"), nb::arg("offset") = P(0, 0),
        nb::arg("align") = epix_label_posn::t);
  m.def("bottom_axis", &bottom_axis, nb::arg("n"), nb::arg("offset") = P(0, 0),
        nb::arg("align") = epix_label_posn::b);
  m.def("left_axis", &left_axis, nb::arg("n"), nb::arg("offset") = P(0, 0),
        nb::arg("align") = epix_label_posn::l);
  m.def("right_axis", &right_axis, nb::arg("n"), nb::arg("offset") = P(0, 0),
        nb::arg("align") = epix_label_posn::r);

  // ---- legend (boxed key; methods chain, returning legend&) ----
  nb::class_<legend>(m, "Legend")
      .def(nb::init<>())
      .def("path_item", &legend::path_item, nb::arg("text"),
           nb::rv_policy::reference)
      .def("fill_item", &legend::fill_item, nb::arg("text"),
           nb::rv_policy::reference)
      .def("mark_item", &legend::mark_item, nb::arg("kind"), nb::arg("text"),
           nb::rv_policy::reference)
      .def("border",
           static_cast<legend& (legend::*)(const Color&, double)>(
               &legend::border),
           nb::arg("color"), nb::arg("width"), nb::rv_policy::reference)
      .def("border", static_cast<legend& (legend::*)(double)>(&legend::border),
           nb::arg("width"), nb::rv_policy::reference)
      .def("item_border",
           static_cast<legend& (legend::*)(const Color&, double)>(
               &legend::item_border),
           nb::arg("color"), nb::arg("width"), nb::rv_policy::reference)
      .def("item_border",
           static_cast<legend& (legend::*)(double)>(&legend::item_border),
           nb::arg("width"), nb::rv_policy::reference)
      .def("backing", &legend::backing, nb::arg("color"),
           nb::rv_policy::reference)
      .def("draw", &legend::draw, nb::arg("loc"), nb::arg("offset") = P(0, 0),
           nb::arg("align") = epix_label_posn::c);

  // ---- derivative / integral plots (Deriv/Integral wrap a function ptr) ----
  m.def(
      "plot_deriv",
      [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f;
        plot(Deriv(tramp_d), a, b, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);
  m.def(
      "plot_integral",
      [](nb::callable f, double x0, double a, double b, unsigned int n) {
        g_fn = f;
        plot(Integral(tramp_d, x0), a, b, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("x0"), nb::arg("t_min"), nb::arg("t_max"),
      nb::arg("n") = 200);
  // evaluate the numerical integral of f from x0 to t (mirrors
  // Integral(f,x0).eval(t)).
  m.def(
      "integral_eval",
      [](nb::callable f, double t, double x0) {
        g_fn = f;
        double r = Integral(tramp_d, x0).eval(t);
        g_fn = nb::callable();
        return r;
      },
      nb::arg("f"), nb::arg("t"), nb::arg("x0") = 0.0);
  m.def(
      "slope_field",
      [](nb::callable F, const P& p, const P& q, unsigned int n1,
         unsigned int n2) {
        g_fn2 = F;
        slope_field(tramp_P2, p, q, n1, n2);
        g_fn2 = nb::callable();
      },
      nb::arg("F"), nb::arg("lower_left"), nb::arg("upper_right"),
      nb::arg("nx"), nb::arg("ny"));

  // evaluate the numerical derivative of f at t; returns the P whose .x2() is
  // the slope (mirrors C++ `Deriv(f)(t)`).
  m.def(
      "deriv",
      [](nb::callable f, double t) {
        g_fn = f;
        P p = Deriv(tramp_d)(P(t));
        g_fn = nb::callable();
        return p;
      },
      nb::arg("f"), nb::arg("t"));

  // numerical derivative value f'(t) (mirrors C++ `Deriv(f).eval(t)`).
  m.def(
      "deriv_eval",
      [](nb::callable f, double t) {
        g_fn = f;
        double r = Deriv(tramp_d).eval(t);
        g_fn = nb::callable();
        return r;
      },
      nb::arg("f"), nb::arg("t"));

  // ---- Camera (the global `camera`) + sphere drawing ----
  nb::class_<Camera>(m, "Camera")
      .def("at", static_cast<Camera& (Camera::*)(const P&)>(&Camera::at),
           nb::arg("at"), nb::rv_policy::reference)
      .def(
          "at",
          static_cast<Camera& (Camera::*)(double, double, double)>(&Camera::at),
          nb::rv_policy::reference)
      .def("look_at",
           static_cast<Camera& (Camera::*)(const P&)>(&Camera::look_at),
           nb::arg("at"), nb::rv_policy::reference)
      .def("range", &Camera::range, nb::arg("r"), nb::rv_policy::reference)
      .def("filter", &Camera::filter, nb::arg("color"),
           nb::rv_policy::reference)
      .def("eye", &Camera::eye)
      .def("viewpt", &Camera::viewpt);
  m.attr("camera") = nb::cast(&camera, nb::rv_policy::reference);
  m.def("cam", &cam, nb::rv_policy::reference);  // the global camera, as a fn

  m.def(
      "sphere", [](const P& ctr, double rad) { sphere(ctr, rad); },
      nb::arg("center") = P(0, 0, 0), nb::arg("radius") = 1.0);
  m.def(
      "latitude", [](double lat, double a, double b) { latitude(lat, a, b); },
      nb::arg("lat"), nb::arg("long_min"), nb::arg("long_max"));
  m.def(
      "longitude", [](double lng, double a, double b) { longitude(lng, a, b); },
      nb::arg("lng"), nb::arg("lat_min"), nb::arg("lat_max"));

  // hidden/visible plotting on the sphere: f, g are two scalar functions
  m.def(
      "backplot_N",
      [](nb::callable f, nb::callable g, double a, double b, int n) {
        g_fa = f;
        g_fb = g;
        backplot_N(tramp_a, tramp_b, a, b, n);
        g_fa = nb::callable();
        g_fb = nb::callable();
      },
      nb::arg("f"), nb::arg("g"), nb::arg("t_min"), nb::arg("t_max"),
      nb::arg("n"));
  m.def(
      "frontplot_N",
      [](nb::callable f, nb::callable g, double a, double b, int n) {
        g_fa = f;
        g_fb = g;
        frontplot_N(tramp_a, tramp_b, a, b, n);
        g_fa = nb::callable();
        g_fb = nb::callable();
      },
      nb::arg("f"), nb::arg("g"), nb::arg("t_min"), nb::arg("t_max"),
      nb::arg("n"));

  // ---- 3-D: mesh / domain / surface ----
  nb::class_<mesh>(m, "Mesh")
      .def(nb::init<int, int, int>(), nb::arg("nx"), nb::arg("ny"),
           nb::arg("nz") = 1)
      .def(nb::init<int>(), nb::arg("n"))
      .def(nb::init<>());

  nb::class_<domain>(m, "Domain")
      .def(nb::init<const P&, const P&, const mesh&, const mesh&>(),
           nb::arg("lower_left"), nb::arg("upper_right"), nb::arg("coarse"),
           nb::arg("fine"))
      .def(nb::init<const P&, const P&, const mesh&>(), nb::arg("lower_left"),
           nb::arg("upper_right"), nb::arg("coarse"))
      .def("resize1", &domain::resize1, nb::arg("a"), nb::arg("b"))
      .def("resize2", &domain::resize2, nb::arg("a"), nb::arg("b"))
      .def("resize3", &domain::resize3, nb::arg("a"), nb::arg("b"))
      .def("slice1", &domain::slice1, nb::arg("a"))
      .def("slice2", &domain::slice2, nb::arg("a"))
      .def("slice3", &domain::slice3, nb::arg("a"))
      // slices2/3 return a std::list<domain>; wrap it into a domain_list (the
      // implicit conversion the C++ relies on) so plot(F, domain_list) works.
      .def(
          "slices2",
          [](const domain& d, unsigned int n) {
            return domain_list(d.slices2(n));
          },
          nb::arg("n") = 0)
      .def(
          "slices3",
          [](const domain& d, unsigned int n) {
            return domain_list(d.slices3(n));
          },
          nb::arg("n") = 0);

  nb::class_<domain_list>(m, "DomainList");

  // surface: F(u, v) -> P over a domain (parametric surface), same trampoline
  // idea
  m.def(
      "surface",
      [](nb::callable f, const domain& R, int cull) {
        g_fn2 = f;
        surface(tramp_P2, R, cull);
        g_fn2 = nb::callable();
      },
      nb::arg("f"), nb::arg("domain"), nb::arg("cull") = 0);

  // surface of revolution: profile (f(t), g(t)) revolved about the frame's
  // axis over a domain. Two scalar function-ptrs -> the g_fa/g_fb trampolines.
  m.def(
      "surface_rev",
      [](nb::callable f, nb::callable g, const domain& R, const frame& coords,
         int cull) {
        g_fa = f;
        g_fb = g;
        surface_rev(tramp_a, tramp_b, R, coords, cull);
        g_fa = nb::callable();
        g_fb = nb::callable();
      },
      nb::arg("f"), nb::arg("g"), nb::arg("domain"), nb::arg("coords"),
      nb::arg("cull") = 0);

  // ===== more state setters, shapes, axis labels, and field/polar plots =====
  m.def("set_black", &black,
        nb::arg("intensity") = 1.0);  // set pen color to a tint
  m.def("set_blue", &blue, nb::arg("intensity") = 1.0);
  m.def("set_green", &green, nb::arg("intensity") = 1.0);
  m.def("set_yellow", &yellow, nb::arg("intensity") = 1.0);
  m.def("set_rgb", static_cast<void (*)(double, double, double)>(&rgb),
        nb::arg("r"), nb::arg("g"), nb::arg("b"));
  m.def("arrow_inset", &arrow_inset, nb::arg("inset") = 0.0);
  m.def("arrow_width", &arrow_width, nb::arg("width") = 3.0);
  m.def("arrow_fill", &arrow_fill, nb::arg("on") = true);
  m.def("v_error_bar", &v_error_bar, nb::arg("loc"), nb::arg("error"),
        nb::arg("kind"), nb::arg("width") = 6.0);
  m.def("h_axis_masklabels",
        static_cast<void (*)(const P&, const P&, unsigned int, const P&,
                             epix_label_posn)>(&h_axis_masklabels),
        nb::arg("tail"), nb::arg("head"), nb::arg("n"), nb::arg("offset"),
        nb::arg("align"));
  m.def("h_axis_labels",
        static_cast<void (*)(const P&, const P&, unsigned int, const P&,
                             epix_label_posn)>(&h_axis_labels),
        nb::arg("tail"), nb::arg("head"), nb::arg("n"), nb::arg("offset"),
        nb::arg("align"));
  m.def("v_axis_labels",
        static_cast<void (*)(const P&, const P&, unsigned int, const P&,
                             epix_label_posn)>(&v_axis_labels),
        nb::arg("tail"), nb::arg("head"), nb::arg("n"), nb::arg("offset"),
        nb::arg("align"));

  m.def("picture", static_cast<void (*)(double, double)>(&picture),
        nb::arg("width"), nb::arg("height"));
  m.def("picture", static_cast<void (*)(const P&)>(&picture), nb::arg("size"));
  m.def("bounding_box", &bounding_box, nb::arg("lower_left"),
        nb::arg("upper_right"));
  m.def("unitlength", &unitlength, nb::arg("units"));
  m.def("pst_format", &pst_format);

  m.def("arc", &arc, nb::arg("center"), nb::arg("radius"), nb::arg("start"),
        nb::arg("finish"));
  m.def("polar_grid",
        static_cast<void (*)(double, unsigned int, unsigned int)>(&polar_grid),
        nb::arg("radius"), nb::arg("nx"), nb::arg("ny"));
  m.def("h_axis_labels",
        static_cast<void (*)(unsigned int, const P&, epix_label_posn)>(
            &h_axis_labels),
        nb::arg("n"), nb::arg("offset"),
        nb::arg("align") = epix_label_posn::none);
  m.def("v_axis_labels",
        static_cast<void (*)(unsigned int, const P&, epix_label_posn)>(
            &v_axis_labels),
        nb::arg("n"), nb::arg("offset"),
        nb::arg("align") = epix_label_posn::none);

  // function-pointer plots/fields (trampolines)
  m.def(
      "polarplot",
      [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f;
        polarplot(tramp_d, a, b, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);
  m.def(
      "tan_field",
      [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f;
        tan_field(tramp_P, a, b, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n"));
  m.def(
      "dart_field",
      [](nb::callable F, const P& p, const P& q, unsigned int n1,
         unsigned int n2) {
        g_fn2 = F;
        dart_field(tramp_P2, p, q, n1, n2);
        g_fn2 = nb::callable();
      },
      nb::arg("F"), nb::arg("lower_left"), nb::arg("upper_right"),
      nb::arg("nx"), nb::arg("ny"));
  m.def(
      "ode_plot",
      [](nb::callable F, const P& start, double t_max, unsigned int n) {
        g_fn2 = F;
        ode_plot(tramp_P2, start, t_max, n);
        g_fn2 = nb::callable();
      },
      nb::arg("F"), nb::arg("start"), nb::arg("t_max"), nb::arg("n"));

  // 3-D fields: F(x,y,z) -> P. (arity-probe shared with the 2-D forms; F is
  // tried with 2 args -> 2-var, else 3-var.)
  m.def(
      "ode_plot",
      [](nb::callable F, const P& start, double a, double b, unsigned int n) {
        if (callable_takes_two(F)) {
          g_fn2 = F;
          ode_plot(tramp_P2, start, a, b, n);
          g_fn2 = nb::callable();
        } else {
          g_fn3 = F;
          ode_plot(tramp_P3, start, a, b, n);
          g_fn3 = nb::callable();
        }
      },
      nb::arg("F"), nb::arg("start"), nb::arg("t_min"), nb::arg("t_max"),
      nb::arg("n"));
  m.def(
      "dart_field",
      [](nb::callable F, const domain& R, double scale) {
        if (callable_takes_two(F)) {
          g_fn2 = F;
          dart_field(tramp_P2, R, scale);
          g_fn2 = nb::callable();
        } else {
          g_fn3 = F;
          dart_field(tramp_P3, R, scale);
          g_fn3 = nb::callable();
        }
      },
      nb::arg("F"), nb::arg("domain"), nb::arg("scale") = 1.0);
  m.def(
      "vector_field",
      [](nb::callable F, const domain& R, double scale) {
        if (callable_takes_two(F)) {
          g_fn2 = F;
          vector_field(tramp_P2, R, scale);
          g_fn2 = nb::callable();
        } else {
          g_fn3 = F;
          vector_field(tramp_P3, R, scale);
          g_fn3 = nb::callable();
        }
      },
      nb::arg("F"), nb::arg("domain"), nb::arg("scale") = 1.0);

  // ---- pair (2-D ordered pair, complex-like; e.g. demoivre) ----
  nb::class_<pair>(m, "Pair")
      .def(nb::init<double, double>(), nb::arg("x1") = 0.0, nb::arg("x2") = 0.0)
      .def("x1", &pair::x1)
      .def("x2", &pair::x2)
      .def("__mul__",
           [](pair a, const pair& b) {
             a *= b;
             return a;
           })
      .def("__mul__",
           [](pair a, double c) {
             a *= c;
             return a;
           })
      .def("__rmul__",
           [](pair a, double c) {
             a *= c;
             return a;
           })
      .def("__add__", [](pair a, const pair& b) { return a + b; })
      .def("__sub__", [](pair a, const pair& b) { return a - b; });

  // ---- Plane (drawable plane region) ----
  nb::class_<Plane>(m, "Plane")
      .def(nb::init<const P&, const P&>(), nb::arg("pt") = P(0, 0, 0),
           nb::arg("normal") = E_3)
      .def(nb::init<const P&, const P&, const P&>())
      .def("shift", &Plane::shift, nb::arg("v"), nb::rv_policy::reference)
      .def("draw", &Plane::draw);

  // ---- Sphere (the geometric object) + frame + the platonic-solid drawers
  // ----
  nb::class_<Circle>(m, "Circle")
      .def(nb::init<const P&, double, const P&>(),
           nb::arg("center") = P(0, 0, 0), nb::arg("radius") = 1.0,
           nb::arg("normal") = E_3)
      .def("center", &Circle::center)
      .def("draw", &Circle::draw);

  nb::class_<Sphere>(m, "Sphere")
      .def(nb::init<const P&, double>(), nb::arg("center") = P(0, 0, 0),
           nb::arg("radius") = 1.0)
      .def(nb::init<const P&, const P&>(), nb::arg("center"), nb::arg("pt"))
      .def("center", &Sphere::center)
      .def("draw", &Sphere::draw)
      // Sphere * Sphere -> Circle (their intersection circle)
      .def("__mul__", [](const Sphere& a, const Sphere& b) { return a * b; });

  nb::class_<frame>(m, "Frame")
      .def(nb::init<>())
      .def(nb::init<P, P, P>(), nb::arg("e1"), nb::arg("e2"), nb::arg("e3"))
      .def("sea", &frame::sea)
      .def("sky", &frame::sky)
      .def("eye", &frame::eye);

  m.def("back_dodeca", &back_dodeca, nb::arg("sphere"), nb::arg("frame"));
  m.def("back_icosa", &back_icosa, nb::arg("sphere"), nb::arg("frame"));
  m.def("front_dodeca", &front_dodeca, nb::arg("sphere"), nb::arg("frame"));
  m.def("front_icosa", &front_icosa, nb::arg("sphere"), nb::arg("frame"));

  // ---- Complex (the plane-complex value type) + complex nth root ----
  nb::class_<Complex>(m, "Complex")
      .def(nb::init<double, double>(), nb::arg("real") = 0.0,
           nb::arg("imag") = 0.0)
      .def("__mul__", [](Complex a, const Complex& b) { return a * b; })
      .def("__add__", [](Complex a, const Complex& b) { return a + b; })
      .def("__sub__", [](Complex a, const Complex& b) { return a - b; });
  // the order-th root, branch `branch`, returned as a P (re, im, 0) -- ePiX
  // implicitly converts Complex -> P, which is how the .xp samples plot it.
  m.def(
      "root_complex",
      [](const Complex& z, int order, int branch) {
        return P(rootC(z, order, branch));
      },
      nb::arg("z"), nb::arg("order"), nb::arg("branch") = 0);

  // ---- data_file / data_bins (tabular data + histograms) ----
  nb::class_<data_file>(m, "DataFile")
      // build two columns by sampling f1, f2 over [t_min, t_max] (n+1 rows);
      // the two scalar function-ptrs route through the g_fa/g_fb trampolines.
      .def(
          "__init__",
          [](data_file* self, nb::callable f1, nb::callable f2, double t_min,
             double t_max, unsigned int n) {
            g_fa = f1;
            g_fb = f2;
            new (self) data_file(tramp_a, tramp_b, t_min, t_max, n);
            g_fa = nb::callable();
            g_fb = nb::callable();
          },
          nb::arg("f1"), nb::arg("f2"), nb::arg("t_min"), nb::arg("t_max"),
          nb::arg("n"))
      .def(
          "transform",
          [](data_file& df, nb::callable f) -> data_file& {
            g_fn2 = f;
            df.transform(tramp_P2);  // P f(double,double), cols 1,2
            g_fn2 = nb::callable();
            return df;
          },
          nb::arg("f"), nb::rv_policy::reference)
      .def(
          "column",
          [](const data_file& df, unsigned int col) { return df.column(col); },
          nb::arg("color"))
      .def(
          "plot",
          [](const data_file& df, epix_mark_type type) { df.plot(type); },
          nb::arg("type"));

  nb::class_<data_bins>(m, "DataBins")
      .def(nb::init<double, double, unsigned int>(), nb::arg("low"),
           nb::arg("high"), nb::arg("n") = 1)
      .def(
          "read",
          [](data_bins& db, const std::vector<double>& v) -> data_bins& {
            return db.read(v);
          },
          nb::arg("data"), nb::rv_policy::reference)
      .def("pop", &data_bins::pop)
      .def("bar_chart", &data_bins::bar_chart, nb::arg("scale") = 1.0);

  // ---- integral type enum + Riemann sums ----
  nb::enum_<epix_integral_type>(m, "IntegralType")
      .value("LEFT", epix_integral_type::LEFT)
      .value("RIGHT", epix_integral_type::RIGHT)
      .value("UPPER", epix_integral_type::UPPER)
      .value("LOWER", epix_integral_type::LOWER)
      .value("TRAP", epix_integral_type::TRAP)
      .value("MIDPT", epix_integral_type::MIDPT);
  m.def(
      "riemann_sum",
      [](nb::callable f, double a, double b, unsigned int n,
         epix_integral_type t) {
        g_fn = f;
        riemann_sum(tramp_d, a, b, n, t);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("a"), nb::arg("b"), nb::arg("n"), nb::arg("type"));

  // ---- misc state / markers / formats ----
  m.def("set_white", &white, nb::arg("intensity") = 1.0);
  m.def("set_magenta", &magenta, nb::arg("intensity") = 1.0);
  m.def("set_gray", &gray, nb::arg("depth") = 0.3);
  m.def("ring", [](const P& p) { ring(p); }, nb::arg("at"));
  m.def("neutral", &Neutral);  // the neutral (50% gray) Color
  m.def("rgb_neutral", &RGB_Neutral);
  m.def("cmy_neutral", &CMY_Neutral);
  m.def("cmyk_neutral", &CMYK_Neutral);
  m.def("c_process", &C_Process, nb::arg("intensity") = 1.0);
  m.def("m_process", &M_Process, nb::arg("intensity") = 1.0);
  m.def("y_process", &Y_Process, nb::arg("intensity") = 1.0);
  m.def("k_process", &K_Process, nb::arg("intensity") = 1.0);
  m.def("tikz_format", &tikz_format);
  m.def("dot_size", &dot_size, nb::arg("diameter") = 0.0);
  m.def("ddot", [](const P& p) { ddot(p); }, nb::arg("at"));
  m.def("ddot",
        static_cast<void (*)(const P&, const P&, const std::string&,
                             epix_label_posn)>(&ddot),
        nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));

  // ---- clipping by half-space / label color / base pen ----
  m.def("label_color", &label_color, nb::arg("color"));
  m.def("label_border", static_cast<void (*)(const Color&)>(&label_border),
        nb::arg("color"));
  m.def("label_border",
        static_cast<void (*)(const Color&, double)>(&label_border),
        nb::arg("color"), nb::arg("width"));
  m.def("label_border",
        static_cast<void (*)(const Color&, std::string)>(&label_border),
        nb::arg("color"), nb::arg("width"));
  m.def("label_mask", static_cast<void (*)(const Color&)>(&label_mask),
        nb::arg("color"));
  m.def("label_mask", static_cast<void (*)()>(&label_mask));
  m.def("label_pad", &label_pad, nb::arg("pad"));
  m.def("clip_face", &clip_face, nb::arg("loc"), nb::arg("normal"));
  m.def("clip_restore", &clip_restore);
  m.def("clip_slice",
        static_cast<void (*)(const P&, const P&, double)>(&clip_slice),
        nb::arg("loc"), nb::arg("normal"), nb::arg("thickness"));
  m.def("base", static_cast<void (*)(const Color&)>(&base), nb::arg("color"));
  m.def("base", static_cast<void (*)(const Color&, double)>(&base),
        nb::arg("color"), nb::arg("width"));
  m.def("base", static_cast<void (*)(const Color&, std::string)>(&base),
        nb::arg("color"), nb::arg("width"));

  // surface with a per-position color function: f(u,v)->P, color(x,y,z)->RGB
  m.def(
      "surface",
      [](nb::callable f, const domain& R, nb::callable color, int cull) {
        g_fn2 = f;
        // color(u,v) over the domain vs color(x,y,z) position
        if (callable_takes_two(color)) {
          g_col = color;
          surface(tramp_P2, R, tramp_col2, cull);
          g_col = nb::callable();
        } else {
          g_fn3 = color;
          surface(tramp_P2, R, tramp_P3, cull);
          g_fn3 = nb::callable();
        }
        g_fn2 = nb::callable();
      },
      nb::arg("f"), nb::arg("domain"), nb::arg("color"), nb::arg("cull") = 0);

  // scenery: a reusable surface object, built from one or more surfaces
  // (f(u,v) or f(x,y,z) over a domain). add() chains; draw() renders.
  nb::class_<PyScenery>(m, "Scenery")
      .def(nb::init<nb::callable, const domain&>(), nb::arg("f"),
           nb::arg("domain"))
      .def(nb::init<nb::callable, const domain&, nb::callable>(), nb::arg("f"),
           nb::arg("domain"), nb::arg("color"))
      .def("add", &PyScenery::add, nb::arg("f"), nb::arg("domain"),
           nb::rv_policy::reference_internal)
      .def("add", &PyScenery::add_c, nb::arg("f"), nb::arg("domain"),
           nb::arg("color"), nb::rv_policy::reference_internal)
      .def(
          "draw", [](PyScenery& s, int cull) { s.sc.draw(cull); },
          nb::arg("cull") = 0);

  // ---- screen (off-screen surface for multi-panel layouts) + inset/border
  // ----
  nb::class_<screen>(m, "Screen")
      .def(nb::init<const P&, const P&>(), nb::arg("lower_left"),
           nb::arg("upper_right"))
      .def(
          "scale", [](screen& s, double f) -> screen& { return s.scale(f); },
          nb::arg("factor"), nb::rv_policy::reference)
      .def(
          "rotate",
          [](screen& s, double th) -> screen& { return s.rotate(th); },
          nb::arg("theta"), nb::rv_policy::reference)
      .def(
          "reflect",
          [](screen& s, double th) -> screen& { return s.reflect(th); },
          nb::arg("theta"), nb::rv_policy::reference)
      .def("apply", &screen::apply, nb::arg("f"), nb::rv_policy::reference)
      .def("set_crop", &screen::set_crop, nb::arg("on") = true,
           nb::rv_policy::reference)
      .def("tr", &screen::tr)
      .def("tl", &screen::tl)
      .def("bl", &screen::bl)
      .def("br", &screen::br)
      .def("c", &screen::c)
      .def("extract_ellipse", &screen::extract_ellipse, nb::arg("lower_left"),
           nb::arg("upper_right"))
      .def("backing",
           static_cast<screen& (screen::*)(const Color&)>(&screen::backing),
           nb::arg("color"), nb::rv_policy::reference)
      .def("border",
           static_cast<screen& (screen::*)(const Color&, const std::string&)>(
               &screen::border),
           nb::arg("color"), nb::arg("width"), nb::rv_policy::reference);
  m.def("activate", static_cast<void (*)(screen&)>(&activate),
        nb::arg("screen"));
  m.def("deactivate", static_cast<void (*)(screen&)>(&deactivate),
        nb::arg("screen"));
  m.def("inset", static_cast<void (*)(const P&, const P&)>(&inset),
        nb::arg("lower_left"), nb::arg("upper_right"));
  m.def("inset",
        static_cast<void (*)(const screen&, const P&, const P&)>(&inset),
        nb::arg("child"), nb::arg("lower_left"), nb::arg("upper_right"));
  m.def("border",
        static_cast<void (*)(const Color&, const std::string&)>(&border),
        nb::arg("color"), nb::arg("width"));
  m.def("backing", static_cast<void (*)(const Color&)>(&backing),
        nb::arg("color"));
  m.def("backing", static_cast<void (*)()>(&backing));

  // ---- Segment (line segment; intersections via *) ----
  nb::class_<Segment>(m, "Segment")
      .def(nb::init<const P&, const P&>(), nb::arg("tail"), nb::arg("head"))
      .def("draw", &Segment::draw, nb::arg("stretch") = 0.0)
      .def("midpoint", &Segment::midpoint, nb::arg("t") = 0.5)
      .def("__mul__", [](const Segment& a, const Segment& b) { return a * b; });

  // ---- more markers / transforms-related free functions ----
  m.def("circle", static_cast<void (*)(const P&, const P&, const P&)>(&circle),
        nb::arg("p1"), nb::arg("p2"), nb::arg("p3"));  // through 3 points
  m.def("aarrow", &aarrow, nb::arg("tail"), nb::arg("head"),
        nb::arg("scale") = 1.0);
  m.def(
      "tan_line",
      [](nb::callable f, double t) {
        g_fn = f;
        tan_line(tramp_d, t);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t"));
  m.def("masklabel",
        static_cast<void (*)(const P&, const std::string&)>(&masklabel),
        nb::arg("at"), nb::arg("text"));
  m.def("quarter_turn", static_cast<P (*)(P)>(&J),
        nb::arg("v"));  // quarter turn about E_3
  m.def("full_turn", &full_turn);
  m.def("font_face", &font_face, nb::arg("face") = "rm");
  m.def("canvas", &canvas, nb::rv_policy::reference);
  m.def("circ", [](const P& p) { circ(p); }, nb::arg("at"));
  m.def("circ",
        static_cast<void (*)(const P&, const P&, const std::string&,
                             epix_label_posn)>(&circ),
        nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));
  m.def("spot", [](const P& p) { spot(p); }, nb::arg("at"));
  m.def("right_angle", &right_angle, nb::arg("loc"), nb::arg("leg1"),
        nb::arg("leg2"), nb::arg("scale") = 8.0);
  m.def(
      "arc_arrow",
      [](const P& c, double r, double a, double b) { arc_arrow(c, r, a, b); },
      nb::arg("center"), nb::arg("radius"), nb::arg("start"),
      nb::arg("finish"));
  m.def(
      "flow",
      [](nb::callable F, const P& start, double t_max, unsigned int n) {
        if (callable_takes_two(F)) {
          g_fn2 = F;
          P p = flow(tramp_P2, start, t_max, n);
          g_fn2 = nb::callable();
          return p;
        }
        g_fn3 = F;
        P p = flow(tramp_P3, start, t_max, n);
        g_fn3 = nb::callable();
        return p;
      },
      nb::arg("F"), nb::arg("start"), nb::arg("t_max"), nb::arg("n") = 0);

  // ---- path (build a curve point-by-point or as an arc; close/fill/draw) ----
  nb::class_<path>(m, "Path")
      .def(nb::init<>())
      .def(nb::init<const std::vector<P>&, bool, bool>(), nb::arg("data"),
           nb::arg("closed"), nb::arg("filled"))
      .def(nb::init<const P&, const P&, double>(), nb::arg("tail"),
           nb::arg("head"), nb::arg("expand") = 0.0)
      .def(nb::init<const P&, const P&, const P&, double, double>(),
           nb::arg("center"), nb::arg("axis1"), nb::arg("axis2"),
           nb::arg("t_min"), nb::arg("t_max"))
      .def("pt", static_cast<path& (path::*)(const P&)>(&path::pt),
           nb::arg("p"), nb::rv_policy::reference)
      .def("close", &path::close, nb::rv_policy::reference)
      .def("fill", static_cast<path& (path::*)(bool)>(&path::fill),
           nb::arg("on") = true, nb::rv_policy::reference)
      .def("draw", static_cast<void (path::*)() const>(&path::draw))
      .def(
          "__iadd__",
          [](path& a, const path& b) -> path& {
            a += b;
            return a;
          },
          nb::rv_policy::reference);

  // ---- affine maps (compose reflect/rotate/shift/scale; apply to a screen)
  // ----
  nb::class_<affine>(m, "Affine")
      .def(nb::init<>())
      .def(nb::init<const pair&, const pair&, const pair&>(), nb::arg("e1"),
           nb::arg("e2"), nb::arg("loc") = pair(0, 0))
      .def(
          "shift", [](affine& a, const P& v) -> affine& { return a.shift(v); },
          nb::arg("v"), nb::rv_policy::reference)
      .def(
          "shift",
          [](affine& a, const pair& v) -> affine& { return a.shift(v); },
          nb::arg("v"), nb::rv_policy::reference)
      .def(
          "rotate",
          [](affine& a, double th) -> affine& { return a.rotate(th); },
          nb::arg("theta"), nb::rv_policy::reference)
      .def(
          "reflect",
          [](affine& a, double th) -> affine& { return a.reflect(th); },
          nb::arg("theta"), nb::rv_policy::reference)
      .def(
          "scale", [](affine& a, double s) -> affine& { return a.scale(s); },
          nb::arg("factor"), nb::rv_policy::reference)
      .def(
          "h_scale",
          [](affine& a, double s) -> affine& { return a.h_scale(s); },
          nb::arg("factor"), nb::rv_policy::reference)
      .def(
          "v_scale",
          [](affine& a, double s) -> affine& { return a.v_scale(s); },
          nb::arg("factor"), nb::rv_policy::reference)
      .def(
          "__call__", [](const affine& a, const pair& p) { return a(p); },
          nb::arg("p"));

  // ---- functions ----
  m.def("sin", &Sin);
  m.def("cos", &Cos);
  m.def("tan", &Tan);
  m.def("atan", &Atan);
  m.def("atan2", &Atan2, nb::arg("y"), nb::arg("x"));
  m.def("asin", &Asin);
  m.def("acos", &Acos);
  m.def("cb", &cb);
  m.def("cis", &cis, nb::arg("t"));
  m.def("recip", &recip);
  m.def("sgn", &sgn, nb::arg("x"));
  m.def("axis_break", &axis_break, nb::arg("tail"), nb::arg("head"),
        nb::arg("scale") = 12.0);
  m.def(
      "shadeplot",
      [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f;
        shadeplot(tramp_d, a, b, n);
        g_fn = nb::callable();
      },
      nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n"));
}
