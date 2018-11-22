#pragma once

#include <QWidget>
#include <QListView>

#include <sys/stat.h>
#include <vector>
#include <map>
#include <string>

QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QStandardItem;
class QSortFilterProxyModel;
QT_END_NAMESPACE

namespace QT_UI
{
	/**
	 * QPreviewWidget Ԥ������
	 */
	class QDragListWidget : public QListView
	{
		Q_OBJECT

	public:
		QDragListWidget(QWidget* parent = 0);
		~QDragListWidget();

		virtual void startDrag(Qt::DropActions supportedActions) override;
	};

	class QPreviewWidget : public QWidget
	{
		Q_OBJECT

		typedef std::map<std::string, std::string>  IconMap;
	public:
		// ItemInfo
		struct ItemInfo
		{
			ItemInfo()
			:m_time(0)
			{

			}
			QString		m_text;		// �ļ���
			QString		m_icon;		// ͼ��
			__time64_t m_time;
		};

	public:
		QPreviewWidget( QWidget* parent=0);
		virtual ~QPreviewWidget();

		// �����
		void addItem(const char* filePath);

		// �����
		void addItem( const QString& text, const QString& icon);

		// 
		void createItem(const char* filePath, std::vector<QStandardItem*>& results);
		QStandardItem* createItem( const QString& text, const QString& icon);

		// ���������
		void clearAllItems();

		// ��������ͼ
		void setThumbnail( const char* text, const char* texture);

		// ��ȡ��ǰѡ����
		QString getCurrentSelect();

		// ��ȡ����װ��widget
		QListView* getListWidget() { return m_listView; }
		QStandardItemModel* getListModel() { return m_listModel; }

		// ������Ѱ����
		//void setSearchName(const char* searchName) { m_searchName = searchName; }

		// �������͹���
		void setTypesFilter(const std::vector<QString>& typesFilter) { m_typesFilter = typesFilter;  }

		// ����ָ����׺����Ӧͼ��
		void SetIcon(const char* ext, const char* icon){ m_iconMaps[ext] = icon; }

		// ����ͼ����
		void setIconWH(int w, int h) { m_iconWidth = w; m_iconHeight = h; }

		// �Ƿ���Ҫȫ·��
		void isNeedFullPath(bool state) { m_isNeedFullPath = state; }

		// �����ʾ
		void addToolTips(QStandardItem* item, const QString& text);

		// ��Ӷ��߳���������ͼ����
		void addThumbnailThreadTask(const char* name, const char* thumbNailFile);

		void editItem(QStandardItem* item);

		QList<QStandardItem*> selectedItems();

		QStandardItem* itemAt(const QPoint& p);

		QStandardItem* currentItem();

		void selectItemByName(const char* const name);

		void setCurrentItem(QModelIndex index);

		void filterName(const char* const);

	public slots:
		// ����ʱ����
		void onClicked(const QModelIndex& index);

		// ˫���Ǵ���
		void onDoubleClicked(const QModelIndex& index);

		// ����ͼ�޸�
		void onThumbnailChanged(const QString& text, const QString& texture);

	public: signals:
		// �����ź�
		void Signal_onClickedItem( const QString&);

		// ˫���ź�
		void Signal_onDoubleClickedItem( const QString&);

		// ��Ϣ�ź�
		void Signal_ThumbChange( const QString&, const QString&);

		// �ı��ı�
		void Signal_onItemTextChanged();

	private:
		// ���ݺ�׺���ж���Դ�Ƿ���ҪԤ��
		bool IsNeedPreview(const char* ext);

	protected:
		int							m_iconWidth;		// ͼ���
		int							m_iconHeight;		// ͼ���
		bool						m_isNeedFullPath;	// �Ƿ��ȫ·��
		std::vector<ItemInfo>		m_itemInfos;		// Ԥ����Ϣ
		std::map<QString, QIcon*>	m_iconCaches;		// ͼ�껺��

		QDragListWidget*			m_listView;			// QListWidgetʵ�ַ�ʽ
		QStandardItemModel*			m_listModel;		// list model
		QSortFilterProxyModel*		m_listProxyModel;	// list sort and filter proxy model

		//WorkerThread*				m_thumbnailThread;	// ����ͼ�����߳�

		std::vector<QString>		m_typesFilter;		// 
		IconMap						m_iconMaps;			// ͼ��
	};
}