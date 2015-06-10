# RPM spec file for RedHat and Fedora
# $Id: lxsplit.spec,v 1.5 2008/10/03 12:25:58 sezero Exp $

%define name	lxsplit
%define version	0.2.4

Name:		%{name}
Summary:	File split / merge utility
Version:	%{version}
Release:	1
Source:		http://download.sourceforge.net/lxsplit/%{name}-%{version}.tar.gz
URL:		http://lxsplit.sourceforge.net/
Group:		Applications/File
BuildRoot:	%{_tmppath}/%{name}-%{version}-build
License:	GPLv2+

%description
lxSplit is a simple tool for splitting files and joining the splitted
files on unix-like platforms, such as Linux, FreeBSD, OpenBSD, etc.
Splitting is done without compression and large files (> 4 GB) are
supported. lxSplit is fully compatible with the HJSplit utility which
is available for other operating systems.

%prep
%setup -q

%build
%{__make}

%install
%{__rm} -rf %{buildroot}
%{__mkdir_p} %{buildroot}/%{_bindir}
%{__install} -D -m755 %{name} %{buildroot}/%{_bindir}/%{name}

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(0755,root,root)
%{_bindir}/%{name}

%changelog
* Wed Oct 03 2008 sezero <sezero@users.sourceforge.net> 0.2.4-1
- v0.2.4

* Wed Jul 02 2008 sezero <sezero@users.sourceforge.net> 0.2.3-1
- v0.2.3

* Mon Mar 22 2008 sezero <sezero@users.sourceforge.net> 0.2.2-1
- v0.2.2

* Mon Feb 04 2008 sezero <sezero@users.sourceforge.net> 0.2.1-1
- v0.2.1

