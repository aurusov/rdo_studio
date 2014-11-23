/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/multi_select_completer.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QLineEdit>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/multi_select_completer.h"
// --------------------------------------------------------------------------------

MultiSelectCompleter::MultiSelectCompleter(const QStringList& items, QObject* parent)
	: QCompleter(items, parent)
	, m_cursorPos(-1)
	, m_selectionLen(-1)
{
	setCaseSensitivity(Qt::CaseSensitive);
	setCompletionMode(QCompleter::InlineCompletion);
	connect(this, &MultiSelectCompleter::changeSelection, this, &MultiSelectCompleter::setSelection);
}

MultiSelectCompleter::~MultiSelectCompleter()
{}

QString MultiSelectCompleter::pathFromIndex(const QModelIndex& index) const
{
	QLineEdit* lineEdit = static_cast<QLineEdit*>(widget());
	QString path = QCompleter::pathFromIndex(index);
	QString text = lineEdit->text();

	const int cursorPos = lineEdit->cursorPosition();
	const int leftPos   = text.lastIndexOf(" ", cursorPos - 1);
	const int rightPos  = text.indexOf(" ", cursorPos);
	const int prefixLen = cursorPos - leftPos - 1;
	const int matchedLen = path.length();
	const int selectionLen = matchedLen - prefixLen;

	if (leftPos >= 0)
	{
		path = text.mid(0, leftPos) + " " + path;
	}

	if (rightPos >= 0)
	{
		path = path + text.mid(rightPos);
	}

	emit changeSelection(cursorPos, selectionLen);
	return path;
}

QStringList MultiSelectCompleter::splitPath(const QString& path) const
{
	QLineEdit* lineEdit = static_cast<QLineEdit*>(widget());

	int cursorPos = lineEdit->cursorPosition();
	QString text = lineEdit->text().left(cursorPos);
	int pos = text.lastIndexOf(' ');

	if (pos >= 0)
	{
		text = text.mid(pos + 1);
	}

	return QStringList(text);
}

void MultiSelectCompleter::onLineEditTextChanged()
{
	if (m_cursorPos != -1)
	{
		QLineEdit* lineEdit = static_cast<QLineEdit*>(widget());
		lineEdit->setSelection(m_cursorPos, m_selectionLen);
		m_cursorPos = -1;
		m_selectionLen = -1;
	}
}

void MultiSelectCompleter::setSelection(int cursorPos, int selectionLen)
{
	m_cursorPos = cursorPos;
	m_selectionLen = selectionLen;
}
