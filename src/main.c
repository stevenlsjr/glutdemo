#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "macros/linmath.h"
#include "macros/dbg.h"


#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "renderer.h"
#include "mesh.h"
#include "image.h"




int main (int argc, char *argv[])
{
	if (setup_gl(argc, argv) < 0) {
		log_err("Error: Context creation failure");
        return -1;
	}
    
    /** init DevIL */
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);
	
	init_resources();
    
    /** load images */
    const char *paths[] = {"resources/art/grad.png", 
                           "resources/art/grad1.png", 
                           "resources/art/grad2.png", 
                           NULL};
    int len = 0;
    for (int i=0; paths[i] != NULL; i++) {
        len ++;
    }
    Images *img = load_images(&paths[0], len);
    set_images(img);
    set_active_img(0);
    bind_active_img();
	
    /** setup callbacks */
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_resize);
    glutKeyboardFunc(on_keystroke);
    glutIdleFunc(on_idle);
    
	/** start main loop */
	glutMainLoop();
	
    free_images(img);
	free_resources();
	return 0;
}
