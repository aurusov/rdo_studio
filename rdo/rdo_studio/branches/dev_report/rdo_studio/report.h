#if !defined (REPORT_H)
#define REPORT_H

#define _ATL_ATTRIBUTES
#include <atlbase.h>
#include <atlcom.h>
#include <string>
#include <vector>

// Подключаем Fastreports
#if _MSC_VER < 1300
//////////////////////////////////////////////////////////////////////////
	//
	// Use this for MS Visual Studio 6
	//
	//////////////////////////////////////////////////////////////////////////
	#import "..\\..\\..\\bin\\FastReport3.dll" named_guids
	#import "..\\..\\..\\frxCOM\\frxCOM.dll" named_guids

#else
	//////////////////////////////////////////////////////////////////////////
	//
	// This code preffered for MS Visual Studio.NET
	//
	//////////////////////////////////////////////////////////////////////////
#import "libid:d3c6fb9b-9edf-48f3-9a02-6d8320eaa9f5" named_guids
#endif

using namespace FastReport;
const int ReportParserKeyWordsQuantity = 9;
const std::string ReportParserKeyWords[] = 
{
	"//#group", 
	"//#endgroup",
	"//#chart",
	"//#endchart",
	"watch_state",      
	"watch_par",        
	"get_value",		
	"watch_quant",      
	"watch_value"
};

class ReportParser
{
private: 
	std::string StringToParse;  
	enum DeleteCommentsFlagType { DC_NOCOMMENT, DC_LINECOMMENT, DC_LONGCOMMENT, DC_ENDLINECOMMENT, DC_ENDLONGCOMMENT };
	enum ReportParserStatus { RPS_COMMENTSNOTDELETED, RPS_COMMENTSDELETED, RPS_LISTCREATED };
public:  
	enum ReportParserKeyWordType { RPKWT_GROUP, RPKWT_ENDGROUP, RPKWT_CHART, RPKWT_ENDCHART, RPKWT_WATCHSTATE, RPKWT_WATCHPAR, RPKWT_GETVALUE, RPKWT_WATCHQUANT, RPKWT_WATCHVALUE, RPKWT_END };
	// Статус парсера     
	ReportParserStatus ParserStatus;     
	// Строка не содержащая комментариев
	std::string StringToParse_wo_Comments; 
	// Список слов, которые содержит строка
	std::vector <std::string> ListToParse;
	// Тип текущего ключевого слова
	ReportParserKeyWordType KeyWordType;
	// Текущее слово
	std::string CurrentWord;
	// Индекс текущего слова в списке
	int CurrentWordIndex;
	// Удаление комментариев
	int DeleteComments();
	// Функция преобразования строку в список слов
	int StringToList();
	// Функция поиска ключевых слов
	int FindNextKeyWord();
	// Конструктор класса
	ReportParser( std::string UserString );
};

// Класс описывающий показатель моделирования
class ReportVar
{
public:
	// Имя показателя
	std::string Name;
	// Тип показателя
	enum VarTypeValues { WATCH_STATE = 4, WATCH_PAR = 5, GET_VALUE = 6, WATCH_QUANT = 7, WATCH_VALUE = 8 };
	VarTypeValues VarType;
	// Список параметров собранных по показателю
	std::vector <std::string> Parameters;
};

// Класс описывающий группу показателей
class Group
{
public:
	// Имя группы
	std::string GroupName;
	// Список переменных, образующих группу
	std::vector < ReportVar > Variables;
};

// Класс описывающий график
class Chart
{
public:
	// Имя графика
	std::string ChartName;
	// Список переменных, образующих график
	std::vector < ReportVar > Variables;
};

// Класс, описывающий работу с группами
class GroupManager
{
public:
	// Список с результатами моделирования
	std::vector <std::string> List;
	// Список сущностей (групп или графиков) входящих в отчет
	std::vector < Group > GroupsVector;
	// Список указывающий на активные сущности
	std::vector < bool > ActivityPattern;
	// Функция добавляющая новую группу к списку
	int AddGroup (std::string GroupName_, bool Activity );
	// Функция добавляющая переменную в группу
	int InsertVar( std::string VarName, ReportVar::VarTypeValues VarType_ );
	// Функция сопоставляющая показателям переменные их характеризующие
	int GetParam();
};

// Класс, описывающий работу с графиками
class ChartManager
{
public:
	// Список с результатами моделирования
	std::vector <std::string> List;
	// Список сущностей (групп или графиков) входящих в отчет
	std::vector < Chart > ChartsVector;
	// Список указывающий на активные сущности
	std::vector < bool > ActivityPattern;
	// Функция добавляющая новую группу к списку
	int AddChart (std::string ChartName_, bool Activity );
	// Функция добавляющая переменную в группу
	int InsertVar( std::string VarName, ReportVar::VarTypeValues VarType_ );
	// Функция сопоставляющая показателям переменные их характеризующие
	int GetParam();
};

