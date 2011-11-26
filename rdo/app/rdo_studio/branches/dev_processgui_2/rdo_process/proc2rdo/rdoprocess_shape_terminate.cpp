// rdoprocess_shape_terminate_MJ.cpp: implementation of the RPShapeTerminate_MJ class.
//
//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeTerminateMJ::RPShapeTerminateMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Terminate") )
{
	m_term_inc=1;
	m_name=_T("Terminate");

	// инициализация типа блока 
	type ="block";
		
	pa_src.push_back( rp::point(-50, 0) );
	pa_src.push_back( rp::point(-25, -25) );
	pa_src.push_back( rp::point(50, -25) );
	pa_src.push_back( rp::point(50, 25) );
	pa_src.push_back( rp::point(-25, 25) );
	pa_src.push_back( rp::point(-50, 0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180, "transact" ) );
}

RPShapeTerminateMJ::~RPShapeTerminateMJ()
{
}

RPObject* RPShapeTerminateMJ::newObject( RPObject* parent )
{
	return new RPShapeTerminateMJ( parent );
}

void RPShapeTerminateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeTerminateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeTerminateMJ::generate()
{
	m_pParams = rdo::Factory<RPShapeDataBlockTerminate>::create(m_name);
	m_pParams->setTermInc(m_term_inc);
	studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_TERMINATE, m_pParams.get());
	m_pParams = NULL;
}

void RPShapeTerminateMJ::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RShapeTerminateMJ/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	node.append_attribute(_T("gname"))           .set_value(getName().c_str());
	node.append_attribute(_T("pos_X"))           .set_value(getCenter().x    );
	node.append_attribute(_T("pos_Y"))           .set_value(getCenter().y    );
	node.append_attribute(_T("scale_X"))         .set_value(getScaleX()      );
	node.append_attribute(_T("scale_Y"))         .set_value(getScaleY()      );
	node.append_attribute(_T("terminateCounter")).set_value(m_term_inc       );
}

void RPShapeTerminateMJ::loadFromXML(CREF(pugi::xml_node) node)
{
	// Считываем атрибуты для загрузки сохраненного блока "Terminate":
	for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		tstring attrName = attr.name();
		if (attrName == _T("gname"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("pos_X"))
		{
			setX(attr.as_double());
		}
		else if (attrName == _T("terminateCounter"))
		{
			m_term_inc = attr.as_int();
		}
		// Присваиваем сохраненные в xml-файле параметры:
		// 1) Для отображения объекта на Flowchart'е
		if ( strcmp(attr.name(), "pos_Y")      == 0 )			setY      (attr.as_double());
		if ( strcmp(attr.name(), "scale_X")    == 0 )			setScaleX (attr.as_double());
		if ( strcmp(attr.name(), "scale_Y")    == 0 )			setScaleY (attr.as_double());
		// 2) Для симулятора (диалоговые окна)
	}
}
