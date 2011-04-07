/*
 * copyright: (c) RDO-Team, 2011
 * filename : isReturn.h
 * author   : Чирков Михаил
 * date     : 
 * bref     : 23.02.2011
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcIsReturn
// ----------------------------------------------------------------------------
class RDOCalcIsReturn
{
public:
	void  resetCnt()      ;
	void  upCnt(   )      ;
	rsint isReturn() const;
private:
	rsint m_returnCnt;
};

inline void RDOCalcIsReturn::resetCnt()
{
	m_returnCnt = 0;
}

inline void RDOCalcIsReturn::upCnt()
{
	++m_returnCnt;
}

inline rsint RDOCalcIsReturn::isReturn() const
{
	return m_returnCnt;
}
CLOSE_RDO_RUNTIME_NAMESPACE