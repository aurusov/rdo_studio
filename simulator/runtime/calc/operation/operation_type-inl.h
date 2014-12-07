// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- OperatorType
// --------------------------------------------------------------------------------
template <OperatorType::Type>
inline std::size_t& OperatorType::getCalcCounter()
{
    static std::size_t s_counter = 0;
    return s_counter;
}

CLOSE_RDO_RUNTIME_NAMESPACE
