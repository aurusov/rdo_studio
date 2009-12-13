namespace rdoRuntime 
{

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
{}
	inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent,CREF(ArrayType) arraytype)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_arrayType     (arraytype       )
{}
inline RDOValue RDOArrayType::cast(CREF(RDOValue) from) const
{
	switch (from.typeID())
	{
		case RDOType::t_array: {
			if (this == &from.type())
				return from;
			break;
		}
	}
	throw RDOTypeException();
}
inline tstring RDOArrayType::asString() const
{
	return " ";
}
}