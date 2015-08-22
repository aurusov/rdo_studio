// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/model_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window_base.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::model;

TabCtrl::TabCtrl(QWidget* pParent, QWidget* pView)
    : QTabWidget(pParent)
{
    createPage(pView, "RTP");
    createPage(pView, "RSS");
    createPage(pView, "EVN");
    createPage(pView, "PAT");
    createPage(pView, "DPT");
    createPage(pView, "PRC");
    createPage(pView, "FRM");
    createPage(pView, "FUN");
    createPage(pView, "SMR");
    createPage(pView, "PMD");

    setCurrentRDOItem(rdo::FileType::RTP);
}

TabCtrl::~TabCtrl()
{}

void TabCtrl::createPage(QWidget* pView, const QString& name)
{
    context_type* pPage = new context_type(this, pView);

    ASSERT(pPage);

    addTab(pPage, name);

    pPage->setEditorStyle(&g_pApp->getStyle()->style_editor);
    pPage->show();
    // TODO qt
    //pPage->getContext().setPopupMenu(&pView->popupMenu);

    m_group.insert(pPage);
    pPage->setGroup(&m_group);
}

rdo::FileType TabCtrl::indexToType(int index) const
{
    switch (index)
    {
        case  0: return rdo::FileType::RTP;
        case  1: return rdo::FileType::RSS;
        case  2: return rdo::FileType::EVN;
        case  3: return rdo::FileType::PAT;
        case  4: return rdo::FileType::DPT;
        case  5: return rdo::FileType::PRC;
        case  6: return rdo::FileType::FRM;
        case  7: return rdo::FileType::FUN;
        case  8: return rdo::FileType::SMR;
        case  9: return rdo::FileType::PMD;
        default: return rdo::FileType::PAT;
    }
}

int TabCtrl::typeToIndex(rdo::FileType type) const
{
    switch (type)
    {
        case rdo::FileType::RTP: return 0;
        case rdo::FileType::RSS: return 1;
        case rdo::FileType::EVN: return 2;
        case rdo::FileType::PAT: return 3;
        case rdo::FileType::DPT: return 4;
        case rdo::FileType::PRC: return 5;
        case rdo::FileType::FRM: return 6;
        case rdo::FileType::FUN: return 7;
        case rdo::FileType::SMR: return 8;
        case rdo::FileType::PMD: return 9;
        default: break;
    }
    return -1;
}

void TabCtrl::setCurrentRDOItem(rdo::FileType type)
{
    int index = typeToIndex(type);
    if (index != -1)
        setCurrentIndex(index);
}

TabCtrl::context_type* TabCtrl::getItemEdit(rdo::FileType type) const
{
    int index = typeToIndex(type);
    return index != -1
        ? getItemEdit(index)
        : NULL;
}

TabCtrl::context_type* TabCtrl::getCurrentEdit() const
{
    return static_cast<context_type*>(currentWidget());
}

TabCtrl::context_type* TabCtrl::getItemEdit(int index) const
{
    return static_cast<context_type*>(widget(index));
}
