/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_array.inl
 * author   : Чирков Михаил
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_arrayType     (NULL            )
{}

inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent, CREF(ArrayType) arrayType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_arrayType     (arrayType       )
{}

inline RDOValue RDOArrayType::cast(CREF(RDOValue) from) const
{
	switch (from.typeID())
	{
		case RDOType::t_array: 
		{
			if (this == &from.type())
				return from;
			break;
		}
	}
	throw RDOTypeException();
}

inline tstring RDOArrayType::asString() const
{
	ASSERT(m_arrayType);
	return rdo::format(_T("array<%s>"), m_arrayType->asString().c_str());
}

CLOSE_RDO_RUNTIME_NAMESPACE
