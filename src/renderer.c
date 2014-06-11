#include "renderer.h"

#include "macros/dbg.h"
#include "macros/linmath.h"
#include "macros/constants.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaderutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "transform.h"



static void draw_mesh(GLenum mode, Mesh *M);

static struct _attributes attrib;
static struct _uniforms unif;

static GLuint xProgram;

static Images *xImages;
int active_img;

static mat4x4 view;
static mat4x4 projection;
static float ticker = 0.0;

static bool xDo_texture;
static bool xTexturesBound = false;

// scene info
static float xFov = 65 * S_PI/180;
//static float xLightPos[] = {-4.0, 1.0, 1.0, 0.0};

static Mesh xMesh;

srTransform xCamera = {
    .pos = {0.0, 0.0, -5.0},
    .scale = {1.0, 1.0, 1.0},
    .rotation = {0.0, 0.0, 0.0, 1.0}
};

void set_images(Images *images)
{
    xImages = images;
}

void set_active_img(int n)
{
    active_img = n;
}

void bind_active_img()
{
    /** bind tex */
    glActiveTexture(GL_TEXTURE0);
    ilBindImage(xImages->nums[active_img%xImages->nelem]);
    ilutGLBindTexImage();
}

void init_resources()
{

    /** setup xProgram */
    xProgram = create_program(
    	"shader.vert",
    	"shader.frag"
   	);
    
    attrib.vertPos = get_attrib(xProgram, "vertPos");
    attrib.vertNormal = get_attrib(xProgram, "vertNormal");
    attrib.vertUv = get_attrib(xProgram, "vertUv");
    attrib.vertColor = get_attrib(xProgram, "vertColor");
    
    unif.projection = get_uniform(xProgram, "projection");
    unif.view = get_uniform(xProgram, "view");
    unif.model = get_uniform(xProgram, "model");
    
    unif.do_texture = get_uniform(xProgram, "do_texture");
    unif.texture = get_uniform(xProgram, "texture");
    unif.time = get_uniform(xProgram, "time");
    
    xMesh = cube_mesh();

    glUseProgram(xProgram);
    /** set do_texture*/
    xDo_texture = true;
    glUniform1i(unif.do_texture, xDo_texture);

    mat4x4_translate(view, 0.0, 0.0, -5.0);
    
}

void free_resources()
{
    free_mesh(&xMesh);

    glDeleteProgram(xProgram);
}

void set_camera(GLint program)
{
    glUseProgram(program);

    
    glUniformMatrix4fv(unif.view, 1, GL_FALSE, &view[0][0]);
    
}

void on_display()
{    
    glClearColor(0.1, 0.1, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    set_camera(xProgram);
    
    draw_mesh(GL_TRIANGLES, &xMesh);
    
    glutSwapBuffers();
}

static void draw_mesh(GLenum mode, Mesh *M)
{
    glUseProgram(xProgram);
    /**
     * push model matrix to shader
     **/
    glUniformMatrix4fv(unif.model, 1, GL_FALSE, &M->transform_mat[0][0]);
    
    

    glUniform1i(unif.texture, 0);
    
    glEnableVertexAttribArray(attrib.vertPos);
    glEnableVertexAttribArray(attrib.vertColor);
    glEnableVertexAttribArray(attrib.vertUv);
    glEnableVertexAttribArray(attrib.vertNormal);
    
    glBindBuffer(GL_ARRAY_BUFFER, M->vbo);
    
    /** describe verts to openGL */
    glVertexAttribPointer(
    	attrib.vertPos,
    	4,
    	GL_FLOAT,
    	GL_FALSE,
    	sizeof(Vertex),
    	(GLvoid *) offsetof(Vertex, pos)
    );
    
    glVertexAttribPointer(
    	attrib.vertColor,
    	4,
    	GL_FLOAT,
    	GL_FALSE,
    	sizeof(Vertex),
    	(GLvoid *) offsetof(Vertex, color)
    );
    glVertexAttribPointer(
    	attrib.vertNormal,
    	4,
    	GL_FLOAT,
    	GL_FALSE,
    	sizeof(Vertex),
    	(GLvoid *) offsetof(Vertex, normal)
    );
    glVertexAttribPointer(
    	attrib.vertUv,
    	2,
    	GL_FLOAT,
    	GL_FALSE,
    	sizeof(Vertex),
    	(GLvoid *) offsetof(Vertex, uv)
    );
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M->ibo);
    int size;
    glGetBufferParameteriv(
        GL_ELEMENT_ARRAY_BUFFER,
        GL_BUFFER_SIZE, &size
    );
    glDrawElements(
        mode, size/sizeof(GLuint),
        GL_UNSIGNED_INT, 0
    );
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(mode, 0);
    
    glDisableVertexAttribArray(attrib.vertPos);
    glDisableVertexAttribArray(attrib.vertColor);
    glDisableVertexAttribArray(attrib.vertUv);
    
}

void on_resize(int w, int h)
{
    float aspect = w / (float) h;
    glViewport(0, 0, w, h);
    
    /** set projection */
    mat4x4_perspective(projection, xFov, aspect, 0.1, 1000);   
    glUniformMatrix4fv(unif.projection, 1, GL_FALSE, &projection[0][0]);
     
}

void on_idle()
{

}

void on_keystroke(unsigned char key, int x, int y)
{
	mat4x4 matcpy;

    float speed = 0.1;
    if (key == 'a') {
        mat4x4_translate_in_place(view, speed, 0, 0);
    } else if (key == 'd') {
        mat4x4_translate_in_place(view, -speed, 0, 0);
    } else if (key == 's') {
        mat4x4_translate_in_place(view, 0, 0, -speed);
    } else if (key == 'w') {
        mat4x4_translate_in_place(view, 0, 0, speed);
    } else if (key == 'i') {
        mat4x4_dup(matcpy, view);
        mat4x4_rotate_X(view, matcpy, speed);
    } else if (key == 'k') {
        mat4x4_dup(matcpy, view);
        mat4x4_rotate_X(view, matcpy, -speed);
    } else if (key == 'j') {
        mat4x4_dup(matcpy, view);
        mat4x4_rotate_Y(view, matcpy, speed);
    } else if (key == 'l') {
        mat4x4_dup(matcpy, view);
        mat4x4_rotate_Y(view, matcpy, -speed);
    } else if (key == 'm') {
        ticker += 0.01;
		printf("%f\n", ticker);
		glUniform1f(unif.time, ticker);
    }
    
    else if(key == 't') {
        xDo_texture ^= true;
        
        glUniform1i(unif.do_texture, xDo_texture);
    } else if (key == 'p') {
        active_img ++;
        bind_active_img();
    }
    
    glutPostRedisplay();
}


int setup_gl(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(2,1);
	unsigned int flags = GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH;
	glutInitDisplayMode(flags);
	glutInitWindowSize(640, 480);
	glutCreateWindow("openGL demo");
    
    glEnable(GL_DEPTH_TEST);
	
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		log_err("Error: %s\n", glewGetErrorString(glew_status));
		return -1;
	}
	return 1;

	glEnable (GL_BLEND);
	
}


