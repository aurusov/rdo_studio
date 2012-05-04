#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/src/application.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

RPShapeCreateMJ::RPShapeCreateMJ(PTR(RPObject) _parent)
	: RPShape_MJ(_parent, _T("Create"))
	, m_currentTransactCount(0)
{
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );

	// инициализация параметров для генерирования
	gname=_T("Create"); // имя
	gfirst=0; // время первого
	gamount=100; // кол-во создаваемых
	gtype=0; // закон прибытия
	base_gen=1234567890;
	//атрибуты законов
	gexp=0;
	gdisp=0;
	gmax=0;

	//второе окно
	inf=1000000; // бесконечноть
	gID=1; // ID группы
	gpar1=0;
	gpar2=0;
	gpar3=0;

	indent = 5;
}

RPShapeCreateMJ::~RPShapeCreateMJ()
{
}

rp::RPXMLNode* RPShapeCreateMJ::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

void RPShapeCreateMJ::saveToXML(REF(pugi::xml_node) parentNode) const
{
	// Записываем узел <RPShapeCreateMJ/>:
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	// Соxраняем атрибуты объекта:
	// 1) Атрибуты графики
	RPObjectMatrix::saveToXML(node);
	RPShape::       saveToXML(node);
	// 2) Атрибуты симулятора
	node.append_attribute(_T("name"))     .set_value(getName().c_str());
	node.append_attribute(_T("amount"))   .set_value(gamount          );
	node.append_attribute(_T("base_gen")) .set_value(base_gen         );
	node.append_attribute(_T("exp"))      .set_value(gexp             );
	node.append_attribute(_T("disp"))     .set_value(gdisp            );
	node.append_attribute(_T("zakon"))    .set_value(gtype            );
}

void RPShapeCreateMJ::loadFromXML(CREF(pugi::xml_node) node)
{
	// Считываем атрибуты для загрузки сохраненного блока "Create":
	for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
	{
		// Присваиваем сохраненные в xml-файле параметры
		// Для симулятора (диалоговые окна)
		tstring attrName = attr.name();
		if (attrName == _T("name"))
		{
			setName(attr.value());
		}
		else if (attrName == _T("amount"))
		{
			gamount = attr.as_int();
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
	}
	// Отображения объекта на Flowchart'е
	RPObjectMatrix::loadFromXML(node);
	RPShape::       loadFromXML(node);
}

RPObject* RPShapeCreateMJ::newObject( RPObject* parent )
{
	return new RPShapeCreateMJ( parent );
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeCreateMJ::generate()
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

	LPRPShapeCreateMJ pThis(this); 
	ASSERT(pThis);

	pInternalStatistics = pThis.interface_cast<rdo::runtime::IInternalStatistics>();
	ASSERT(pInternalStatistics);

	m_pParams = rdo::Factory<rdo::compiler::gui::RPShapeDataBlockCreate>::create(zakon, gname);
	m_pParams->setBase(base_gen);
	m_pParams->setAmount(gamount);
	m_pParams->setDisp(gdisp);
	m_pParams->setExp(gexp);
	m_pParams->setMax(gmax);
	m_pParams->setStatistics(pInternalStatistics);

	studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_PROCGUI_BLOCK_CREATE, m_pParams.get());

	m_pParams = NULL;
}

void RPShapeCreateMJ::setTransCount(ruint count)
{
	m_currentTransactCount = count;
	update();
}

void RPShapeCreateMJ::drawCustom(REF(CDC) dc)
{
	dc.SetTextColor(RGB(0x00, 0x64, 0x00));
	dc.TextOut((3*(this->pa_global.getMaxX()) + (this->pa_global.getMinX()))/4 - indent, this->pa_global.getMaxY() + indent, rp::string::fromint(m_currentTransactCount).c_str());
}
