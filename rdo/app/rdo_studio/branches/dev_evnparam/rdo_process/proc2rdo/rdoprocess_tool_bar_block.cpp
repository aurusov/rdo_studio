// rdoprocess_tool_bar_blockMJ.cpp : implementation file
//

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_tool_bar_block.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPToolBarBlockMJ

RPToolBarBlockMJ::RPToolBarBlockMJ()
{
}

RPToolBarBlockMJ::~RPToolBarBlockMJ()
{
}


BEGIN_MESSAGE_MAP(RPToolBarBlockMJ, CStatusBarCtrl)
	//{{AFX_MSG_MAP(RPToolBarBlockMJ)
	ON_COMMAND(ID_BUTTON32779, OnButton32779)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPToolBarBlockMJ message handlers


void RPToolBarBlockMJ::OnButton32779() 
{
//	RPShapeCreateMJ* shape_create = new RPShapeCreateMJ( flowobj );
//	shape_create->setPosition( 100,100 );
}
