#pragma once

#include <map>
#include <thirdparty/pugixml/pugixml.hpp>
#include <engine/core/util/StringUtil.h>

namespace Studio
{
	class ConfigMgr
	{
	public:
		ConfigMgr();
		~ConfigMgr();

	public:
		// ��ȡ�����ļ�
		bool loadCfgFile( );

		// ���һ������򿪵���Ŀ
		bool addRecentProject( const char* fileName);

		// ʹĳ��Ŀ�л�����ǰ
		void switchProjectToTop(const char* fileName);

		// ���������ļ�

		bool saveCfgFile( );

		// �ļ��Ƿ���ڣ�����������򴴽���
		bool isFileExit( );

		// ��ǰ�����Ƿ��Ѿ�����
		bool isPathExist( Echo::String path );

		// �������·��
		void setOutPutDir( const char* path ) { m_outPutDir = path; }

		// ��ȡ���·��
		const char* getOutPutDir( ) { return m_outPutDir.c_str(); }

		// ��ȡ����򿪵���Ŀ�ļ�
		Echo::String getLastOpenProjectFile();

		// �������ƻ�ȡ����ֵ
		Echo::String getValue( const char* property);

		// ��������
		void setValue( const char* property, const char* value);

		// ��ȡ��������򿪵Ĺ���
		void getAllRecentProject(Echo::list<Echo::String>::type& projects);

	private:
		// ���浽�ļ�
		void saveData( pugi::xml_document& doc, pugi::xml_node* projectNode );

		// ��ȡ����򿪵Ĺ���
		void loadRecentProject( pugi::xml_node* node);

		// ��ȡ��Դ���·��
		void loadOutPutDir( pugi::xml_node* node);

		// ��������ֵ
		void loadPropertys( pugi::xml_node* node);

	private:
		Echo::String						m_cfgFile;			// �����ļ�
		Echo::list<Echo::String>::type		m_recentProjects;	// ����򿪵Ĺ���
		Echo::String						m_outPutDir;		// ��Դת�����·��
		size_t								m_maxRecentProjects;// ��¼�����
		std::map<Echo::String, Echo::String>m_propertys;		// ����
	};
}