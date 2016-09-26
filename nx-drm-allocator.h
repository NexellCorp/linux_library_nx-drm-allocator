#ifndef _NX_DRM_ALLOCATOR_H
#define _NX_DRM_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

int open_drm_device(void);
int alloc_gem(int drm_fd, unsigned int size, int flags);
void free_gem(int drm_fd, int gem);
int gem_to_dmafd(int drm_fd, int gem_fd);
int get_vaddr(int drm_fd, int gem_fd, int size, void **vaddr);
unsigned int get_flink_name(int drm_fd, int gem_fd);
int import_gem_from_flink(int drm_fd, unsigned int flink_name);

#ifdef __cplusplus
}
#endif

#endif
