/*!
  \copyright (c) RDO-Team, 2013
  \file      rdo_plugin_test/src/plugininfo.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_PLUGIN_INFO_H_
#define _RDO_STUDIO_PLUGIN_INFO_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QString>
#include <QUuid>
#include <QPluginLoader>
#include <QMetaType>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class PluginInfo
{
public:
	PluginInfo(const QString& name, QPluginLoader* loader, bool autoload, const QUuid& GUID, const QString& author, const QString& version , int state);
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
	
	void setAutoload (bool value);
	void setState    (int  value);
	void setActive   (bool value);

	bool pluginSignInfoIsEqual(const PluginInfo& scndPlgn);
	bool isAvailable();

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

typedef std::list<PluginInfo> PluginInfoList;

namespace rdo {
	namespace Plugin {
		enum LoadState{Unique = -52,IdOnlyMatched,ExactMatched,Deleted};
	}
};

Q_DECLARE_METATYPE(PluginInfoList::iterator)

#endif // _RDO_STUDIO_PLUGIN_INFO_H_