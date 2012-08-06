#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/src/application.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

RPProcessShapeTerminate::RPProcessShapeTerminate( RPObject* _parent )
	: RPProcessShape(_parent, _T("Terminate"))
	, m_terminatedTransactCount(0)
{
	m_term_inc = 1;
	m_name     = _T("Terminate");

	// инициализация типа блока 
	type = "block";
		
	pa_src.push_back( rp::point(-50,   0) );
	pa_src.push_back( rp::point(-25, -25) );
	pa_src.push_back( rp::point( 50, -25) );
	pa_src.push_back( rp::point( 50,  25) );
	pa_src.push_back( rp::point(-25,  25) );
	pa_src.push_back( rp::point(-50,   0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180, "transact" ) );

	indent = 5;
}

RPProcessShapeTerminate::~RPProcessShapeTerminate()
{
}

RPObject* RPProcessShapeTerminate::newObject( RPObject* parent )
{
	return new RPProcessShapeTerminate( parent );
}

void RPProcessShapeTerminate::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPProcessShapeTerminateDlg1 dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPProcessShapeTerminate::generate()
{
	LPRPProcessShapeTerminate pThis(this);
	ASSERT(pThis);

	pInternalStatistics = pThis.interface_cast<rdo::runtime::IInternalStatistics>();
	ASSERT(pInternalStatistics);

	m_pParams = rdo::Factory<rdo::compiler::gui::RPShapeDataBlockTerminate>::create(m_name);
	m_pParams->setTermInc   (m_term_inc         );
	m_pParams->setStatistics(pInternalStatistics);

	studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_TERMINATE, m_pParams.get());

	m_pParams = NULL;

	m_terminatedTransactCount = 0;
	update();
}

void RPProcessShapeTerminate::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPProcessShapeTerminate/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) Атрибуты симулятора
	node.append_attribute(_T("name"))                .set_value(getName().c_str());
	node.append_attribute(_T("terminateCounter"))    .set_value(m_term_inc       );
}

void RPProcessShapeTerminate::loadFromXML(CREF(pugi::xml_node) node)
{
	// Считываем атрибуты для загрузки сохраненного блока "Terminate":
	for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// Присваиваем сохраненные в xml-файле параметры:
		// Для симулятора (диалоговые окна)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("terminateCounter"))
		{
			m_term_inc = attr.as_int();
		}
	}
	// Отображения объекта на Flowchart'е
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}

void RPProcessShapeTerminate::setTransCount(ruint count)
{
	m_terminatedTransactCount = count;
	update();
}

void RPProcessShapeTerminate::drawCustom(REF(CDC) dc)
{
	dc.SetTextColor(RGB(0x00, 0x64, 0x00));
	dc.TextOut(this->pa_global.getMaxX() - 2*indent, this->pa_global.getMaxY() + indent, rp::string::fromint(m_terminatedTransactCount).c_str());
}
