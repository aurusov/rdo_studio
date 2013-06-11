/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      frame_options_view.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      07.02.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_
#define _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QWidget>
#include "ui_frame_options_widget.h"
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace frame {

class OptionsView 
	: public  QWidget
	, private Ui::FrameOptionsWidget
{
private:
	QFont       font;
	FrameStyle* m_style;

public:
	explicit OptionsView(QWidget* pParent);

	void setStyle(PTR(FrameStyle) style);
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_
