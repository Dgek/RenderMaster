#pragma once

#include "../Graphics/Resources/Views/RenderTargetView.h"

struct Viewport;
class DepthStencilView;
class DX11API
{
private:

	static ID3D11Device* g_d3d11Device;
	static ID3D11DeviceContext* g_d3d11DeviceContext;
	static IDXGISwapChain* g_pSwapChain;

	static RenderTargetView* g_pBackBufferRTV;
	static DepthStencilView* g_pDepthStencilView;
	static Viewport* g_pViewport;

public:
	/** Initialize graphics API
	*
	* @param hwnd Handle to the window
	* @param width Window width
	* @param height Window height
	*/
	static bool InitializeGraphics(HWND hWnd);

	static void Dispatch(int groupCountX, int groupCountY, int groupCountZ);
	static void Draw(int vertexCount, int startVertexLocation);
	static void DrawIndexed(int indexCount, int startIndexLocation, int baseVertexLocation);

	static ID3D11Device* D3D11Device();
	static ID3D11DeviceContext* D3D11DeviceContext();

	static void BindGlobalRenderTargetView();
	static void BindGlobalRenderTargetView(DepthStencilView* pDepthStencilView);
	static void BindGlobalDepthStencilView(DepthStencilView * pDepthStencilView);
	static void BindGlobalViewport();

	//*** Fill render target with color ***//
	//------------------------------------------
	//-- 1: Color to fill with in float[4] format
	//-- 2*: Render target to fill
	//------------------------------------------
	static void ClearRenderTargetView(const float * color, RenderTargetView * pView);

	//*** Fill render target with color ***//
	//------------------------------------------
	//-- 1: Set to true if we need to clean depth
	//-- 2: Set to true if we need to clean stencil
	//-- 3: Value to fill depth buffer
	//-- 4: Value to fill stencil buffer
	//-- 5*: Depth stencil view to fill
	//------------------------------------------
	static void ClearDepthStencilView(bool depth, bool stencil, float depthValue,
		unsigned int stencilValue, DepthStencilView * pView);

	//*** Unbind shader resource views from the pipeline ***/
	//----------------------------------------------------------
	//-- 1: Slot to unbind
	//-- 2: Number of views
	//-- 3: Shader type to unbind from
	//----------------------------------------------------------
	static void UnbindShaderResourceViews(unsigned int slot, unsigned int numviews, ShaderType shadertype);

	//*** Unbind unordered access views from the pipeline ***/
	//----------------------------------------------------------
	//-- 1: Slot to unbind
	//-- 2: Number of views
	//----------------------------------------------------------
	static void UnbindUnorderedAccessViews(unsigned int slot, unsigned int numviews);

	//*** Unbind render target views from the pipeline ***/
	//--------------------------------------------------------
	//-- 1: Number of views
	//--------------------------------------------------------
	static void UnbindRenderTargetViews(unsigned int numviews);

	//*** Unbind geoemtry shader from the pipeline ***/
	//----------------------------------------------------
	//--
	//----------------------------------------------------
	static void UnbindGeometryShader();

	/**
	* Swap back buffer
	*/
	static void PresentBackBuffer();
};