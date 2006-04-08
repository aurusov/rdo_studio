// rdoprocess_shape_process_MJ.cpp: implementation of the RPShapeProcessMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <list>
//#include "rdo_process.h"
#include "rdoprocess_shape_process_MJ.h"
#include "rdoprocess_shape_process_dlg1_MJ.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeProcessMJ::RPShapeProcessMJ( RPObject* _parent ):
	RPShape( _parent, _T("Process") )
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

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point( -50, 0 ), 180 ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0 ) );
	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in,  rp::point(  0, 25), 270 ) );
}

RPShapeProcessMJ::~RPShapeProcessMJ()
{
}

void RPShapeProcessMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeProcessDlg1_MJ dlg(NULL,this);
	dlg.DoModal();
//	rpapp.project().list_name();
}

void RPShapeProcessMJ::list_name()
{
	RPShape::list_name();
	TRACE( "*** Process ***\n" );
}


// // ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.fun
void RPShapeProcessMJ::define_rule()
{
		int cur = gtype;
		CString rule;
		switch(cur) // определяем активные окна исходя из закона
		{
	case 0: // константа 

	rpapp.RDOfiles->pattern<<gexp;

				break;	

	case 1: // нормальный
	
	rpapp.RDOfiles->pattern<<"Нормальный_закон_" <<getName().c_str()<<"("<<gexp<<","<<gdisp<<")";
		

	rpapp.RDOfiles->function
	<<endl<<endl<<endl<<"$Sequence Нормальный_закон_" <<getName().c_str()<<" : real "
	<<endl<<"$Type = normal "<<base_gen
	<<endl<<"$End";
						break;
						
	case 2: // равномерный закон
	
		
	rpapp.RDOfiles->pattern
	<<"Равномерный_закон_" <<getName().c_str()<<"("<<gmin<<","<<gmax<<")";
	
	rpapp.RDOfiles->function
	<<endl<<endl<<endl<<"$Sequence Равномерный_закон_" <<getName().c_str()<<" : real "
	<<endl<<"$Type = uniform "<<base_gen
	<<endl<<"$End";
			break;
	case 3: // экспоненциальный
	rpapp.RDOfiles->pattern
	<<"Экспонинциальный_закон_" <<getName().c_str()<<"("<<gexp<<")";

	rpapp.RDOfiles->function
	<<endl<<endl<<endl<<"$Sequence Экспонинциальный_закон_" <<getName().c_str()<<" : real "
	<<endl<<"$Type = exponential "<<base_gen
	<<endl<<"$End";
						break;

		}


}


