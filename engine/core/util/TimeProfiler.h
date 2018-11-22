#pragma once

#include <functional>
#include <string>
#include "engine/core/base/echo_def.h"

namespace Echo
{
	class TimeProfiler
	{
	public:
		// ��������
		struct Trace
		{
			int			m_id;			// ��ʶ
			int			m_tab;			// ����
			String		m_tag;			// ��ʶ(��ʾ)
			ulong		m_startTime;	// ��ʼʱ��
			ulong		m_totalTime;
		};

	public:
		TimeProfiler();
		~TimeProfiler();

		// ��ȡȫ��ʵ��
		static TimeProfiler& instance();

		// ����
		void reset();

		// ��ʼ��¼
		void beginSection(std::size_t section, const String& tag, const char* func);

		// ������¼
		void endSection(std::size_t section);

		// ���
		void output();

		// ͨ��id��ȡtrace
		bool getTraceByID( Trace& trace, int id);

		static size_t lordStrHash(const char *str);

	private:
		int							m_tab;					// ��ǰ����(��ջ��ջ)
		map<std::size_t, Trace>::type	m_traces;
		Dword						m_totalStartTime;		// �ܷ�����ʼʱ��
	};
}
//vs2010 only takes a subset of the string's characters to determine the hash value.so we have to implement it ourselves

#define ECHO_STR_EXPAND(token) #token
#define ECHO_STR(token) ECHO_STR_EXPAND(token)

#define ECHO_TIME_PROFILE_LINENAME_CAT(name, line) ECHO_STR(name+line)
#define ECHO_TIME_PROFILE_LINENAME(name, line) ECHO_TIME_PROFILE_LINENAME_CAT(name, line)
#define ECHO_STR_IDENTIFY ECHO_TIME_PROFILE_LINENAME(__FILE__, __LINE__)
// FIXME: hash collision !!!
#define ECHO_IDENTIFY (Echo::TimeProfiler::lordStrHash(ECHO_STR_IDENTIFY) + __LINE__)

#define TIME_PROFILE_ON		0
#if TIME_PROFILE_ON 
	#define TIME_PROFILE_RESET				Echo::TimeProfiler::instance().reset();
#define TIME_PROFILE(codeModule)		Echo::TimeProfiler::instance().beginSection(ECHO_IDENTIFY, ECHO_STR_IDENTIFY, __FUNCTION__); codeModule Echo::TimeProfiler::instance().endSection(ECHO_IDENTIFY);
	#define TIME_PROFILE_OUTPUT				Echo::TimeProfiler::instance().output();
#else
	#define TIME_PROFILE_RESET
	#define TIME_PROFILE(codeModule) codeModule
	#define TIME_PROFILE_OUTPUT
#endif

