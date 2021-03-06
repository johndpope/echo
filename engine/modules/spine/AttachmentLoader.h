#pragma once

#include <spine/spine.h>
#include <interface/GPUBuffer.h>
#include <interface/Renderable.h>
#include "interface/Texture.h"

extern "C"
{
	typedef struct EchoAttachmentLoader
	{
		spAttachmentLoader		 super;
		spAtlasAttachmentLoader* atlasAttachmentLoader;
	} EchoAttachmentLoader;

	/* The EchoAttachmentLoader must not be disposed until after the skeleton data has been disposed. */
	EchoAttachmentLoader* EchoAttachmentLoader_create(spAtlas* atlas);
}

namespace Echo
{
	struct SpineVertexFormat
	{
		Vector3	m_position;		// 位置
		ui32	m_diffuse;		// 颜色值
		Vector2	m_uv;			// UV坐标
	};

	/**
	* Spine AttachmentVertices
	*/
	class AttachmentVertices
	{
	public:
		AttachmentVertices();
		AttachmentVertices(Texture* texture, int verticesCount, ui16* triangles, int indicesCount);
		virtual ~AttachmentVertices();

		// render
		void merge(const AttachmentVertices& other);

		// clear
		void clear();

	public:
		TexturePtr						m_texture;			// 纹理
		vector<SpineVertexFormat>::type	m_verticesData;		// 顶点数据
		vector<ui16>::type				m_indicesData;		// 索引数据
	};
}
