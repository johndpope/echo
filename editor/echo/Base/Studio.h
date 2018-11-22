#pragma once

#include <string>
#include <engine/core/log/LogOutput.h>
#include <engine/core/editor/editor.h>
#include "ProjectMgr.h"
#include "ConfigMgr.h"
#include "IInputController.h"
#include "Logic/ThumbnailMgr/ThumbnailMgr.h"
#include <QWidget>
#include <QDockWidget>

namespace Studio
{
	class MainWindow;
	class ProjectWnd;
	class RenderWindow;
	class LogPanel;
	class AStudio : public Echo::Editor
	{
	public:
		~AStudio();

		// instance
		static AStudio* instance();

		// ����
		void Start();

		// �ر�
		void Close();

		// ���ó�����·��
		void setAppPath( const char* appPath);

		// ��ȡ������·��
		const char* getAppPath() const { return m_appPath.c_str(); }

		// get editor root path
		const Echo::String& getRootPath();

		// ��ȡ���ù�����
		ConfigMgr* getConfigMgr() { return m_projectCfg; }

		// ��ȡ��־����
		LogPanel* getLogPanel() { return m_logPanel; }

		// ��ȡ��Ⱦ����
		virtual RenderWindow* getRenderWindow();

		// ��������ͼ
		virtual bool saveThumbnail( const Echo::String& fileName, int type = 0 );

		// �����ļ�����ȡ����ͼȫ·��
		Echo::String getThumbnailPath( const Echo::String& filePath,bool needOldExt = true);
		
		// �������������
		virtual void resetCamera(float diroffset = 0);

		// ������Ⱦ���ڿ�����
		virtual void setRenderWindowController( IRWInputController* controller);

		virtual IRWInputController* getRenderWindowController();

	public:
		// ��Դ�Ƿ�ɱ�ɾ��
		virtual bool isResCanbeDeleted( const char* res);

		// ɾ����Դ
		virtual bool deleteResource(const char* res);

	public:
		// ��ȡ������
		QWidget* getMainWindow();
		ProjectWnd* getProjectWindow();
		QDockWidget* getPropertyPanel();

		// ����Ŀ�ļ�
		void OpenProject( const char* fileName);

	public:
		// show bottom panel
		virtual void showBottomPanel(Echo::BottomPanelTab* bottomPanel) override;

	public:
		// select a node object
		virtual const Echo::String selectANodeObject()override;

		// select a setting object
		virtual const Echo::String selectASettingObject() override;

		// select a resource object
		virtual const Echo::String selectAResObject() override;

	private:
		AStudio();

		// ��ʼ����־ϵͳ
		bool initLogSystem();

		// ����ͼ�Ƿ����
		bool isThumbnailExists(const Echo::String& name);

		bool replaceTraverseAllWidget(QWidget* parent, QWidget* from, QWidget* to);

		void loadAllRecentProjects();

	private:
		Echo::String					m_rootPath;			// root direcory
		Echo::String					m_appPath;			// ������ӪĿ¼
		MainWindow*						m_mainWindow;		// ������
		ProjectWnd*						m_projectWindow;
		RenderWindow*					m_renderWindow;		// ��Ⱦ����
		Echo::LogDefault*				m_log;				// ��־ʵ��
		LogPanel*						m_logPanel;			// ��־���
		ConfigMgr*						m_projectCfg;		// �����ļ�
	};
}

#define Studio_ProjectMgr Studio::AStudio::Instance()->getProjectMgr()
