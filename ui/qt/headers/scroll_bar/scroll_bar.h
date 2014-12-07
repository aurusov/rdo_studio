#ifndef _UI_QT_SCROLL_BAR_H_
#define _UI_QT_SCROLL_BAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		struct ScrollMetric
		{
			int position;
			int posMax;
			int pageSize;

			ScrollMetric();

			bool applyInc(int delta);
		};
	} // namespace gui
} // namespace rdo

#endif // _UI_QT_SCROLL_BAR_H_
