#pragma once

#include "../General.h"

struct Viewport : public D3D11_VIEWPORT
{
	/*
	* Constructor
	*/
	__forceinline Viewport() = default ;

	/*
	*Copy Constructor
	*
	*/
	__forceinline Viewport(const Viewport & viewport) = default;

	/*
	*Constructor
	*
	* @param topLeftX coordinate of top left corner
	* @param topLeftY coordinate of top left corner
	* @param width viewport width in pixels
	* @param height viewport height in pixels
	* @param minDepth minimum depth to render
	* @param maxDepth maximum depth to render
	*/
	Viewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth);
	
	~Viewport();

	void Set(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth);
	
	void Bind();
};

__forceinline Viewport::Viewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	TopLeftX = topLeftX;
	TopLeftY = topLeftY;
	Width = width;
	Height = height;
	MinDepth = minDepth;
	MaxDepth = maxDepth;
}

__forceinline void Viewport::Set(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	TopLeftX = topLeftX;
	TopLeftY = topLeftY;
	Width = width;
	Height = height;
	MinDepth = minDepth;
	MaxDepth = maxDepth;
}

__forceinline void Viewport::Bind()
{
	DX11API::D3D11DeviceContext()->RSSetViewports(1, static_cast<const D3D11_VIEWPORT *>(this));
}