#pragma once

#include "Mesh.h"

class PlaneMesh : public Mesh
{
public:
	PlaneMesh(float scalex, float scaley, float scalez);
};

__forceinline PlaneMesh::PlaneMesh(float scalex, float scaley, float scalez)
{}