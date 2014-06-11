#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "macros/linmath.h"
#include "macros/dbg.h"


#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "image.h"

Images *load_images(const char **paths, int n)
{
    Images *images = malloc(sizeof(Images));
    if (images == NULL) {
        log_err("memory error");
        return NULL;
    }
    images->nelem = n;
    images->nalloc = n + 1;
    images->nums = calloc(images->nalloc, sizeof(ILuint));
    ilGenImages(images->nelem, images->nums);
    
    if (images->nums == NULL) {
        log_err("memory error");
        return NULL;
    }
    
    for (int i=0; i < images->nelem; i++) {
        ilBindImage(images->nums[i]);
        if (paths[i] != NULL) {
            bool loaded = ilLoadImage(paths[i]);
            assert(loaded);
            printf("%s\n", paths[i]);
        } else {
            printf("null\n");
        }
    }
    
    return images;
}

void free_images(Images *images)
{
    if (images->nums != NULL){
        ilDeleteImages(images->nelem, images->nums);
        free(images->nums);
    }
    
    if (images != NULL){
        free(images);
    }
}
