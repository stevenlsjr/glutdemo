#include "macros/dbg.h"
#include "macros/linmath.h"
#include "macros/constants.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shaderutils.h"
#include "mesh.h"
#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>


static Mesh _mesh_factory(Vertex *verts, GLuint *index,int nverts,int nindex);

static Mesh _mesh_factory(Vertex *verts, GLuint *index,int nverts,int nindex)
{
    Mesh mesh;
    
    /** setup vbo */
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        nverts * sizeof(Vertex),
        verts,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    /** setup ibo */

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        nindex * sizeof(GLuint),
        index,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    /**
     * set model transform matrix to identity
     **/
    mat4x4_identity(mesh.transform_mat);
    
    return mesh;
}

void free_mesh(Mesh *M)
{
    glDeleteBuffers(1, &M->vbo);
    glDeleteBuffers(1, &M->ibo);
}

Mesh cube_mesh()
{
    
    Vertex verts[] = {
        //front
        {.pos={-1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={-1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={ 1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={ 1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 0.0}},
        //back
        {.pos={-1.0, -1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={-1.0,  1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={ 1.0,  1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={ 1.0, -1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 0.0}},
        //top
        {.pos={-1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={ 1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={ 1.0,  1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={-1.0,  1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 0.0}},
        //bottom
        {.pos={-1.0, -1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={ 1.0, -1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={ 1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={-1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 0.0}},
        //left                                                      
        {.pos={-1.0, -1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={-1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={-1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={-1.0,  1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 0.0}},
        //right                                                     
        {.pos={ 1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={ 1.0, -1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={ 1.0,  1.0,-1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={ 1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={1.0, 1.0, 1.0, 1.0}, .uv={1.0, 0.0}}
    };

    GLuint index[] = {
        // front
         0,  1,  2,
         2,  3,  0,
        // top
         4,  5,  6,
         6,  7,  4,
        // back
         8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20
    };
    int nverts = sizeof(verts)/sizeof(Vertex);
    int nindex = sizeof(index)/sizeof(int);
    Vertex *vertptr = malloc(sizeof(Vertex) * (nverts + 1));
    GLuint *indptr = malloc(sizeof(GLuint) * (nindex + 1));
    
    memcpy(vertptr, &verts[0], sizeof(verts));
    memcpy(indptr, &index[0], sizeof(index));
    
    Mesh cube = _mesh_factory(vertptr, indptr, nverts, nindex);
    free(vertptr);
    free(indptr);
    return cube;
}

Mesh plane_mesh()
{
    Vertex verts[] = {
        {.pos={-1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 1.0, 0.0, 1.0}, .uv={0.0, 0.0}},
        {.pos={-1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 1.0, 0.0, 1.0}, .uv={0.0, 1.0}},
        {.pos={ 1.0,  1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 1.0, 0.0, 1.0}, .uv={1.0, 1.0}},
        {.pos={ 1.0, -1.0, 1.0, 1.0}, .normal={0.0, 0.0, 1.0, 0.0}, .color={0.0, 1.0, 0.0, 1.0}, .uv={1.0, 0.0}},
    };
    GLuint index[] = {
        // front
         0,  1,  2,
         2,  3,  0,
    };
    
    int nverts = sizeof(verts)/sizeof(Vertex);
    int nindex = sizeof(index)/sizeof(int);
    Vertex *vertptr = malloc(sizeof(Vertex) * (nverts + 1));
    GLuint *indptr = malloc(sizeof(GLuint) * (nindex + 1));
    
    memcpy(vertptr, &verts[0], sizeof(verts));
    memcpy(indptr, &index[0], sizeof(index));
    
    Mesh plane = _mesh_factory(vertptr, indptr, nverts, nindex);
    free(vertptr);
    free(indptr);
    return plane;
}

static inline void print_verts(Vertex *verts, int nverts) {
    for (int i=0; i<nverts; i++) {
        for (int j=0; j<4; j++) {
            printf("%f, ", verts[i].pos[j]);
        }
        printf("\n");
    }
}

 srSceneNode *sr_new_node(const char *name)
 {
    srSceneNode *node = malloc(sizeof(srSceneNode));

    return node;
 }

 void sr_delete_node(srSceneNode *node)
 {}

 void sr_draw_node(srSceneNode *node, mat4x4 view)
 {}