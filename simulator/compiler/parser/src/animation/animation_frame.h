#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
class RDOFRMFrame: public RDOFRMCommandList
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	const rdo::runtime::LPRDOFRMFrame& frame() const;

private:
	RDOFRMFrame(const RDOParserSrcInfo& srcInfo);
	virtual ~RDOFRMFrame();

	rdo::runtime::LPRDOFRMFrame m_pFrame;

	rdo::runtime::LPRDOFRMSprite list() const;
};
DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_PARSER_NAMESPACE
