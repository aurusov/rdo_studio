/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/src/plugins/plugininfo.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_PLUGIN_INFO_H_
#define _RDO_STUDIO_PLUGIN_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QUuid>
#include <QPluginLoader>
#include <QMetaType>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OBJECT(PluginInfo)
{
DECLARE_FACTORY(PluginInfo);
public:
	PluginInfo(const QString& name, QPluginLoader* loader, bool autoload, const QUuid& GUID, 
	           const QString& author, const QString& version , int state);
	PluginInfo() {};
	~PluginInfo() {};
	const QString& getName    () const;
	const QString& getAuthor  () const;
	const QString& getVersion () const;
	bool           getAutoload() const;
	const QUuid&   getGUID    () const;
	int            getState   () const;
	bool           isActive   () const;
	QPluginLoader* getLoader  ();
	
	void setAutoload(bool value);
	void setState   (int  value);
	void setActive  (bool value);

	bool pluginSignInfoIsEqual(const PluginInfo& scndPlgn);
	bool isAvailable();
	bool operator==(const PluginInfo& other);

private:
	QString        pluginName;
	QString        pluginVersion;
	QString        pluginAuthor;
	bool           pluginAutoload;
	bool           pluginIsActive;
	QUuid          pluginGUID;
	int            pluginState;
	QPluginLoader* pluginLoader;
};

OBJECT(PluginInfoList)
AND INSTANCE_OF(std::list<LPPluginInfo>)
{
	DECLARE_FACTORY(PluginInfo);
};

namespace rdo
{
	namespace Plugin
	{
		enum LoadState{Unique = -52,IdOnlyMatched,ExactMatched,Deleted};
	}
};

Q_DECLARE_METATYPE(LPPluginInfo)

#endif // _RDO_STUDIO_PLUGIN_INFO_H_