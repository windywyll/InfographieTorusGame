#ifndef BASE_MESH_RENDERER_H
#define BASE_MESH_RENDERER_H

#include "abstractMeshRenderer.h"

class BaseMeshRenderer : public AbstractMeshRenderer
{
public:
	BaseMeshRenderer();
	~BaseMeshRenderer();
	void draw();

	Mesh *getMesh();
	void setMesh(Mesh *mesh);

	void calculateBoundingBox();

	void calculateModelMatrix();
private:
	Mesh *m_mesh;
};

#endif
