Name:      epix
Version:   1.0.0
Release:   1
Summary:   Utility for generating figures for LaTeX.
License:   GPL
URL:       http://math.holycross.edu/~ahwang/current/ePiX.html
Group:     Applications/Engineering
Prefix:    %{_prefix}
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Source:    http://math.holycross.edu/~ahwang/software/epix_complete.tar.gz

%description
ePiX (pronounced like "epic" with a soft "k", as in "TeX") is a lightweight
command-based utility for generating camera quality, mathematically accurate
line figures for LaTeX from user-friendly source files.  The utility
consists of a library written in C++ and some shell scripts; GNU bash and a
C++ compiler are *required* for ordinary use.
 
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
%doc BUGS CHANGELOG COPYING POST-INSTALL README README-authors README-changes
%doc THANKS VERSION contrib/doc/ doc/ samples/
%{_bindir}/*
%{_prefix}/man/man1/*
%{_libdir}/*
%{_prefix}/include/epix.h
