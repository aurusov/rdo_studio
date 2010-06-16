#include "stdafx.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_method_proc2rdo_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_create_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_resource.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_process_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_terminate_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_generation_type_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/res/method_big.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/method_small.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/generate.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/generate_setup.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_create.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_decide.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_terminate.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_process.xpm"
#include "rdo_studio/rdo_process/proc2rdo/res/block_resource.xpm"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_object_chart.h"
#include "rdo_studio/rdo_process/rp_method/rdoprocess_shape.h"
#include <afxole.h>		// для rdostudioapp
#include <afxpriv.h>    // для rdostudioapp
#include "rdo_studio/rdostudioapp.h"
#include "rdo_repository/rdorepository.h"
#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdostudiomodel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RPMethodProc2RDO_MJ* proc2rdo = NULL;

RPMethodProc2RDO_MJ::RPMethodProc2RDO_MJ( RPObject* _parent ):
	rpMethod::RPMethod( _parent, "РДО-Процесс" ),
	RPObject_MJ( get_this() ),
	btn_generate( -1 ),
	btn_generate_setup( -1 ),
	RDOfiles( NULL ) // инициализация
{
	proc2rdo = this;
	generate_time_MJ = 100;
	RDOfiles = new RPCreationRDOFilesMJ();
	pixmap_big   = rpMethod::project->createBitmap( method_big_xpm );
	pixmap_small = rpMethod::project->createBitmap( method_small_xpm );
}

RPMethodProc2RDO_MJ::~RPMethodProc2RDO_MJ()
{
	delete RDOfiles;
	proc2rdo = NULL;
}

void RPMethodProc2RDO_MJ::registerObject()
{
	// Базовый класс
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPObject_MJ", NULL ) );

	// Потомки RPMethod
	RPObjectClassInfo* class_info = new RPObjectClassInfo( "RPMethodProc2RDO_MJ", "RPMethod" );
	class_info->multiParent( "RPObject_MJ" );
	rpMethod::factory->insertFactory( class_info );

	// Потомки RPObjectFlowChart
	class_info = new RPObjectClassInfo( "RPObjectFlowChart_MJ", "RPObjectFlowChart", RPObjectFlowChart_MJ::newObject, this, _T("РДО-Процесс") );
	class_info->multiParent( "RPObject_MJ" );
	rpMethod::factory->insertFactory( class_info );

	// Потомки RPShape
	class_info = new RPObjectClassInfo( "RPShape_MJ", "RPShape" );
	class_info->multiParent( "RPObject_MJ" );
	rpMethod::factory->insertFactory( class_info );

	// Потомки RPShape_MJ
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeCreateMJ", "RPShape_MJ", RPShapeCreateMJ::newObject, this, _T("Генератор"), block_create_xpm, 0 ) );

	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeTerminateMJ", "RPShape_MJ", RPShapeTerminateMJ::newObject, this, _T("Терминатор"), block_terminate_xpm, 1 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeProcessMJ", "RPShape_MJ", RPShapeProcessMJ::newObject, this, _T("Процесс"), block_process_xpm, 2 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeResource_MJ", "RPShape_MJ", RPShapeResource_MJ::newObject, this, _T("Ресурс"), block_resource_xpm, 3 ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeDecide", "RPShape_MJ", RPShapeDecide::newObject, this, _T("Десайд"), block_decide_xpm, 4 ) );

	RPCtrlToolbar* toolbar = rpMethod::project->createToolBar( _T("РДО-Процесс") );
	btn_generate       = toolbar->insertButton( this, generate_xpm, _T("Создать модель") );
	btn_generate_setup = toolbar->insertButton( this, generate_setup_xpm, _T("Настройки") );
}

rpMethod::RPMethod* RPMethodProc2RDO_MJ::registerMethod()
{
	new RPMethodProc2RDO_MJ( NULL );
	proc2rdo->registerObject();

	return proc2rdo;
}

RPObjectFlowChart* RPMethodProc2RDO_MJ::makeFlowChart( RPObject* parent )
{
	return new RPObjectFlowChart_MJ( parent );
}

