ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

AM_CFLAGS = \
	$(WARN_CFLAGS) \
	-I./ \
	-I./drm \
	-I${includedir}/libdrm

libnx_drm_allocator_la_LTLIBRARIES = libnx_drm_allocator.la
libnx_drm_allocator_ladir = ${libdir}

libnx_drm_allocator_la_SOURCES = \
	allocator.c

libnx_drm_allocatorincludedir = ${includedir}
libnx_drm_allocatorinclude_HEADERS = \
	nx-drm-allocator.h

