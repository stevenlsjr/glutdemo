/**
 * 
 */


#include "macros/constants.h"
#include "macros/dbg.h"
#include "macros/linmath.h"
#include "renderer.h"
#include "shaderutils.h"
#include "transform.h"
#include "mesh.h"

#ifndef EMSCRIPTEN

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#endif // !EMSCRIPTEN

#include <GL/freeglut.h>
#include <GL/glew.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

typedef enum SceneName {
  SCENE_SQUARES,
  SCENE_CIRCLE,
  SCENE_POLY
} SceneName;

void draw_mesh(GLenum mode, Mesh *M);

void switch_scene(SceneName name);

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
static float xFov = 65 * S_PI / 180;
//static float xLightPos[] = {-4.0, 1.0, 1.0, 0.0};

static Mesh xMesh;

static Mesh xSquare_black;
static Mesh xSquare_blue;
static Mesh xCircle;
static Mesh xPoly;

static double xDt = 0.1;

static SceneName xScene_name = SCENE_POLY;

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
  /*
   * TODO(Steven): bind textures correctly
   **/
}

void make_squares()
{
  Vertex square[] = {
      {.pos={-1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 0.0, 0.0, 1.0}, .uv={0.0, 0.0}},
      {.pos={-1.0, 1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 0.0, 0.0, 1.0}, .uv={0.0, 1.0}},
      {.pos={1.0, 1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 0.0, 0.0, 1.0}, .uv={1.0, 1.0}},
      {.pos={1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 0.0, 0.0, 1.0}, .uv={1.0, 0.0}},
  };
  GLuint index[] = {
      // front
      0, 1, 2,
      2, 3, 0,
  };

  // make black sqare
  int n_verts = sizeof(square) / sizeof(Vertex);
  int n_indices = sizeof(index) / sizeof(GLuint);

  xSquare_black = make_mesh(square, n_verts, index, n_indices);

  // set colors to blue to make blue square
  vec4 blue_color = {0.0, 0.0, 1.0, 0.0};
  for (int i = 0; i < n_verts; ++i) {
    float *c = square[i].color;
    memcpy(c, blue_color, sizeof(vec4));
  }

  xSquare_blue = make_mesh(square, n_verts, index, n_indices);

  // assign the black square a slightly larger scale to
  // create an outline
  mat4x4 tmp = {};
  mat4x4 mul = {};
  mat4x4_identity(tmp);
  mat4x4_scale_aniso(xSquare_black.transform_mat, tmp, 0.5, 0.5, 1.0);

  mat4x4_identity(tmp);
  mat4x4_scale_aniso(xSquare_blue.transform_mat, tmp, 0.4, 0.4, 1.0);

  memcpy(tmp, xSquare_blue.transform_mat, sizeof(mat4x4));
  // move blue square's z axis so it renders in front of black square
  mat4x4_translate(mul, 0, 0, 0.1);
  mat4x4_mul(xSquare_blue.transform_mat, tmp, mul);
}

void make_poly()
{

  // convex polygon vertices
  // default colors for debugging
  Vertex verts[] = {
      {.pos={-1.0, -1.0, 0.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0},
          .color={0.0, 1.0, 0.0, 1.0}, .uv={0.0, 0.0}},  // 0, green
      {.pos={-0.9, 0.9, 0.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0},
          .color={0.0, 0.0, 1.0, 1.0}, .uv={0.0, 0.0}}, // 1, blue
      {.pos={1.0, 1.0, 0.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0},
          .color={1.0, 0.0, 0.0, 1.0}, .uv={0.0, 0.0}}, // 2, red
      {.pos={0.0, -0.5, 0.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0},
          .color={1.0, 1.0, 0.0, 1.0}, .uv={0.0, 0.0}},  // 3, yellow
      {.pos={0.0, 0.0, 0.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0},
          .color={0.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}}, // 4, cyan
  };

  const int n_verts = sizeof(verts) / sizeof(Vertex);
  float color[4] = {39/(float)255, 97/(float)255, 39/(float)255};

  for (int i=0; i<n_verts; ++i) {
    memcpy(verts[i].color, color, sizeof(float[4]));
  }

  GLuint indices[] = {
      0, 4, 1,
      1, 4, 2,
      2, 3, 4
  };
  xPoly = make_mesh(verts, n_verts,
                    indices, sizeof(indices) / sizeof(GLuint));
}

void make_circle()
{

  const int n_verts = 31;
  Vertex *verts = calloc(n_verts, sizeof(Vertex));


  GLuint *indices = calloc(n_verts * 4, sizeof(GLuint));


  verts[0] = (Vertex) {{0,   0,   0.0, 1.0},
                       {0.0, 0.0, 1.0, 0.0},
                       {1.0, 1.0, 1.0, 1.0},
                       {0.0, 0.0}};
  //hsv_to_rgba(verts[0].color, 0, 0, 0.5);

  for (int i = 1; i < n_verts; ++i) {
    //indices[i] = i;

    const float theta = 2.0 * M_PI * ((float) i / (float) (n_verts - 5));
    const float hue = (i - 1) / (float) (n_verts - 5);

    Vertex v = {{cos(theta), sin(theta), 0.0, 1.0},
                {0.0,        0.0,        1.0, 0.0},
                {0.0,        0.0,        1.0, 1.0},
                {0.0,        0.0}};


    hsv_to_rgba(v.color, hue, 1.0, 0.5);
    memcpy(verts + i, &v, sizeof(Vertex));

  }

  int n_indices = 0;
  for (int i = 0; i < n_verts - 2; ++i) {
    // just set index to corespond to vert location. Draw Circle with
    // GL_TRIANGLE_FAN
    n_indices = i;
    indices[i] = (GLuint)i;
  }

  xCircle = make_mesh(verts, n_verts, indices, n_indices);

  free(verts);
  free(indices);
}

