#include "FrameBuffer.h"
#include "Renderer.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"

namespace Echo
{
	FrameBuffer::FrameBuffer()
		: m_pDSV(NULL)
		, m_pViewport(NULL)
	{
		for(ui32 i = 0; i < MAX_RENDERTARGETS; ++i)
		{
			m_pRTVs[i] = NULL;
		}
	}

	FrameBuffer::~FrameBuffer()
	{
		for(ui32 i = 0; i < MAX_RENDERTARGETS; ++i)
			EchoSafeDelete(m_pRTVs[i], RenderTargetView);

		EchoSafeDelete(m_pDSV, DepthStencilView);
		EchoSafeDelete(m_pViewport, Viewport);
	}

	void FrameBuffer::attachRTV(ui32 idx, RenderTargetView* pRTV)
	{
		EchoAssert(pRTV);

		detachRTV(idx);

		m_pRTVs[idx] = pRTV;
		pRTV->onAttached(idx);
	}

	void FrameBuffer::detachRTV(ui32 idx)
	{
		EchoAssert(0 <= idx && idx < MAX_RENDERTARGETS);

		if(m_pRTVs[idx])
		{
			m_pRTVs[idx]->onDetached();
			EchoSafeDelete(m_pRTVs[idx], RenderTargetView);
		}
	}

	void FrameBuffer::attachDSV(DepthStencilView* pDSV)
	{
		detachDSV();
		m_pDSV = pDSV;
		m_pDSV->onAttached();
	}

	void FrameBuffer::detachDSV()
	{
		if(m_pDSV)
		{
			m_pDSV->onDetached();
			EchoSafeDelete(m_pDSV, DepthStencilView);
		}
	}

	RenderTargetView* FrameBuffer::getRenderTargetView(ui32 idx) const
	{
		EchoAssert(0 <= idx && idx < MAX_RENDERTARGETS);
		return m_pRTVs[idx];
	}

	DepthStencilView* FrameBuffer::getDepthStencilView() const
	{
		return m_pDSV;
	}

	void FrameBuffer::setViewport(Viewport* pViewport)
	{
		EchoAssert(pViewport);
		EchoSafeDelete(m_pViewport, Viewport);
		m_pViewport = pViewport;
	}

	Viewport* FrameBuffer::getViewport() const
	{
		return m_pViewport;
	}
}
