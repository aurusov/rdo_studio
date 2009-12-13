namespace rdoRuntime 
{

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
	inline RDOArrayType(ArrayType arrayID)
		:RDOType         (RDOType::t_array)
	{}
	
	inline RDOArrayType::ArrayType RDOArrayType::ArrayID() const
	{
		return m_arrayID;
	}
}