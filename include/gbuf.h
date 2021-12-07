#ifndef GBUF_H
#define GBUF_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "texture.h"

struct gbuf{
    struct texture pos, normal, albedo, specular;
};

#endif //GBUF_H