// Обертка к датасету для вывода данных
class CfrxGroupsDataSetEvents : public CComObjectRoot,public IDispatchImpl< IfrxUserDataSetEvents, &__uuidof(IfrxUserDataSetEvents), &LIBID_FastReport>
{
	int idx;
	int* Master_Detail_counter;
	GroupManager Groups_output;
	DWORD dwCookie;

	BEGIN_COM_MAP(CfrxGroupsDataSetEvents)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst ( ) 
	{ 
		idx = 0;
	    return S_OK;
	}
	STDMETHODIMP raw_OnNext ( ) 
	{ 
	    idx++;	
		return S_OK; 
	}
    STDMETHODIMP raw_OnPrior( ) 
	{ 
		idx--;
		return S_OK; 
	}
	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT * Value ) 
	{ 
		if (_variant_t("GroupName") == ColumnName) 
		{			
			_variant_t		v(Groups_output.GroupsVector.at(idx).GroupName.c_str());
			VariantCopy(Value, &v);
			v.Clear();				
		    // Сообщить дочернему датасету, что счетчик увеличился на 1
			(*Master_Detail_counter)++;
		}	
		return S_OK;
	}
    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
		*IsEOF = ! ( idx < ( int ) Groups_output.GroupsVector.size() );
		return S_OK; 
	}
public:
	HRESULT Advise(IfrxUserDataSet*	GroupsDataSet, const GroupManager& GroupManagerBuffer, int *counter)
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		Master_Detail_counter = counter;
		Groups_output = GroupManagerBuffer;
		do {
			hr = GroupsDataSet->QueryInterface( __uuidof( IConnectionPointContainer ), ( void** )&pCPC );
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint( __uuidof(IfrxUserDataSetEvents), &pCP );
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Advise( pUnk, &dwCookie);
			if( FAILED( hr ) ) break;
		} while( false );
		return hr;
	}
	HRESULT Unadvise(IfrxUserDataSet* GroupsDataSet)
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		do 
		{
			hr = GroupsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Unadvise( dwCookie);
			if( FAILED( hr ) ) break;
		}
		while(false);
		return hr;
	}
}; 

class CfrxVarsDataSetEvents : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int*  Master_Detail_counter ;
	GroupManager  Groups_output ; 

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxVarsDataSetEvents)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst () 
	{ 
		idx = 0;
	    return S_OK; 
	}
	STDMETHODIMP raw_OnNext () 
	{ 
	    idx++;
		return S_OK; 
	}
    STDMETHODIMP raw_OnPrior() 
	{ 
		idx--;
		return S_OK; 
	}
	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT*  Value ) 
	{ 
		if ( _variant_t( "VarName" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Name.c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_1" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(0).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_2" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(1).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_3" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(2).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_4" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(3).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_5" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(4).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
		if ( _variant_t( "Par_6" ) == ColumnName ) 
		{
			_variant_t		v( Groups_output.GroupsVector.at( *Master_Detail_counter ).Variables.at( idx ).Parameters.at(5).c_str() );
			VariantCopy( Value, &v );
			v.Clear();	
			return S_OK;
		}
	}
    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			if ( *Master_Detail_counter == -1 )
				*IsEOF = ! ( idx < ( int ) Groups_output.GroupsVector.at(0).Variables.size()) ;
			else
		    *IsEOF = ! ( idx < ( int ) Groups_output.GroupsVector.at(*Master_Detail_counter).Variables.size()) ;
			return S_OK ; 
	}
public:
	HRESULT Advise(IfrxUserDataSet*	GroupsDataSet, const GroupManager& GroupManagerBuffer, int *counter)
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		Master_Detail_counter = counter ;
		Groups_output = GroupManagerBuffer ;
		do {
			hr = GroupsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Advise( pUnk, &dwCookie);
			if( FAILED( hr ) ) break;
		} while(false);
		return hr;
	}

	HRESULT Unadvise(IfrxUserDataSet* GroupsDataSet)
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		do {
			hr = GroupsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Unadvise( dwCookie);
			if( FAILED( hr ) ) break;
		} while(false);
		return hr;
	}
}; 

// Обертка к датасету для вывода данных
class CfrxChartsNamesDataSetEvents : public CComObjectRoot, public IDispatchImpl< IfrxUserDataSetEvents, &__uuidof(IfrxUserDataSetEvents), &LIBID_FastReport>
{
	int	idx;
	int* Master_Detail_counter;
	ChartManager Charts_output;
	DWORD dwCookie;

