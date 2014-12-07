#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/plugins/plugin_info.h"
#include "app/rdo_studio/src/plugins/plugin_interface.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace plugin
	{
		class Loader;
	};
};

class rdo::plugin::Loader
{
public:
	Loader ();
	virtual ~Loader();

	const LPPluginInfoList& getPluginInfoList     () const;
	void                    stopPlugin            (const LPPluginInfo& pluginInfo);
	void                    startPlugin           (const LPPluginInfo& pluginInfo, const std::string& commandLine = "");
	void                    autoStartPlugins(const std::map<int, std::string>& options);
	void                    setPluginInfoList     (const PluginInfoList& value) const;
	void                    init                  (QWidget* pParent);

private:
	LPPluginInfoList m_pMergedPluginInfoList;
	QWidget*         m_pPluginsParent;

	QStringList       getFileList            (const QString& startDir) const;
	PluginInfoList    getMergedPluginInfoList() const;
	PluginInfoList    getPluginsHistory      () const;
	PluginInfoList    getCurrentPlugins      () const;

	PluginInterface*  loadPlugin             (QPluginLoader* pluginLoader) const;
	PluginInfo        generatePluginInfo     (PluginInterface* pluginInterface, QPluginLoader* pluginLoader) const;
	int               matchPluginInfo        (const PluginInfoList& list, const LPPluginInfo& pluginInfo) const;
};
