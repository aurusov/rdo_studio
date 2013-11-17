/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/src/plugins/plugininterface.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_PLUGIN_INTERFACE_H_
#define _RDO_STUDIO_PLUGIN_INTERFACE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QString>
#include <QUuid>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class PluginInterface
{
public:
	virtual ~PluginInterface() {}
	virtual QUuid   getGUID      () = 0;
	virtual QString getPluginName() = 0;
	virtual QString getAuthor    () = 0;
	virtual QString getVersion   () = 0;

	virtual void plgnStartAction(QWidget* parent) = 0;
	virtual void plgnStopAction (QWidget* parent) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface,"RDO_PLUGIN_INTERFACE");

#endif // _RDO_STUDIO_PLUGIN_INTERFACE_H_