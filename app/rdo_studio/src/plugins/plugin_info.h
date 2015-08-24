#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QUuid>
#include <QPluginLoader>
#include <QMetaType>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

PREDECLARE_POINTER(PluginInfo);
class PluginInfo: public rdo::counter_reference
{
DECLARE_FACTORY(PluginInfo);
public:
    enum class State
    {
        UNIQUE = -52,
        ID_ONLY_MATCHED,
        EXACT_MATCHED,
        DELETED
    };

    PluginInfo(const QString& name, QPluginLoader* loader, bool autoload, const QUuid& GUID,
               const QString& author, const QString& version, PluginInfo::State state);
    ~PluginInfo();
    const QString& getName    () const;
    const QString& getAuthor  () const;
    const QString& getVersion () const;
    bool           getAutoload() const;
    const QUuid&   getGUID    () const;
    State          getState   () const;
    bool           isActive   () const;
    QPluginLoader* getLoader  ();

    void setAutoload(bool value);
    void setState   (PluginInfo::State  value);
    void setActive  (bool value);

    bool pluginSignInfoIsEqual(const PluginInfo& pluginInfo);
    bool isAvailable() const;
    bool operator==(const PluginInfo& other);

private:
    QString        pluginName;
    QPluginLoader* pluginLoader;
    bool           pluginAutoload;
    QUuid          pluginGUID;
    QString        pluginAuthor;
    QString        pluginVersion;
    State          pluginState;
    bool           pluginIsActive;
};

PREDECLARE_POINTER(PluginInfoList);
class PluginInfoList
    : public rdo::counter_reference
    , public std::list<LPPluginInfo>
{
    DECLARE_FACTORY(PluginInfo);
};

Q_DECLARE_METATYPE(LPPluginInfo)
