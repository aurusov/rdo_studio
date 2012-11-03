/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditortabctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORTABCTRL_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORTABCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qtabwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/src/mfc_qt_wrapper.h"
// --------------------------------------------------------------------------------

class RDOStudioEditBaseView;

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorTabCtrl
// --------------------------------------------------------------------------------
class RDOEditorTabCtrl: public QTabWidget
{
public:
	typedef RDOEditorEdit context_type;

	RDOEditorTabCtrl(PTR(QWidget) pParent, PTR(RDOStudioEditBaseView) pView);
	virtual ~RDOEditorTabCtrl();

	rdoModelObjects::RDOFileType indexToType(int index) const;
	int   typeToIndex  (rdoModelObjects::RDOFileType type) const;
	rbool typeSupported(rdoModelObjects::RDOFileType type) const   { return typeToIndex(type) != -1;     }

	rdoModelObjects::RDOFileType getCurrentRDOItem() const         { return indexToType(currentIndex()); }
	void setCurrentRDOItem(rdoModelObjects::RDOFileType type);

	PTR(context_type) getCurrentEdit() const;
	PTR(context_type) getItemEdit   (int index) const;
	PTR(context_type) getItemEdit   (rdoModelObjects::RDOFileType type) const;

private:
	rdoEditCtrl::RDOBaseEditGroup group;

	void createPage(PTR(RDOStudioEditBaseView) pView, CREF(QString) name);
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITORTABCTRL_H_
