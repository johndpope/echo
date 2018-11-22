#pragma once

#include "device_features.h"
#include "RenderState.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureCube.h"
#include "Renderable.h"
#include "RenderTarget.h"
#include "GPUBuffer.h"

namespace Echo
{
	class Renderer
	{
		typedef RasterizerState::PolygonMode PolygonFillMode;
	public:
		// config
		struct Config
		{
			ui32	screenWidth;
			ui32	screenHeight;
			float	nearClip;
			float	farClip;
			bool	bFullscreen;
			bool	bVSync;
			bool	bSeparatedAlpha;
			bool	enableThreadedRendering;

			// for Windows Editor.
			size_t windowHandle;

			Config()
				: screenWidth(800)
				, screenHeight(600)
				, nearClip(0.1f)
				, farClip(10000.0f)
				, bFullscreen(false)
				, bVSync(false)
				, bSeparatedAlpha(false)
				, enableThreadedRendering(true)
                , windowHandle(0)
			{}
		};

		static Color BGCOLOR;
		typedef map<ui32, SamplerState*>::type	StageSamplerMap;

	public:
		Renderer();
		virtual ~Renderer();

		// ����ʵ��
		static Renderer* instance();

		// �滻ʵ��
		static bool replaceInstance(Renderer* inst);

		// ��ʼ��
		bool initialize(const Config& config);
		
		// Ӳ��֧���Լ��
		virtual void checkOpenGLExtensions() = 0;

		// �����ӿ�
		virtual void setViewport(Viewport* pViewport) = 0;

		// ���ù�դ��״̬
		virtual void setRasterizerState(RasterizerState* pState);

		// �������ģ��״̬
		virtual void setDepthStencilState(DepthStencilState* pState);

		// ���û��״̬
		virtual void setBlendState(BlendState* pState);

		// set texture
		virtual void setTexture(ui32 index, Texture* texture, bool needUpdate = false) = 0;

		// is full screen
		virtual bool isFullscreen() const;

		// get render config
		const Config& getCfg() const { return m_cfg; }

		virtual bool				isVSync() const;
		virtual ui32				getMaxStageNum() const = 0;
		virtual FrameBuffer*		getFrameBuffer() const;
		virtual RasterizerState*	getDefaultRasterizerState() const;
		virtual DepthStencilState*	getDefaultDepthStencilState() const;
		virtual BlendState*			getDefaultBlendState() const;
		virtual RasterizerState*	getRasterizerState() const;
		virtual DepthStencilState*	getDepthStencilState() const;
		virtual BlendState*			getBlendState() const;
		//virtual SamplerState*		getSamplerState(ui32 stage) const;

		// scissor command
		virtual void scissor(ui32 left, ui32 top, ui32 width, ui32 height)=0;
		virtual void endScissor()=0;

		// draw
		virtual void draw(Renderable* program)=0;

		virtual void project(Vector3& screenPos, const Vector3& worldPos, const Matrix4& matVP, Viewport* pViewport = NULL);
		virtual void unproject(Vector3& worldPos, const Vector3& screenPos, const Matrix4& matVP, Viewport* pViewport = NULL);
		virtual void getDepthRange(Vector2& vec) = 0;
		virtual void convertMatOrho(Matrix4& mat, const Matrix4& matOrth, Real zn, Real zf) = 0;
		virtual void convertMatProj(Matrix4& mat, const Matrix4& matProj) = 0;

		// create buffer
		virtual GPUBuffer* createVertexBuffer(Dword usage, const Buffer& buff) = 0;
		virtual GPUBuffer* createIndexBuffer(Dword usage, const Buffer& buff) = 0;

		// create texture
		virtual Texture*     createTexture2D(const String& name)=0;
		virtual TextureCube* createTextureCube(const String& name) = 0;

		void dirtyTexSlot(){ m_dirtyTexSlot = true; }

