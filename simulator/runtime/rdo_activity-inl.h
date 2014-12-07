// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/runtime/rdo_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOActivityPattern
// --------------------------------------------------------------------------------
template<class T>
inline RDOActivityPattern<T>::RDOActivityPattern(const rdo::intrusive_ptr<T>& pPattern, bool trace, const std::string& name)
	: RDOActivity(trace, name)
	, m_pPattern (pPattern   )
{}

template<class T>
inline RDOActivityPattern<T>::~RDOActivityPattern()
{}

template<class T>
inline void RDOActivityPattern<T>::writeModelStructure(std::ostream& stream) const
{
	stream << m_oprName << " " << tracePatternId() << std::endl;
}

template<class T>
inline const std::string& RDOActivityPattern<T>::tracePatternId() const
{
	return m_pPattern->traceId();
}

CLOSE_RDO_RUNTIME_NAMESPACE
