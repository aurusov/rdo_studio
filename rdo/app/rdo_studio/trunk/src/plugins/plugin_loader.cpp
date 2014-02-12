/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/src/plugins/plugin_loader.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDir>
#include <QApplication>
#include <QSettings>
#include <boost/foreach.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/plugins/plugin_loader.h"
// --------------------------------------------------------------------------------

using namespace rdo::Plugin;

Loader::Loader()
	: m_pPluginsParent(NULL)
	, m_pMergedPluginInfoList(rdo::Factory<PluginInfoList>::create(getMergedPluginInfoList()))
{}

Loader::~Loader()
{
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, *m_pMergedPluginInfoList)
	{
		if (pluginInfo->isActive())
			stopPlugin(pluginInfo);
	}
}

PluginInfoList Loader::getMergedPluginInfoList() const
{
	PluginInfoList pluginHistory = getPluginsHistory();
	PluginInfoList currentLoadedPlugin = getCurrentPlugins();
	PluginInfoList mergedPlugin;
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, currentLoadedPlugin)
	{
		matchPluginInfo(pluginHistory, pluginInfo);
		mergedPlugin.push_back(pluginInfo);
	}
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, pluginHistory)
	{
		if (matchPluginInfo(mergedPlugin, pluginInfo) != rdo::Plugin::ExactMatched)
		{
			pluginInfo->setState(rdo::Plugin::Deleted);
			mergedPlugin.push_back(pluginInfo);
		}
	}
	return mergedPlugin;
}

PluginInfoList Loader::getPluginsHistory() const
{
	QSettings settings("RAO-studio", "RAO-studio");
	PluginInfoList list;
	int size = settings.beginReadArray("plugins");
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		QString pluginName     = settings.value("plgnName"    , "").toString();
		bool    pluginAutoLoad = settings.value("plgnAutoLoad", false).toBool();
		QString pluginAuthor   = settings.value("plgnAuthor"  , "").toString();
		QString pluginVer      = settings.value("plgnVer"     , "").toString();
		QUuid   pluginGUID     = settings.value("plgnGUID"    , QUuid()).toUuid();
		PluginInfo pluginInfo(pluginName, NULL, pluginAutoLoad, pluginGUID, pluginAuthor, pluginVer, rdo::Plugin::Unique);
		list.push_back(rdo::Factory<PluginInfo>::create(pluginInfo));
	}
	return list;
}

void Loader::setPluginInfoList(const PluginInfoList& value) const
{
	QSettings settings;
	settings.remove("plugins");
	settings.beginWriteArray("plugins");
	int index = 0;
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, value)
	{
		if (pluginInfo->getState() != rdo::Plugin::IdOnlyMatched)
		{
			settings.setArrayIndex(index);
			settings.setValue("plgnName"    , pluginInfo->getName());
			settings.setValue("plgnAutoLoad", pluginInfo->getAutoload());
			settings.setValue("plgnGUID"    , pluginInfo->getGUID());
			settings.setValue("plgnAuthor"  , pluginInfo->getAuthor());
			settings.setValue("plgnVer"     , pluginInfo->getVersion());
			index++;
		}
	}
	settings.endArray();
}

PluginInfoList Loader::getCurrentPlugins() const
{
	PluginInfoList list;
	QDir dir(qApp->applicationDirPath());
	if (dir.dirName().toLower() == "debug" || dir.dirName().toLower() == "release")
	{
		dir.cdUp();
	}
	if (dir.cd("plugins"))
	{
		QStringList fileList = getFileList(dir.path());
		BOOST_FOREACH(const QString& filePath, fileList)
		{
			QPluginLoader* pluginLoader = new QPluginLoader(filePath);
			PluginInterface* pluginInterface = loadPlugin(pluginLoader);
			if (pluginInterface)
			{
				PluginInfo pluginInfo = generatePluginInfo(pluginInterface, pluginLoader);
				LPPluginInfo pPlgnInfo = rdo::Factory<PluginInfo>::create(pluginInfo);
				pluginLoader->unload();
				if (matchPluginInfo(list, pPlgnInfo) != rdo::Plugin::ExactMatched)
				{
					list.push_back(pPlgnInfo);
				}
				else
				{
					delete pluginLoader;
				}
			}
		}
	}
	return list;
}

int Loader::matchPluginInfo(const PluginInfoList& list, const LPPluginInfo& pluginInfo) const
{
	int pluginState = pluginInfo->getState();
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, list)
	{
		if (pluginInfo->getGUID() == pluginInfo->getGUID())
		{
			if (pluginInfo->pluginSignInfoIsEqual(*pluginInfo))
			{
				pluginInfo->setAutoload(pluginInfo->getAutoload());
				pluginState = rdo::Plugin::ExactMatched;
				break;
			}
			else
			{
				pluginState = rdo::Plugin::IdOnlyMatched;
			}
		}
	}
	pluginInfo->setState(pluginState);
	return pluginState;
}

QStringList Loader::getFileList(const QString& startDir) const
{
	QDir dir(startDir);
	QStringList list;

	BOOST_FOREACH(const QString& file, dir.entryList(QDir::Files))
	{
		list += dir.absoluteFilePath(file);
	}

	BOOST_FOREACH(const QString& subdir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		list += getFileList(startDir + "/" + subdir);
	}
	return list;
}

PluginInterface* Loader::loadPlugin(QPluginLoader* pluginLoader) const
{
	QObject* plugin = pluginLoader->instance();
	if (plugin)
	{
		PluginInterface* pluginInterface = qobject_cast<PluginInterface*>(plugin);
		return pluginInterface;
	}
	return NULL;
}

PluginInfo Loader::generatePluginInfo(PluginInterface* pluginInterface, QPluginLoader* pluginLoader) const
{
	QString pluginName     = pluginInterface->getPluginName();
	QUuid   pluginGUID     = pluginInterface->getGUID();
	QString pluginAuthor   = pluginInterface->getAuthor();
	QString pluginVersion  = pluginInterface->getVersion();
	bool    pluginAutoload = false;
	PluginInfo pluginInfo(pluginName, pluginLoader, pluginAutoload, pluginGUID, pluginAuthor, pluginVersion, rdo::Plugin::Unique);
	return pluginInfo; 
}

void Loader::stopPlugin(const LPPluginInfo& pluginInfo)
{
	PluginInterface* pluginInterface = loadPlugin(pluginInfo->getLoader());
	if (pluginInterface)
	{
		pluginInterface->plgnStopAction(m_pPluginsParent);
		pluginInfo->setActive(false);
	}
}

void Loader::startPlugin(const LPPluginInfo& pluginInfo)
{
	PluginInterface* pluginInterface = loadPlugin(pluginInfo->getLoader());
	if (pluginInterface)
	{
		pluginInterface->plgnStartAction(m_pPluginsParent);
		pluginInfo->setActive(true);
	}
}

void Loader::startAutoloadedPlugins()
{
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, *m_pMergedPluginInfoList)
	{
		if (pluginInfo->getAutoload() && pluginInfo->isAvailable())
		{
			startPlugin(pluginInfo);
		}
	}
}

const LPPluginInfoList& Loader::getPluginInfoList() const
{
	return m_pMergedPluginInfoList;
}

void Loader::initPluginParent(QWidget* pParent)
{
	ASSERT(!m_pPluginsParent);
	ASSERT(pParent);
	m_pPluginsParent = pParent;
}
