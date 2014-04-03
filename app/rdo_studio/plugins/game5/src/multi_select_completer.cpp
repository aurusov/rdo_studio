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
{
	setCaseSensitivity(Qt::CaseSensitive);
	setCompletionMode(QCompleter::InlineCompletion);
}

MultiSelectCompleter::~MultiSelectCompleter()
{
}

QString MultiSelectCompleter::pathFromIndex(const QModelIndex& index) const
{
	QString path = QCompleter::pathFromIndex(index);
	QString text = static_cast<QLineEdit*>(widget())->text();

	int pos = text.lastIndexOf(' ');
	if ( pos >= 0 )
	{
		path = text.left(pos) + " " + path;
	}

	return path;
}

QStringList MultiSelectCompleter::splitPath(const QString& path) const
{
	int pos = path.lastIndexOf(' ') + 1;

	while (pos < path.length() && path.at(pos) == ' ')
	{
		pos++;
	}

	return QStringList(path.mid(pos));
}