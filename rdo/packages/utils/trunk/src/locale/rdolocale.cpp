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
#include "utils/src/common/platform.h"
#ifdef COMPILER_MINGW
	#undef __STRICT_ANSI__
#endif
// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <boost/math/special_functions/nonfinite_num_facets.hpp>
#include <iostream>

#ifdef OST_WINDOWS
#include <fcntl.h>
#include <io.h>
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
// --------------------------------------------------------------------------------

namespace rdo
{

void locale::init()
{
	initForTest();

#ifdef OST_WINDOWS
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif
}

void locale::initForTest()
{
	std::locale globalLocale(get().generate("en_EN.UTF-8"), new boost::math::nonfinite_num_put<char>);
	std::locale::global(globalLocale);

	std::cout.imbue(globalLocale);
	std::cerr.imbue(globalLocale);
	std::clog.imbue(globalLocale);
	std::cin.imbue (globalLocale);
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
	return convertToWStr(txt, locale).length();
}

std::wstring locale::convertToWStr(const std::string& txt, const std::locale& locale)
{
	return boost::locale::conv::to_utf<wchar_t>(txt, locale);
}

std::string locale::convertFromWStr(const std::wstring& txt, const std::locale& locale)
{
	return boost::locale::conv::from_utf(txt, locale);
}

void locale::cout(const std::string& txt)
{
#ifdef OST_WINDOWS
		std::wcout << convertToWStr(txt) << std::endl;
#else
		std::cout << txt << std::endl;
#endif
}

} // namespace rdo
