#include "stdafx.h"
#include "rdo_process_project_RDO_proc_MJ.h"
#include "../../rdoprocess_object_chart.h"
#include "../../rdoprocess_shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPProject_RDO_Proc_MJ::RPProject_RDO_Proc_MJ():
	RPProject(),
	RPObject_MJ( get_this() ),
	RDOfiles( NULL ) // инициализация
{
	RDOfiles = new RPCreationRDOFilesMJ();
}

RPProject_RDO_Proc_MJ::~RPProject_RDO_Proc_MJ()
{
}

void RPProject_RDO_Proc_MJ::generate()
{
	RDOfiles->pattern.open("aaa\\RDO_PROCESS.pat");
	RDOfiles->resourse.open("aaa\\RDO_PROCESS.rss");
	RDOfiles->function.open("aaa\\RDO_PROCESS.fun");
	
	RDOfiles->typeres.open("aaa\\RDO_PROCESS.rtp");
	RDOfiles->operations.open("aaa\\RDO_PROCESS.opr");
	RDOfiles->smr.open("aaa\\RDO_PROCESS.smr");
	RDOfiles->statistic.open("aaa\\RDO_PROCESS.pmd");
	
	blank_rdo_MJ();
	list_name();
	
	RDOfiles->pattern.close();
	
	RDOfiles->resourse<<endl<<endl<<"$End";
	RDOfiles->resourse.close();
	
	RDOfiles->function.close();
	RDOfiles->typeres.close();
	
	
	RDOfiles->smr.close();
	
	
	
	RDOfiles->statistic<<
	endl<<"$End";
	RDOfiles->statistic.close();
	
	
	//ГЕНЕРИРОВАНИЕ  ОПЕРАЦИЙ *.opr тут т.к. должны быть уже сгенерированны все файлы

		list_pattern_names.push_back("Блок_останова_моделирования_по_времени");

					RDOfiles->operations<<"$Operations";

				std::list<CString>::iterator it = list_pattern_names.begin();
				  int amount = 1;	
				
				  while( it != list_pattern_names.end() ) 
					{
					TRACE( "%s\n", (*it) );
					RDOfiles->operations<<endl
					<<endl<<"operation_"<<amount<<" : "<<(*it);

					it++; amount++;
					}


					RDOfiles->operations
					<<endl<<"$End";
list_pattern_names.clear();

	RDOfiles->operations.close();
}

