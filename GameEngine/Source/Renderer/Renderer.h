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
	virtual void VAddCamera(shared_ptr<Camera> pCamera);
	virtual void VRemoveCamera(Camera* pCamera);

	virtual void VUpdate(unsigned int deltaMilliseconds) = 0;

	/*** Rendering Core ***/
	virtual void VRender() = 0;

	/*** Accessors ***/
	__forceinline bool LightningOn() const;
	__forceinline bool TexturingOn() const;
};