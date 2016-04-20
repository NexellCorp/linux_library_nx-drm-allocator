#ifndef _NX_DRM_ALLOCATOR_H
#define _NX_DRM_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

int open_drm_device(void);
int alloc_gem(int drm_fd, int size, int flags);
void free_gem(int drm_fd, int gem);
int gem_to_dmafd(int drm_fd, int gem_fd);
int get_vaddr(int drm_fd, int gem_fd, int size, void **vaddr);

#ifdef __cplusplus
}
#endif

#endif
