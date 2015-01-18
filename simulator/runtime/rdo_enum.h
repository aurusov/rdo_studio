#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
    typedef std::vector<std::string> Enums;
    typedef Enums::const_iterator CIterator;
    typedef Enums::const_iterator const_iterator;

    static const std::size_t END = std::size_t(~0);

    void add(const std::string& next);
    std::size_t findEnum(const std::string& val) const;
    bool exist(const std::string& val) const;

    bool empty() const;
    const CIterator begin() const;
    const CIterator end() const;
    const Enums& getValues() const;

protected:
    RDOEnumType();
    RDOEnumType(const Enums& enums);

    Enums m_enum;
};
DECLARE_POINTER(RDOEnumType);

CLOSE_RDO_RUNTIME_NAMESPACE
