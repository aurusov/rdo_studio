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
#include <QWidget>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_frame_options_widget.h"
#include "app/rdo_studio/src/frame/frame_style.h"
// --------------------------------------------------------------------------------

class FrameOptionsView 
	: public  QWidget
	, private Ui::FrameOptionsWidget
{
private:
	QFont   font;
	RDOStudioFrameStyle* m_style;

public:
	explicit FrameOptionsView(PTR(QWidget) pParent);

	void setStyle(PTR(RDOStudioFrameStyle) style);
};

#endif // _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_
