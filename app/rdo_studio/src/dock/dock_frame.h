#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/frame/frame_tree_ctrl.h"
// --------------------------------------------------------------------------------

class DockFrame: public DockFocusable
{
public:
    typedef  rdo::gui::frame::TreeCtrl  context_type;

    DockFrame(QWidget* pParent);
    virtual ~DockFrame();

    context_type& getContext();
};
