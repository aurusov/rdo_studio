/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/multi_select_completer.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_
#define _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QCompleter>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------
class MultiSelectCompleter : public QCompleter
{
Q_OBJECT
public:
	MultiSelectCompleter(const QStringList& items, QObject* parent);
	~MultiSelectCompleter();

public:
	QString pathFromIndex(const QModelIndex& index) const;
	QStringList splitPath(const QString& path) const;
};


#endif // _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_