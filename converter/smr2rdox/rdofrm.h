/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOFRM_H_
#define _CONVERTOR_RDOFRM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_frmparse(void* lexer);
int  cnv_frmlex  (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_frmerror(const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFRMFrame);
class RDOFRMFrame
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	const std::string& name() const { return src_info().src_text(); }
	const rdo::runtime::LPRDOFRMFrame& frame() const { return m_pFrame; }

	typedef boost::optional<std::size_t> Seek;

	void setShowIfBlock(const Seek& firstSeek);
	void addItem(const rdo::runtime::LPRDOCalc& pItem, std::size_t lastSeek);
	void setFrameConditionPos(std::size_t firstSeek, std::size_t lastSeek);
	void onAfterBackPicture(std::size_t lastSeek);

private:
	RDOFRMFrame(const RDOParserSrcInfo& src_info);

	typedef  boost::optional<std::pair<std::size_t, std::size_t>> PosPair;

	rdo::runtime::LPRDOFRMFrame m_pFrame;
	Seek m_firstSeek;
	Seek m_lastSeek;
	std::size_t m_itemCount;
	PosPair m_frameConditionPos;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOFRM_H_
