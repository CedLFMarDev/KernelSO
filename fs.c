#include "fs.h"
#include <stddef.h>

static struct fs_header *root_fs = NULL;

void fs_init(unsigned int module_addr)
{
    root_fs = (struct fs_header *)module_addr;
}

void *fs_get_body(struct fs_header *header)
{
    if (!header) return NULL;
    return (void *)(header + 1);
}

struct fs_header *fs_find(struct fs_header *dir, const char *name)
{
    (void)dir;
    (void)name;
    if (!dir || !name) return NULL;
    return NULL;
}
