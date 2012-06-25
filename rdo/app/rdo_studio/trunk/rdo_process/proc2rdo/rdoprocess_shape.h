#ifndef _PROC2RDO_RDOPROCESS_SHAPE_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_H_

#include <fstream>

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"

// --------------------------------------------------------------------------------
// -------------------- RPProcessObject
// --------------------------------------------------------------------------------
class RPProcessObject
{
private:
	RPObject* obj;

public:
	RPProcessObject( RPObject* _obj ): obj( _obj ), id_next( "" ), type( "block" ) {}

	rp::string id_next; // MJ 30.03.06 id следующего блока, заведено здесь для функции, которая пробегает по листу
	rp::string id_next2; // на случай блока десайд, там 2 выхода.
	rp::string type; // MJ 1.04.06 для определения типа блоков

	virtual void generate() {};
};

// --------------------------------------------------------------------------------
// -------------------- RPProcessShape
// --------------------------------------------------------------------------------
class RPProcessShape: public RPShape, public RPProcessObject
{
public:
	RPProcessShape( RPObject* parent, const rp::string& name = "ShapeProcess" ): RPShape( parent, name ), RPProcessObject( get_this() ) {}
	virtual rp::string getClassName() const { return "RPProcessShape"; }
};

// --------------------------------------------------------------------------------
// -------------------- RPProcessFlowChart
// --------------------------------------------------------------------------------
class RPProcessFlowChart: public RPObjectFlowChart, public RPProcessObject
{
friend class RPMethodProc2RDO;
private:
	static RPObject* newObject( RPObject* _parent );
public:
	RPProcessFlowChart( RPObject* parent );
	virtual rp::string getClassName() const { return "RPProcessFlowChart"; }
	virtual rpMethod::RPMethod* getMethod();
	// Переопределенная для FlowChart виртуальная функция saveToXML и loadFromXML:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_H_
