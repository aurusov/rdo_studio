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

namespace locale
{

void init()
{
	generator& gen = list();

	std::locale sourceCodeLocale = gen.cp1251();
	std::string s = std::use_facet<boost::locale::info>(sourceCodeLocale).name();
	std::locale::global(sourceCodeLocale);

#ifdef COMPILER_VISUAL_STUDIO
	setlocale(LC_ALL, "Russian_Russia.1251");
#endif
}

generator::generator()
{
	m_generator.locale_cache_enabled(true);
	system();
	cp1251();
	utf8  ();
}

std::locale generator::get(const std::string& name)
{
	return m_generator.generate(name);
}

std::locale generator::system()
{
	return get("");
}

std::locale generator::cp1251()
{
	return get("ru_RU.CP1251");
}

std::locale generator::utf8()
{
	return get("ru_RU.UTF-8");
}

generator& list()
{
	static generator gen;
	return gen;
}

std::string convert(const std::string& txt, const std::locale& to, const std::locale& from)
{
	std::string result;
	try
	{
		result = boost::locale::conv::between(
			txt,
			std::use_facet<boost::locale::info>(to).encoding(),
			std::use_facet<boost::locale::info>(from).encoding()
		);
	}
	catch (const boost::locale::conv::conversion_error& e)
	{}
	catch (const boost::locale::conv::invalid_charset_error& e)
	{}
	return result;
}

}} // namespace rdo::locale
