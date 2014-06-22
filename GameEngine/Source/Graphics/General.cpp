#include "Core.h"

#include "General.h"

#include "../Graphics/Other/Viewport.h"


ID3D11Device* DX11API::g_d3d11Device = nullptr;
ID3D11DeviceContext* DX11API::g_d3d11DeviceContext = nullptr;
IDXGISwapChain* DX11API::g_pSwapChain = nullptr;

RenderTargetView* DX11API::g_pBackBufferRTV = nullptr;
Viewport* DX11API::g_pViewport = nullptr;

bool DX11API::InitializeGraphics(HWND hWnd)
{
	/* Back buffer properties*/
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = SCREEN_WIDTH;
	bufferDesc.Height = SCREEN_HEIGHT;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/* Describe SwapChain structure */
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); //zero memory for the structure

	swapChainDesc.BufferDesc = bufferDesc; //set created back buffer
	swapChainDesc.SampleDesc.Count = SAMPLES_COUNT;
	swapChainDesc.SampleDesc.Quality = SAMPLES_QUALITY;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //use back buffer as render target
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	/* Create swap chain */
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &g_pSwapChain, &g_d3d11Device, nullptr, &g_d3d11DeviceContext);

	//assert(hr == S_OK && "Error creating device and swap chain");

	/* Create back buffer render target */
	ID3D11Texture2D* pBackBuffer;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	//assert(hr == S_OK && "Error acquiring data of back buffer");

	ID3D11RenderTargetView* pRTV;
	hr = g_d3d11Device->CreateRenderTargetView(pBackBuffer, nullptr, &pRTV);

	//assert(hr == S_OK && "Error creating render target");

	//g_pBackBufferRTV = make_unique<RenderTargetView>(pRTV);
	g_pBackBufferRTV = new RenderTargetView(pRTV);
	g_d3d11DeviceContext->OMSetRenderTargets(1, &pRTV, nullptr);

	/* Initialize and set viewport */
	//g_pViewport = make_unique<Viewport>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
	g_pViewport = new Viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
	g_pViewport->Bind();

	return true;
}

void DX11API::Dispatch(int groupCountX, int groupCountY, int groupCountZ)
{
	g_d3d11DeviceContext->Dispatch(groupCountX, groupCountY, groupCountZ);
}

void DX11API::Draw(int vertexCount, int startVertexLocation)
{
	g_d3d11DeviceContext->Draw(vertexCount, startVertexLocation);
}

void DX11API::DrawIndexed(int indexCount, int startIndexLocation, int baseVertexLocation)
{
	g_d3d11DeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

ID3D11Device* DX11API::D3D11Device()
{
	return g_d3d11Device;
}

ID3D11DeviceContext* DX11API::D3D11DeviceContext()
{
	return g_d3d11DeviceContext;
}

void DX11API::PresentBackBuffer()
{
	g_pSwapChain->Present(0, 0);
}

void DX11API::BindGlobalRenderTargetView()
{
	g_d3d11DeviceContext->OMSetRenderTargets(1, g_pBackBufferRTV->GetView(), nullptr);
}

void DX11API::BindGlobalRenderTargetView(DepthStencilView* pDepthStencilView)
{
	g_d3d11DeviceContext->OMSetRenderTargets(1, g_pBackBufferRTV->GetView(), pDepthStencilView->GetView());
}

void DX11API::BindGlobalDepthStencilView(DepthStencilView * pDepthStencilView)
{
	g_d3d11DeviceContext->OMSetRenderTargets(0, nullptr, pDepthStencilView->GetView());
}

void DX11API::BindGlobalViewport()
{
	g_pViewport->Bind();
}

void DX11API::ClearRenderTargetView(const float * color, RenderTargetView * pView)
{
	if (!pView)
	{
		DX11API::D3D11DeviceContext()->ClearRenderTargetView(*g_pBackBufferRTV->GetView(), color);
	}
	else
	{
		DX11API::D3D11DeviceContext()->ClearRenderTargetView(*pView->GetView(), color);
	}
}

void DX11API::ClearDepthStencilView(bool depth, bool stencil, float depthValue,
	unsigned int stencilValue, DepthStencilView * pView)
{
	if (!pView) return;

	if (depth && stencil)
		DX11API::D3D11DeviceContext()->ClearDepthStencilView(pView->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depthValue, stencilValue);
	else if (depth && !stencil)
		DX11API::D3D11DeviceContext()->ClearDepthStencilView(pView->GetView(), D3D11_CLEAR_DEPTH, depthValue, stencilValue);
	else if (!depth && stencil)
		DX11API::D3D11DeviceContext()->ClearDepthStencilView(pView->GetView(), D3D11_CLEAR_STENCIL, depthValue, stencilValue);
}

void DX11API::UnbindShaderResourceViews(unsigned int slot, unsigned int numviews, ShaderType shadertype)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;

	switch (shadertype)
	{
	case ST_Vertex:
		for (auto i = 0; i < numviews; i++)
			DX11API::D3D11DeviceContext()->VSSetShaderResources(slot + i, 1, &pNullSRV);
		break;
	case ST_Domain:
		for (auto i = 0; i < numviews; i++)
			DX11API::D3D11DeviceContext()->DSSetShaderResources(slot + i, 1, &pNullSRV);
		break;
	case ST_Geometry:
		for (auto i = 0; i < numviews; i++)
			DX11API::D3D11DeviceContext()->GSSetShaderResources(slot + i, 1, &pNullSRV);
		break;
	case ST_Pixel:
		for (auto i = 0; i < numviews; i++)
			DX11API::D3D11DeviceContext()->PSSetShaderResources(slot + i, 1, &pNullSRV);
		break;
	case ST_Compute:
		for (auto i = 0; i < numviews; i++)
			DX11API::D3D11DeviceContext()->CSSetShaderResources(slot + i, 1, &pNullSRV);
		break;
	default:
		assert(0);
		break;
	};
}

void DX11API::UnbindUnorderedAccessViews(unsigned int slot, unsigned int numviews)
{
	ID3D11UnorderedAccessView* pNullUAV = nullptr;

	for (auto i = 0; i < numviews; i++)
		DX11API::D3D11DeviceContext()->CSSetUnorderedAccessViews(slot + i, 1, &pNullUAV, nullptr);
}

void DX11API::UnbindRenderTargetViews(unsigned int numviews)
{

	ID3D11RenderTargetView* pNullRTV = nullptr;
	for (auto i = 0; i < numviews; i++)
		DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, &pNullRTV, NULL);
}

void DX11API::UnbindGeometryShader()
{
	ID3D11GeometryShader* pNullShader = nullptr;
	DX11API::D3D11DeviceContext()->GSSetShader(pNullShader, nullptr, 0);
}