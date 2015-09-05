#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/editor/results_edit.h"
// --------------------------------------------------------------------------------

class DockResults: public DockFocusable
{
public:
    typedef rdo::gui::editor::Results context_type;

    DockResults(QWidget* pParent);
    virtual ~DockResults();

    void appendString(const QString& str);
    void clear();

    context_type& getContext();
};
