// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rdoprocess_shape_create_MJ.h"
#include "rdoprocess_shape_create_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"
#include <rdoprocess_shape.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RPShapeCreateMJ::RPShapeCreateMJ( RPObject* _parent ):
	RPShape_MJ( _parent, _T("_Create") )
{
	pa_src.push_back( rp::point(-50, -25) );
	pa_src.push_back( rp::point(25, -25) );
	pa_src.push_back( rp::point(50, 0) );
	pa_src.push_back( rp::point(25, 25) );
	pa_src.push_back( rp::point(-50, 25) );
	pa_src.push_back( rp::point(-50, -25) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  50, 0 ), 0, "transact" ) );

	// инициализация параметров для генерирования
	gname; // имя
	 gfirst=0; // время первого
	gamount=1000000; // кол-во создаваемых
	gtype=0; // закон прибытия
	base_gen=1234567890;
	//атрибуты законов
	gexp=0;
	gdisp=0;
	gmax=0;
	gmin=0;

	//второе окно
	inf=1000000; // бесконечноть
	gID=1; // ID группы
	gpar1=0;
	gpar2=0;
	gpar3=0;

}

RPShapeCreateMJ::~RPShapeCreateMJ()
{
}

rp::RPXMLNode* RPShapeCreateMJ::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

RPObject* RPShapeCreateMJ::newObject( RPObject* parent )
{
	return new RPShapeCreateMJ( parent );
}

void RPShapeCreateMJ::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

/*void RPShapeCreateMJ::list_name()
{
	RPShape_MJ::list_name();
	TRACE( "*** Create ***\n" );
}
*/


