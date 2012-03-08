/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorresultsstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTSSTYLE_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTSSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorResultsStyle
// --------------------------------------------------------------------------------
class RDOEditorResultsStyle: public RDOEditorBaseEditStyle
{
public:
	RDOEditorResultsStyle();
	virtual ~RDOEditorResultsStyle();
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORRESULTSSTYLE_H_