		//virtual Texture*			createTextureFromImage(Image* pImage, Dword usage = Texture::TU_DEFAULT);
		virtual ShaderProgram*		createShaderProgram(ShaderProgramRes* material)=0;
		virtual Shader*				createShader(Shader::ShaderType type, const Shader::ShaderDesc& desc, const String& filename) = 0;
		virtual Shader*				createShader(Shader::ShaderType type, const Shader::ShaderDesc& desc, const char* srcBuffer, ui32 size) = 0;
		virtual RenderTargetView*	createRenderTargetView(PixelFormat fmt, ui32 width, ui32 height) = 0;
		virtual DepthStencilView*	createDepthStencilView(PixelFormat fmt, ui32 width, ui32 height) = 0;
		virtual RasterizerState*	createRasterizerState(const RasterizerState::RasterizerDesc& desc) = 0;
		virtual DepthStencilState*	createDepthStencilState(const DepthStencilState::DepthStencilDesc& desc) = 0;
		virtual BlendState*			createBlendState(const BlendState::BlendDesc& desc) = 0;
		virtual const SamplerState*	getSamplerState(const SamplerState::SamplerDesc& desc) = 0;

		// ������ȾĿ��
		virtual RenderTarget* createRenderTarget(ui32 _id, ui32 _width, ui32 _height, PixelFormat _pixelFormat, const RenderTarget::Options& option) = 0;

		// create Renderable
		Renderable* createRenderable(const String& renderStage, ShaderProgramRes* material);

		// ��ȡRenderable
		Renderable* getRenderable(RenderableID id);

		// ����Renderable
		void destroyRenderables(Renderable** renderables, int num);

		// ����Renderable
		void destroyRenderables(vector<Renderable*>::type& renderables);

		// ��ȡ�豸����
		DeviceFeature&	getDeviceFeatures() { return m_deviceFeature; }

		// ��Ļ��
		virtual ui32 getScreenWidth() = 0;

		// ��Ļ��
		virtual ui32 getScreenHeight() = 0;

		// ��ȡ��ʵ�ӿڴ�С
		virtual void getViewportReal(Viewport& pViewport)=0;

		// ���������ʽ
		PixelFormat	getBackBufferPixelFormat() const { return m_backBufferFormat; }

		// �л���ǰ̨
		bool present();

		// �����Ƿ�ִ��֡����
		void setEnableFrameProfile(bool _enable){ m_isEnableFrameProfile = _enable; }

		// �ж���Ⱦ���Ƿ�����֡����
		bool isEnableFrameProfile() const { return m_isEnableFrameProfile; }

		// ���ó�ʼ����Mipmap����
		void setStartMipmap(ui32 mipmap) { m_startMipmap = mipmap; }

		// ��ȡ
		ui32 getStartMipmap() const { return m_startMipmap; }

		// ����RendererSize
		virtual void onSize(int width, int height) = 0; 

	protected:
		virtual bool doPresent() = 0;
		virtual bool initializeImpl(const Config& config) = 0;
		virtual void createSystemResource() = 0;
		virtual Renderable* createRenderableInernal(const String& renderStage, ShaderProgramRes* shader, int identifier)=0;

	protected:
		Config				m_cfg;
		bool				m_bVSync;
		FrameBuffer*		m_pFrameBuffer;
		RasterizerState*	m_pDefaultRasterizerState;
		DepthStencilState*	m_pDefaultDepthStencilState;
		BlendState*			m_pDefaultBlendState;
		RasterizerState*	m_pRasterizerState;
		DepthStencilState*	m_pDepthStencilState;
		BlendState*			m_pBlendState;
		bool				m_bSupportsDXT;
		bool				m_bSupportsPVRTC;
		bool				m_bSupportsATITC;
		bool				m_bSupportsDepthTextures;
		bool				m_bSupportsAnisotropy;
		ui32				m_backBufferBlueBits;
		ui32				m_backBufferRedBits;
		ui32				m_backBufferGreenBits;
		ui32				m_backBufferAlphaBits;
		ui32				m_backBufferBits;
		ui32				m_depthBufferBits;
		PixelFormat			m_backBufferFormat;
		ui16				m_bBind2RGBTexture;
		ui16				m_bBind2RGBATexture;
		int					m_renderableIdentifier;			// ��Ⱦ��Ԫ����
		std::map<ui32, Renderable*>	m_renderables;			// ����Ⱦ��λ
		ui32				m_startMipmap;					// ���������ʼmipmap���𣬿�����������������
		DeviceFeature		m_deviceFeature;				// �豸������Ϣ
		bool				m_isEnableFrameProfile;			// �Ƿ�ִ��֡����
		bool				m_dirtyTexSlot;
	};
}
