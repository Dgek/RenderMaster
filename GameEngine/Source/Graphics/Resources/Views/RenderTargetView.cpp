#include "Core.h"

#include "RenderTargetView.h"
#include "../../General.h"

RenderTargetView::RenderTargetView()
: m_ppViews{ nullptr }, m_numViews{ 1 }
{}

RenderTargetView::RenderTargetView(int numViews)
: m_ppViews{ nullptr }, m_numViews{ numViews }
{}

RenderTargetView::RenderTargetView(ID3D11RenderTargetView * pView)
{
	m_ppViews = &pView;
}

ID3D11RenderTargetView** RenderTargetView::GetView(int index) const
{
	return &m_ppViews[index];
}

//void Set(DepthStencilViewDX11 * pView) const;
void RenderTargetView::BindOneView(int index)
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, m_ppViews + index, nullptr);
}

void RenderTargetView::Bind(const DepthStencilView & depthview) const
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, m_ppViews, *depthview.GetView());
}
void RenderTargetView::Bind() const
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargets(1, m_ppViews, nullptr);
}

void RenderTargetView::BindWithUAV(unsigned int uavStartSlot, unsigned int uavNum,
	ID3D11UnorderedAccessView * const * ppUnorderedAccessViews, const unsigned int * pUAVInitialCounts)
{
	DX11API::D3D11DeviceContext()->OMSetRenderTargetsAndUnorderedAccessViews(1, m_ppViews, nullptr, uavStartSlot, uavNum, ppUnorderedAccessViews, pUAVInitialCounts);

}

void RenderTargetView::Clear(float bgColor[4])
{
	for (auto i = 0; i < m_numViews; i++)
		DX11API::D3D11DeviceContext()->ClearRenderTargetView(m_ppViews[i], bgColor);
}

void RenderTargetView::Clear()
{
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (auto i = 0; i < m_numViews; i++)
		DX11API::D3D11DeviceContext()->ClearRenderTargetView(m_ppViews[i], color);
}