void RPMethodProc2RDO_MJ::buttonCommand( int button_id )
{
	if ( button_id == btn_generate ) {
		generate();
		model->closeModel();
		model->openModel();
	} else if ( button_id == btn_generate_setup ) {
		RP_GENERATION_TYPE_MJ dlg( AfxGetMainWnd() );
		dlg.DoModal();	
	}
}

void RPMethodProc2RDO_MJ::buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update )
{
	button_update.enable = true;
}

tstring RPMethodProc2RDO_MJ::getDirectory() const
{
	rdoRepository::RDOThreadRepository::FileInfo data(rdoModelObjects::PAT);
	studioApp.studioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);

	tstring::size_type pos = data.m_full_name.find(data.m_extention);
	ASSERT(pos != tstring::npos);

	tstring result(data.m_full_name.substr(0, pos));
	return result;
}

tstring RPMethodProc2RDO_MJ::getName() const
{
	rdoRepository::RDOThreadRepository::FileInfo data(rdoModelObjects::PAT);
	studioApp.studioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);

	tstring result = data.m_name;
	return result;
}

void RPMethodProc2RDO_MJ::generate()
{
	//RPConnectorDock* dock = NULL;
	//if (RPConnectorDock::can_connect(dock));
//	CreateDirectory("C:/Mymodel01", NULL);
tstring str = rdo::format(_T("%s.pat"), getDirectory().c_str()).c_str();
	RDOfiles->pattern.open  (rdo::format(_T("%s.pat"), getDirectory().c_str()).c_str());
	RDOfiles->pattern.is_open();
	RDOfiles->resourse.open  (rdo::format(_T("%s.rss"), getDirectory().c_str()).c_str());
	RDOfiles->function.open  (rdo::format(_T("%s.fun"), getDirectory().c_str()).c_str());
	RDOfiles->typeres.open   (rdo::format(_T("%s.rtp"), getDirectory().c_str()).c_str());
	RDOfiles->operations.open(rdo::format(_T("%s.opr"), getDirectory().c_str()).c_str());
	RDOfiles->smr.open       (rdo::format(_T("%s.smr"), getDirectory().c_str()).c_str());
	RDOfiles->statistic.open (rdo::format(_T("%s.pmd"), getDirectory().c_str()).c_str());

	blank_rdo_MJ();

	// Заполнили ID
	std::list< RPObject* > all_child;
	std::list< rp::string > class_names;
	class_names.push_back( "RPShapeCreateMJ" );
	class_names.push_back( "RPShapeProcessMJ" );
	class_names.push_back( "RPShapeTerminateMJ" );
	class_names.push_back( "RPShapeDecide" );
	rpMethod::project->getAllChildByClasses( all_child, class_names, true );
	std::list< RPObject* >::const_iterator block_it = all_child.begin();
	while ( block_it != all_child.end() ) {
		std::list< RPShape* > list = static_cast<RPShape*>(*block_it)->getNextBlock();
		if ( !list.empty() ){
			dynamic_cast<RPObject_MJ*>(*block_it)->id_next = list.front()->getName();
			rp::string name1 = dynamic_cast<RPObject_MJ*>(*block_it)->id_next;
			if((*block_it)->getClassName()=="RPShapeDecide"){
				if(list.size()==2){
					dynamic_cast<RPObject_MJ*>(*block_it)->id_next2 = list.back()->getName();
					rp::string name2 = dynamic_cast<RPObject_MJ*>(*block_it)->id_next2;
				}else{
						TRACE1( "%s\n", "ДЕСАЙД ДОЛЖЕН ИМЕТЬ 2 ВЕТВИ!!\nА НЕ ОДНУ!!" );
				}
			}
		}else{
			if((*block_it)->getClassName()!="RPShapeTerminateMJ")
			{
				TRACE1( "%s\n", "БЛОК ДОЛЖЕН ИМЕТЬ КОННЕКТОР" );
			}
		}
		block_it++;
	}

	// Вызвали генерацию у объектов
	all_child.clear();
	rpMethod::project->getAllChildByClass( all_child, "RPShape_MJ", true );
  	std::list< RPObject* >::iterator shape_it = all_child.begin();
	while ( shape_it != all_child.end() ) {
		dynamic_cast<RPObject_MJ*>(static_cast<RPShape_MJ*>(*shape_it))->generate();
		shape_it++;
	}

	RDOfiles->pattern.close();

	RDOfiles->resourse<<std::endl<<std::endl<<"$End";
	RDOfiles->resourse.close();

	RDOfiles->function.close();
	RDOfiles->typeres.close();

	RDOfiles->smr.close();

	RDOfiles->statistic<<
	std::endl<<"$End";
	RDOfiles->statistic.close();

	//ГЕНЕРИРОВАНИЕ  ОПЕРАЦИЙ *.opr тут т.к. должны быть уже сгенерированны все файлы

	//list_pattern_names.push_back("{Блок_останова_моделирования_по_времени}");

					RDOfiles->operations<<"$Operations";

				std::list<CString>::iterator it = list_pattern_names.begin();
				  int amount = 1;	

				  while( it != list_pattern_names.end() ) 
					{
					TRACE1( "%s\n", (*it) );
					RDOfiles->operations<<std::endl
					<<std::endl<<"operation_"<<amount<<" : "<<(*it);

					it++; amount++;
					}


					RDOfiles->operations
					<<std::endl<<"$End";
list_pattern_names.clear();

	RDOfiles->operations.close();
}

