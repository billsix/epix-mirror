project('epix', 'cpp',
  version: '1.2.22',
  license: 'GPL-2.0-or-later',
  meson_version: '>=0.56.0',
  default_options: ['warning_level=0'])

# ePiX build (Meson port — Phase 1: the core build that produces a working
# libepix.a + epix.h + the four driver scripts, installed to the same layout as
# the autotools build.  doc/ and samples/ subdirs + the LaTeX manual are Phase 2.)
#
# The autoconf feature checks (AC_HEADER_STDC / AC_C_CONST / AC_C_INLINE /
# AC_CHECK_FUNCS(strtod)) are intentionally dropped: no source references a
# config.h or any HAVE_* macro, so they were vestigial.

# ---------------------------------------------------------------------------
# Programs resolved at configure time and baked into the wrapper scripts.
# (configure.ac's AC_PATH_PROG + --with-bash / --with-runtime / --with-ps2eps.)
# ---------------------------------------------------------------------------
bash_opt = get_option('bash_path')
bash_path = bash_opt != '' ? bash_opt : find_program('bash').full_path()

rc_opt = get_option('runtime_compiler')
runtime_compiler = rc_opt != '' ? rc_opt : find_program('g++').full_path()

ps2eps_opt = get_option('ps2eps')
if ps2eps_opt != ''
  ps2eps = ps2eps_opt
else
  ps2eps_prog = find_program('ps2epsi', required: false)
  ps2eps = ps2eps_prog.found() ? ps2eps_prog.full_path() : 'ps2epsi'
endif

# ---------------------------------------------------------------------------
# Install directories.
#   *_rel  -> relative to prefix, used for install_dir.
#   *_abs  -> absolute, baked into the scripts so they locate their lib /
#             headers / epix-lib.sh at runtime (matching the autotools @vars@).
# pkglibdir = libdir/epix, pkgdatadir = datadir/epix, docdir = datadir/doc/epix.
# ---------------------------------------------------------------------------
prefix         = get_option('prefix')
rel_bindir     = get_option('bindir')
rel_libdir     = get_option('libdir')
rel_includedir = get_option('includedir')
rel_datadir    = get_option('datadir')
rel_infodir    = get_option('infodir')

pkglibdir_rel  = rel_libdir  / 'epix'
pkgdatadir_rel = rel_datadir / 'epix'
docdir_rel     = rel_datadir / 'doc' / 'epix'

abs_bindir     = prefix / rel_bindir
abs_includedir = prefix / rel_includedir
abs_infodir    = prefix / rel_infodir
pkglibdir_abs  = prefix / pkglibdir_rel
pkgdatadir_abs = prefix / pkgdatadir_rel
docdir_abs     = prefix / docdir_rel

# ---------------------------------------------------------------------------
# The static library (libepix_a_CXXSOURCES from Makefile.am).
# ---------------------------------------------------------------------------
epix_sources = files(
  'Color.cc', 'Color_Base.cc', 'Color_CMY.cc', 'Color_CMYK.cc', 'Color_Gray.cc',
  'Color_Neutral.cc', 'Color_RGB.cc', 'Color_Sep.cc', 'Color_Utils.cc', 'Complex.cc',
  'Sline.cc', 'active_screen.cc', 'affine.cc', 'angle_units.cc', 'arrow_data.cc',
  'arrow_style.cc', 'axis.cc', 'camera.cc', 'circle.cc', 'clipping.cc',
  'cropping.cc', 'curves.cc', 'data_bins.cc', 'data_file.cc', 'data_mask.cc',
  'domain.cc', 'eepic.cc', 'facet.cc', 'format.cc',
  'frac.cc', 'frame.cc', 'functions.cc', 'geometry.cc', 'glyph.cc',
  'halfspace.cc', 'hatching.cc', 'intersections.cc', 'interval.cc', 'label_data.cc',
  'label_style.cc', 'legend.cc', 'legend_item.cc', 'legend_tile.cc', 'length.cc',
  'lens.cc', 'marker_style.cc', 'markers.cc', 'mask_diamond.cc', 'mask_ellipse.cc',
  'mask_rectangle.cc', 'paint_style.cc', 'pairs.cc', 'path.cc', 'path_data.cc',
  'path_style.cc', 'pen_arrow.cc', 'pen_data.cc', 'pen_fill.cc', 'pen_line.cc',
  'picture.cc', 'picture_data.cc', 'plane.cc', 'plots.cc', 'pst.cc',
  'screen.cc', 'screen_crop.cc', 'screen_data.cc', 'screen_mask.cc', 'segment.cc',
  'sphere.cc', 'spline.cc', 'state.cc', 'surface.cc', 'tikz.cc',
  'tile.cc', 'triples.cc', 'utils.cc', 'verbatim.cc',
)

libepix = static_library('epix', epix_sources,
  install: true,
  install_dir: pkglibdir_rel)

