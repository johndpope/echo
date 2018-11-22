#pragma once

#include "engine/core/base/type_def.h"
#include "engine/core/memory/MemAllocDef.h"

namespace Echo
{
	class FrameState
	{
	public:
		FrameState()
			: m_uploadedTextureSizeInBytes(0)
			, m_uploadedGeometricSize(0)
			, m_rendertargetSize(0)
			, m_drawCallTimes(0)
			, m_lockTimes(0)
			, m_fps(0)
			, m_triangleNum(0)
		{}

		~FrameState()
		{}

		// ����
		void reset()
		{
			m_triangleNum = 0;
			m_drawCallTimes = 0;
			m_lockTimes = 0;
			m_fps = 0;
			m_maxFrameTimePerSecond = 0;
		}

		// ��������ռ�ô�С(��λ���ֽ�)
		void incrUploadTextureSizeInBytes(ui32 _sizeInBytes) { m_uploadedTextureSizeInBytes += _sizeInBytes; }

		// ��������ռ�ô�С(��λ���ֽ�)
		void decrUploadTextureSizeInBytes(ui32 _sizeInBytes) { m_uploadedTextureSizeInBytes -= _sizeInBytes; }

		// ���Ӽ����������δ�С
		void incrUploadGeometricSize(ui32 _sizeInBytes) { m_uploadedGeometricSize += _sizeInBytes; }

		// ���ټ������ڼ�¼��С
		void decrUploadGeometricSize(ui32 _sizeInBytes) { m_uploadedGeometricSize -= _sizeInBytes; }

		// ������Ⱦ����
		void incrDrawCallTimes(ui32 _times) { m_drawCallTimes += _times; }

		// ����������
		void incrLockTimes(ui32 _time) { m_lockTimes += _time; }

		// ��������������
		void incrTriangleNum(ui32 _triangles) { m_triangleNum += _triangles; }

		// ��ȡ��Ⱦ����
		ui32 getDrawCalls() const { return m_drawCallTimes; }

		// ��ȡ����ߴ�
		ui32 getTextureSizeInBytes() const { return m_uploadedTextureSizeInBytes; }

		// ��ȡ�����С
		ui32 getVertexSize() const { return m_uploadedGeometricSize; }

		// ��ȡ����������
		ui32 getTriangleNum() const { return m_triangleNum; }

		// ����rendertarget�ڴ��С
		void incrRendertargetSize(const ui32 size) { m_rendertargetSize += size; }
		void decrRendertargetSize(const ui32 size) { m_rendertargetSize -= size; }

		// ��ȡrendertarget�ڴ��С
		ui32 getRendertargetSize() const { return m_rendertargetSize; }

	protected:
		ui32	m_uploadedTextureSizeInBytes;
		ui32	m_uploadedGeometricSize;
		ui32	m_rendertargetSize;
		ui32	m_drawCallTimes;
		ui32	m_lockTimes;
		ui32	m_fps;
		ui32	m_maxFrameTimePerSecond;
		ui32	m_minTimeFrame;
		ui32	m_maxTimeFrame;
		ui32	m_triangleNum;
		String	m_fpsMsg;
		String	m_maxFrameTimeMsg;
		String	m_lockTimesMsg;
		String	m_drawCallTimesMsg;
		String	m_triangleNumMsg;
		String	m_uploadedTextureSizeInBytesMsg;
		String	m_uploadedGeometricSizeMsg;
		String	m_streamThreadCount;
		String	m_mathOpCount;
	};
}
