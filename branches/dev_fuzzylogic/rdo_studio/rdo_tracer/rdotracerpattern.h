#ifndef RDOTRACERPATTERN_H
#define RDOTRACERPATTERN_H
#pragma once

#include "tracer_ctrls/rdotracertreeitem.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerPattern
// ----------------------------------------------------------------------------
enum RDOTracerPatternKind {
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

	std::string Name;
	RDOTracerPatternKind getPatternKind() const { return patKind; };
};

#endif // RDOTRACERPATTERN_H