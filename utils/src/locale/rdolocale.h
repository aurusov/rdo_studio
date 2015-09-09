#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <boost/locale.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
    class locale
    {
    public:
        static void    init();
        static void    initForTest();
        static locale& get ();

        std::locale generate(const std::string& name);

        std::locale system();
        std::locale utf8  ();

        static std::string  convert           (const std::string& txt, const std::locale& to, const std::locale& from = std::locale());
        static std::string  convert           (const std::string& txt, const std::string& to, const std::string& from);
        static std::string  convertToCLocale  (const std::string& txt, const std::locale& from = std::locale());
        static std::string  convertFromCLocale(const std::string& txt, const std::locale& to = std::locale());
        static unsigned int length            (const std::string& txt, const std::locale& locale = std::locale());
        static std::wstring convertToWStr     (const std::string& txt, const std::locale& locale = std::locale());
        static std::string  convertFromWStr   (const std::wstring& txt, const std::locale& locale = std::locale());

        static void cout(const std::string& txt);

    private:
        locale();

        boost::locale::generator  m_generator;

#ifdef COMPILER_VISUAL_STUDIO
        std::locale               m_modelLocale;
#endif

        static std::string getCLocaleName();
    };

} // namespace rdo
