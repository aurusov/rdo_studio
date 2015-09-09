// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/plugin_loader_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/plugins/plugin_info.h"
// --------------------------------------------------------------------------------

PluginInfo::PluginInfo(const QString& name, QPluginLoader* loader, bool  autoload,
                       const QUuid&   GUID, const QString& author, const QString& version, PluginInfo::State state)
    : pluginName    (name)
    , pluginLoader  (loader)
    , pluginAutoload(autoload)
    , pluginGUID    (GUID)
    , pluginAuthor  (author)
    , pluginVersion (version)
    , pluginState   (state)
    , pluginIsActive(false)
{}

PluginInfo::~PluginInfo()
{}

const QString& PluginInfo::getName() const
{
    return pluginName;
}

QPluginLoader* PluginInfo::getLoader()
{
    return pluginLoader;
}

bool PluginInfo::getAutoload() const
{
    return pluginAutoload;
}

const QUuid& PluginInfo::getGUID() const
{
    return pluginGUID;
}

PluginInfo::State PluginInfo::getState() const
{
    return pluginState;
}

const QString& PluginInfo::getVersion() const
{
    return pluginVersion;
}

const QString& PluginInfo::getAuthor() const
{
    return pluginAuthor;
}

void PluginInfo::setState(PluginInfo::State value)
{
    pluginState = value;
}

void PluginInfo::setAutoload(bool value)
{
    pluginAutoload = value;
}

bool PluginInfo::isActive() const
{
    return pluginIsActive;
}

void PluginInfo::setActive(bool value)
{
    pluginIsActive = value;
}

bool PluginInfo::pluginSignInfoIsEqual(const PluginInfo& pluginInfo)
{
    return pluginName    == pluginInfo.getName() &&
           pluginAuthor  == pluginInfo.getAuthor() &&
           pluginVersion == pluginInfo.getVersion();
}

bool PluginInfo::isAvailable() const
{
    return !(pluginState == State::DELETED ||
             pluginState == State::ID_ONLY_MATCHED);
}

bool PluginInfo::operator==(const PluginInfo& other)
{
    return pluginName     == other.pluginName     &&
           pluginVersion  == other.pluginVersion  &&
           pluginAuthor   == other.pluginAuthor   &&
           pluginAutoload == other.pluginAutoload &&
           pluginIsActive == other.pluginIsActive &&
           pluginGUID     == other.pluginGUID     &&
           pluginState    == other.pluginState    &&
           pluginLoader   == other.pluginLoader;
}
