Name:    nx-drm-allocator
Version: 0.0.1
Release: 0
License: Apache 2.0
Summary: Nexell drm allocator library
Group: Development/Libraries
Source:  %{name}-%{version}.tar.gz

BuildRequires:  pkgconfig(glib-2.0)

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
Nexell allocator library

%package devel
Summary: Nexell drm allocator library
Group: Development/Libraries
License: Apache 2.0
Requires: %{name} = %{version}-%{release}

%description devel
Nexell drm allocator library (devel)

%prep
%setup -q

%build
make

%postun -p /sbin/ldconfig

%install
rm -rf %{buildroot}

mkdir -p %{buildroot}/usr/include
cp %{_builddir}/%{name}-%{version}/nx-drm-allocator.h %{buildroot}/usr/include

mkdir -p %{buildroot}/usr/lib
cp %{_builddir}/%{name}-%{version}/libnx-drm-allocator.so  %{buildroot}/usr/lib

%files
%attr (0644, root, root) %{_libdir}/libnx-drm-allocator.so

%files devel
%attr (0644, root, root) %{_includedir}/nx-drm-allocator.h
