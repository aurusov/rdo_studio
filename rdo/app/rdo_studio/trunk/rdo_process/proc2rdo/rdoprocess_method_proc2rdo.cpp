#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_generation_type.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/method_big.xpm"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/method_small.xpm"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/block_create.xpm"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/block_decide.xpm"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/block_terminate.xpm"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/block_process.xpm"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/res/block_resource.xpm"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_chart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

RPMethodProc2RDO* proc2rdo = NULL;

RPMethodProc2RDO::RPMethodProc2RDO( RPObject* _parent ):
	rpMethod::RPMethod( _parent, "РДО-Процесс" ),
	RPProcessObject( get_this() ),
	btn_generate( -1 ),
	btn_generate_setup( -1 ),
	RDOfiles( NULL ) // инициализация
{
	proc2rdo = this;
	m_generateTime = 100;
	RDOfiles = new RPCreationRDOFilesMJ();
	pixmap_big   = rpMethod::project->createBitmap( method_big_xpm );
	pixmap_small = rpMethod::project->createBitmap( method_small_xpm );
}

RPMethodProc2RDO::~RPMethodProc2RDO()
{
	delete RDOfiles;
	proc2rdo = NULL;
}

void RPMethodProc2RDO::registerObject()
{
	// Базовый класс
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPProcessObject", NULL ) );

	// Потомки RPMethod
	RPObjectClassInfo* class_info = new RPObjectClassInfo( "RPMethodProc2RDO", "RPMethod" );
	class_info->multiParent( "RPProcessObject" );
	rpMethod::factory->insertFactory( class_info );

	// Потомки RPObjectFlowChart
	class_info = new RPObjectClassInfo( "RPProcessFlowChart", "RPObjectFlowChart", RPProcessFlowChart::newObject, this, _T("РДО-Процесс") );
	class_info->multiParent( "RPProcessObject" );
	rpMethod::factory->insertFactory( class_info );

	// Потомки RPShape
	class_info = new RPObjectClassInfo( "RPProcessShape", "RPShape" );
	class_info->multiParent( "RPProcessObject" );
	rpMethod::factory->insertFactory( class_info );

	// Потомки RPProcessShape
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPProcessShapeCreate", "RPProcessShape", RPProcessShapeCreate::newObject, this, _T("Генератор"), block_create_xpm, 0 ) );

	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPProcessShapeTerminate", "RPProcessShape", RPProcessShapeTerminate::newObject, this, _T("Терминатор"), block_terminate_xpm, 1 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPProcessShapeProcess", "RPProcessShape", RPProcessShapeProcess::newObject, this, _T("Процесс"), block_process_xpm, 2 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPProcessShapeResource", "RPProcessShape", RPProcessShapeResource::newObject, this, _T("Ресурс"), block_resource_xpm, 3 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPProcessShapeDecide", "RPProcessShape", RPProcessShapeDecide::newObject, this, _T("Десайд"), block_decide_xpm, 4 ) );
}

rpMethod::RPMethod* RPMethodProc2RDO::registerMethod()
{
	new RPMethodProc2RDO( NULL );
	proc2rdo->registerObject();

	return proc2rdo;
}

RPObjectFlowChart* RPMethodProc2RDO::makeFlowChart( RPObject* parent )
{
	return new RPProcessFlowChart( parent );
}

void RPMethodProc2RDO::buttonCommand( int button_id )
{
	if ( button_id == btn_generate ) {
		generate();
//		model->closeModel();
//		model->openModel();
	} else if ( button_id == btn_generate_setup ) {
		RPProcessGenerationType dlg( AfxGetMainWnd() );
		dlg.DoModal();	
	}
}

void RPMethodProc2RDO::buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update )
{
	button_update.enable = true;
}

void RPMethodProc2RDO::generate()
{
	std::list< RPObject* > all_child;
	// Вызвали генерацию у объектов
	all_child.clear();
	rpMethod::project->getAllChildByClass( all_child, "RPProcessShape", true );
	std::list< RPObject* >::iterator shape_it = all_child.begin();
	while ( shape_it != all_child.end() ) {
		dynamic_cast<RPProcessObject*>(static_cast<RPProcessShape*>(*shape_it))->generate();
		shape_it++;
	}
}

void RPMethodProc2RDO::saveToXML(pugi::xml_node &parentNode) const
{
	UNUSED(parentNode);
	NEVER_REACH_HERE;
}

void RPMethodProc2RDO::loadFromXML(const pugi::xml_node &node)
{
	UNUSED(node);
	NEVER_REACH_HERE;
}

rbool RPMethodProc2RDO::checkModelStructure()
{
// Заполнили ID
	std::list< RPObject* > all_child;
	std::list< rp::string > class_names;
	class_names.push_back( "RPProcessShapeCreate" );
	class_names.push_back( "RPProcessShapeProcess" );
	class_names.push_back( "RPProcessShapeTerminate" );
	class_names.push_back( "RPProcessShapeDecide" );
	rpMethod::project->getAllChildByClasses( all_child, class_names, true );
	if(all_child.size()>0)
	{
		std::list< RPObject* >::const_iterator block_it = all_child.begin();
		while ( block_it != all_child.end() )
		{
			std::list< RPShape* > list = static_cast<RPShape*>(*block_it)->getNextBlock();
			if(block_it == all_child.begin() && (*block_it)->getClassName()!="RPProcessShapeCreate")
			{
				TRACE1( "%s\n", "Модель должна содержать блок Create" );
				return false;
			}
			else
			{
				if ( !list.empty() )
				{
					dynamic_cast<RPProcessObject*>(*block_it)->id_next = list.front()->getName();
					rp::string name1 = dynamic_cast<RPProcessObject*>(*block_it)->id_next;
					if((*block_it)->getClassName()=="RPProcessShapeDecide"){
						if(list.size()==2)
						{
							dynamic_cast<RPProcessObject*>(*block_it)->id_next2 = list.back()->getName();
							rp::string name2 = dynamic_cast<RPProcessObject*>(*block_it)->id_next2;
						}
						else
						{
							rp::string error="БЛОК "+(*block_it)->getName()+" ДОЛЖЕН ИМЕТЬ 2 ВЕТВИ!!\nА НЕ ОДНУ!!";
							TRACE1( "%s\n", error.c_str() );
							return false;
						}
					}
				}else{
					if((*block_it)->getClassName()!="RPProcessShapeTerminate")
					{
						rp::string error="БЛОК "+(*block_it)->getName()+" ДОЛЖЕН ИМЕТЬ КОННЕКТОР";
						TRACE1( "%s\n", error.c_str() );
						return false;
					}
				}
				block_it++;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
