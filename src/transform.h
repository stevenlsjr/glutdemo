#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "macros/linmath.h"
#include <GL/glew.h>

typedef struct {
    float x;
    float y;
    float z;
} srVec3f;

typedef struct {
    srVec3f pos;
    srVec3f scale;
    quat rotation;
} srTransform;

srTransform sr_new_tranform();

#endif //TRANSFORM_H
