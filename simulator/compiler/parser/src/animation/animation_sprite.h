#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOFRMSprite: public RDOFRMCommandList
{
DECLARE_FACTORY(RDOFRMSprite);
private:
	RDOFRMSprite(const RDOParserSrcInfo& src_info);
	virtual ~RDOFRMSprite();

	rdo::runtime::LPRDOFRMSprite m_pSprite;

	rdo::runtime::LPRDOFRMSprite list() const;
};
DECLARE_POINTER(RDOFRMSprite);

CLOSE_RDO_PARSER_NAMESPACE
