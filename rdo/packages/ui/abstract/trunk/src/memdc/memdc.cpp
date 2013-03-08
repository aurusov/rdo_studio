/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      ui/abstract/src/memdc/memdc.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.07.2012
  \brief     Абстрактный DeviceContext, созданный в памяти
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "ui/abstract/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/abstract/headers/memdc/memdc.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		MemDCBase::MemDCBase()
			: m_width (0)
			, m_height(0)
		{}

		MemDCBase::~MemDCBase()
		{}

		ruint MemDCBase::width() const
		{
			return m_width;
		}

		ruint MemDCBase::height() const
		{
			return m_height;
		}

	} // namespace gui
} // namespace rdo
