#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_tool_bar_block.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessToolBarBlock::RPProcessToolBarBlock()
{
}

RPProcessToolBarBlock::~RPProcessToolBarBlock()
{
}


BEGIN_MESSAGE_MAP(RPProcessToolBarBlock, CStatusBarCtrl)
	ON_COMMAND(ID_BUTTON32779, OnButton32779)
END_MESSAGE_MAP()

void RPProcessToolBarBlock::OnButton32779() 
{
//	RPProcessShapeCreate* shape_create = new RPProcessShapeCreate( flowobj );
//	shape_create->setPosition( 100,100 );
}
