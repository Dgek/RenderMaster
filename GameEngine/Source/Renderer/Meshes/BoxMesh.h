#pragma once

#include "Mesh.h"

class BoxMesh : public Mesh
{
public:

	BoxMesh(float scalex, float scaley, float scalez);
};

__forceinline BoxMesh::BoxMesh(float scalex, float scaley, float scalez)
{}