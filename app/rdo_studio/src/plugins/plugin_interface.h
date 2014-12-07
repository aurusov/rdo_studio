#ifndef _RDO_STUDIO_PLUGIN_INTERFACE_H_
#define _RDO_STUDIO_PLUGIN_INTERFACE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QUuid>
#include <QString>
#include <QWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class PluginInterface
{
public:
	virtual ~PluginInterface() {}
	virtual QUuid   getGUID      () const = 0;
	virtual QString getPluginName() const = 0;
	virtual QString getAuthor    () const = 0;
	virtual QString getVersion   () const = 0;

	virtual void pluginStartAction(QWidget* parent,  const std::string& commandLine) = 0;
	virtual void pluginStopAction (QWidget* parent) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "RDO_PLUGIN_INTERFACE");

#endif // _RDO_STUDIO_PLUGIN_INTERFACE_H_
