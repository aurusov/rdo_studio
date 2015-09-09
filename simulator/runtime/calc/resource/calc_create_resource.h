#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcCreateResource);
class RDOCalcCreateResource: public RDOCalc
{
DECLARE_FACTORY(RDOCalcCreateResource)
private:
    // relResID == ~0 для ресурсов, создаваемых при инициализации модели
    RDOCalcCreateResource(
        std::size_t resourceTypeID,
        const std::vector<LPRDOCalc>& rParamCalcList,
        bool traceFlag, bool permanentFlag,
        bool isNested = 0,
        std::size_t relResID = ~0,
        boost::optional<std::size_t> resourceID = boost::optional<std::size_t>()
    );

    boost::optional<std::size_t> m_resourceID;
    std::size_t m_resourceTypeID;
    std::vector<LPRDOCalc> m_paramCalcList;
    bool m_traceFlag;
    bool m_permanentFlag;
    std::size_t m_relResID;
    bool m_isNested;

    DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE
