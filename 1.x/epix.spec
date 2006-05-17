Name:      epix
Version:   1.0.7
Release:   1
Summary:   Utility for generating LaTeX figures.
License:   GPL
URL:       http://math.holycross.edu/~ahwang/current/ePiX.html
Group:     Applications/Engineering
Prefix:    %{_prefix}
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Source:    http://math.holycross.edu/~ahwang/epix/epix_complete.tar.gz

%description
ePiX, a collection of batch-oriented utilities for *nix, creates
mathematically accurate line figures, plots, and movies using
easy-to-learn syntax. LaTeX and dvips comprise the typographical
rendering engine, while ImageMagick is used to create bitmapped
images and animations. GNU bash and a C++ compiler are required
for ordinary use.
 
%prep
%setup -n %{name}-%{version}_complete

%build
CFLAGS="$RPM_OPT_FLAGS"
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir} $RPM_BUILD_ROOT%{_mandir}/man1 $RPM_BUILD_ROOT%{_libdir}
mkdir -p $RPM_BUILD_ROOT%{_prefix}/include
make install prefix=$RPM_BUILD_ROOT%{_prefix}

%clean
rm -rf "$RPM_BUILD_ROOT"

%files
%defattr(-,root,root)
%doc BUGS ChangeLog COPYING INSTALL NEWS POST-INSTALL README README-changes
%doc README-porting THANKS VERSION contrib/doc/ doc/ samples/
%{_bindir}/*
%{_prefix}/man/man1/*
%{_libdir}/*
%{_prefix}/include/epix.h
