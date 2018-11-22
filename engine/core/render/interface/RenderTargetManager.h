#pragma once

#include "engine/core/base/echo_def.h"
#include "engine/core/render/interface/Texture.h"
#include "engine/core/render/interface/RenderTarget.h"
#include "engine/core/render/interface/Renderer.h"

namespace Echo
{
	enum RenderTargetID
	{
		RTI_DefaultBackBuffer,
		RTI_End
	};

	class RenderTargetManager
	{
	public:
		virtual ~RenderTargetManager();

		// ��ȡ��Ⱦʵ��
		static RenderTargetManager* instance();

		// ��ʼ��
		bool initialize();

		// ��ʼ��ȾĿ��
		bool beginRenderTarget( ui32 _id,bool _clearColor = true, const Color& _backgroundColor = Renderer::BGCOLOR,  bool _clearDepth = true, float _depthValue = 1.0f, bool _clearStencil = false, ui8 stencilValue = 0, ui32 rbo = RenderTarget::Invalid_Value );

		// ������ȾĿ��
		bool endRenderTarget(ui32 _id);

		// ������ȾĿ��
		RenderTarget* createRenderTarget(ui32 _id, ui32 _width, ui32 _height, PixelFormat _pixelFormat, RenderTarget::Options option = RenderTarget::Options());

		// destory render target
		void destroyRenderTargetByID(ui32 _id);

		// ����ID��ȡ��ȾĿ��
		RenderTarget* getRenderTargetByID(ui32 _id);

		// get current render target
		ui32 getCurrentRenderTarget() const { return m_currentRenderTarget; }

		// ����Ļ��С�ı�ʱ���� 
		void onScreensizeChanged( ui32 _width, ui32 _height );

	private:
		RenderTargetManager();

		// invalide
		bool invalidate(ui32 id, bool invalidateColor, bool invalidateDepth, bool invalidateStencil);

	protected:
		RenderTargetMap			m_renderTargets;					// ������ȾĿ��
		ui32					m_currentRenderTarget;
	};
}
