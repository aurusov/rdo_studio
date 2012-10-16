/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerpattern.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACERPATTERN_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACERPATTERN_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerPattern
// --------------------------------------------------------------------------------
enum RDOTracerPatternKind {
	RDOPK_UNDEFINED = ~0,
	RDOPK_OPERATION = 0,
	RDOPK_IRREGULAREVENT,
	RDOPK_RULE,
	RDOPK_KEYBOARD
};

class RDOTracerPattern : public RDOTracerTreeItem
{
protected:
	RDOTracerPatternKind patKind;
public:
	RDOTracerPattern( const RDOTracerPatternKind kind );
	virtual ~RDOTracerPattern();

	tstring Name;
	RDOTracerPatternKind getPatternKind() const { return patKind; };
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERPATTERN_H_
