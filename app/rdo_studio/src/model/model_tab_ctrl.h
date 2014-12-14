#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QTabWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/model_edit.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace model {

class TabCtrl: public QTabWidget
{
public:
    typedef editor::Model context_type;

    TabCtrl(QWidget* pParent, QWidget* pView);
    virtual ~TabCtrl();

    rdo::FileType indexToType(int index) const;
    int typeToIndex(rdo::FileType type) const;
    bool typeSupported(rdo::FileType type) const { return typeToIndex(type) != -1; }

    rdo::FileType getCurrentRDOItem() const { return indexToType(currentIndex()); }
    void setCurrentRDOItem(rdo::FileType type);

    context_type* getCurrentEdit() const;
    context_type* getItemEdit(int index) const;
    context_type* getItemEdit(rdo::FileType type) const;

private:
    editor::Edit::Group m_group;

    void createPage(QWidget* pView, const QString& name);
};

}}} // namespace rdo::gui::model