void RPShapeProcessMJ::generate_MJ()
{
	RPShape::generate_MJ();

	//заполняем лист названиями паттернов для *.opr
		CString name_value1("Блок_постановка_в_очередь_");
		CString name_value2(getName().c_str());
		CString name_value(name_value1 + name_value2);
		rpapp.project().list_pattern_names.push_back(name_value);

		name_value1 = "Блок_переход_из_очереди_к_процессу_";
		name_value2 = getName().c_str();
		name_value = name_value1 + name_value2;
		rpapp.project().list_pattern_names.push_back(name_value);

		name_value1 = "Блок_опперации_";
		name_value2 = getName().c_str();
		name_value = name_value1 + name_value2;
		rpapp.project().list_pattern_names.push_back(name_value);



	// ГЕНЕРАЦИЯ статистики РДО ФАЙЛ *.pmd
	rpapp.RDOfiles->statistic

	<<endl<<"Занятость_Блока_" <<getName().c_str()<<
	  ":  watch_state"<<" Block_process_"<<getName().c_str()<<".Состояние = занят"

	<<endl<<"Очередь_Блока_" <<getName().c_str()<<
	  ":  watch_par"<<" Queue_"<<getName().c_str()<<".Размер_очереди";



// ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.res
	rpapp.RDOfiles->resourse<<endl<<endl<<"{-------блок process ------" <<getName().c_str()<<"-------------------}" <<endl

	<<"Block_process_"<<getName().c_str()<<" : Block_process свободен"
	<<endl<<"Queue_"<<getName().c_str()<<": Queue 0 свободен {кол-во человек в очереди и состояния выходной очереди }";


// ГЕНЕРАЦИЯ паттерное РДО ФАЙЛ *.pat
rpapp.RDOfiles->pattern <<endl<<endl<<"{---------постановка в очекредь--------"<<getName().c_str()<<"-----------------------}"
<<endl

	<<endl<<"$Pattern Блок_постановка_в_очередь_"<<getName().c_str()<<" : rule {}trace"
	<<endl<<"$Relevant_resources"
	<<endl<<"	 _transact  : Group_of_transacts_X  Keep"
	<<endl<<"	_block     : Queue_"<<getName().c_str()<<" Keep"
	<<endl
	<<endl<<"$Body"
	<<endl<<"_transact"
	<<endl<<"Choice from   _transact.Состояние_транспортировки = ожидает and" 
	<<endl<<"	_transact.Место_нахождения = "<<getName().c_str()<<" and _transact.Место_нахождения_будущее = "<<getName().c_str()
	<<endl<<"	first"
	<<endl<<"Convert_rule "
	<<endl<<"	Состояние_транспортировки set в_очереди"
	<<endl	
	<<endl<<"_block"
	<<endl<<"Choice from 1=1"
	<<endl<<"first"
	<<endl<<"	Convert_rule"
	<<endl<<"	Размер_очереди set _block.Размер_очереди + 1"
	<<endl<<"$End"
	<<endl

	//------------------------------------------------
	
	<<endl<<"{---------освобождение из очереди------"<<getName().c_str()<<"---------------}"<<endl
	<<endl<<"$Pattern Блок_переход_из_очереди_к_процессу_"<<getName().c_str()<<" : rule {}trace"
	<<endl
	<<endl<<"$Relevant_resources"
	<<endl<<"	_transact  : Group_of_transacts_X  Keep"
	<<endl<<"	_block     : Queue_"<<getName().c_str()<<" Keep"
	<<endl<<"	_block_proc : Block_process_"<<getName().c_str()<<" NoChange";


	// перечисление рессурсов
	std::list<CString>::iterator it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	rpapp.RDOfiles->pattern <<endl
	<<endl<<"_resource_"<<(*it)<<" : Resource_"<<(*it)<<" NoChange";

	it++;
	}


	rpapp.RDOfiles->pattern <<endl

	<<endl<<"	$Body"
	<<endl<<"_transact"
	<<endl<<"	Choice from   _transact.Состояние_транспортировки = в_очереди and" 
	<<endl<<"	_transact.Место_нахождения = "<<getName().c_str()<<" and _transact.Место_нахождения_будущее = "<<getName().c_str()
	<<endl<<"		first"
	<<endl<<"	Convert_rule" 
	<<endl<<"		Состояние_транспортировки set в_очереди_выход"
	<<endl<<"		Место_нахождения_будущее  set "<<id_next.c_str()
	<<endl
	<<endl<<"_block"
	<<endl<<"	Choice from _block.Выход_очереди = свободен"
	<<endl<<"		first"
	<<endl<<"	Convert_rule"
	<<endl<<"		Размер_очереди set _block.Размер_очереди - 1"
	<<endl<<"		Выход_очереди set занят"
	<<endl
	<<endl<<"_block_proc"
	<<endl<<"	Choice from 1=1"
	<<endl<<"		first"
	<<endl;

	it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	rpapp.RDOfiles->pattern <<endl


	<<endl<<"_resource_"<<(*it) 
    <<endl<<"	Choice from" 
	<<endl<<"	_resource_"<<(*it)<<".Состояние=свободен"
	<<endl<<"		first";

	it++;
	}


rpapp.RDOfiles->pattern
	<<endl<<"$End"
	
	//----------------------------------------------------------------------------
		
	<<endl<<"{---------процесс-----"<<getName().c_str()<<"----------------}"<<endl



	<<endl<<"$Pattern Блок_опперации_"<<getName().c_str()<<" : operation  {срабатывание закона}trace"
	<<endl<<"	$Relevant_resources"
	<<endl<<"	{непосредственно сам БЛОК процесса}"
	<<endl<<"_block_queue     : Queue_"<<getName().c_str()<<" Keep NoChange"
	<<endl
	<<endl
	<<endl

	<<endl<<"	{тут перечислены релевантные ресурсы, которые стрелкой подходят снизу к "
	<<endl<<"	блоку, а если это просто блок задержуи то ненадо}"
	<<endl
	<<endl;

// перечисление рессурсов
 it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	rpapp.RDOfiles->pattern <<endl
	<<endl<<"_resource_"<<(*it)<<" : Resource_"<<(*it)<<" Keep Keep";

	it++;
	}



rpapp.RDOfiles->pattern <<endl
	<<endl<<"	{перечислить все группы транзактов которые создаются - ВРУЧЕУЮ ПОЛЬЗОВАТЕЛЕМ}"
	<<endl<<"_transact_X : Group_of_transacts_X Keep Keep"
	<<endl<<"_block : Block_process_"<<getName().c_str()<<" Keep Keep"
	<<endl
	<<"$Time = ";
	define_rule(); // функция будет создавать закон и записывать его также в *.fun
	rpapp.RDOfiles->pattern<<" {закон определяющий продолжительность действия}"
	<<endl
	<<endl
	<<endl<<"$Body"
	<<endl
	<<endl<<"_block_queue"
	<<endl<<"	Choice from _block_queue.Выход_очереди = занят "
	<<endl<<"		first"
	<<endl<<"	Convert_begin"
	<<endl<<"		Выход_очереди set свободен"
    <<endl
	<<endl
	  
	<<endl<<"_transact_X"	
	<<endl<<"	Choice from _transact_X.Состояние_транспортировки = в_очереди_выход and"
	<<endl<<"	_transact_X.Место_нахождения = "<<getName().c_str() 
	<<endl<<"		first"
	<<endl<<"	Convert_begin"
	
	
	<<endl<<"		Состояние_транспортировки set захвачен"
	<<endl<<"		Место_нахождения_будущее set "<<id_next.c_str()
	<<endl
	<<endl<<"	Convert_end"
	<<endl<<"		Состояние_транспортировки set ожидает"
	
	
	<<endl<<"{ТАКИМ ОБРАЗОМ ИДЕНТИЧНО НУЖНО ПРОЙТИСЬ ПО ВСЕМ РЕЛЛЕВАНТНЫМ РЕСУРСАМ}	"
	<<endl<<endl;




