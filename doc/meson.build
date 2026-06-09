# doc/ — Phase 2.
#
# epix.info (the Texinfo manual) is committed, so install it directly — no
# makeinfo run needed.  The printable PDF manual is heavyweight (LaTeX +
# pstricks + makeindex) and off by default; build it with -Dmanual=true.  It
# builds from manual.tex + the committed .eepic figures (no epix run required).

install_data('epix.info', install_dir: get_option('infodir'))

if get_option('manual')
  build_manual = find_program('../build-aux/build_manual.sh')
  custom_target('manual-pdf',
    output: 'manual.pdf',
    command: [build_manual, meson.current_source_dir(), '@OUTPUT@'],
    build_by_default: true,
    install: true,
    install_dir: docdir_rel)   # docdir_rel comes from the top-level meson.build
endif
