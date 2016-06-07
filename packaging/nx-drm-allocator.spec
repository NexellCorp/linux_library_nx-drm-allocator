Name:    nx-drm-allocator
Version: 0.0.1
Release: 1
License: Apache 2.0
Summary: Nexell drm allocator library
Group: Development/Libraries
Source:  %{name}-%{version}.tar.gz

BuildRequires:  pkgconfig automake autoconf libtool
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
autoreconf -v --install || exit 1
%configure
make %{?_smp_mflags}

%postun -p /sbin/ldconfig

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

find %{buildroot} -type f -name "*.la" -delete

mkdir -p %{buildroot}/usr/include
cp %{_builddir}/%{name}-%{version}/nx-drm-allocator.h %{buildroot}/usr/include

%files
%{_libdir}/libnx_drm_allocator.so
%{_libdir}/libnx_drm_allocator.so.*

%files devel
%{_includedir}/nx-drm-allocator.h