void RPShapeCreateMJ::generate()
{
/*
RDOfiles->function <<"имя следующего блока - "<<id_next
<<std::endl<<"имя - "<<getName().c_str()
<<std::endl<<"время первого - "<<gfirst
<<std::endl<< "кол-во создаваемых - "<<gamount
<<std::endl<<"закон прибытия - "<<gtype
<<std::endl<<"база генератора - "<<base_gen
<<std::endl<<"атрибуты закона - "<<"exp - "<<gexp<<" disp"<<gdisp<<" max - " <<gmax<<" min - "<<gmin
<<std::endl<<"второе окно! бесконечноть - "<<inf
<<std::endl<<"ID группы - "<<gID
<<std::endl<<"параметры - "<<"1-  "<<gpar1<< " 2 - "<<gpar2<<" 3 - "<<gpar3;
*/

CString name_value1("Блок_создания_транзактов_");
CString name_value2(getName().c_str());
CString name_value(name_value1 + name_value2);
proc2rdo->list_pattern_names.push_back(name_value);


// ГЕНЕРАЦИЯ ресурсов РДО ФАЙЛ *.res
	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;
	RDOfiles->resourse<<std::endl<<std::endl<<"{-------блок cretae ------" <<getName().c_str()<<"-------------------}" <<std::endl

	<<"Create_"<<getName().c_str()<<" : Creates false 1";



// ГЕНЕРАЦИЯ паттернов РДО ФАЙЛ *.pat

	RDOfiles->pattern <<std::endl<<"{-------блок cretae ------" <<getName().c_str()<<"-------------------}" <<std::endl


	<<std::endl<<"$Pattern  Блок_создания_транзактов_" <<getName().c_str()<<" : irregular_event trace"
	<<std::endl<<"$Relevant_resources"
	<<std::endl<<"_parameter : Create_"<<getName().c_str()<<" Keep"
	<<std::endl<<" _transact  : Group_of_transacts_X  Create"
    <<std::endl<<"$Time = fun_"<<getName().c_str()<<"(_parameter.par_1, _parameter.par_amount)" 
	<<std::endl<<"$Body"
	<<std::endl<<"_parameter"
	<<std::endl<<" Convert_event"
	<<std::endl<<"par_1      set true {говорит о том , что это уже не первый}"
	<<std::endl<<"par_amount set _parameter.par_amount + 1" 
	<<std::endl	
	<<std::endl<<"_transact"
	<<std::endl<<"    Convert_event" 
	<<std::endl	 
	<<std::endl	 
	<<std::endl<<"		Место_нахождения            set "<<getName().c_str()<<" {ID создающего блока}"
	<<std::endl<<"		Место_нахождения_будущее    set "<<id_next.c_str()          <<" {ID куда идет коннектор}"
	
	
    //Место_в_очереди set 0
	//Состояние_транзакта         set 0
	
	<<std::endl<<"		Идентификационный_номер_транзакта set (_parameter.par_amount - 1)"
    <<std::endl<<"		Идентификационный_номер_группы_транзактов set "<<getName().c_str()<<" {ID создающего блока}"
	<<std::endl<<"		Состояние_транспортировки set ожидает	"
	<<std::endl<<"		Состояние_транзакта set 0"
	<<std::endl<<"		Место_в_очереди     set 0"
	<<std::endl<<"$End";







// ГЕНЕРАЦИЯ ФУНКЦИЙ РДО ФАЙЛ *.fun
// генерация закона



	RDOfiles->function<<std::endl<<"{-------блок cretae ------" <<getName().c_str()<<"-------------------}" <<std::endl;

int cur = gtype;
CString rule;
	switch(cur) // определяем активные окна исходя из закона
	{
case 0: // константа 
			break;	

case 1: // нормальный

	RDOfiles->function

	<<std::endl<<"$Sequence Нормальный_закон_" <<getName().c_str()<<" : real "
	<<std::endl<<"$Type = normal "<<base_gen
	<<std::endl<<"$End";

			break;
case 2: // равномерный закон
	RDOfiles->function
	<<std::endl<<"$Sequence Равномерный_закон_" <<getName().c_str()<<" : real "
	<<std::endl<<"$Type = uniform "<<base_gen
	<<std::endl<<"$End";
			break;
case 3: // экспоненциальный
	RDOfiles->function
	<<std::endl<<"$Sequence Экспонинциальный_закон_" <<getName().c_str()<<" : real "
	<<std::endl<<"$Type = exponential "<<base_gen
	<<std::endl<<"$End";
			break;
	}
// генерация ф-ии которая определяет время возвращаемое, когда будет создаваться транзакт
RDOfiles->function<<std::endl
<<std::endl<<"$Function  fun_"<<getName().c_str()<<" : integer"
<<std::endl<<"$Type = algorithmic"
<<std::endl<<"$Parameters"
<<std::endl<<" _par        : such_as Creates.par_1 {проверяет первый или нет}"
<<std::endl<<"  _par_amount : integer {проверка не перешёл ли максим}"
<<std::endl<<"$Body"
<<std::endl<<"  Calculate_if _par_amount > "<<gamount<<"{колличество создаваемых танзактов}  fun_"<<getName().c_str()<<"="<<inf
<<std::endl 
<<std::endl<<"  Calculate_if _par = false and _par_amount <= "<<gamount<<"{колличество создаваемых танзактов} "
<<std::endl<<"  fun_"<<getName().c_str()<<"="<<gfirst<<"{время появление первого транзакта}"
<<std::endl  
<<std::endl<<"  Calculate_if _par = true and _par_amount <= "<<gamount<<"{колличество создаваемых танзактов} "
<<std::endl<<"  fun_"<<getName().c_str()<<"=";

// добовляем закон
 cur = gtype;

	switch(cur) // определяем активные окна исходя из закона
	{
case 0: // константа 

	RDOfiles->function<<gexp<<" {закон появления транзактов}"
	<<std::endl<<"$End";
			break;	

case 1: // нормальный

	RDOfiles->function
	<<"Нормальный_закон_" <<getName().c_str()<<"("<<gexp<<","<<gdisp<<") {закон появления транзактов}"
	<<std::endl
	<<std::endl<<"$End";

			break;
case 2: // равномерный закон
	RDOfiles->function
	<<"Равномерный_закон_" <<getName().c_str()<<"("<<gmin<<","<<gmax<<") {закон появления транзактов}"
	<<std::endl
	<<std::endl<<"$End";

			break;
case 3: // экспоненциальный
	RDOfiles->function
	<<"Экспонинциальный_закон_" <<getName().c_str()<<"("<<gexp<<") {закон появления транзактов}"
	<<std::endl
	<<std::endl<<"$End";

			break;



	}
}