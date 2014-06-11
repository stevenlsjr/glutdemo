#ifndef GLUTDEMO_IMAGE_H
#define GLUTDEMO_IMAGE_H

#include <IL/il.h>

typedef struct {
    int nelem;
    int nalloc;
    ILuint *nums;
} Images;

Images *load_images(const char **paths, int n);
void free_images(Images *images);

#endif //GLUTDEMO_IMAGE_H
