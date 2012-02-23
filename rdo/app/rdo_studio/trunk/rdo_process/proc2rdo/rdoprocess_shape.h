#ifndef RDO_PROCESS_SHAPE_MJ_H
#define RDO_PROCESS_SHAPE_MJ_H

#include <fstream>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"

// --------------------------------------------------------------------------------
// -------------------- RPObject_MJ
// --------------------------------------------------------------------------------
class RPObject_MJ
{
private:
	RPObject* obj;

public:
	RPObject_MJ( RPObject* _obj ): obj( _obj ), id_next( "" ), type( "block" ) {}

	rp::string id_next; // MJ 30.03.06 id следующего блока, заведено здесь для функции, которая пробегает по листу
	rp::string id_next2; // на случай блока десайд, там 2 выхода.
	rp::string type; // MJ 1.04.06 для определения типа блоков

	virtual void generate() {};
};

// --------------------------------------------------------------------------------
// -------------------- RPShape_MJ
// --------------------------------------------------------------------------------
class RPShape_MJ: public RPShape, public RPObject_MJ
{
public:
	RPShape_MJ( RPObject* parent, const rp::string& name = "shape_mj" ): RPShape( parent, name ), RPObject_MJ( get_this() ) {}
	virtual rp::string getClassName() const { return "RPShape_MJ"; }
};

// --------------------------------------------------------------------------------
// -------------------- RPObjectFlowChart_MJ
// --------------------------------------------------------------------------------
class RPObjectFlowChart_MJ: public RPObjectFlowChart, public RPObject_MJ
{
friend class RPMethodProc2RDO_MJ;
private:
	static RPObject* newObject( RPObject* _parent );
public:
	RPObjectFlowChart_MJ( RPObject* parent );
	virtual rp::string getClassName() const { return "RPObjectFlowChart_MJ"; }
	virtual rpMethod::RPMethod* getMethod();
	// Переопределенная для FlowChart виртуальная функция saveToXML и loadFromXML:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // RDO_PROCESS_SHAPE_MJ_H