void make_lab_meshes()
{

  make_squares();
  make_poly();
  make_circle();


}

void init_resources()
{

  /** setup xProgram */
  xProgram = create_program(
      "resources/shader.vert",
      "resources/shader.frag"
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

  xMesh = plane_mesh();

  make_lab_meshes();


  glUseProgram(xProgram);
  /** set do_texture*/
  xDo_texture = true;
  glUniform1i(unif.do_texture, xDo_texture);

  mat4x4_translate(view, 0.0, 0.0, -5.0);

}

void free_resources()
{
  free_mesh(&xMesh);
  free_mesh(&xSquare_black);
  free_mesh(&xSquare_blue);
  free_mesh(&xCircle);
  free_mesh(&xPoly);

  glDeleteProgram(xProgram);
}

void set_camera(GLint program)
{
  glUseProgram(program);

  glUniformMatrix4fv(unif.view, 1, GL_FALSE, &view[0][0]);

}

void on_display()
{
  glClearColor(1, 1, 1, 1);
  // don't use depth tests
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  set_camera(xProgram);

  switch (xScene_name) {
    case SCENE_SQUARES: {
      draw_mesh(GL_TRIANGLES, &xSquare_blue);
      draw_mesh(GL_TRIANGLES, &xSquare_black);
      break;
    }
    case SCENE_CIRCLE: {
      draw_mesh(GL_TRIANGLE_FAN, &xCircle);
      break;
    }
    case SCENE_POLY: {
      draw_mesh(GL_TRIANGLES, &xPoly);
      break;
    }
    default:
      break;
  }

  glutSwapBuffers();
}

void draw_mesh(GLenum mode, Mesh *M)
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
      mode, size / sizeof(GLuint),
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
  float aspect = 1.0;;
  glViewport(0, 0, w, h);

  /** set projection */
  if (w > h) {
    aspect = w / (float) h;
    mat4x4_ortho(projection, -aspect, aspect, -1, 1, -1, 1000);
  } else {
    aspect = h / (float) w;
    mat4x4_ortho(projection, -1, 1, -aspect, aspect, -1, 1000);


  }
  glUniformMatrix4fv(unif.projection, 1, GL_FALSE, &projection[0][0]);

}

void on_idle()
{
  static clock_t last_time = NAN;
  if (last_time == NAN) { last_time = clock(); }

  clock_t t = clock();

  xDt = (t - last_time) / (double) CLOCKS_PER_SEC;


  last_time = t;
}

void switch_scene(SceneName name)
{
  xScene_name = (name != xScene_name) ?
                name :
                xScene_name;
}

void on_keystroke(unsigned char key, int x, int y)
{
  mat4x4 matcpy;

  float speed = 5000.0 * xDt;


  switch (key) {

    // moving the camera
    case 'a': {
      mat4x4_translate_in_place(view, -speed, 0, 0);
      break;
    }
    case 'd': {
      mat4x4_translate_in_place(view, speed, 0, 0);
      break;
    }
    case 's': {
      mat4x4_translate_in_place(view, 0, -speed, 0);
      break;
    }
    case 'w': {
      mat4x4_translate_in_place(view, 0,  speed, 0);
      break;
    }
    case 'e': {
      float scaling = 1.0 + speed;
      mat4x4_dup(matcpy, view);
      mat4x4_scale_aniso(view, matcpy, scaling, scaling, scaling);
      break;
    }
    case 'q': {
      float scaling = 1.0 - speed;
      mat4x4_dup(matcpy, view);
      mat4x4_scale_aniso(view, matcpy, scaling, scaling, scaling);
      break;
    }
    case 'i': {
      mat4x4_dup(matcpy, view);
      mat4x4_rotate_X(view, matcpy, speed);
      break;
    }
    case 'k': {
      mat4x4_dup(matcpy, view);
      mat4x4_rotate_X(view, matcpy, -speed);
      break;
    }
    case 'j': {
      mat4x4_dup(matcpy, view);
      mat4x4_rotate_Y(view, matcpy, speed);
      break;
    }
    case 'l': {
      mat4x4_dup(matcpy, view);
      mat4x4_rotate_Y(view, matcpy, -speed);
      break;
    }
    case 'm': {
      ticker += 0.01;
      printf("%f\n", ticker);
      glUniform1f(unif.time, ticker);
      break;
    }

      // scene transitions
    case '1': {
      switch_scene(SCENE_SQUARES);
      break;
    }
    case '2': {
      switch_scene(SCENE_CIRCLE);

      break;
    }
    case '3': {
      switch_scene(SCENE_POLY);

      break;
    }
    default:
      break;
  }


  glutPostRedisplay();
}


int setup_gl(int argc, char *argv[])
{
  glutInit(&argc, argv);
  //glutInitContextVersion(2,1);
  unsigned int flags = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;
  glutInitDisplayMode(flags);
  glutInitWindowSize(640, 480);
  glutCreateWindow("openGL demo");

  glEnable(GL_DEPTH_TEST);

  glewExperimental = GL_TRUE;

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    log_err("Error: %s\n", glewGetErrorString(glew_status));
    return -1;
  }

  printf("%s\n", glGetString(GL_VERSION));



  glEnable(GL_BLEND);

  glEnable(GL_POINT_SIZE);
  glPointSize(10.0);
  return 1;


}


