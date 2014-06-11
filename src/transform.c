#include "transform.h"
#include "macros/linmath.h"

#include <GL/glew.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>




srTransform sr_new_tranform()
{
    srTransform transform = {
        .pos = {0.0, 0.0, 0.0},
        .scale = {1.0, 1.0, 1.0},
    };
    quat_identity(transform.rotation);
    
    return transform;
}
