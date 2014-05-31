#pragma once

struct Viewport;
class RenderTargetView;
class DepthStencilView;
class DX11API
{
private:

	static ID3D11Device* g_d3d11Device;
	static ID3D11DeviceContext* g_d3d11DeviceContext;
	static IDXGISwapChain* g_pSwapChain;

	static unique_ptr<RenderTargetView> g_pBackBufferRTV;
	static unique_ptr<DepthStencilView> g_pDepthStencilView;
	static unique_ptr<Viewport> g_pViewport;

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

	/**
	* Swap back buffer
	*/
	static void PresentBackBuffer();
};