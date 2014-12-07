// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/plugin_loader_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDir>
#include <QApplication>
#include <QSettings>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/plugins/plugin_loader.h"
// --------------------------------------------------------------------------------

using namespace rdo::plugin;

Loader::Loader()
	: m_pPluginsParent(NULL)
{}

Loader::~Loader()
{
	for (const LPPluginInfo& pluginInfo: *m_pMergedPluginInfoList)
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
	for (const LPPluginInfo& pluginInfo: currentLoadedPlugin)
	{
		matchPluginInfo(pluginHistory, pluginInfo);
		mergedPlugin.push_back(pluginInfo);
	}
	for (const LPPluginInfo& pluginInfo: pluginHistory)
	{
		if (matchPluginInfo(mergedPlugin, pluginInfo) != rdo::plugin::ExactMatched)
		{
			pluginInfo->setState(rdo::plugin::Deleted);
			mergedPlugin.push_back(pluginInfo);
		}
	}
	return mergedPlugin;
}

PluginInfoList Loader::getPluginsHistory() const
{
	QSettings settings;
	PluginInfoList list;
	int size = settings.beginReadArray("plugins");
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		QString pluginName     = settings.value("pluginName"    , "").toString();
		bool    pluginAutoLoad = settings.value("pluginAutoLoad", false).toBool();
		QString pluginAuthor   = settings.value("pluginAuthor"  , "").toString();
		QString pluginVer      = settings.value("pluginVer"     , "").toString();
		QUuid   pluginGUID     = settings.value("pluginGUID"    , QUuid()).toUuid();
		PluginInfo pluginInfo(pluginName, NULL, pluginAutoLoad, pluginGUID, pluginAuthor, pluginVer, rdo::plugin::Unique);
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
	for (const LPPluginInfo& pluginInfo: value)
	{
		if (pluginInfo->getState() != rdo::plugin::IdOnlyMatched)
		{
			settings.setArrayIndex(index);
			settings.setValue("pluginName"    , pluginInfo->getName());
			settings.setValue("pluginAutoLoad", pluginInfo->getAutoload());
			settings.setValue("pluginGUID"    , pluginInfo->getGUID());
			settings.setValue("pluginAuthor"  , pluginInfo->getAuthor());
			settings.setValue("pluginVer"     , pluginInfo->getVersion());
			index++;
		}
	}
	settings.endArray();
}

PluginInfoList Loader::getCurrentPlugins() const
{
	PluginInfoList list;
	QDir dir(qApp->applicationDirPath());
	if (dir.cd("plugins"))
	{
		QStringList fileList = getFileList(dir.path());
		for (const QString& filePath: fileList)
		{
			QPluginLoader* pluginLoader = new QPluginLoader(filePath);
			PluginInterface* pluginInterface = loadPlugin(pluginLoader);
			if (pluginInterface)
			{
				PluginInfo pluginInfo = generatePluginInfo(pluginInterface, pluginLoader);
				LPPluginInfo pPlgnInfo = rdo::Factory<PluginInfo>::create(pluginInfo);
				pluginLoader->unload();
				if (matchPluginInfo(list, pPlgnInfo) != rdo::plugin::ExactMatched)
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

int Loader::matchPluginInfo(const PluginInfoList& list, const LPPluginInfo& matchingPluginInfo) const
{
	int pluginState = matchingPluginInfo->getState();
	for (const LPPluginInfo& pluginInfo: list)
	{
		if (matchingPluginInfo->getGUID() == pluginInfo->getGUID())
		{
			if (matchingPluginInfo->pluginSignInfoIsEqual(*pluginInfo))
			{
				matchingPluginInfo->setAutoload(pluginInfo->getAutoload());
				pluginState = rdo::plugin::ExactMatched;
				break;
			}
			else
			{
				pluginState = rdo::plugin::IdOnlyMatched;
			}
		}
	}
	matchingPluginInfo->setState(pluginState);
	return pluginState;
}

QStringList Loader::getFileList(const QString& startDir) const
{
	QDir dir(startDir);
	QStringList list;

	for (const QString& file: dir.entryList(QDir::Files))
	{
		list += dir.absoluteFilePath(file);
	}

	for (const QString& subdir: dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
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
	PluginInfo pluginInfo(pluginName, pluginLoader, pluginAutoload, pluginGUID, pluginAuthor, pluginVersion, rdo::plugin::Unique);
	return pluginInfo; 
}

void Loader::stopPlugin(const LPPluginInfo& pluginInfo)
{
	PluginInterface* pluginInterface = loadPlugin(pluginInfo->getLoader());
	if (pluginInterface && pluginInfo->isActive())
	{
		pluginInterface->pluginStopAction(m_pPluginsParent);
		pluginInfo->setActive(false);
	}
}

void Loader::startPlugin(const LPPluginInfo& pluginInfo, const std::string& commandLine)
{
	PluginInterface* pluginInterface = loadPlugin(pluginInfo->getLoader());
	if (pluginInterface && !pluginInfo->isActive())
	{
		pluginInterface->pluginStartAction(m_pPluginsParent, commandLine);
		pluginInfo->setActive(true);
	}
}

void Loader::autoStartPlugins(const std::map<int, std::string>& options)
{
	int index = 0;
	for (const LPPluginInfo& pluginInfo: *m_pMergedPluginInfoList)
	{
		if (options.count(index))
		{
			startPlugin(pluginInfo, options.at(index));
		}
		else
		{
			if (pluginInfo->getAutoload() && pluginInfo->isAvailable())
			{
				startPlugin(pluginInfo);
			}
		}

		index++;
	}
}

const LPPluginInfoList& Loader::getPluginInfoList() const
{
	return m_pMergedPluginInfoList;
}

void Loader::init(QWidget* pParent)
{
	m_pMergedPluginInfoList = rdo::Factory<PluginInfoList>::create(getMergedPluginInfoList());

	ASSERT(!m_pPluginsParent);
	ASSERT(pParent);
	m_pPluginsParent = pParent;
}
