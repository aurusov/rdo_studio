#ifndef _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_
#define _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include "ui_frame_options_widget.h"
#include "utils/src/common/warning_enable.h"
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

	void setStyle(FrameStyle* style);
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_OPTIONS_VIEW_H_
