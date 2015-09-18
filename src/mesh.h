#ifndef MESH_H
#define MESH_H

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaderutils.h"
#include "transform.h"
#include "macros/linmath.h"

typedef struct {
    GLuint vbo;
    GLuint ibo;
    mat4x4 transform_mat;
} Mesh;

typedef struct {
    float pos[4];
    float normal[4];
    float color[4];
    float uv[2];
} Vertex;

Mesh cube_mesh();
Mesh plane_mesh();

Mesh make_mesh(Vertex *verts, 
               int n_verts, 
               uint32_t *indices, 
               int n_indices);

/**
 * frees buffers from GPU
 **/
void free_mesh(Mesh *M);

/**
 * scene graph info
 */

 typedef struct _sceneNode {
    char *name;
    Mesh *mesh;
    mat4x4 transform;
    
    /**
     * @brief number of elements allocated
     */
    size_t size;

    /**
     * @brief number of nodes included
     */
    size_t len;

    /**
     * @brief children node data
     */
    struct _sceneNode **children;

    /**
     * @brief parent node
     */
    struct _sceneNode *parent;
 } srSceneNode;

/**
 * @brief allocates a new pointer to a SceneNode
 * @details [long description]
 * @return [description]
 */
srSceneNode *sr_new_node(const char *name);

/**
 * @brief frees memory associated with node and its children
 * @details [long description]
 * 
 * @param node pointer to scene node
 */
void sr_delete_node(srSceneNode *node);

void sr_draw_node(srSceneNode *node, mat4x4 view);




#endif //MESH_H
