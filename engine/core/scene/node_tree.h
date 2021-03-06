#pragma once

#include "node.h"
#include "engine/core/gizmos/Gizmos.h"
#include "engine/core/camera/Camera.h"
#include "engine/core/camera/CameraShadow.h"

namespace Echo
{
	class NodeTree
	{	
	public:
		virtual ~NodeTree();

		// instance
		static NodeTree* instance();

		// 初始化
		bool init();

		// 销毁
		void destroy();

		Node* getInvisibleRootNode() { return &m_invisibleRoot; }

	public:
		// 获取主摄像机
		Camera* get3dCamera() const { return m_3dCamera; }

		// 设置主摄像机
		void set3dCamera(Camera* camera) { m_3dCamera = camera; }

		// 获取2D摄像机
		Camera* get2dCamera() const { return m_2dCamera; }

		// 获取界面摄像机
		Camera* getGUICamera() const { return m_uiCamera; }

		// 设置界面摄像机
		void setGUICamera(Camera* pCamera) { m_uiCamera = pCamera; }

	public:
		void update( float elapsedTime);

		// 获取阴影摄像机
		CameraShadow& getShadowCamera() { EchoAssert( m_shadowCamera);  return *m_shadowCamera; }

	private:
		NodeTree();

	protected:
		Camera*			    m_3dCamera;			// 主摄像机
		Camera*				m_2dCamera;			// 2D摄像机
		Camera*				m_uiCamera;			// 界面摄像机
		CameraShadow*		m_shadowCamera;		// 阴影图摄像机		
		Node				m_invisibleRoot;	// invisible root node
	};
}
