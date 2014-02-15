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
#include "utils/src/common/warning_disable.h"
#include <QCompleter>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------
class MultiSelectCompleter : public QCompleter
{
public:
	MultiSelectCompleter(const QStringList& items, QObject* parent);
	~MultiSelectCompleter();

	virtual QString pathFromIndex(const QModelIndex& index) const;
	virtual QStringList splitPath(const QString& path) const;
};


#endif // _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_