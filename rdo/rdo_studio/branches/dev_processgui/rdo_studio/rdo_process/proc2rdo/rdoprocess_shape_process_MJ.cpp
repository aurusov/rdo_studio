// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <list>
#include "rdoprocess_shape_process_MJ.h"
#include "rdoprocess_shape_process_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"

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
	
	gname; // имя
	
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
	RPShapeProcessDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
//	project->list_name();
}

void RPShapeProcessMJ::list_name()
{
	TRACE( "*** Process ***\n" );
}


// // ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.fun
void RPShapeProcessMJ::define_rule()
{
	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;
		int cur = gtype;
		CString rule;
		switch(cur) // определяем активные окна исходя из закона
		{
	case 0: // константа 

	RDOfiles->pattern<<gexp;

				break;	

	case 1: // нормальный
	
	RDOfiles->pattern<<"Нормальный_закон_" <<getName().c_str()<<"("<<gexp<<","<<gdisp<<")";
		

	RDOfiles->function
	<<std::endl<<std::endl<<std::endl<<"$Sequence Нормальный_закон_" <<getName().c_str()<<" : real "
	<<std::endl<<"$Type = normal "<<base_gen
	<<std::endl<<"$End";
						break;
						
	case 2: // равномерный закон
	
		
	RDOfiles->pattern
	<<"Равномерный_закон_" <<getName().c_str()<<"("<<gmin<<","<<gmax<<")";
	
	RDOfiles->function
	<<std::endl<<std::endl<<std::endl<<"$Sequence Равномерный_закон_" <<getName().c_str()<<" : real "
	<<std::endl<<"$Type = uniform "<<base_gen
	<<std::endl<<"$End";
			break;
	case 3: // экспоненциальный
	RDOfiles->pattern
	<<"Экспонинциальный_закон_" <<getName().c_str()<<"("<<gexp<<")";

	RDOfiles->function
	<<std::endl<<std::endl<<std::endl<<"$Sequence Экспонинциальный_закон_" <<getName().c_str()<<" : real "
	<<std::endl<<"$Type = exponential "<<base_gen
	<<std::endl<<"$End";
						break;

		}


}

void RPShapeProcessMJ::generate()
{
	RPShape_MJ::generate();

	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;

	//заполняем лист названиями паттернов для *.opr
		CString name_value1("Блок_постановка_в_очередь_");
		CString name_value2(getName().c_str());
		CString name_value(name_value1 + name_value2);
		proc2rdo->list_pattern_names.push_back(name_value);

		name_value1 = "Блок_переход_из_очереди_к_процессу_";
		name_value2 = getName().c_str();
		name_value = name_value1 + name_value2;
		proc2rdo->list_pattern_names.push_back(name_value);

		name_value1 = "Блок_опперации_";
		name_value2 = getName().c_str();
		name_value = name_value1 + name_value2;
		proc2rdo->list_pattern_names.push_back(name_value);



	// ГЕНЕРАЦИЯ статистики РДО ФАЙЛ *.pmd
	RDOfiles->statistic

	<<std::endl<<"Занятость_Блока_" <<getName().c_str()<<
	  ":  watch_state"<<" Block_process_"<<getName().c_str()<<".Состояние = занят"

	<<std::endl<<"Очередь_Блока_" <<getName().c_str()<<
	  ":  watch_par"<<" Queue_"<<getName().c_str()<<".Размер_очереди";



// ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.res
	RDOfiles->resourse<<std::endl<<std::endl<<"{-------блок process ------" <<getName().c_str()<<"-------------------}" <<std::endl

	<<"Block_process_"<<getName().c_str()<<" : Block_process свободен"
	<<std::endl<<"Queue_"<<getName().c_str()<<": Queue trace 0 свободен {кол-во человек в очереди и состояния выходной очереди }";


// ГЕНЕРАЦИЯ паттерное РДО ФАЙЛ *.pat
RDOfiles->pattern <<std::endl<<std::endl<<"{---------постановка в очекредь--------"<<getName().c_str()<<"-----------------------}"
<<std::endl

	<<std::endl<<"$Pattern Блок_постановка_в_очередь_"<<getName().c_str()<<" : rule {}trace"
	<<std::endl<<"$Relevant_resources"
	<<std::endl<<"	 _transact  : Group_of_transacts_X  Keep"
	<<std::endl<<"	_block     : Queue_"<<getName().c_str()<<" Keep"
	<<std::endl
	<<std::endl<<"$Body"
	<<std::endl<<"_transact"
	<<std::endl<<"Choice from   _transact.Состояние_транспортировки = ожидает and" 
	<<std::endl<<"	_transact.Место_нахождения = "<<getName().c_str()<<" and _transact.Место_нахождения_будущее = "<<getName().c_str()
	<<std::endl<<"	first"
	<<std::endl<<"Convert_rule "
	<<std::endl<<"	Состояние_транспортировки set в_очереди"
	<<std::endl	
	<<std::endl<<"_block"
	<<std::endl<<"Choice from 1=1"
	<<std::endl<<"first"
	<<std::endl<<"	Convert_rule"
	<<std::endl<<"	Размер_очереди set _block.Размер_очереди + 1"
	<<std::endl<<"$End"
	<<std::endl

	//------------------------------------------------
	
	<<std::endl<<"{---------освобождение из очереди------"<<getName().c_str()<<"---------------}"<<std::endl
	<<std::endl<<"$Pattern Блок_переход_из_очереди_к_процессу_"<<getName().c_str()<<" : rule {}trace"
	<<std::endl
	<<std::endl<<"$Relevant_resources"
	<<std::endl<<"	_transact  : Group_of_transacts_X  Keep"
	<<std::endl<<"	_block     : Queue_"<<getName().c_str()<<" Keep"
	<<std::endl<<"	_block_proc : Block_process_"<<getName().c_str()<<" NoChange";


	// перечисление рессурсов
	std::list<CString>::iterator it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	RDOfiles->pattern <<std::endl
	<<std::endl<<"_resource_"<<(*it)<<" : Resource_"<<(*it)<<" NoChange";

	it++;
	}


	RDOfiles->pattern <<std::endl

	<<std::endl<<"	$Body"
	<<std::endl<<"_transact"
	<<std::endl<<"	Choice from   _transact.Состояние_транспортировки = в_очереди and" 
	<<std::endl<<"	_transact.Место_нахождения = "<<getName().c_str()<<" and _transact.Место_нахождения_будущее = "<<getName().c_str()
	<<std::endl<<"		first"
	<<std::endl<<"	Convert_rule" 
	<<std::endl<<"		Состояние_транспортировки set в_очереди_выход"
	<<std::endl<<"		Место_нахождения_будущее set "<<id_next.c_str()
	<<std::endl
	<<std::endl<<"_block"
	<<std::endl<<"	Choice from _block.Выход_очереди = свободен"
	<<std::endl<<"		first"
	<<std::endl<<"	Convert_rule"
	<<std::endl<<"		Размер_очереди set _block.Размер_очереди - 1"
	<<std::endl<<"		Выход_очереди set занят"
	<<std::endl
	<<std::endl<<"_block_proc"
	<<std::endl<<"	Choice from 1=1"
	<<std::endl<<"		first"
	<<std::endl;

	it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	RDOfiles->pattern <<std::endl


	<<std::endl<<"_resource_"<<(*it) 
    <<std::endl<<"	Choice from" 
	<<std::endl<<"	_resource_"<<(*it)<<".Состояние=свободен"
	<<std::endl<<"		first";

	it++;
	}


