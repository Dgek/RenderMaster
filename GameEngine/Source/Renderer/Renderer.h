#pragma once

class Mesh;
class Light;
class Camera;
class Renderer
{
public:
	Renderer();

	/***	Put something to render	***/
	virtual void VPushMesh(shared_ptr<Mesh> pMesh) = 0;
	virtual void VPushLight(shared_ptr<Light> pLight) = 0;

	/***	Cameras management	***/
	virtual void AddCamera(shared_ptr<Camera> pCamera);
	virtual void RemoveCamera(Camera* pCamera);

	/*** Rendering Core ***/
	virtual void VRender() = 0;
};