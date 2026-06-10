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
// surface grows -- libepix has latent uninitialized-pimpl bugs (screen was one).

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "epix.h"

namespace nb = nanobind;
using namespace ePiX;

// ---- plot() trampolines: ePiX plot takes raw C function pointers, so we route
// through a module-global Python callable (one plot at a time; fine for notebooks).
static nb::callable g_fn;
static P      tramp_P(double t) { return nb::cast<P>(g_fn(t)); }
static double tramp_d(double t) { return nb::cast<double>(g_fn(t)); }
// two-variable trampoline for surface(P F(double,double), ...)
static nb::callable g_fn2;
static P tramp_P2(double u, double v) { return nb::cast<P>(g_fn2(u, v)); }
// two-function trampolines for back/frontplot_N(double f(double), double g(double), ...)
static nb::callable g_fa, g_fb;
static double tramp_a(double t) { return nb::cast<double>(g_fa(t)); }
static double tramp_b(double t) { return nb::cast<double>(g_fb(t)); }
// three-variable trampoline for plot(P f(double,double,double), domain)
static nb::callable g_fn3;
static P tramp_P3(double x, double y, double z) { return nb::cast<P>(g_fn3(x, y, z)); }

NB_MODULE(_epix, m) {
    m.doc() = "Low-level nanobind bindings over libepix (use the `epix` package).";

    // ---- P (ordered triple / 3-vector) ----
    nb::class_<P>(m, "P")
        .def(nb::init<double, double, double>(),
             nb::arg("x") = 0.0, nb::arg("y") = 0.0, nb::arg("z") = 0.0)
        .def("x1", &P::x1).def("x2", &P::x2).def("x3", &P::x3)
        .def("__add__", [](const P& a, const P& b) { return a + b; })
        .def("__sub__", [](const P& a, const P& b) { return a - b; })
        .def("__neg__", [](const P& a) { return -a; })
        .def("__mul__", [](const P& a, double c) { return c * a; })        // P * scalar
        .def("__rmul__", [](const P& a, double c) { return c * a; })       // scalar * P
        .def("__xor__", [](const P& a, const P& b) { return a * b; })      // cross: a ^ b
        .def("dot", [](const P& a, const P& b) { return a | b; })
        .def("norm", [](const P& a) { return norm(a); })
        .def("__repr__", [](const P& p) {
            return "P(" + std::to_string(p.x1()) + ", " + std::to_string(p.x2())
                 + ", " + std::to_string(p.x3()) + ")";
        });

    // ---- Color (opaque value type; constructed via the color functions below) ----
    nb::class_<Color>(m, "Color");

    // ---- scoped enums (the C++ enum class types) ----
    nb::enum_<epix_mark_type>(m, "MarkType")
        .value("PATH", epix_mark_type::PATH).value("CIRC", epix_mark_type::CIRC)
        .value("SPOT", epix_mark_type::SPOT).value("RING", epix_mark_type::RING)
        .value("DOT", epix_mark_type::DOT).value("DDOT", epix_mark_type::DDOT)
        .value("PLUS", epix_mark_type::PLUS).value("OPLUS", epix_mark_type::OPLUS)
        .value("TIMES", epix_mark_type::TIMES).value("OTIMES", epix_mark_type::OTIMES)
        .value("DIAMOND", epix_mark_type::DIAMOND).value("UP", epix_mark_type::UP)
        .value("DOWN", epix_mark_type::DOWN).value("BOX", epix_mark_type::BOX)
        .value("BBOX", epix_mark_type::BBOX).value("HTICK", epix_mark_type::HTICK)
        .value("VTICK", epix_mark_type::VTICK).value("TEXT", epix_mark_type::TEXT);

    nb::enum_<epix_label_posn>(m, "LabelPos")
        .value("none", epix_label_posn::none).value("c", epix_label_posn::c)
        .value("r", epix_label_posn::r).value("tr", epix_label_posn::tr)
        .value("rt", epix_label_posn::rt).value("t", epix_label_posn::t)
        .value("tl", epix_label_posn::tl).value("lt", epix_label_posn::lt)
        .value("l", epix_label_posn::l).value("bl", epix_label_posn::bl)
        .value("lb", epix_label_posn::lb).value("b", epix_label_posn::b)
        .value("br", epix_label_posn::br).value("rb", epix_label_posn::rb);

    // color constructors (return Color)
    m.def("RGB", &RGB, nb::arg("r") = 0.0, nb::arg("g") = 0.0, nb::arg("b") = 0.0);
    for (auto&& [name, fn] : {std::pair{"Black", &Black}, {"White", &White},
                              {"Red", &Red}, {"Green", &Green}, {"Blue", &Blue},
                              {"Yellow", &Yellow}, {"Cyan", &Cyan},
                              {"Magenta", &Magenta}})
        m.def(name, fn, nb::arg("d") = 1.0);

    // ---- picture setup / lifecycle ----
    m.def("picture",
          static_cast<void (*)(const P&, const P&, std::string)>(&picture),
          nb::arg("sw"), nb::arg("ne"), nb::arg("size"));
    m.def("begin", &begin);
    m.def("end_picture", &end_picture);
    m.def("print_eepic", &print_eepic, nb::arg("filename"));

    m.def("xmin", []() { return xmin(); });
    m.def("xmax", []() { return xmax(); });
    m.def("ymin", []() { return ymin(); });
    m.def("ymax", []() { return ymax(); });
    m.def("xsize", []() { return xsize(); });
    m.def("ysize", []() { return ysize(); });
    m.def("tix",  []() { return tix(); });
    m.def("set_tix", [](double t) { tix() = t; }, nb::arg("t"));  // animation frame param

    // ---- coordinate constructors (return P) ----
    m.def("xyz", &xyz, nb::arg("x"), nb::arg("y"), nb::arg("z") = 0.0);
    m.def("polar", &polar, nb::arg("r"), nb::arg("t"));
    m.def("sph", &sph, nb::arg("r"), nb::arg("t"), nb::arg("phi"));
    m.def("cyl", &cyl, nb::arg("r"), nb::arg("t"), nb::arg("z"));

    // ---- pen / fill / color state ----
    m.def("pen", static_cast<void (*)(const Color&)>(&pen), nb::arg("color"));
    m.def("pen", static_cast<void (*)(double)>(&pen), nb::arg("width"));
    m.def("pen", static_cast<void (*)(const Color&, double)>(&pen),
          nb::arg("color"), nb::arg("width"));
    m.def("pen", static_cast<void (*)(const Color&, std::string)>(&pen),
          nb::arg("color"), nb::arg("width"));   // width as a length string, e.g. "0.1pt"
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
    m.def("red", &red, nb::arg("d") = 1.0);          // set pen to a red tint
    m.def("label_angle", static_cast<void (*)(double)>(&label_angle), nb::arg("t") = 0.0);

    // ---- drawing ----
    m.def("label",
          static_cast<void (*)(const P&, const std::string&)>(&label),
          nb::arg("at"), nb::arg("text"));
    m.def("line",
          static_cast<void (*)(const P&, const P&, double)>(&line),
          nb::arg("tail"), nb::arg("head"), nb::arg("expand") = 0.0);
    m.def("Line", static_cast<void (*)(const P&, const P&)>(&Line),
          nb::arg("tail"), nb::arg("head"));   // infinite line through two points
    m.def("grid", static_cast<void (*)(unsigned int, unsigned int)>(&grid),
          nb::arg("n1") = 1, nb::arg("n2") = 1);
    m.def("grid", static_cast<void (*)(const P&, const P&, unsigned int, unsigned int)>(&grid),
          nb::arg("sw"), nb::arg("ne"), nb::arg("n1"), nb::arg("n2"));
    m.def("border", static_cast<void (*)()>(&border));
    m.def("h_axis", [](unsigned int n) { h_axis(n, epix_label_posn::c); }, nb::arg("n") = 10);
    m.def("v_axis", [](unsigned int n) { v_axis(n, epix_label_posn::c); }, nb::arg("n") = 10);

    // ---- plot: f may return P (parametric) or float (graph y=f(t)) ----
    m.def("plot", [](nb::callable f, double t_min, double t_max, unsigned int n) {
        g_fn = f;
        nb::object probe = f(t_min);
        if (nb::isinstance<P>(probe)) plot(tramp_P, t_min, t_max, n);
        else                          plot(tramp_d, t_min, t_max, n);
        g_fn = nb::callable();
    }, nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);

    // plot over a 3-D domain: f(x, y, z) -> P (e.g. drawing a cube's edges)
    m.def("plot", [](nb::callable f, const domain& R) {
        g_fn3 = f; plot(tramp_P3, R); g_fn3 = nb::callable();
    }, nb::arg("f"), nb::arg("domain"));

    // envelope of tangent lines to a parametric curve f(t) -> P
    m.def("envelope", [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f; envelope(tramp_P, a, b, n); g_fn = nb::callable();
    }, nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);

    // aligned label overload: label(at, offset, text, align)
    m.def("label",
          static_cast<void (*)(const P&, const P&, const std::string&, epix_label_posn)>(&label),
          nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));
    m.def("masklabel",
          static_cast<void (*)(const P&, const P&, const std::string&, epix_label_posn)>(&masklabel),
          nb::arg("at"), nb::arg("offset"), nb::arg("text"), nb::arg("align"));

    // ---- markers ----
    m.def("marker", static_cast<void (*)(const P&, epix_mark_type)>(&marker),
          nb::arg("at"), nb::arg("kind"));
    m.def("dot", [](const P& p) { dot(p); }, nb::arg("at"));
    m.def("dot", [](const P& p, const std::string& s) { dot(p, P(0, 0), s); },
          nb::arg("at"), nb::arg("text"));
    m.def("box", [](const P& p) { box(p); }, nb::arg("at"));
    m.def("arrow",
          [](const P& tail, const P& head) {
              arrow(tail, head, P(0, 0), "", epix_label_posn::none);
          },
          nb::arg("tail"), nb::arg("head"));

    // ---- shapes ----
    m.def("circle", static_cast<void (*)(const P&, double, const P&)>(&circle),
          nb::arg("center") = P(0, 0, 0), nb::arg("radius") = 1.0, nb::arg("perp") = E_3);
    m.def("rect", static_cast<void (*)(const P&, const P&)>(&rect),
          nb::arg("sw"), nb::arg("ne"));
    m.def("triangle", &triangle, nb::arg("a"), nb::arg("b"), nb::arg("c"));
    m.def("ellipse",
          static_cast<void (*)(const P&, const P&, const P&)>(&ellipse),
          nb::arg("center"), nb::arg("axis1"), nb::arg("axis2"));
    m.def("ellipse", static_cast<void (*)(const P&, const P&)>(&ellipse),
          nb::arg("center"), nb::arg("radius"));
    m.def("dart", &dart, nb::arg("tail"), nb::arg("head"));

    // ---- 3-D viewpoint + angle mode ----
    m.def("viewpoint", static_cast<void (*)(double, double, double)>(&viewpoint),
          nb::arg("x"), nb::arg("y"), nb::arg("z"));
    m.def("viewpoint", static_cast<void (*)(const P&)>(&viewpoint), nb::arg("at"));
    m.def("radians", &radians);
    m.def("degrees", &degrees);
    m.def("revolutions", &revolutions);

    // ---- line styles + clipping ----
    m.def("solid", &solid);
    m.def("dashed", &dashed);
    m.def("dotted", &dotted);
    m.def("line_style", &line_style, nb::arg("style") = "-");
    m.def("clip_box", static_cast<void (*)(const P&, const P&)>(&clip_box),
          nb::arg("sw"), nb::arg("ne"));
    m.def("clip_box", static_cast<void (*)()>(&clip_box));
    m.def("clip", &clip, nb::arg("on") = true);
    m.def("set_crop", &set_crop, nb::arg("on") = true);
    m.def("crop", static_cast<void (*)()>(&crop));

    // ---- axis (annotated axis; methods chain, returning axis&) ----
    nb::class_<axis>(m, "axis")
        .def(nb::init<const P&, const P&, unsigned int, const P&, epix_label_posn>(),
             nb::arg("tail"), nb::arg("head"), nb::arg("n"),
             nb::arg("offset") = P(0, 0), nb::arg("align") = epix_label_posn::none)
        .def("draw", &axis::draw)
        .def("draw_labels", &axis::draw_labels)
        .def("draw_ticks", &axis::draw_ticks)
        .def("unmark", &axis::unmark, nb::arg("at") = 0.0, nb::rv_policy::reference)
        .def("frac", &axis::frac, nb::rv_policy::reference)
        .def("trig", &axis::trig, nb::rv_policy::reference)
        .def("sci", &axis::sci, nb::rv_policy::reference)
        .def("log", &axis::log, nb::arg("base") = 10, nb::rv_policy::reference)
        .def("align", &axis::align, nb::arg("posn"), nb::rv_policy::reference)
        .def("align_labels", &axis::align_labels, nb::arg("posn"), nb::rv_policy::reference)
        .def("subdivide", &axis::subdivide, nb::arg("n"), nb::rv_policy::reference)
        .def("precision", &axis::precision, nb::arg("digits") = 0, nb::rv_policy::reference)
        .def("tick_ratio", &axis::tick_ratio, nb::arg("r"), nb::rv_policy::reference);

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
    nb::class_<legend>(m, "legend")
        .def(nb::init<>())
        .def("path_item", &legend::path_item, nb::arg("text"), nb::rv_policy::reference)
        .def("fill_item", &legend::fill_item, nb::arg("text"), nb::rv_policy::reference)
        .def("mark_item", &legend::mark_item, nb::arg("kind"), nb::arg("text"),
             nb::rv_policy::reference)
        .def("border", static_cast<legend& (legend::*)(const Color&, double)>(&legend::border),
             nb::arg("color"), nb::arg("width"), nb::rv_policy::reference)
        .def("border", static_cast<legend& (legend::*)(double)>(&legend::border),
             nb::arg("width"), nb::rv_policy::reference)
        .def("item_border",
             static_cast<legend& (legend::*)(const Color&, double)>(&legend::item_border),
             nb::arg("color"), nb::arg("width"), nb::rv_policy::reference)
        .def("item_border", static_cast<legend& (legend::*)(double)>(&legend::item_border),
             nb::arg("width"), nb::rv_policy::reference)
        .def("backing", &legend::backing, nb::arg("color"), nb::rv_policy::reference)
        .def("draw", &legend::draw, nb::arg("loc"), nb::arg("offset") = P(0, 0),
             nb::arg("align") = epix_label_posn::c);

    // ---- derivative / integral plots (Deriv/Integral wrap a function ptr) ----
    m.def("plot_deriv", [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f; plot(Deriv(tramp_d), a, b, n); g_fn = nb::callable();
    }, nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);
    m.def("plot_integral", [](nb::callable f, double x0, double a, double b, unsigned int n) {
        g_fn = f; plot(Integral(tramp_d, x0), a, b, n); g_fn = nb::callable();
    }, nb::arg("f"), nb::arg("x0"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);

    // ---- Camera (the global `camera`) + sphere drawing ----
    nb::class_<Camera>(m, "Camera")
        .def("at", static_cast<Camera& (Camera::*)(const P&)>(&Camera::at),
             nb::arg("at"), nb::rv_policy::reference)
        .def("at", static_cast<Camera& (Camera::*)(double, double, double)>(&Camera::at),
             nb::rv_policy::reference)
        .def("look_at", static_cast<Camera& (Camera::*)(const P&)>(&Camera::look_at),
             nb::arg("at"), nb::rv_policy::reference)
        .def("range", &Camera::range, nb::arg("r"), nb::rv_policy::reference);
    m.attr("camera") = nb::cast(&camera, nb::rv_policy::reference);

    m.def("sphere", [](const P& ctr, double rad) { sphere(ctr, rad); },
          nb::arg("center") = P(0, 0, 0), nb::arg("radius") = 1.0);
    m.def("latitude", [](double lat, double a, double b) { latitude(lat, a, b); },
          nb::arg("lat"), nb::arg("long_min"), nb::arg("long_max"));
    m.def("longitude", [](double lng, double a, double b) { longitude(lng, a, b); },
          nb::arg("lng"), nb::arg("lat_min"), nb::arg("lat_max"));

    // hidden/visible plotting on the sphere: f, g are two scalar functions
    m.def("backplot_N", [](nb::callable f, nb::callable g, double a, double b, int n) {
        g_fa = f; g_fb = g; backplot_N(tramp_a, tramp_b, a, b, n);
        g_fa = nb::callable(); g_fb = nb::callable();
    }, nb::arg("f"), nb::arg("g"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n"));
    m.def("frontplot_N", [](nb::callable f, nb::callable g, double a, double b, int n) {
        g_fa = f; g_fb = g; frontplot_N(tramp_a, tramp_b, a, b, n);
        g_fa = nb::callable(); g_fb = nb::callable();
    }, nb::arg("f"), nb::arg("g"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n"));

    // ---- 3-D: mesh / domain / surface ----
    nb::class_<mesh>(m, "mesh")
        .def(nb::init<int, int, int>(), nb::arg("n1"), nb::arg("n2"), nb::arg("n3") = 1)
        .def(nb::init<int>(), nb::arg("n"))
        .def(nb::init<>());

    nb::class_<domain>(m, "domain")
        .def(nb::init<const P&, const P&, const mesh&, const mesh&>(),
             nb::arg("sw"), nb::arg("ne"), nb::arg("coarse"), nb::arg("fine"))
        .def(nb::init<const P&, const P&, const mesh&>(),
             nb::arg("sw"), nb::arg("ne"), nb::arg("coarse"));

    // surface: F(u, v) -> P over a domain (parametric surface), same trampoline idea
    m.def("surface", [](nb::callable f, const domain& R, int cull) {
        g_fn2 = f;
        surface(tramp_P2, R, cull);
        g_fn2 = nb::callable();
    }, nb::arg("f"), nb::arg("domain"), nb::arg("cull") = 0);

    // ===== more state setters, shapes, axis labels, and field/polar plots =====
    m.def("black", &black, nb::arg("d") = 1.0);     // set pen color to a tint
    m.def("blue", &blue, nb::arg("d") = 1.0);
    m.def("green", &green, nb::arg("d") = 1.0);
    m.def("rgb", static_cast<void (*)(double, double, double)>(&rgb),
          nb::arg("r"), nb::arg("g"), nb::arg("b"));
    m.def("arrow_inset", &arrow_inset, nb::arg("inset") = 0.0);
    m.def("arrow_width", &arrow_width, nb::arg("w") = 3.0);

    m.def("picture", static_cast<void (*)(double, double)>(&picture),
          nb::arg("width"), nb::arg("height"));
    m.def("bounding_box", &bounding_box, nb::arg("sw"), nb::arg("ne"));
    m.def("unitlength", &unitlength, nb::arg("units"));
    m.def("pst_format", &pst_format);

    m.def("arc", &arc, nb::arg("center"), nb::arg("r"), nb::arg("start"), nb::arg("finish"));
    m.def("polar_grid", static_cast<void (*)(double, unsigned int, unsigned int)>(&polar_grid),
          nb::arg("r"), nb::arg("n1"), nb::arg("n2"));
    m.def("h_axis_labels",
          static_cast<void (*)(unsigned int, const P&, epix_label_posn)>(&h_axis_labels),
          nb::arg("n"), nb::arg("offset"), nb::arg("align") = epix_label_posn::b);
    m.def("v_axis_labels",
          static_cast<void (*)(unsigned int, const P&, epix_label_posn)>(&v_axis_labels),
          nb::arg("n"), nb::arg("offset"), nb::arg("align") = epix_label_posn::l);

    // function-pointer plots/fields (trampolines)
    m.def("polarplot", [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f; polarplot(tramp_d, a, b, n); g_fn = nb::callable();
    }, nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n") = 200);
    m.def("tan_field", [](nb::callable f, double a, double b, unsigned int n) {
        g_fn = f; tan_field(tramp_P, a, b, n); g_fn = nb::callable();
    }, nb::arg("f"), nb::arg("t_min"), nb::arg("t_max"), nb::arg("n"));
    m.def("dart_field",
          [](nb::callable F, const P& p, const P& q, unsigned int n1, unsigned int n2) {
              g_fn2 = F; dart_field(tramp_P2, p, q, n1, n2); g_fn2 = nb::callable();
          }, nb::arg("F"), nb::arg("sw"), nb::arg("ne"), nb::arg("n1"), nb::arg("n2"));
    m.def("ode_plot", [](nb::callable F, const P& start, double t_max, unsigned int n) {
        g_fn2 = F; ode_plot(tramp_P2, start, t_max, n); g_fn2 = nb::callable();
    }, nb::arg("F"), nb::arg("start"), nb::arg("t_max"), nb::arg("n"));

    // ---- functions ----
    m.def("Sin", &Sin); m.def("Cos", &Cos);
    m.def("Atan", &Atan); m.def("Atan2", &Atan2, nb::arg("y"), nb::arg("x"));
}
