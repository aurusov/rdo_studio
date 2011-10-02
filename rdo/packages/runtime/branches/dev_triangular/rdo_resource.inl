/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.inl
  \author    Ћущан ƒмитрий (dluschan@rk9.bmstu.ru)
  \date      15.06.2011
  \brief     RDOResource inline implementation
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
inline void RDOResource::setRuntime(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	/// @todo походу надо удалить метод
	NEVER_REACH_HERE;
}

inline tstring RDOResource::whoAreYou()
{
	return "rdoRes";
}

inline void RDOResource::makeTemporary(rbool value)
{
	m_temporary = value;
}

inline RDOResource::ConvertStatus RDOResource::getState() const
{
	return m_state;
}

inline void RDOResource::setState(RDOResource::ConvertStatus value)
{
	m_state = value;
}

inline rbool RDOResource::checkType(ruint type) const
{
	return m_type == type;
}

inline CREF(LPIResourceType) RDOResource::getResType() const
{
	return m_resType;
}

inline ruint RDOResource::getType() const
{
	return m_type;
}

inline CREF(std::vector<RDOValue>) RDOResource::getParams() const
{
	return m_params;
}

inline CREF(RDOValue) RDOResource::getParam(ruint index) const
{
	ASSERT(index < m_params.size());
	return m_params[index];
}

inline REF(RDOValue) RDOResource::getParamRaw(ruint index)
{
	ASSERT(index < m_params.size());
	setState(CS_Keep);
	return m_params[index];
}

inline void RDOResource::setParam(ruint index, CREF(RDOValue) value)
{
	if (m_params.size() <= index)
	{
		m_params.resize(index + 1);
	}
	setState(CS_Keep);
	m_params.at(index) = value;
}

inline ruint RDOResource::paramsCount() const
{
	return m_params.size();
}

inline void RDOResource::appendParams(const std::vector<RDOValue>::const_iterator& from_begin, const std::vector<RDOValue>::const_iterator& from_end)
{
	m_params.insert(m_params.end(), from_begin, from_end);
}

inline rbool RDOResource::canFree() const
{
	return m_referenceCount == 0;
}

inline void RDOResource::incRef()
{
	++m_referenceCount;
}

inline void RDOResource::decRef()
{
	--m_referenceCount;
}

inline tstring RDOResource::traceTypeId()
{
	return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
}

CLOSE_RDO_RUNTIME_NAMESPACE
