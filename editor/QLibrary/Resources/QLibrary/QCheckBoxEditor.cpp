#include "QCheckBoxEditor.h"
#include <QColorDialog>
#include <engine/core/util/StringUtil.h>
#include <QStyleOptionButton>
#include <QApplication>

namespace QT_UI
{
	// MVC��Ⱦ
	bool QCheckBoxEditor::ItemDelegatePaint(QPainter *painter, const QRect& rect, const string& val)
	{
		QStyleOptionButton opt;
		opt.state |= Echo::StringUtil::ParseBool(val.c_str()) ? QStyle::State_On : QStyle::State_Off;
		opt.state |= QStyle::State_Enabled;
		opt.rect = QRect(rect.left() + 3, rect.top(), rect.width() - 3, rect.height());

		QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);

		return true;
	}
}