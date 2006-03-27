// rdoprocess_shape_create_MJ.h: interface for the RPShapeCreateMJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_shape.h"

class RPShapeCreateMJ : public RPShape  
{
public:
	RPShapeCreateMJ( RPObject* parent );
	virtual ~RPShapeCreateMJ();
	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void list_name();
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_MJ_H__B043A584_EC30_4198_83AE_4C54E356FDAF__INCLUDED_)
