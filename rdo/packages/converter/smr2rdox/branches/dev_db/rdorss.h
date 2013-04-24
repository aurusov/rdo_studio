/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDORSS_RSS_H_
#define _CONVERTOR_RDORSS_RSS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdortp.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_rssparse(PTR(void)    lexer);
int  cnv_rsslex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_rsserror(PTR(char)    message);

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
OBJECT(RDORSSResource)
	IS  INSTANCE_OF(RDOParserSrcInfo  )
	AND INSTANCE_OF(boost::noncopyable)
{
DECLARE_FACTORY(RDORSSResource);
public:
	class Param
	{
	public:
		explicit Param(CREF(LPRDOValue) pValue)
			: m_pValue(pValue)
		{}

		CREF(LPRDOValue) param() const
		{
			return m_pValue;
		}

	private:
		LPRDOValue m_pValue;
	};
	typedef std::vector<Param> ParamList;
	enum { UNDEFINED_ID = ~0 };

	virtual rdo::runtime::LPRDOCalc createCalc() const;

	CREF(tstring)    name   () const { return src_info().src_text(); }
	LPRDORTPResType  getType() const { return m_pResType;            }

	int              getID  () const { return m_id;                  }

	CREF(ParamList)  params () const { return m_paramList;           }

	void  addParam(CREF(LPRDOValue) pParam);
	rbool getTrace() const      { return trace;  }
	void  setTrace(rbool value) { trace = value; }
	rbool defined () const;

	void writeModelStructure(REF(std::ostream) stream, PTR(IDB) db) const;

protected:
	RDORSSResource(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);

	LPRDORTPResType m_pResType;
	const int       m_id;        //! in system
	ParamList       m_paramList;
	rbool           trace;

private:
	RDORTPResType::ParamList::const_iterator m_currParam;
};

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
class RDOPROCResource: public RDORSSResource
{
DECLARE_FACTORY(RDOPROCResource);
private:
	RDOPROCResource(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id = UNDEFINED_ID);
	virtual rdo::runtime::LPRDOCalc createCalc() const;
};
DECLARE_POINTER(RDOPROCResource);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDORSS_RSS_H_
