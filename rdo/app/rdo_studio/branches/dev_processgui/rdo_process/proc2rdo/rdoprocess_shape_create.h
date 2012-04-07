#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPShapeCreateMJ)
	IS  INSTANCE_OF      (RPShape_MJ                )
	AND IMPLEMENTATION_OF(rdoRuntime::IProcAnimation)
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPShapeCreateMJ( RPObject* parent );
	virtual ~RPShapeCreateMJ();

	virtual rp::string getClassName() const { return "RPShapeCreateMJ"; }
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// Переопределенная виртуальная функция saveToXML для RPShapeCreateMJ:
	void saveToXML(REF(pugi::xml_node) parentNode) const;
	// Переопределенная виртуальная функция loadFromXML для RPShapeCreateMJ:
	void loadFromXML(CREF(pugi::xml_node) node);

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	//virtual void list_name();
	virtual void generate();

	virtual void setTransCount(int count);

	//параметры для симулятора
	LPRPShapeDataBlockCreate m_pParams;
	//переменные для генерации
	tstring gname; // имя
	double gfirst; // время первого
	int gamount; // кол-во создаваемых
	int gtype; // закон прибытия
	int base_gen;
	ruint m_currentTransactCount;
	
	//атрибуты законов
	double gexp;
	double gdisp;
	double gmax;

	//второе окно
	int inf; // бесконечноть
	int gID; // ID группы
	double gpar1, gpar2, gpar3;
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
