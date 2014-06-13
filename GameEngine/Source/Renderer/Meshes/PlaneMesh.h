#pragma once

#include "Mesh.h"

class PlaneMesh : public Mesh
{
public:
	__forceinline PlaneMesh(float scalex, float scaley, float scalez);
};