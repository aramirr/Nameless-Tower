#ifndef INC_RENDER_OBJECTS_H_
#define INC_RENDER_OBJECTS_H_

bool createRenderObjects();
void destroyRenderObjects();

// To be converted in resources and registered in the resources manager
extern CRenderMesh* axis;
extern CRenderMesh* grid;

#endif

