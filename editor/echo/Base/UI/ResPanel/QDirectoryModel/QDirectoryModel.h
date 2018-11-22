#pragma once

#include <engine/core/Base/echo_def.h>
#include <vector>
#include <string>
#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

using namespace std;

namespace QT_UI
{
	//---------------------------------------------
	// DirectoryModel for Qt  2012-6-3 ����
	//---------------------------------------------
	class QDirectoryModel : public QStandardItemModel
	{
		Q_OBJECT

		typedef map<string, QIcon>  IconMap;

	public:
		// ���캯��
		QDirectoryModel();

		// ������Ŀ¼, �ļ����͹���
		void SetRootPath( const char* rootPath, const char* extFilter, QTreeView* treeView, QSortFilterProxyModel* proxy);

		void updateRootPath(const char* rootPath);

		// �Ƿ�֧���ļ�����
		bool IsSupportExt( const string& ext);

		// ˢ����ʾ
		void Refresh() { Refresh(NULL); }
		void Refresh(volatile bool* interrupt);

		// ����ͼ��
		void SetIcon( const char* name, const QIcon& icon){ m_iconMaps[name] = icon; }

		// ���õ�ǰѡ��
		void setCurrentSelect( const char* dir);

		// �������λ�û�ȡĿ¼
		QString getFileUnderMousePos(const QPoint& pos);

		// ���
		void Clean();

	private:
		// ����Ŀ¼
		void RecursiveDir( const string& dir, QStandardItem* parentItem) { RecursiveDir(dir, parentItem, NULL); }
		void RecursiveDir( const string& dir, QStandardItem* parentItem, volatile bool* interrupt);

		// ��ȡ�ļ�ͼ��
		QIcon getFileIcon( const char* fullPath);

		void _addToRingQueue(QStandardItem* parent, QStandardItem* child);

	public: signals:
		// �༭����
		void FileSelected( const char* path);

		// �༭�ļ�
		void FileEdit( const char* path);

		void signalThreadFindDirectory();

	private slots:
		// չ��ĳFiter
		void OnExpandedFilter(const QModelIndex& pIndex);

		// ѡ��ĳ�ļ�
		void OnSelectedFile( const QModelIndex& pIndex);

		// ˫��ĳ�ļ�
		void OnEditFile( const QModelIndex& pIndex);

	private:
		QTreeView*						m_treeView;		// treeView
		QSortFilterProxyModel*			m_proxy;		// �������ģ��
		string							m_rootPath;		// ��Ŀ¼	
		vector<string>					m_exts;			// �ļ�����
		IconMap							m_iconMaps;		// ͼ��
		QStandardItem*					m_activeItem;	// active item
		std::vector< QStandardItem*>	m_dirItems;		// ����Ŀ¼item
		QModelIndex						m_currentSelect;// ��ǰѡ��
		Echo::Dword						m_selectTime;	// ��ǰѡ��ʱ��
	};
}