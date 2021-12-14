#ifndef RESOURCE_HANDLE_H
#define RESOURCE_HANDLE_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

struct resource_handle{
    char *name;
    char *path;
};

int resource_handle_init(struct resource_handle *dst, const char *name, const char *path);
void resource_handle_free(struct resource_handle *dst);

int resource_handle_name_set(struct resource_handle *dst, const char *name);
int resource_handle_path_set(struct resource_handle *dst, const char *path);

int resource_handle_name_comp(struct resource_handle *src, const char *name);
int resource_handle_path_comp(struct resource_handle *src, const char *path);

#endif //RESOURCE_HANDLE_H