RDOfiles->pattern
	<<std::endl<<"$End"
	
	//----------------------------------------------------------------------------
		
	<<std::endl<<"{---------процесс-----"<<getName().c_str()<<"----------------}"<<std::endl



	<<std::endl<<"$Pattern Блок_опперации_"<<getName().c_str()<<" : operation  {срабатывание закона}trace"
	<<std::endl<<"	$Relevant_resources"
	<<std::endl<<"	{непосредственно сам БЛОК процесса}"
	<<std::endl<<"_block_queue     : Queue_"<<getName().c_str()<<" Keep NoChange"
	<<std::endl
	<<std::endl
	<<std::endl

	<<std::endl<<"	{тут перечислены релевантные ресурсы, которые стрелкой подходят снизу к "
	<<std::endl<<"	блоку, а если это просто блок задержуи то ненадо}"
	<<std::endl
	<<std::endl;

// перечисление рессурсов
 it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	RDOfiles->pattern <<std::endl
	<<std::endl<<"_resource_"<<(*it)<<" : Resource_"<<(*it)<<" Keep Keep";

	it++;
	}



RDOfiles->pattern <<std::endl
	<<std::endl<<"	{перечислить все группы транзактов которые создаются - ВРУЧЕУЮ ПОЛЬЗОВАТЕЛЕМ}"
	<<std::endl<<"_transact_X : Group_of_transacts_X Keep Keep"
	<<std::endl<<"_block : Block_process_"<<getName().c_str()<<" Keep Keep"
	<<std::endl
	<<"$Time = ";
	define_rule(); // функция будет создавать закон и записывать его также в *.fun
	RDOfiles->pattern<<" {закон определяющий продолжительность действия}"
	<<std::endl
	<<std::endl
	<<std::endl<<"$Body"
	<<std::endl
	<<std::endl<<"_block_queue"
	<<std::endl<<"	Choice from _block_queue.Выход_очереди = занят "
	<<std::endl<<"		first"
	<<std::endl<<"	Convert_begin"
	<<std::endl<<"		Выход_очереди set свободен"
    <<std::endl
	<<std::endl;
	  
