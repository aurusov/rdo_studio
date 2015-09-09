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

        std::size_t MemDCBase::width() const
        {
            return m_width;
        }

        std::size_t MemDCBase::height() const
        {
            return m_height;
        }

    } // namespace gui
} // namespace rdo
