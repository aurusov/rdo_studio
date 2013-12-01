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
	: m_pPluginsParent       (NULL)
	, m_pMergedPluginInfoList(rdo::Factory<PluginInfoList>::create(getMergedPluginInfoList()))
{
}

Loader::~Loader()
{
	BOOST_FOREACH(const LPPluginInfo& plgnInfo, *m_pMergedPluginInfoList)
	{
		if (plgnInfo->isActive())
		{
			stopPlugin(plgnInfo);
		}
	}
}

PluginInfoList Loader::getMergedPluginInfoList() const
{
	PluginInfoList plgnsHistory   = getPluginsHistory();
	PluginInfoList curLoadedPlgns = getCurrentPlugins();
	PluginInfoList mergedPlgns;
	for (PluginInfoList::const_iterator curItrt = curLoadedPlgns.begin(); curItrt != curLoadedPlgns.end(); ++curItrt)
	{
		LPPluginInfo plgnInfo = *curItrt;
		matchPluginInfo(plgnsHistory, plgnInfo);
		mergedPlgns.push_back(plgnInfo);
	}
	for (PluginInfoList::const_iterator histrItrt = plgnsHistory.begin(); histrItrt != plgnsHistory.end(); ++histrItrt)
	{
		LPPluginInfo plgnInfo = *histrItrt;
		if (matchPluginInfo(mergedPlgns, plgnInfo) != rdo::Plugin::ExactMatched)
		{
			plgnInfo->setState(rdo::Plugin::Deleted);
			mergedPlgns.push_back(plgnInfo);
		}
	}
	return mergedPlgns;
}

PluginInfoList Loader::getPluginsHistory() const
{
	QSettings settings("RAO-studio", "RAO-studio");
	PluginInfoList list;
	int size = settings.beginReadArray("plugins");
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		QString plgnName     = settings.value("plgnName"    , "").toString();
		bool    plgnAutoLoad = settings.value("plgnAutoLoad", false).toBool();
		QString plgnAuthor   = settings.value("plgnAuthor"  , "").toString();
		QString plgnVer      = settings.value("plgnVer"     , "").toString();
		QUuid   plgnGUID     = settings.value("plgnGUID"    , QUuid()).toUuid();
		PluginInfo plgn(plgnName, NULL, plgnAutoLoad, plgnGUID, plgnAuthor, plgnVer, rdo::Plugin::Unique);
		list.push_back(rdo::Factory<PluginInfo>::create(plgn));
	}
	return list;
}

void Loader::setPluginInfoList(const PluginInfoList& value) const
{
	QSettings settings;
	settings.remove("plugins");
	settings.beginWriteArray("plugins");
	int index = 0;
	BOOST_FOREACH(const LPPluginInfo& plgnInfo, value)
	{
		if (plgnInfo->getState() != rdo::Plugin::IdOnlyMatched)
		{
			settings.setArrayIndex(index);
			settings.setValue("plgnName"    , plgnInfo->getName());
			settings.setValue("plgnAutoLoad", plgnInfo->getAutoload());
			settings.setValue("plgnGUID"    , plgnInfo->getGUID());
			settings.setValue("plgnAuthor"  , plgnInfo->getAuthor());
			settings.setValue("plgnVer"     , plgnInfo->getVersion());
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
		BOOST_FOREACH (QString filePath, fileList)
		{
			QPluginLoader* pluginLoader = new QPluginLoader(filePath);
			PluginInterface* plgn = loadPlugin(pluginLoader);
			if (plgn)
			{
				PluginInfo plgnInfo = generatePluginInfo(plgn, pluginLoader);
				LPPluginInfo pPlgnInfo = rdo::Factory<PluginInfo>::create(plgnInfo);
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

int Loader::matchPluginInfo(const PluginInfoList& list, const LPPluginInfo& plgnInfo) const
{
	bool notFoundFullMatch = true;
	int plgnState = plgnInfo->getState();
	for (PluginInfoList::const_iterator listItr = list.begin(); listItr != list.end() && notFoundFullMatch; ++listItr)
	{
		if (plgnInfo->getGUID() == (*listItr)->getGUID())
		{
			if (plgnInfo->pluginSignInfoIsEqual(*(*listItr)))
			{
				notFoundFullMatch = false;
				plgnInfo->setAutoload((*listItr)->getAutoload());
				plgnState = rdo::Plugin::ExactMatched;
			}
			else
			{
				plgnState = rdo::Plugin::IdOnlyMatched;
			}
		}
	}
	plgnInfo->setState(plgnState);
	return plgnState;
}

QStringList Loader::getFileList(const QString &startDir) const
{
	QDir dir(startDir);
	QStringList list;

	BOOST_FOREACH (QString file, dir.entryList(QDir::Files))
	{
		list += dir.absoluteFilePath(file);
	}

	BOOST_FOREACH (QString subdir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
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
		PluginInterface* plgn = qobject_cast<PluginInterface*>(plugin);
		return plgn;
	}
	return NULL;
}

PluginInfo Loader::generatePluginInfo(PluginInterface* plgn, QPluginLoader* pluginLoader) const
{
	QString plgnName     = plgn->getPluginName();
	QUuid   plgnGUID     = plgn->getGUID();
	QString plgnAuthor   = plgn->getAuthor();
	QString plgnVersion  = plgn->getVersion();
	bool    plgnAutoload = false;
	PluginInfo plgnInfo(plgnName, pluginLoader, plgnAutoload, plgnGUID, plgnAuthor, plgnVersion, rdo::Plugin::Unique);
	return plgnInfo; 
}

void Loader::stopPlugin(const LPPluginInfo& plgnInfo)
{
	PluginInterface* plgn = loadPlugin(plgnInfo->getLoader());
	if (plgn)
	{
		plgn->plgnStopAction(m_pPluginsParent);
		plgnInfo->setActive(false);
	}
}

void Loader::startPlugin(const LPPluginInfo& plgnInfo)
{
	PluginInterface* plgn = loadPlugin(plgnInfo->getLoader());
	if (plgn)
	{
		plgn->plgnStartAction(m_pPluginsParent);
		plgnInfo->setActive(true);
	}
}

void Loader::startAutoloadedPlugins()
{
	for (PluginInfoList::const_iterator plgnInfoItrt = m_pMergedPluginInfoList->begin();
		  plgnInfoItrt != m_pMergedPluginInfoList->end();
		  ++plgnInfoItrt
	)
	{
		if ((*plgnInfoItrt)->getAutoload() && (*plgnInfoItrt)->isAvailable())
		{
			startPlugin(*plgnInfoItrt);
		}
	}
}

const LPPluginInfoList& Loader::getPluginInfoList() const
{
	return m_pMergedPluginInfoList;
}

void Loader::initPluginParent(QWidget* pParent)
{
	m_pPluginsParent = pParent;
}