# ---------------------------------------------------------------------------
# Public headers (pkginclude_HEADERS -> includedir/epix): the 78 sources' headers
# plus 9 header-only files.  Excludes debug.h and fmt_template.h (internal/dead —
# fmt_template.cc isn't in the library either; it #includes a nonexistent fmt.h).
# ---------------------------------------------------------------------------
epix_headers = files(
  'Color.h', 'Color_Base.h', 'Color_CMY.h', 'Color_CMYK.h', 'Color_Gray.h',
  'Color_Neutral.h', 'Color_RGB.h', 'Color_Sep.h', 'Color_Utils.h', 'Complex.h',
  'Sline.h', 'active_screen.h', 'affine.h', 'angle_units.h', 'arrow_data.h',
  'arrow_style.h', 'axis.h', 'camera.h', 'circle.h', 'clipping.h',
  'constants.h', 'crop_algorithms.h', 'cropping.h', 'curves.h', 'data_bins.h',
  'data_file.h', 'data_mask.h', 'deriv.h', 'domain.h',
  'edge_data.h', 'eepic.h', 'enums.h', 'errors.h', 'facet.h',
  'format.h', 'frac.h', 'frame.h', 'functions.h',
  'geometry.h', 'glyph.h', 'halfspace.h', 'hatching.h', 'intersections.h',
  'interval.h', 'label_data.h', 'label_style.h', 'legend.h', 'legend_item.h',
  'legend_tile.h', 'length.h', 'lens.h', 'map.h', 'marker_style.h',
  'markers.h', 'mask_diamond.h', 'mask_ellipse.h', 'mask_rectangle.h', 'paint_style.h',
  'pairs.h', 'path.h', 'path_data.h', 'path_style.h', 'pen_arrow.h',
  'pen_data.h', 'pen_fill.h', 'pen_line.h', 'picture.h', 'picture_data.h',
  'plane.h', 'plot_algorithms.h', 'plots.h', 'pst.h', 'screen.h',
  'screen_crop.h', 'screen_data.h', 'screen_mask.h', 'segment.h', 'sphere.h',
  'spline.h', 'spline_data.h', 'state.h', 'surface.h', 'tikz.h',
  'tile.h', 'triples.h', 'utils.h', 'verbatim.h',
)
install_headers(epix_headers, subdir: 'epix')

# ---------------------------------------------------------------------------
# Generated umbrella header epix.h (make_header concatenates a curated, ordered
# subset of the headers).  Built via a wrapper so the source tree stays clean.
# ---------------------------------------------------------------------------
gen_header = find_program('build-aux/gen_header.sh')
epix_h = custom_target('epix-header',
  output: 'epix.h',
  command: [gen_header, meson.current_source_dir(), '@OUTPUT@'],
  depend_files: epix_headers,
  build_by_default: true,
  install: true,
  install_dir: rel_includedir)

# ---------------------------------------------------------------------------
# Driver scripts, man page, and Emacs mode from *.in templates.
# configure_file does the @var@ substitution the Makefile did with sed.
# (The shebang stays "#! /bin/bash" — substituting it would require editing the
# templates, which would break the parallel autotools build during transition.)
# ---------------------------------------------------------------------------
conf = configuration_data()
conf.set('RUNTIME_COMPILER', runtime_compiler)
conf.set('PS2EPS_BIN', ps2eps)
conf.set('includedir', abs_includedir)
conf.set('pkglibdir', pkglibdir_abs)
conf.set('pkgdatadir', pkgdatadir_abs)
conf.set('bindir', abs_bindir)
conf.set('docdir', docdir_abs)
conf.set('infodir', abs_infodir)
conf.set('bashpath', bash_path)

foreach s : ['epix', 'elaps', 'flix', 'laps']
  configure_file(
    input: s + '.in',
    output: s,
    configuration: conf,
    install: true,
    install_dir: rel_bindir,
    install_mode: 'rwxr-xr-x')
endforeach

# man pages: epix.1 is generated; elaps/flix/laps.1 are ".so man1/epix.1" stubs.
epix_man = configure_file(input: 'epix.1.in', output: 'epix.1', configuration: conf)
install_man(epix_man, 'elaps.1', 'flix.1', 'laps.1')

# ---------------------------------------------------------------------------
# Data: the epix-lib.sh helper (pkgdata), notes, and config files.
# (Parity with the autotools install + install-data-local hook.)
# ---------------------------------------------------------------------------
install_data('epix-lib.sh', install_dir: pkgdatadir_rel)

install_data(
  ['AUTHORS', 'BUGS', 'ChangeLog', 'COPYING', 'INSTALL', 'NEWS',
   'POST-INSTALL', 'README.md', 'THANKS', 'VERSION'],
  install_dir: docdir_rel / 'notes')

install_data('bash_completions', install_dir: docdir_rel / 'config')
if get_option('epix_el')
  epix_el = configure_file(input: 'epix.el.in', output: 'epix.el', configuration: conf)
  install_data(epix_el, install_dir: docdir_rel / 'config')
endif

# ---------------------------------------------------------------------------
# samples/ and doc/ (Phase 2).
# Samples: install the sample sources (uncompressed) to docdir/samples — more
# useful than the autotools sample_src.tar.gz tarball.  doc/ installs epix.info
# and, with -Dmanual=true, builds the printable manual.
# ---------------------------------------------------------------------------
install_subdir('samples',
  install_dir: docdir_rel,
  exclude_files: ['Makefile.am', 'Makefile.in', 'meson.build'])

subdir('doc')

summary({
    'prefix'           : prefix,
    'runtime compiler' : runtime_compiler,
    'ps2eps'           : ps2eps,
    'bash'             : bash_path,
    'install emacs el' : get_option('epix_el'),
  }, section: 'ePiX')
