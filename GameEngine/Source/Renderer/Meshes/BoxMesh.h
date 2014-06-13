#pragma once

#include "Mesh.h"

class BoxMesh : public Mesh
{
public:

	__forceinline BoxMesh(float scale, float scaley, float scalez);
};