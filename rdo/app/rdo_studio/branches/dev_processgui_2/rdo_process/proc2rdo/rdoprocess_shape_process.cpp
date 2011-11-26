// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"
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
RPShapeProcessMJ::RPShapeProcessMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Process") )
{
	
	gname=_T("Process"); // имя
	
	gtype = 0;
	base_gen = 1234567;
	
	//атрибуты законов
	gexp=0;
	gdisp=0;
	gmax=0;
	gmin=0;

	action=0; // тип действия по отношению к ресурсу
	prior=0;
	queue=0;
	parameter=888; // ПОКА НЕ ТРОГАЛ ЭТОТ ПАРАМЕТР
		
		// инициализация типа блока 
	type ="block";
		
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(50, -25) );
	pa_src.push_back( rp::point(50, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point( -50, 0 ), 180, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point(  0, 25), 270, "resource" ) );
}

RPShapeProcessMJ::~RPShapeProcessMJ()
{
}

RPObject* RPShapeProcessMJ::newObject( RPObject* parent )
{
	return new RPShapeProcessMJ( parent );
}

void RPShapeProcessMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeProcessDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeProcessMJ::generate()
{
	RPShapeDataBlock::zakonRaspr zakon;
	switch(gtype)
	{
		case 0: // константа
			zakon = RPShapeDataBlock::Const;
			break;	
		case 1: // нормальный
			zakon = RPShapeDataBlock::Normal;
			break;
		case 2: // равномерный закон
			zakon = RPShapeDataBlock::Uniform;
			break;
		case 3: // треугольный
			zakon = RPShapeDataBlock::Triangular;
			break;
		case 4: // экспоненциальный
			zakon = RPShapeDataBlock::Exp;
			break;
	}

	m_pParams = rdo::Factory<RPShapeDataBlockProcess>::create(zakon, gname);
	ASSERT(m_pParams);
	m_pParams->setBase(base_gen);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);
	m_pParams->setMax(gmax);

	switch(action)
	{
		case 0://advance
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			break;
		case 1://sieze,advance,release
			m_pParams->addAction(RPShapeDataBlockProcess::A_SEIZE  );
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			m_pParams->addAction(RPShapeDataBlockProcess::A_RELEASE);
			break;
		case 2://seize,advance
			m_pParams->addAction(RPShapeDataBlockProcess::A_SEIZE  );
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			break;
		case 3://seize,advance
			m_pParams->addAction(RPShapeDataBlockProcess::A_ADVANCE);
			m_pParams->addAction(RPShapeDataBlockProcess::A_RELEASE);
			break;
	}

	std::list<CString>::iterator it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
		m_pParams->addRes(static_cast<tstring>(*it));
		it++;
	}

	studioApp.studioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_PROCESS, m_pParams.get());
	m_pParams = NULL;
}

void RPShapeProcessMJ::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPShapeProcessMJ/>:
	pugi::xml_node      node        = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	pugi::xml_attribute nameAttr    = node.append_attribute("gname");
						nameAttr.set_value(getName().c_str());
	pugi::xml_attribute position_X  = node.append_attribute("pos_X");
						position_X.set_value(getCenter().x);
	pugi::xml_attribute position_Y  = node.append_attribute("pos_Y");
						position_Y.set_value(getCenter().y);
	pugi::xml_attribute scale_X     = node.append_attribute("scale_X");
						scale_X.set_value(getScaleX());
	pugi::xml_attribute scale_Y     = node.append_attribute("scale_Y");
						scale_Y.set_value(getScaleY());
	// 2) Атрибуты симулятора
	pugi::xml_attribute basegenAttr = node.append_attribute("base_gen");
						basegenAttr.set_value(base_gen);
	pugi::xml_attribute gexpAttr    = node.append_attribute("gexp");
						gexpAttr.set_value(gexp);
	pugi::xml_attribute gdispAttr   = node.append_attribute("gdisp");
						gdispAttr.set_value(gdisp);
	pugi::xml_attribute zakonAttr   = node.append_attribute("zakon");
						zakonAttr.set_value(gtype);
	pugi::xml_attribute actionAttr  = node.append_attribute("action");
						actionAttr.set_value(action);
	pugi::xml_attribute priorAttr   = node.append_attribute("prior");
						priorAttr.set_value(prior);
	pugi::xml_attribute queueAttr   = node.append_attribute("queue");
						queueAttr.set_value(queue);
}

void RPShapeProcessMJ::loadFromXML(CREF(pugi::xml_node) node)
{
	// Считываем атрибуты для загрузки сохраненного блока "Process":
	for(pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// Присваиваем сохраненные в xml-файле параметры:
		// 1) Для отображения объекта на Flowchart'е
		if ( strcmp(attr.name(), "gname")    == 0 )				setName   (attr.value());
		if ( strcmp(attr.name(), "pos_X")    == 0 )				setX      (attr.as_double());
		if ( strcmp(attr.name(), "pos_Y")    == 0 )				setY      (attr.as_double());
		if ( strcmp(attr.name(), "scale_X")  == 0 )				setScaleX (attr.as_double());
		if ( strcmp(attr.name(), "scale_Y")  == 0 )				setScaleY (attr.as_double());
		// 2) Для симулятора (диалоговые окна)
		if ( strcmp(attr.name(), "base_gen") == 0 )				base_gen  = attr.as_int();
		if ( strcmp(attr.name(), "gexp")     == 0 )				gexp      = attr.as_double();
		if ( strcmp(attr.name(), "gdisp")    == 0 )				gdisp     = attr.as_double();
		if ( strcmp(attr.name(), "zakon")    == 0 )				gtype     = attr.as_int();
		if ( strcmp(attr.name(), "action")   == 0 )				action    = attr.as_int();
		if ( strcmp(attr.name(), "prior")    == 0 )				prior     = attr.as_int();
		if ( strcmp(attr.name(), "queue")    == 0 )				queue     = attr.as_int();
	}
}
