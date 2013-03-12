/*!
  \copyright (c) RDO-Team, 2012
  \file      rdolocale.cpp
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
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

void locale::init()
{
	rdo::locale& locale = get();

	std::locale sourceCodeLocale = locale.utf8();
	std::locale::global(sourceCodeLocale);

	std::locale C99_out_locale(sourceCodeLocale, new boost::math::nonfinite_num_put<char>);
	std::locale C99_in_locale (sourceCodeLocale, new boost::math::nonfinite_num_get<char>);

	std::cout.imbue(sourceCodeLocale);
	std::cerr.imbue(sourceCodeLocale);
	std::clog.imbue(sourceCodeLocale);
	std::cin.imbue (sourceCodeLocale);
}

locale& locale::get()
{
	static rdo::locale locale;
	return locale;
}

locale::locale()
{
#ifdef COMPILER_VISUAL_STUDIO
	m_modelLocale = std::locale("Russian_Russia.1251");
#endif

	m_generator.locale_cache_enabled(true);
	system();
	cp1251();
	utf8  ();
	model ();
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

std::locale locale::model()
{
#ifdef COMPILER_VISUAL_STUDIO
	return m_modelLocale;
#else
	return cp1251();
#endif
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

std::string locale::convertToCLocale(const std::string& txt, const std::locale& from)
{
	return convert(txt, getCLocaleName(), std::use_facet<boost::locale::info>(from).encoding());
}

std::string locale::convertFromCLocale(const std::string& txt, const std::locale& to)
{
	return convert(txt, std::use_facet<boost::locale::info>(to).encoding(), getCLocaleName());
}

std::string locale::getCLocaleName()
{
	return "CP1251";
}

unsigned int locale::length(const std::string& txt, const std::locale& locale)
{
	return boost::locale::conv::to_utf<wchar_t>(txt, locale).length();
}

} // namespace rdo