void RPProject_RDO_Proc_MJ::blank_rdo_MJ()
{
//ГЕНЕРИРОВАНИЕ ТИПОВ РЕССУРСОВ *.rtp
RDOfiles->typeres
<<endl	
<<endl<<"{ res }"
<<endl<<"$Resource_type Resource : permanent {Параметры блока Create}"
<<endl<<"$Parameters"
<<endl<<"Состояние : (занят,свободен)"
<<endl<<"Колличество_транзактов : integer {сколько обслуживается транзактов в текущий момент}"
<<endl<<"Максим_колличество : integer {макс колличество которое может обслуживаться"
<<endl<<"после которого рессурс переходит в состяоние занят}"
<<endl<<"$End"
<<endl
<<endl
<<endl
<<endl
<<endl
<<endl<<"{1}"
<<endl<<"$Resource_type Creates : permanent {Параметры блока Create}"
<<endl<<"$Parameters"
<<endl<<"  par_1 : (true, false) {false} {связанно с созданием первого только}"
<<endl<<"  par_amount: integer   {1}     {соклько выполнилось раз} "
<<endl<<"$End"
<<endl
<<endl
<<endl
<<endl<<"$Resource_type Group_of_transacts_X : temporary    {Параметры создаваемого транзакта}"
<<endl<<"$Parameters"
<<endl<<"   {параметры транзакта задаются пользователем"
<<endl<<"    и генерируются автоматически}"
<<endl
<<endl<<"{ ПАРАМЕТРЫ МЕСТО НАХОЖДЕНИЯ }"
<<endl
<<endl<<"	Место_нахождения :(";

list_name_for_type_res(); // ф-ия заполняет имнами всех блоков
RDOfiles->typeres<<"ОШИБКА_НАДО_УСТРАНИТЬ)"


<<endl<<"	Место_нахождения_будущее : such_as  Group_of_transacts_X.Место_нахождения {ID модуля, куда планируется отправится на след шаге}"
<<endl
<<endl	
<<endl
<<endl<<"{ CОСТОЯНИЕ ТРАНЗАКТА}	"
<<endl<<"	Состояние_транзакта : integer {}"
<<endl
<<endl
<<endl<<"{PRIMARY KEY}"
<<endl<<"    Идентификационный_номер_транзакта : integer"
<<endl<<"    Идентификационный_номер_группы_транзактов : such_as  Group_of_transacts_X.Место_нахождения"
<<endl<<"{-----------}"
<<endl
<<endl<<"	Место_в_очереди : integer"
<<endl<<"	Состояние_транспортировки : (ожидает, захвачен, в_очереди, в_очереди_выход)"
<<endl
<<endl<<"{ ДОПОЛНИТЕЛЬНЫЕ ПАРАМЕТРЫ ОПРЕДЕЛЯЮЩИЕСЯ ПОЛЬЗОВАТЕЛЕМ}"
<<endl
<<endl<<"{" 
<<endl<<"... : ..."
<<endl<<" }"
<<endl<<"$End "
<<endl 
<<endl<<" {2 очередь}"
<<endl<<" $Resource_type Queue : permanent"
<<endl<<"$Parameters"
<<endl<<"  Размер_очереди : integer"
<<endl<<"	 Выход_очереди: (свободен, занят)"
<<endl<<"$End"
<<endl
<<endl
<<endl<<"{3 процесс}"
<<endl<<"$Resource_type Block_process : permanent {Параметры блока Create}"
<<endl<<"$Parameters"
<<endl<<"  Состояние : (занят,свободен)"
<<endl<<"$End"
<<endl
<<endl<<"{4del}"
<<endl<<" $Resource_type Block_Del : permanent"
<<endl<<"$Parameters"
<<endl<<"  Колличество_удаленных : integer"
<<endl
<<endl<<"$End";


//ГЕНЕРИРОВАНИЕ  РЕССУРСОВ *.rtp
RDOfiles->resourse<<"$Resources";


//ГЕНЕРИРОВАНИЕ  СТАТИСТИКИ *.pmd
RDOfiles->statistic<<

endl<<"$Results";



//ГЕНЕРИРОВАНИЕ  SMR *.smr
RDOfiles->smr<<
endl<<"Model_name = RDO_PROCESS"
<<endl
<<endl<<"Resource_file  = RDO_PROCESS"
<<endl<<"OprIev_file    = RDO_PROCESS" // OPR
<<endl<<"Trace_file     = RDO_PROCESS"
<<endl<<"Statistic_file = RDO_PROCESS" //PMD
<<endl<<"Results_file   = RDO_PROCESS"
//<<endl<<"Frame_file     = RDO_PROCESS"
//<<endl<<"Frame_number   = 1"
//<<endl<<"Show_mode      = Animation"
//<<endl<<"Show_rate      = 10000000.0"
<<endl
<<endl<<"Terminate_if  Time_now >= "<< generate_time_MJ;



// ГЕНЕРИРОВАНИЕ ОПРЕЦИЙ в *.pat генерация блока перемещения
list_pattern_names.push_back("Блок_перемещения");// добавляем имя для *.opr

RDOfiles->pattern
<<"{----------------ГЕНЕРАЦИЯ БЛОКА ПЕРЕМЕЩЕНИЯ-------------------------------------------}"
<<endl<<"$Pattern Блок_перемещения : rule  {срабатывание закона}trace"
<<endl<<"	$Relevant_resources"
<<endl<<"		_transact_X : Group_of_transacts_X Keep"
<<endl
<<endl<<"	$Body"
<<endl
<<endl<<"	_transact_X"
<<endl<<"		Choice from _transact_X.Состояние_транспортировки = ожидает  and"
<<endl<<"		_transact_X.Место_нахождения_будущее<>_transact_X.Место_нахождения "
<<endl<<"	first"
<<endl
<<endl<<"	Convert_rule"
<<endl<<"		Место_нахождения set  _transact_X.Место_нахождения_будущее"
<<endl
<<endl<<"	$End";


RDOfiles->pattern
<<endl<<"{----------------ГЕНЕРАЦИЯ если останавливается по времени-------------------------------------------}"
<<endl
<<endl<<"$Pattern  Блок_останова_моделирования_по_времени : irregular_event trace "
<<endl<<"$Relevant_resources"
<<endl<<"_parameter : Group_of_transacts_X  NoChange {нужен для того, что без него глючит}"
<<endl<<"$Time ="<< generate_time_MJ
<<endl<<"$Body"
<<endl<<"_parameter"
<<endl<<"$End";
}