	BEGIN_COM_MAP(CfrxGroupsDataSetEvents)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst ( ) 
	{ 
		idx = 0;
	    return S_OK;
	}
	STDMETHODIMP raw_OnNext ( ) 
	{ 
	    idx++;	
		return S_OK; 
	}
    STDMETHODIMP raw_OnPrior( ) 
	{ 
		idx--;
		return S_OK; 
	}
	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT * Value ) 
	{ 
		if (_variant_t("ChartName") == ColumnName) 
		{		
			_variant_t		v(Charts_output.ChartsVector.at(idx).ChartName.c_str());
			VariantCopy(Value, &v);
			v.Clear();
			(*Master_Detail_counter)++;
		}	
		return S_OK;
	}
    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
		*IsEOF = ! ( idx < ( int ) Charts_output.ChartsVector.size() );
		return S_OK; 
	}
public:
	HRESULT Advise(IfrxUserDataSet*	ChartsNamesDataSet, const ChartManager& ChartManagerBuffer, int* counter )
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		Master_Detail_counter = counter;
		Charts_output = ChartManagerBuffer;
		do {
			hr = ChartsNamesDataSet->QueryInterface( __uuidof( IConnectionPointContainer ), ( void** )&pCPC );
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint( __uuidof(IfrxUserDataSetEvents), &pCP );
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Advise( pUnk, &dwCookie);
			if( FAILED( hr ) ) break;
		} while( false );
		return hr;
	}
	HRESULT Unadvise(IfrxUserDataSet* ChartsNamesDataSet)
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		do 
		{
			hr = ChartsNamesDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Unadvise( dwCookie);
			if( FAILED( hr ) ) break;
		}
		while( false );
		return hr;
	}
}; 

class CfrxChartsVarsDataSetEvents :	public CComObjectRoot, public IDispatchImpl< IfrxUserDataSetEvents, &__uuidof(IfrxUserDataSetEvents), &LIBID_FastReport>
{
	int idx, i ;
	ChartManager Charts_output ;
	int*   Master_Detail_counter;
	DWORD  dwCookie ;

	BEGIN_COM_MAP(CfrxChartsVarsDataSetEvents)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IfrxUserDataSetEvents)
	END_COM_MAP()

    STDMETHODIMP raw_OnFirst ( ) 
	{ 
		idx = 0 ;
	    return S_OK ; 
	}
	STDMETHODIMP raw_OnNext ( ) 
	{ 
	    idx++ ;
		return S_OK; 
	}
    STDMETHODIMP raw_OnPrior( ) 
	{ 
		idx-- ;
		return S_OK ; 
	}
	STDMETHODIMP raw_OnGetValue ( VARIANT ColumnName, VARIANT * Value ) 
	{ 
		if ( _variant_t("Name") == ColumnName ) 
		{		
			_variant_t		v( Charts_output.ChartsVector.at(*Master_Detail_counter).Variables.at( idx ).Name.c_str() );
			VariantCopy( Value, &v );
			v.Clear();				
			return S_OK;
		}
		if ( _variant_t("Var") == ColumnName ) 
		{		
			_variant_t		v( Charts_output.ChartsVector.at(*Master_Detail_counter).Variables.at( idx ).Parameters.at(2).c_str() );
			VariantCopy( Value, &v );
			v.Clear();				
			return S_OK;
		}
	}
    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			if ( *Master_Detail_counter == -1 )
				*IsEOF = ! ( idx < ( int ) Charts_output.ChartsVector.at(0).Variables.size()) ;
			else
				*IsEOF = ! ( idx < ( int ) Charts_output.ChartsVector.at(*Master_Detail_counter).Variables.size()) ;
			return S_OK ; 	
	}
public:
	HRESULT Advise(IfrxUserDataSet*	ChartsVarsDataSet, const ChartManager& ChartManagerBuffer, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		Master_Detail_counter = counter;
		Charts_output = ChartManagerBuffer;
		do {
			hr = ChartsVarsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Advise( pUnk, &dwCookie);
			if( FAILED( hr ) ) break;
		} while(false);
		return hr;
	}
	HRESULT Unadvise(IfrxUserDataSet* ChartsVarsDataSet)
	{
		HRESULT hr;
		IConnectionPointContainerPtr pCPC;
		IConnectionPointPtr pCP;
		CComPtr<IUnknown> pUnk;
		
		do {
			hr = ChartsVarsDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if( FAILED( hr ) ) break;
			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if( FAILED( hr ) ) break;
			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if( FAILED( hr ) ) break;
			hr = pCP->Unadvise(dwCookie);
			if( FAILED( hr ) ) break;
		} while(false);
		return hr;
	}
}; 

#endif