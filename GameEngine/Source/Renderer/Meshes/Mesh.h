#pragma once

class Mesh
{
protected:
	Mat4x4 m_objecttransform;
	Mat4x4 m_worldTransform;
};

typedef vector<shared_ptr<Mesh>> Meshes;