void RPMethodProc2RDO_MJ::blank_rdo_MJ()
{
//ГЕНЕРИРОВАНИЕ ТИПОВ РЕССУРСОВ *.rtp
RDOfiles->typeres
<<std::endl	
<<std::endl<<"{ res }"
<<std::endl<<"$Resource_type Resource : permanent {Параметры блока Create}"
<<std::endl<<"$Parameters"
<<std::endl<<"Состояние : (занят,свободен)"
<<std::endl<<"Колличество_транзактов : integer {сколько обслуживается транзактов в текущий момент}"
<<std::endl<<"Максим_колличество : integer {макс колличество которое может обслуживаться"
<<std::endl<<"после которого рессурс переходит в состяоние занят}"
<<std::endl<<"$End"
<<std::endl
<<std::endl
<<std::endl
<<std::endl
<<std::endl
<<std::endl<<"{1}"
<<std::endl<<"$Resource_type Creates : permanent {Параметры блока Create}"
<<std::endl<<"$Parameters"
<<std::endl<<"  par_1 : (_true, _false) {false} {связанно с созданием первого только}"
<<std::endl<<"  par_amount: integer   {1}     {соклько выполнилось раз} "
<<std::endl<<"$End"
<<std::endl
<<std::endl
<<std::endl
<<std::endl<<"$Resource_type Group_of_transacts_X : temporary    {Параметры создаваемого транзакта}"
<<std::endl<<"$Parameters"
<<std::endl<<"   {параметры транзакта задаются пользователем"
<<std::endl<<"    и генерируются автоматически}"
<<std::endl
<<std::endl<<"{ ПАРАМЕТРЫ МЕСТО НАХОЖДЕНИЯ }"
<<std::endl
<<std::endl<<"	Место_нахождения :(";

	std::list< RPObject* > blocks;
	rpMethod::project->getAllChildByClass( blocks, "RPShape_MJ", true );
	std::list< RPObject* >::const_iterator it = blocks.begin();
	while ( it != blocks.end() ) {
		RDOfiles->typeres << (*it)->getName() << ", ";
		it++;
	}

RDOfiles->typeres<<")"


<<std::endl<<"	Место_нахождения_будущее : such_as  Group_of_transacts_X.Место_нахождения {ID модуля, куда планируется отправится на след шаге}"
<<std::endl
<<std::endl	
<<std::endl
<<std::endl<<"{ CОСТОЯНИЕ ТРАНЗАКТА}	"
<<std::endl<<"	Состояние_транзакта : integer {}"
<<std::endl
<<std::endl
<<std::endl<<"{PRIMARY KEY}"
<<std::endl<<"    Идентификационный_номер_транзакта : integer"
<<std::endl<<"    Идентификационный_номер_группы_транзактов : such_as  Group_of_transacts_X.Место_нахождения"
<<std::endl<<"{-----------}"
<<std::endl
<<std::endl<<"	Место_в_очереди : integer"
<<std::endl<<"	Состояние_транспортировки : (ожидает, захвачен, в_очереди, в_очереди_выход)"
<<std::endl
<<std::endl<<"{ ДОПОЛНИТЕЛЬНЫЕ ПАРАМЕТРЫ ОПРЕДЕЛЯЮЩИЕСЯ ПОЛЬЗОВАТЕЛЕМ}"
<<std::endl
<<std::endl<<"{" 
<<std::endl<<"... : ..."
<<std::endl<<" }"
<<std::endl<<"$End "
<<std::endl 
<<std::endl<<" {2 очередь}"
<<std::endl<<" $Resource_type Queue : permanent"
<<std::endl<<"$Parameters"
<<std::endl<<"  Размер_очереди : integer"
<<std::endl<<"	 Выход_очереди: (свободен, занят)"
<<std::endl<<"$End"
<<std::endl
<<std::endl
<<std::endl<<"{3 процесс}"
<<std::endl<<"$Resource_type Block_process : permanent {Параметры блока Create}"
<<std::endl<<"$Parameters"
<<std::endl<<"  Состояние : (занят,свободен)"
<<std::endl<<"$End"
<<std::endl
<<std::endl<<"{4del}"
<<std::endl<<" $Resource_type Block_Del : permanent"
<<std::endl<<"$Parameters"
<<std::endl<<"  Колличество_удаленных : integer"
<<std::endl
<<std::endl<<"$End";


//ГЕНЕРИРОВАНИЕ  РЕССУРСОВ *.rtp
RDOfiles->resourse<<"$Resources";


//ГЕНЕРИРОВАНИЕ  СТАТИСТИКИ *.pmd
RDOfiles->statistic<<

std::endl<<"$Results";



//ГЕНЕРИРОВАНИЕ  SMR *.smr
RDOfiles->smr<<
std::endl<<"Model_name = "<< getName()
<<std::endl
<<std::endl<<"Resource_file  = "<< getName()
<<std::endl<<"OprIev_file    = "<< getName() // OPR
<<std::endl<<"Trace_file     = "<< getName()
<<std::endl<<"Statistic_file = "<< getName() //PMD
<<std::endl<<"Results_file   = "<< getName()
//<<std::endl<<"Frame_file     = RDO_PROCESS"
//<<std::endl<<"Frame_number   = 1"
//<<std::endl<<"Show_mode      = Animation"
//<<std::endl<<"Show_rate      = 10000000.0"
<<std::endl
<<std::endl<<"Terminate_if  Time_now >= "<< generate_time_MJ;



// ГЕНЕРИРОВАНИЕ ОПРЕЦИЙ в *.pat генерация блока перемещения
list_pattern_names.push_back("Блок_перемещения");// добавляем имя для *.opr

RDOfiles->pattern
<<"{----------------ГЕНЕРАЦИЯ БЛОКА ПЕРЕМЕЩЕНИЯ-------------------------------------------}"
<<std::endl<<"$Pattern Блок_перемещения : rule  {срабатывание закона}trace"
<<std::endl<<"	$Relevant_resources"
<<std::endl<<"		_transact_X : Group_of_transacts_X Keep"
<<std::endl
<<std::endl<<"	$Body"
<<std::endl
<<std::endl<<"	_transact_X"
<<std::endl<<"		Choice from _transact_X.Состояние_транспортировки = ожидает  and"
<<std::endl<<"		_transact_X.Место_нахождения_будущее<>_transact_X.Место_нахождения "
<<std::endl<<"	first"
<<std::endl
<<std::endl<<"	Convert_rule"
<<std::endl<<"		Место_нахождения set  _transact_X.Место_нахождения_будущее"
<<std::endl
<<std::endl<<"	$End";


RDOfiles->pattern
<<std::endl<<"{----------------ГЕНЕРАЦИЯ если останавливается по времени-------------------------------------------}"
<<std::endl<<"{"
<<std::endl<<"$Pattern  Блок_останова_моделирования_по_времени : irregular_event trace "
<<std::endl<<"$Relevant_resources"
<<std::endl<<"_parameter : Group_of_transacts_X  NoChange {нужен для того, что без него глючит}"
<<std::endl<<"{"
<<std::endl<<"$Time ="<< generate_time_MJ
<<std::endl<<"$Body"
<<std::endl<<"_parameter"
<<std::endl<<"$End"
<<std::endl<<"}";
}
