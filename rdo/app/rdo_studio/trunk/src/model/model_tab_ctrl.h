/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      model_tab_ctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MODEL_TAB_CTRL_H_
#define _RDO_STUDIO_MODEL_TAB_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QTabWidget>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/model_edit.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class ModelTabCtrl: public QTabWidget
{
public:
	typedef Model context_type;

	ModelTabCtrl(PTR(QWidget) pParent, PTR(QWidget) pView);
	virtual ~ModelTabCtrl();

	rdoModelObjects::RDOFileType indexToType(int index) const;
	int   typeToIndex  (rdoModelObjects::RDOFileType type) const;
	rbool typeSupported(rdoModelObjects::RDOFileType type) const   { return typeToIndex(type) != -1;     }

	rdoModelObjects::RDOFileType getCurrentRDOItem() const         { return indexToType(currentIndex()); }
	void setCurrentRDOItem(rdoModelObjects::RDOFileType type);

	PTR(context_type) getCurrentEdit() const;
	PTR(context_type) getItemEdit   (int index) const;
	PTR(context_type) getItemEdit   (rdoModelObjects::RDOFileType type) const;

private:
	Edit::Group m_group;

	void createPage(PTR(QWidget) pView, CREF(QString) name);
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_MODEL_TAB_CTRL_H_
