#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPProcessShapeCreate)
	IS  INSTANCE_OF      (RPProcessShape                   )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	RPProcessShapeCreate( RPObject* parent );
	virtual ~RPProcessShapeCreate();

	virtual rp::string getClassName() const { return "RPProcessShapeCreate"; }
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );
	// Переопределенная виртуальная функция saveToXML для RPProcessShapeCreate:
	void saveToXML(REF(pugi::xml_node) parentNode) const;
	// Переопределенная виртуальная функция loadFromXML для RPProcessShapeCreate:
	void loadFromXML(CREF(pugi::xml_node) node);

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	//virtual void list_name();
	virtual void generate();

	virtual void setTransCount(ruint count);

	virtual void drawCustom(REF(CDC) dc);

	//параметры для симулятора
	rdo::compiler::gui::LPRPShapeDataBlockCreate m_pParams;
	rdo::runtime      ::LPIInternalStatistics    pInternalStatistics;
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

	ruint indent;
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
