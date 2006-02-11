#ifndef RDO_PROCESS_SHAPE_H
#define RDO_PROCESS_SHAPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_chart_object.h"

#include <vector>

// ----------------------------------------------------------------------------
// ---------- RDOPROCShape
// ----------------------------------------------------------------------------
class RDOPROCShape: public RDOPROCChartObject
{
friend class RDOPROCFlowChart;

protected:
//	CPoint snap_to_point;

	virtual void drawPolyline( CDC& dc );
//	virtual void drawConnectorsInput( CDC& dc );
//	virtual void drawConnectorsOutput( CDC& dc );

public:
	RDOPROCShape( RDOPROCObject* parent, RDOPROCFlowChart* flowchart );
	virtual ~RDOPROCShape();

	virtual void setPosition( int x, int y );

//	const CPoint& getSnapToPoint() const { return snap_to_point; }

	virtual void draw( CDC& dc );
};

#endif // RDO_PROCESS_SHAPE_H
