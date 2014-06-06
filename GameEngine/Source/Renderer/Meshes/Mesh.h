#pragma once

#include "../Objects/IMovable.h"

class Mesh : public IMovable
{
protected:
	Mat4x4 m_objectTransform;

public:

	virtual void SetWorldTransform(const Mat4x4 & transform) override;
};

typedef vector<shared_ptr<Mesh>> Meshes;