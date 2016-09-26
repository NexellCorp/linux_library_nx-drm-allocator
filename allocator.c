#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <drm/nexell_drm.h>

#define DRM_IOCTL_NR(n)         _IOC_NR(n)
#define DRM_IOC_VOID            _IOC_NONE
#define DRM_IOC_READ            _IOC_READ
#define DRM_IOC_WRITE           _IOC_WRITE
#define DRM_IOC_READWRITE       _IOC_READ|_IOC_WRITE
#define DRM_IOC(dir, group, nr, size) _IOC(dir, group, nr, size)

static int drm_ioctl(int drm_fd, unsigned long request, void *arg)
{
	int ret;

	do {
		ret = ioctl(drm_fd, request, arg);
	} while (ret == -1 && (errno == EINTR || errno == EAGAIN));
	return ret;
}

static int drm_command_write_read(int fd, unsigned long command_index,
				  void *data, unsigned long size)
{
	unsigned long request;

	request = DRM_IOC(DRM_IOC_READ|DRM_IOC_WRITE, DRM_IOCTL_BASE,
			  DRM_COMMAND_BASE + command_index, size);
	if (drm_ioctl(fd, request, data))
		return -errno;
	return 0;
}

/**
 * return drm_fd
 */
int open_drm_device(void)
{
	const char *dev = "/dev/dri/card0";
	int fd = open(dev, O_RDWR);
	if (fd < 0)
		perror("open\n");

	return fd;
}

/**
 * return gem_fd
 */
int alloc_gem(int drm_fd, unsigned int size, int flags)
{
	struct nx_drm_gem_create arg = { 0, };
	int ret;

	arg.size = size;
	arg.flags = flags;

	ret = drm_command_write_read(drm_fd, DRM_NX_GEM_CREATE, &arg,
				     sizeof(arg));
	if (ret) {
		perror("drm_command_write_read\n");
		return ret;
	}

	/* printf("[DRM ALLOC] gem %d, size %d, flags 0x%x\n", */
	/*        arg.handle, size, flags); */

	return arg.handle;
}

void free_gem(int drm_fd, int gem)
{
	struct drm_gem_close arg = {0, };

	arg.handle = gem;
	drm_ioctl(drm_fd, DRM_IOCTL_GEM_CLOSE, &arg);
}

/**
 * return dmabuf fd
 */
int gem_to_dmafd(int drm_fd, int gem_fd)
{
	int ret;
	struct drm_prime_handle arg = {0, };

	arg.handle = gem_fd;
	ret = drm_ioctl(drm_fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &arg);
	if (ret) {
		perror("DRM_IOCTL_PRIM_HANDLE_TO_FD\n");
		return -1;
	}

	return arg.fd;
}

int get_vaddr(int drm_fd, int gem_fd, int size, void **vaddr)
{
	int ret;
	struct drm_mode_map_dumb arg = {0, };
	void *map = NULL;

	arg.handle = gem_fd;
	ret = drm_ioctl(drm_fd, DRM_IOCTL_MODE_MAP_DUMB, &arg);
	if (ret) {
		perror("DRM_IOCTL_MODE_MAP_DUMB\n");
		return -1;
	}

	map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, drm_fd,
		   arg.offset);
	if (map == MAP_FAILED) {
		perror("mmap\n");
		return -1;
	}

	*vaddr = map;
	return 0;
}

unsigned int get_flink_name(int fd, int gem)
{
	struct drm_gem_flink arg = { 0, };

	arg.handle = gem;
	if (drm_ioctl(fd, DRM_IOCTL_GEM_FLINK, &arg)) {
		fprintf(stderr,
			"fail : get flink from gem:%d (DRM_IOCTL_GEM_FLINK)\n",
			gem);
		return 0;
	}
	return (unsigned int)arg.name;
}

int import_gem_from_flink(int fd, unsigned int flink_name)
{
	struct drm_gem_open arg = { 0, };
	/* struct nx_drm_gem_info info = { 0, }; */

	arg.name = flink_name;
	if (drm_ioctl(fd, DRM_IOCTL_GEM_OPEN, &arg)) {
		fprintf(stderr, "fail : cannot open gem name=%d\n", flink_name);
		return -EINVAL;
	}

	return arg.handle;
}
