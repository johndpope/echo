#include <QApplication>
#include <QDesktopWidget>
#include "GameMainWindow.h"
#include "engine/core/util/PathUtil.h"

namespace Game
{
	static GameMainWindow* g_inst = nullptr;

	GameMainWindow::GameMainWindow(QWidget* parent/*=0*/)
		: QMainWindow( parent)
		, m_renderWindow(nullptr)
	{
		setupUi( this);

		m_renderWindow = new Window(this);
		setCentralWidget(m_renderWindow);;

		// 隐藏标题
		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

		// 设置菜单左上控件
		menubar->setTopLeftCornerIcon(":/icon/Icon/icon.png");

		//resize(800, 490);
		assert(!g_inst);
		g_inst = this;

	}

	GameMainWindow::~GameMainWindow()
	{
	}

	GameMainWindow* GameMainWindow::instance()
	{
		return g_inst;
	}

	void GameMainWindow::start(const Echo::String& echoProject)
	{
		Echo::String iconPath = Echo::PathUtil::GetFileDirPath(echoProject);

		setWindowTitle(echoProject.c_str());
		menubar->setTopLeftCornerIcon((iconPath + "icon.png").c_str());

		m_renderWindow->start(echoProject);
	}

	void GameMainWindow::moveToCenter()
	{
		move((qApp->desktop()->availableGeometry().width() - width()) / 2 + qApp->desktop()->availableGeometry().x(),
			(qApp->desktop()->availableGeometry().height() - height()) / 2 + qApp->desktop()->availableGeometry().y());
	}
}