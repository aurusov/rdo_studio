/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditortabctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORTABCTRL_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORTABCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QTabWidget>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditoredit.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorTabCtrl
// --------------------------------------------------------------------------------
class RDOEditorTabCtrl: public QTabWidget
{
public:
	typedef RDOEditorEdit context_type;

	RDOEditorTabCtrl(PTR(QWidget) pParent, PTR(QWidget) pView);
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
	rdoEditCtrl::RDOBaseEdit::Group m_group;

	void createPage(PTR(QWidget) pView, CREF(QString) name);
};

} // namespace rdoEditor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORTABCTRL_H_
