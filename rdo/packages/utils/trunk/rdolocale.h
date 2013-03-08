/*!
  \copyright (c) RDO-Team, 2012
  \file      rdolocale.h
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.10.2012
  \brief     Настройка локали для РДО
  \indent    4T
*/

#ifndef _UTILS_LOCALE_H_
#define _UTILS_LOCALE_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <boost/locale.hpp>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
	void setup_locale();

	class locale
	{
	public:
		static void    init();
		static locale& get ();

		std::locale generate(const std::string& name);

		std::locale system();
		std::locale cp1251();
		std::locale utf8  ();

		static std::string convert         (const std::string& txt, const std::locale& to, const std::locale& from = std::locale());
		static std::string convert         (const std::string& txt, const std::string& to, const std::string& from);
		       std::string convertToCLocale(const std::string& txt, const std::locale& from = std::locale()) const;

	private:
		locale();

		boost::locale::generator  m_generator;
	};

} // namespace rdo

#endif // _UTILS_LOCALE_H_
