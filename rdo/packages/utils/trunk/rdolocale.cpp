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
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdolocale.h"
// --------------------------------------------------------------------------------

namespace rdo
{

void setup_locale()
{
	std::locale default_locale(std::locale::classic());

	std::locale C99_out_locale(default_locale, new boost::math::nonfinite_num_put<char>);
	std::locale C99_in_locale(default_locale, new boost::math::nonfinite_num_get<char>);

	std::cout.imbue(C99_out_locale);
	std::cerr.imbue(C99_out_locale);
	std::clog.imbue(C99_out_locale);
	std::cin.imbue(C99_in_locale);
}

void locale::init()
{
	rdo::locale& locale = get();

	std::locale sourceCodeLocale = locale.cp1251();
	std::locale::global(sourceCodeLocale);

#ifdef COMPILER_VISUAL_STUDIO
	setlocale(LC_ALL, ".ACP");
#endif

	std::locale C99_out_locale(std::locale::classic(), new boost::math::nonfinite_num_put<char>);
	std::locale C99_in_locale (std::locale::classic(), new boost::math::nonfinite_num_get<char>);

	std::cout.imbue(C99_out_locale);
	std::cerr.imbue(C99_out_locale);
	std::clog.imbue(C99_out_locale);
	std::cin.imbue (C99_in_locale );
}

locale& locale::get()
{
	static rdo::locale locale;
	return locale;
}

locale::locale()
{
	m_generator.locale_cache_enabled(true);
	system();
	cp1251();
	utf8  ();
}

std::locale locale::generate(const std::string& name)
{
	return m_generator.generate(name);
}

std::locale locale::system()
{
	return generate("");
}

std::locale locale::cp1251()
{
	return generate("ru_RU.CP1251");
}

std::locale locale::utf8()
{
	return generate("ru_RU.UTF-8");
}

std::string locale::convert(const std::string& txt, const std::locale& to, const std::locale& from)
{
	return convert(txt, std::use_facet<boost::locale::info>(to).encoding(), std::use_facet<boost::locale::info>(from).encoding());
}

std::string locale::convert(const std::string& txt, const std::string& to, const std::string& from)
{
	std::string result;
	try
	{
		result = boost::locale::conv::between(txt, to, from);
	}
	catch (const boost::locale::conv::conversion_error&)
	{}
	catch (const boost::locale::conv::invalid_charset_error&)
	{}
	return result;
}

std::string locale::convertToCLocale(const std::string& txt, const std::locale& from) const
{
	std::string cLocale = setlocale(LC_ALL, NULL);

#ifdef COMPILER_VISUAL_STUDIO
	std::string::size_type pos = cLocale.find('.');
	if (pos != std::string::npos)
	{
		cLocale = "CP" + cLocale.substr(pos + 1);
	}
#endif

	return convert(txt, cLocale, std::use_facet<boost::locale::info>(from).encoding());
}

} // namespace rdo
