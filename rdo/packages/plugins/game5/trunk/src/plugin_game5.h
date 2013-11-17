/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_PLUGIN_GAME5_H_
#define _RDO_PLUGIN_PLUGIN_GAME5_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QObject>
#include <QtPlugin>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/plugins/plugin_interface.h"
// --------------------------------------------------------------------------------

class plugin : public QObject,PluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "RDO_PLUGIN_INTERFACE" FILE "plugin_game5.json")
	Q_INTERFACES(PluginInterface)
public:
	QUuid   getGUID       ();
	QString getPluginName ();
	QString getAuthor     ();
	QString getVersion    ();

	void plgnStartAction  (QWidget * parent);
	void plgnStopAction   (QWidget * parent);
private slots:
	void pluginSlot();
};

#endif // _RDO_PLUGIN_PLUGIN_GAME5_H_