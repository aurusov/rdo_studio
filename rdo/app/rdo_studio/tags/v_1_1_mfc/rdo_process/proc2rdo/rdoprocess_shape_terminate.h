#ifndef _PROC2RDO_RDOPROCESS_SHAPE_TERMINATE_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_TERMINATE_H_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/runtime/process/generate.h"

OBJECT(RPProcessShapeTerminate)
	IS  INSTANCE_OF      (RPProcessShape                   )
	AND IMPLEMENTATION_OF(rdo::runtime::IInternalStatistics)
{
friend class RPMethodProc2RDO;

private:
	static RPObject* newObject( RPObject* parent );

public:
	tstring m_name;
	int     m_term_inc;
	ruint   m_terminatedTransactCount;
	ruint   indent;

	rdo::compiler::gui::LPRPShapeDataBlockTerminate m_pParams;
	rdo::runtime::      LPIInternalStatistics       pInternalStatistics;

	RPProcessShapeTerminate( RPObject* parent );
	virtual ~RPProcessShapeTerminate();

	virtual rp::string getClassName() const { return "RPProcessShapeTerminate"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	virtual void setTransCount(ruint count);

	virtual void drawCustom(REF(CDC) dc);

	// Переопределенная виртуальная функция saveToXML и loadFromXML для RPProcessShapeTerminate:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_TERMINATE_H_