//описание рессурса
	it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	RDOfiles->pattern <<std::endl


	<<std::endl<<"_resource_"<<(*it) 
    <<std::endl<<"	Choice from" 
	<<std::endl<<"	_resource_"<<(*it)<<".Состояние=свободен"
	<<std::endl<<"		first"
    <<std::endl<<"	Convert_begin"
	<<std::endl<<"		Колличество_транзактов set _resource_"<<(*it)<<".Колличество_транзактов + 1"
	<<std::endl<<"		Состояние              set" 
	<<std::endl<<"		fun_resource_"<<(*it)<<"(_resource_"<<(*it)<<".Колличество_транзактов,_resource_"<<(*it)<<".Состояние,"
	<<std::endl<<"		_resource_"<<(*it)<<".Максим_колличество)"
	<<std::endl
    <<std::endl<<"	Convert_end"
	<<std::endl<<"		Колличество_транзактов set _resource_"<<(*it)<<".Колличество_транзактов - 1"
	<<std::endl<<"		Состояние              set" 
	<<std::endl<<"		fun_resource_"<<(*it)<<"(_resource_"<<(*it)<<".Колличество_транзактов,_resource_"<<(*it)<<".Состояние,"
	<<std::endl<<"		_resource_"<<(*it)<<".Максим_колличество)";

	it++;
	}

	RDOfiles->pattern <<std::endl
	<<std::endl<<"_transact_X"	
	<<std::endl<<"	Choice from _transact_X.Состояние_транспортировки = в_очереди_выход and"
	<<std::endl<<"	_transact_X.Место_нахождения = "<<getName().c_str() 
	<<std::endl<<"		first"
	<<std::endl<<"	Convert_begin"
	
	
	<<std::endl<<"		Состояние_транспортировки set захвачен"
	<<std::endl<<"		Место_нахождения_будущее set "<<id_next.c_str()
	<<std::endl
	<<std::endl<<"	Convert_end"
	<<std::endl<<"		Состояние_транспортировки set ожидает"
	
	
	<<std::endl<<"{ТАКИМ ОБРАЗОМ ИДЕНТИЧНО НУЖНО ПРОЙТИСЬ ПО ВСЕМ РЕЛЛЕВАНТНЫМ РЕСУРСАМ}	"
	<<std::endl<<std::endl;

RDOfiles->pattern <<std::endl<<std::endl

	<<std::endl<<"_block"
	<<std::endl<<"  Choice from"
	<<std::endl<<"  1=1"
	<<std::endl<<"		first"
	<<std::endl<<"	Convert_begin"
	<<std::endl<<"		Состояние set занят" 
	
			/*
	fun_resource_"<<getName().c_str()<<"_seize(";


it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	it++;
	if(it==list_resource_procMJ.end())
	{
	it--;
		RDOfiles->pattern <<std::endl
		<<std::endl<<"_resource_"<<(*it)<<".Состояние";
	}
	else
	{
	it--;
		RDOfiles->pattern <<std::endl
		<<std::endl<<"_resource_"<<(*it)<<".Состояние,";
	}

	it++;
	}

    RDOfiles->pattern <<")"<<std::endl


*/

	<<std::endl<<"Convert_end  "
    <<std::endl<<std::endl<<"		Состояние set свободен "
	/*
	fun_resource_"<<getName().c_str()<<"_seize(";


	it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	it++;
	if(it==list_resource_procMJ.end())
	{
	it--;
		RDOfiles->pattern <<std::endl
		<<std::endl<<"_resource_"<<(*it)<<".Состояние";
	}
	else
	{
	it--;
		RDOfiles->pattern <<std::endl
		<<std::endl<<"_resource_"<<(*it)<<".Состояние,";
	}

	it++;
	}

    RDOfiles->pattern <<")"<<std::endl

  */
	<<std::endl<<"$End";


// ГЕНЕРАЦИЯ ФУНКЦИЙ РДО ФАЙЛ *.fun эта ф-ия проверяет свободен ли блок, еще в функц генерится сверху
RDOfiles->function<<std::endl<<"{-------блок cretae ------" <<getName().c_str()<<"-------------------}" <<std::endl

	<<std::endl<<"$Function  fun_resource_"<<getName().c_str()<<"_seize : such_as Block_process.Состояние"
	<<std::endl<<"$Type = algorithmic"
	<<std::endl<<"$Parameters";
	
		it = list_resource_procMJ.begin();
		while( it != list_resource_procMJ.end() ) 
		{
			
	RDOfiles->function 
	<<std::endl<<"		_resource_"<<(*it)<<": such_as Resource.Состояние";
		it++;
		}
	
	RDOfiles->function 
	<<std::endl<<"	$Body";
	
		it = list_resource_procMJ.begin();
		while( it != list_resource_procMJ.end() ) 
		{
			
	RDOfiles->function 
	<<std::endl<<"	Calculate_if _resource_"<<(*it)<<" = занят fun_resource_"<<getName().c_str()<<"_seize = занят";
		it++;
		}
	
	RDOfiles->function 
	<<std::endl  
	<<std::endl<<"  Calculate_if 1=1 fun_resource_"<<getName().c_str()<<"_seize=свободен"
	<<std::endl
	<<std::endl<<"	$End";	




}