//описание рессурса
	it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	
	rpapp.RDOfiles->pattern <<endl


	<<endl<<"_resource_"<<(*it) 
    <<endl<<"	Choice from" 
	<<endl<<"	_resource_"<<(*it)<<".Состояние=свободен"
	<<endl<<"		first"
    <<endl<<"	Convert_begin"
	<<endl<<"		Колличество_транзактов set _resource_"<<(*it)<<".Колличество_транзактов + 1"
	<<endl<<"		Состояние              set" 
	<<endl<<"		fun_resource_"<<(*it)<<"(_resource_"<<(*it)<<".Колличество_транзактов,_resource_"<<(*it)<<".Состояние,"
	<<endl<<"		_resource_"<<(*it)<<".Максим_колличество)"
	<<endl
    <<endl<<"	Convert_end"
	<<endl<<"		Колличество_транзактов set _resource_"<<(*it)<<".Колличество_транзактов - 1"
	<<endl<<"		Состояние              set" 
	<<endl<<"		fun_resource_"<<(*it)<<"(_resource_"<<(*it)<<".Колличество_транзактов,_resource_"<<(*it)<<".Состояние,"
	<<endl<<"		_resource_"<<(*it)<<".Максим_колличество)";

	it++;
	}




rpapp.RDOfiles->pattern <<endl<<endl

	<<endl<<"_block"
	<<endl<<"  Choice from"
	<<endl<<"  1=1"
	<<endl<<"		first"
	<<endl<<"	Convert_begin"
	<<endl<<"		Состояние set занят" 
	
			/*
	fun_resource_"<<getName().c_str()<<"_seize(";


it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	it++;
	if(it==list_resource_procMJ.end())
	{
	it--;
		rpapp.RDOfiles->pattern <<endl
		<<endl<<"_resource_"<<(*it)<<".Состояние";
	}
	else
	{
	it--;
		rpapp.RDOfiles->pattern <<endl
		<<endl<<"_resource_"<<(*it)<<".Состояние,";
	}

	it++;
	}

    rpapp.RDOfiles->pattern <<")"<<endl


*/

	<<endl<<"Convert_end  "
    <<endl<<endl<<"		Состояние set свободен "
	/*
	fun_resource_"<<getName().c_str()<<"_seize(";


	it = list_resource_procMJ.begin();
	while( it != list_resource_procMJ.end() ) 
	{
	it++;
	if(it==list_resource_procMJ.end())
	{
	it--;
		rpapp.RDOfiles->pattern <<endl
		<<endl<<"_resource_"<<(*it)<<".Состояние";
	}
	else
	{
	it--;
		rpapp.RDOfiles->pattern <<endl
		<<endl<<"_resource_"<<(*it)<<".Состояние,";
	}

	it++;
	}

    rpapp.RDOfiles->pattern <<")"<<endl

  */
	<<endl<<"$End";


// ГЕНЕРАЦИЯ ФУНКЦИЙ РДО ФАЙЛ *.fun эта ф-ия проверяет свободен ли блок, еще в функц генерится сверху
rpapp.RDOfiles->function<<endl<<"{-------блок cretae ------" <<getName().c_str()<<"-------------------}" <<endl

	<<endl<<"$Function  fun_resource_"<<getName().c_str()<<"_seize : such_as Block_process.Состояние"
	<<endl<<"$Type = algorithmic"
	<<endl<<"$Parameters";
	
		it = list_resource_procMJ.begin();
		while( it != list_resource_procMJ.end() ) 
		{
			
	rpapp.RDOfiles->function 
	<<endl<<"		_resource_"<<(*it)<<": such_as Resource.Состояние";
		it++;
		}
	
	rpapp.RDOfiles->function 
	<<endl<<"	$Body";
	
		it = list_resource_procMJ.begin();
		while( it != list_resource_procMJ.end() ) 
		{
			
	rpapp.RDOfiles->function 
	<<endl<<"	Calculate_if _resource_"<<(*it)<<" = занят fun_resource_"<<getName().c_str()<<"_seize = занят";
		it++;
		}
	
	rpapp.RDOfiles->function 
	<<endl  
	<<endl<<"  Calculate_if 1=1 fun_resource_"<<getName().c_str()<<"_seize=свободен"
	<<endl
	<<endl<<"	$End";	




}