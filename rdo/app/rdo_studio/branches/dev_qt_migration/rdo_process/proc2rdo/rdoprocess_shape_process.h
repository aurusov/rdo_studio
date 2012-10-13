#ifndef _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_H_

#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPProcessShapeProcess)
	IS  INSTANCE_OF      (RPProcessShape                   )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeProcess( RPObject* parent );
	virtual ~RPProcessShapeProcess();
	virtual rp::string getClassName() const { return "RPProcessShapeProcess"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	virtual void setTransCount(ruint count);

	virtual void drawCustom(REF(CDC) dc);

	//параметры для симулятора
	rdo::compiler::gui::LPRPShapeDataBlockProcess m_pParams;
	rdo::runtime::      LPIInternalStatistics     pInternalStatistics;
	//переменные для генерации
	tstring gname; // имя
	int gtype; // закон прибытия
	int base_gen;
	ruint m_inProcessTransactCount;
	
	//атрибуты законов
	double gexp;
	double gdisp;
	double gmax;
	double gmin;

	int action; // тип действия по отношению к ресурсу
	int prior;
	int queue;
	int parameter;

	ruint indent;

	std::list< CString> m_resourceList;

	// Перопределенная виртуальная функция saveToXML и loadFromXML для RPProcessShapeProcess:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_H_
