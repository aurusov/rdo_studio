#ifndef _PROC2RDO_RDOPROCESS_SHAPE_RESOURCE_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_RESOURCE_H_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"

class RPProcessShapeResource : public RPProcessShape
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeResource( RPObject* parent );
	virtual ~RPProcessShapeResource();

	virtual rp::string getClassName() const { return "RPProcessShapeResource"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	int gamount;

	// Переопределенная виртуальная функция saveToXML и loadFromXML для RPProcessShapeResource:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_RESOURCE_H_
