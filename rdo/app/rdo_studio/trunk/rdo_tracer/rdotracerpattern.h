#ifndef RDOTRACERPATTERN_H
#define RDOTRACERPATTERN_H

#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreeitem.h"

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

#endif // RDOTRACERPATTERN_H