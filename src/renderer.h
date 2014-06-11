#ifndef RENDERER_H
#define RENDERER_H


#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaderutils.h"
#include "mesh.h"
#include "macros/linmath.h"

#include "image.h"

struct _attributes {
    GLint vertPos;
    GLint vertNormal;
    GLint vertColor;
    GLint vertUv;
};
struct _uniforms {
	GLint time;
	GLint aspect;

	GLint view;
	GLint projection;
	GLint model;
    
    GLint texture;
    GLint do_texture;
};

typedef struct {
    float x;
    float y;
} Vec2;


typedef struct {
    Vec2 pos;
    Vec2 scale;
    float zrot;
} transform2d;

typedef struct {
    vec3 pos;
    vec3 scale;
    float xrot;
    float yrot;
    float zrot;
} transform3d;

/**
 * glut callback functions
 **/
void on_display();
void on_resize(int w, int h);
void on_idle();
void on_keystroke(unsigned char key, int x, int y);

/**
 * set active image set
 **/
void set_images(Images *images);
void set_active_img(int n);
void bind_active_img();

void init_resources();
void free_resources();

void set_camera(GLint program);
int setup_gl(int argc, char *argv[]);

#endif //RENDERER_H
