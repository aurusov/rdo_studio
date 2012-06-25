#ifndef _PROC2RDO_RDOPROCESS_SHAPE_DECIDE_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_DECIDE_H_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"

class RPProcessShapeDecide : public RPProcessShape  
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeDecide( RPObject* parent );
	virtual ~RPProcessShapeDecide();

	virtual rp::string getClassName() const { return "RPProcessShapeDecide"; }
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// Переопределенная виртуальная функция saveToXML и loadFromXML для RPProcessShapeDecide:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();
	double ptrue; 
	double pfalse; 
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_DECIDE_H_
