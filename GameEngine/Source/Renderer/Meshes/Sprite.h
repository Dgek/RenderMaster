#pragma once

#include "Mesh.h"

class Sprite : public Mesh
{
public:
	__forceinline Sprite(float scalex, float scaley);
};

__forceinline Sprite::Sprite(float scalex, float scaley)
{}