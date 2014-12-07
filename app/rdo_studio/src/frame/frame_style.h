#ifndef _RDO_STUDIO_FRAME_STYLE_H_
#define _RDO_STUDIO_FRAME_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace frame {

// --------------------------------------------------------------------------------
// -------------------- FrameStyle
// --------------------------------------------------------------------------------
class FrameStyle: public rdo::gui::style::StyleBase
{
public:
	FrameStyle();
	~FrameStyle();

	FrameStyle& operator =( const FrameStyle& style );
	bool operator ==( const FrameStyle& style ) const;
	bool operator !=( const FrameStyle& style ) const;

	static FrameStyle getDefaultStyle();
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_STYLE_H_
