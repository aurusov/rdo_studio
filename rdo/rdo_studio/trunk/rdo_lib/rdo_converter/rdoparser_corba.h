/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_corba.h
 * author   : Раевнев Никита, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_CORBA_H_
#define _CONVERTOR_RDOCONVERTER_CORBA_H_

#ifdef CORBA_ENABLE

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdoparser_rdo.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRTP);
private:
	RDOParserCorbaRTP()
		: RDOParserRDOItem(rdoModelObjectsConvertor::RTP, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(Converter) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRSS);
private:
	RDOParserCorbaRSS()
		: RDOParserRDOItem(rdoModelObjectsConvertor::RSS, NULL, NULL, NULL)
	{}
	virtual void parse(PTR(Converter) pParser);
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! CORBA_ENABLE

#endif //! _CONVERTOR_RDOCONVERTER_CORBA_H_
