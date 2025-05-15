#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <memory>
#include <boost/config.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
    namespace gui
    {
        class MemDCBase
        {
        public:
             MemDCBase();
            ~MemDCBase();

            std::size_t width() const;
            std::size_t height() const;

        protected:
            std::size_t m_width;
            std::size_t m_height;
        };

        template <class TDC, class TBMP>
        class MemDC: public MemDCBase
        {
        public:
             MemDC();
            ~MemDC();

            bool valid() const;
            bool create(std::size_t width, std::size_t height);
            bool create(std::size_t width, std::size_t height, TDC& from);
            bool resize(std::size_t width, std::size_t height);

            TDC& dc();
            TBMP& buffer();

        private:
            std::unique_ptr<TDC> m_pDC;
            std::unique_ptr<TBMP> m_pBitmap;

        private:
            bool onCreate();

            void clear();
        };

    } // namespace gui
} // namespace rdo

#include "ui/abstract/headers/memdc/memdc-inl.h"
