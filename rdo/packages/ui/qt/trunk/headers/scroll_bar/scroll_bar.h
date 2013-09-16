/*!
  \copyright (c) RDO-Team, 2013
  \file      scroll_bar.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.02.2013
  \brief     Информация о скролбаре
  \indent    4T
*/

#ifndef _UI_QT_SCROLL_BAR_H_
#define _UI_QT_SCROLL_BAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		struct ScrollMetric
		{
			rsint position;
			rsint posMax;
			rsint pageSize;

			ScrollMetric();

			rbool applyInc(rsint delta);
		};
	} // namespace gui
} // namespace rdo

#endif // _UI_QT_SCROLL_BAR_H_
