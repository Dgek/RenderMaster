#pragma once

#include "../Objects/IMovable.h"

class Light : public IMovable
{
public:

	virtual void SetWorldTransform(const Mat4x4 & transform) override;
};

typedef vector<shared_ptr<Light>> Lights;