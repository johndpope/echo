#pragma once

#include <engine/core/Thread/Pool/CpuThreadPool.h>
#include "engine/core/Base/echo_def.h"

namespace Echo
{
	/**
	 * ���߳������������ 2014-10-27 
	 * �ο� http://www.openmp.org
	 */
	class OpenMPTaskMgr
	{
	public:
		// ��������
		enum TaskType
		{
			TT_AnimationUpdate = 0,		// ��������
			TT_EffectSystem,			// ������Ч
		};

	public:
		~OpenMPTaskMgr();

		// instance
		static OpenMPTaskMgr* instance();

		// �������
		void addTask(TaskType type, CpuThreadPool::Job* task);

		// ִ������(����߼�����һ֡�ӳ�,���޴�)
		void execTasks(TaskType type);

		// �ȴ������������
		void waitForAnimationUpdateComplete();

		// �ȴ���Ч�������
		void waitForEffectSystemUpdateComplete();

	private:
		OpenMPTaskMgr();

	private:
		vector<CpuThreadPool::Job*>::type	m_animationUpdateTasks;				// ������������
		vector<CpuThreadPool::Job*>::type	m_animationUpdateTasksFinished;		// �����������������
		vector<CpuThreadPool::Job*>::type	m_effectSystemUpdateTasks;			// ��Ч��������
		vector<CpuThreadPool::Job*>::type	m_effectSystemUpdateTasksFinished;	// ��Ч��������
		CpuThreadPool*						m_threadPool;						// �̳߳�
	};
}