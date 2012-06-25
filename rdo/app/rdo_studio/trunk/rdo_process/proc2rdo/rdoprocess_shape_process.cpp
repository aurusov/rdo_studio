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

RPProcessShapeProcess::RPProcessShapeProcess( RPObject* _parent )
	: RPProcessShape( _parent, _T("Process") )
	, m_currentTransactCountProc(0)
{
	
	gname = _T("Process"); // имя
	
	gtype    = 0;
	base_gen = 1234567;
	
	//атрибуты законов
	gexp  = 0;
	gdisp = 0;
	gmax  = 0;
	gmin  = 0;

	action = 0; // тип действия по отношению к ресурсу
	prior  = 0;
	queue  = 0;
	parameter = 888; // ПОКА НЕ ТРОГАЛ ЭТОТ ПАРАМЕТР
	indent = 5;
		
		// инициализация типа блока 
	type ="block";
		
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point( 50, -25) );
	pa_src.push_back( rp::point( 50,  25) );
	pa_src.push_back( rp::point(-50,  25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDock   (this, RPConnectorDock::in,  rp::point(-50,  0), 180, "transact"));
	docks.push_back( new RPConnectorDockOne(this, RPConnectorDock::out, rp::point( 50,  0),   0, "transact"));
	docks.push_back( new RPConnectorDock   (this, RPConnectorDock::in,  rp::point(  0, 25), 270, "resource"));
}

RPProcessShapeProcess::~RPProcessShapeProcess()
{
}

RPObject* RPProcessShapeProcess::newObject( RPObject* parent )
{
	return new RPProcessShapeProcess( parent );
}

void RPProcessShapeProcess::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPProcessShapeProcessDlg1 dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPProcessShapeProcess::generate()
{
	rdo::compiler::gui::RPShapeDataBlock::zakonRaspr zakon;
	switch(gtype)
	{
		case 0: // константа
			zakon = rdo::compiler::gui::RPShapeDataBlock::Const;
			break;	
		case 1: // нормальный
			zakon = rdo::compiler::gui::RPShapeDataBlock::Normal;
			break;
		case 2: // равномерный закон
			zakon = rdo::compiler::gui::RPShapeDataBlock::Uniform;
			break;
		case 3: // треугольный
			zakon = rdo::compiler::gui::RPShapeDataBlock::Triangular;
			break;
		case 4: // экспоненциальный
			zakon = rdo::compiler::gui::RPShapeDataBlock::Exp;
			break;
	}

	LPRPProcessShapeProcess pThis(this);
	ASSERT(pThis);

	pInternalStatistics = pThis.interface_cast<rdo::runtime::IInternalStatistics>();
	ASSERT(pInternalStatistics);

	m_pParams = rdo::Factory<rdo::compiler::gui::RPShapeDataBlockProcess>::create(zakon, gname);
	ASSERT(m_pParams);
	m_pParams->setBase(base_gen);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);
	m_pParams->setMax(gmax);
	m_pParams->setStatistics(pInternalStatistics);

	switch(action)
	{
		case 0://advance
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_ADVANCE);
			break;
		case 1://sieze,advance,release
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_SEIZE  );
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_ADVANCE);
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_RELEASE);
			break;
		case 2://seize,advance
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_SEIZE  );
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_ADVANCE);
			break;
		case 3://advance,release
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_ADVANCE);
			m_pParams->addAction(rdo::compiler::gui::RPShapeDataBlockProcess::A_RELEASE);
			break;
	}

	std::list<CString>::iterator it = m_resourceList.begin();
	while( it != m_resourceList.end() ) 
	{
		m_pParams->addRes(static_cast<tstring>(*it));
		it++;
	}

	studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_PROCESS, m_pParams.get());

	m_pParams = NULL;
}

void RPProcessShapeProcess::setTransCount(ruint count)
{
	m_currentTransactCountProc = count;
	update();
}

void RPProcessShapeProcess::drawCustom(REF(CDC) dc)
{
	dc.SetTextColor(RGB(0x00, 0x64, 0x00));
	dc.TextOut(this->pa_global.getMaxX() - 2*indent, this->pa_global.getMaxY() + indent, rp::string::fromint(m_currentTransactCountProc).c_str());
}

void RPProcessShapeProcess::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPProcessShapeProcess/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) Атрибуты симулятора
	node.append_attribute(_T("name"))       .set_value(getName().c_str());
	node.append_attribute(_T("base_gen"))   .set_value(base_gen         );
	node.append_attribute(_T("exp"))        .set_value(gexp             );
	node.append_attribute(_T("disp"))       .set_value(gdisp            );
	node.append_attribute(_T("zakon"))      .set_value(gtype            );
	node.append_attribute(_T("action"))     .set_value(action           );
	node.append_attribute(_T("prior"))      .set_value(prior            );
	node.append_attribute(_T("queue"))      .set_value(queue            );
}

void RPProcessShapeProcess::loadFromXML(CREF(pugi::xml_node) node)
{
	// Считываем атрибуты для загрузки сохраненного блока "Process":
	for(pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// Присваиваем сохраненные в xml-файле параметры:
		// Для симулятора (диалоговые окна)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("base_gen"))
		{
			base_gen = attr.as_int();
		}
		else if (attrName == _T("exp"))
		{
			gexp = attr.as_double();
		}
		else if (attrName == _T("disp"))
		{
			gdisp = attr.as_double();
		}
		else if (attrName == _T("zakon"))
		{
			gtype = attr.as_int();
		}
		else if (attrName == _T("action"))
		{
			action = attr.as_int();
		}
		else if (attrName == _T("prior"))
		{
			prior = attr.as_int();
		}
		else if (attrName == _T("queue"))
		{
			queue = attr.as_int();
		}
	}
	// Для отображения объекта на Flowchart'е
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}
