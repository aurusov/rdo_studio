#define _ATL_ATTRIBUTES

#include <atlbase.h>
//CComModule _Module;
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
using namespace std ;

// Написать функцию добавления параметра
class ReportVar
{
public:
	// Имя переменной
	std::string Name ;
	// Тип переменной
	enum VarTypeValues {WATCH_STATE = 2, WATCH_PAR = 3, GET_VALUE = 4, WATCH_QUANT = 5, WATCH_VALUE = 6} ;
    // Как сделать enum??????
	int VarType ;
	// Список собранных параметров по переменной
	vector <std::string> Parameters ;
} ;



class Groop
{
public:
	// Имя группы
	std::string GroopName ;
	// Тип отображения группы
	enum GroopType {GR_NORMAL = 0, GR_CHART} ;
	//Список переменных, образующих группу
	vector<ReportVar> Variables ;
	// Является ли группа активной
	bool IsActive ;
} ;


// Количество ключевых слов
#define KEYWORDS_QUANTITY 7

#define NOCOMMENT  1
#define LINECOMMENT 2
#define LONGCOMMENT 3
#define ENDLINECOMMENT 4
#define ENDLONGCOMMENT 5

// Функция преобразует текст в список слов   !!!будет в классе - парсер :)
int StringToList(std::string, vector <std::string> *) ;

// Функция удаляет комментарии из текста     !!!будет в классе - парсер :)
int DeleteComments(std::string, std::string *) ;

// Функция добавляет группу к списку групп, делает ее активной или неактивной  !!!будет в классе групс
int AddGroop(vector <Groop> *, std::string, bool ) ;

// Функция ищет ключевые слова в списке и возвращает значение на следующий элемент !!!будет в классе - парсер)
int FindKeyWord(vector <std::string> List, int StartIndex, int *WhatWord) ;

// Функция добавляет переменные в активные группы !!! будет в классе группс
int InsertVar(vector <Groop> *Groops, std::string VarName, int VarType_) ;

// Функция сопоставляет переменные с соответствующими им параметрами !!! будет в классе группс
int GetParam(vector <Groop> *Groops, vector <std::string> List) ;


// Обертка к датасету для вывода данных
class CfrxUserDataSetEvents1 : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int                     *int_counter ;
	vector <Groop> *Groops_output ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxUserDataSetEvents1)
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
		if (_variant_t("GroopName") == ColumnName) 
		{		
			_variant_t		v(Groops_output->at(idx).GroopName.c_str()) ;
			VariantCopy(Value, &v) ;
			v.Clear() ;				
		    (*int_counter)++ ;
		}
		
		//if (_variant_t("VarName") == ColumnName) 
		//{
	//		if (flag)
	//		{
	//			flag = false ;
	//			idxx = 0 ;
	//		}
	//		_variant_t		v(Groops_output->at(idx).Variables.at(idxx++).Name.c_str());
	//		VariantCopy(Value, &v) ;
	//		v.Clear() ;	
	//	    if (idxx  == Groops_output->at(idx).Variables.size()) 
	//			EOFflag = false ;
	//	}

		return S_OK;
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			*IsEOF = ! ( idx < Groops_output->size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet	*	pDataSet, vector <Groop> *Groops, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		int_counter = counter ;
		Groops_output = Groops ;

		do {

			// Check that this is a connectable object.
			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Advise( pUnk, &dwCookie);
			if(FAILED(hr)) break;

		} while(false);

		return hr;
	}

	HRESULT Unadvise(IfrxUserDataSet	*	pDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Unadvise( dwCookie);
			if(FAILED(hr)) break;

		} while(false);
		
		return hr;
	}
}; 


class CfrxUserDataSetEvents2 : 
	public CComObjectRoot,
	public IDispatchImpl< 
		IfrxUserDataSetEvents, 
		&__uuidof(IfrxUserDataSetEvents), 
		&LIBID_FastReport>
{
	
	int						idx ;
	int                     *int_counter ;
	vector <Groop> *Groops_output ;

	DWORD					dwCookie ;

	BEGIN_COM_MAP(CfrxUserDataSetEvents2)
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
		//if (_variant_t("GroopName") == ColumnName) 
		//{		
	//		_variant_t		v(Groops_output->at(idx).GroopName.c_str()) ;
	//		VariantCopy(Value, &v) ;
//			v.Clear() ;				
//		}
		
		if (_variant_t("GroopName") == ColumnName) 
		{
	//		if (flag)
	//		{
	//			flag = false ;
	//			idxx = 0 ;
	//		}
			_variant_t		v(Groops_output->at(*int_counter).Variables.at(idx).Name.c_str());
			VariantCopy(Value, &v) ;
			v.Clear() ;	
	
			//_variant_t value = pReport->GetVariable(_bstr_t("second_var")) ;

//			if (idxx  == Groops_output->at(idx).Variables.size()) 
	//			EOFflag = false ;
		}

		return S_OK;
	}

    STDMETHODIMP raw_OnCheckEOF (VARIANT_BOOL * IsEOF ) 
	{ 
			if (*int_counter==-1)
				*IsEOF = ! ( idx < Groops_output->at(0).Variables.size()) ;
			else
		    *IsEOF = ! ( idx < Groops_output->at(*int_counter).Variables.size()) ;
			return S_OK ; 
	}

public:
	HRESULT Advise(IfrxUserDataSet	*	pDataSet, vector <Groop> *Groops, int *counter)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		int_counter = counter ;
		Groops_output = Groops ;

		do {

			// Check that this is a connectable object.
			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Advise( pUnk, &dwCookie);
			if(FAILED(hr)) break;

		} while(false);

		return hr;
	}

	HRESULT Unadvise(IfrxUserDataSet	*	pDataSet)
	{
		HRESULT								hr;
		IConnectionPointContainerPtr		pCPC;
		IConnectionPointPtr					pCP;
		CComPtr<IUnknown> pUnk;
		
		do {

			hr = pDataSet->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if(FAILED(hr)) break;

			hr = pCPC->FindConnectionPoint(__uuidof(IfrxUserDataSetEvents), &pCP);
			if(FAILED(hr)) break;

			hr = QueryInterface(IID_IUnknown, (void**)&pUnk);
			if(FAILED(hr)) break;

			hr = pCP->Unadvise( dwCookie);
			if(FAILED(hr)) break;

		} while(false);
		
		return hr;
	}
}; 