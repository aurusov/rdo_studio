// rdoprocess_shape_create_MJ.h: interface for the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../../rdoprocess_app.h"

#include "rdoprocess_shape_MJ.h"

class RPShapeCreateMJ : public RPShape_MJ  
{
public:
	RPShapeCreateMJ( RPObject* parent );
	virtual ~RPShapeCreateMJ();

	virtual rp::string getType() { return "RPShapeCreateMJ"; }

	void define_rule();
	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	//virtual void list_name();
	virtual void generate();

	//переменные для генерации
	CString gname; // имя
	double gfirst; // время первого
	double gamount; // кол-во создаваемых
	int gtype; // закон прибытия
	int base_gen;
	
	//атрибуты законов
	double gexp;
	double gdisp;
	double gmax;
	double gmin;

	//второе окно
	int inf; // бесконечноть
	int gID; // ID группы
	double gpar1, gpar2, gpar3;


};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
