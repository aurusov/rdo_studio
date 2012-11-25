/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.h
  \authors   Áàðñ Àëåêñàíäð
  \authors   Óðóñîâ Àíäðåé (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOFRM_H_
#define _CONVERTOR_RDOFRM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDOFRMFrame);
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_frmparse(PTR(void) lexer);
int  cnv_frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_frmerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
OBJECT(RDOFRMFrame) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring)                     name () const  { return src_info().src_text(); }
	CREF(rdo::runtime::LPRDOFRMFrame) frame() const  { return m_pFrame;              }

	typedef  boost::optional<ruint>  Seek;

	void  setShowIfBlock      (CREF(Seek) firstSeek);
	void  addItem             (CREF(rdo::runtime::LPRDOCalc) pItem, ruint lastSeek);
	void  setFrameConditionPos(ruint firstSeek, ruint lastSeek);
	void  onAfterBackPicture  (ruint lastSeek);

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic = NULL);

	typedef  boost::optional<std::pair<ruint, ruint> > PosPair;

	rdo::runtime::LPRDOFRMFrame m_pFrame;
	Seek                        m_firstSeek;
	Seek                        m_lastSeek;
	ruint                       m_itemCount;
	PosPair                     m_frameÑonditionPos;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOFRM_H_
