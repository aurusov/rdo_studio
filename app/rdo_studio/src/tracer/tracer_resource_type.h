#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- ParamInfo
// --------------------------------------------------------------------------------
class ParamInfo
{
public:
    enum class Type
    {
        INTEGER = 0,
        REAL,
        ENUMERATIVE,
        BOOL,
        ARRAY,
        STRING,
        RESOURCE
    };

    ParamInfo(const Type type);
    virtual ~ParamInfo();

    const QString& getName() const;
    void           setName(const QString& name);

    Type getParamType() const;

    int addEnumValue(const std::string& value);
    int addStringValue(const std::string& value);
    std::string getEnumValue (unsigned int index) const;
    int getEnumCount() const;

private:
    typedef std::vector<std::string> EnumValueList;

    QString m_name;
    EnumValueList* m_enumValueList;
    Type m_paramType;
};

// --------------------------------------------------------------------------------
// -------------------- ResourceType
// --------------------------------------------------------------------------------
class ResourceType: public ChartTreeItem
{
DECLARE_FACTORY(ResourceType)
public:
    enum class Kind
    {
        PERMANENT = 0,
        TEMPORARY
    };

    const QString& getName() const;
    void           setName(const QString& name);

    Kind getKind() const;

    int        addParamInfo(ParamInfo* const value);
    ParamInfo* getParamInfo(unsigned int index) const;
    int        getParamsCount() const;

private:
    ResourceType(Kind kind);
    virtual ~ResourceType();

    typedef  std::vector<ParamInfo*>  ParamInfoList;

    QString       m_name;
    Kind          m_kind;
    ParamInfoList m_paramInfoList;
};

typedef rdo::intrusive_ptr<ResourceType> LPResourceType;

}}} // namespace rdo::gui::tracer
