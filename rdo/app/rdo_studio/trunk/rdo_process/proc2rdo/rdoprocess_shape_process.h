#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_

#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"

class RPShapeProcessMJ : public RPShape_MJ
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPShapeProcessMJ( RPObject* parent );
	virtual ~RPShapeProcessMJ();
	virtual rp::string getClassName() const { return "RPShapeProcessMJ"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	//параметры для симулятора
	LPRPShapeDataBlockProcess m_pParams;
	//переменные для генерации
	tstring gname; // имя
	
	int gtype; // закон прибытия
	int base_gen;
	
	//атрибуты законов
	double gexp;
	double gdisp;
	double gmax;
	double gmin;

	int action; // тип действия по отношению к ресурсу
	int prior;
	int queue;
	int parameter;

	std::list< CString> list_resource_procMJ;

	// Перопределенная виртуальная функция saveToXML и loadFromXML для RPShapeProcessMJ:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_MJ_H__A5AEB64C_2CE8_46D3_9E6B_86368CB68269__INCLUDED_)
