/*!
  \copyright (c) RDO-Team, 2012
  \file      rdolocale.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      20.10.2012
  \brief     Настройка локали для РДО
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <boost/math/special_functions/nonfinite_num_facets.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdolocale.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

void setup_locale()
{
    std::locale default_locale (std::locale::classic());

    std::locale C99_out_locale(default_locale, new boost::math::nonfinite_num_put<char>);
    std::locale C99_in_locale(default_locale, new boost::math::nonfinite_num_get<char>);

    std::cout.imbue(C99_out_locale);
    std::cerr.imbue(C99_out_locale);
    std::clog.imbue(C99_out_locale);
    std::cin.imbue(C99_in_locale);
}

CLOSE_RDO_NAMESPACE