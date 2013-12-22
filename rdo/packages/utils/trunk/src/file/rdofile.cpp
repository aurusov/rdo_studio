/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      07.11.2020
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
#ifdef COMPILER_MINGW
	#undef __STRICT_ANSI__
#endif
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#include <Windows.h>
	#include <io.h>
#elif defined(COMPILER_MINGW)
	#include <io.h>
#endif // COMPILER_VISUAL_STUDIO
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/file/rdofile.h"
#include "utils/src/common/rdocommon.h"
#include "utils/src/locale/rdolocale.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

rbool File::create(CREF(boost::filesystem::path) name)
{
	return create(name, "");
}

rbool File::create(CREF(boost::filesystem::path) name, CREF(tstring) content)
{
	boost::filesystem::fstream file(name, std::ios::out | std::ios::binary);
	file << content << std::endl;
	file.close();
	return true;
}

rbool File::exist(CREF(boost::filesystem::path) name)
{
	return boost::filesystem::exists(name);
}

rbool File::read_only(CREF(boost::filesystem::path) name)
{
#ifdef COMPILER_VISUAL_STUDIO
	return _waccess(name.wstring().c_str(), 04) == 0 && _waccess(name.wstring().c_str(), 06) == -1;
#endif  // COMPILER_VISUAL_STUDIO

#ifdef COMPILER_GCC
	std::string fileName = rdo::locale::convertToCLocale(name.string());
	return access(fileName.c_str(), R_OK) == 0 && access(fileName.c_str(), W_OK) == -1;
#endif // COMPILER_GCC
}

rbool File::unlink(CREF(boost::filesystem::path) name)
{
	return boost::filesystem::remove(name);
}

rbool File::splitpath(CREF(boost::filesystem::path) from, REF(boost::filesystem::path) fileDir, REF(boost::filesystem::path) fileName, REF(boost::filesystem::path) fileExt)
{
	boost::filesystem::path parentDir(from.parent_path());
	boost::filesystem::path rootName      = parentDir.root_name();
	boost::filesystem::path rootDirectory = parentDir.root_directory();
	if ((rootName.empty() && rootDirectory.empty()) || parentDir != (rootName / rootDirectory))
	{
		parentDir /= boost::filesystem::path("/");
	}

	fileDir  = parentDir.make_preferred();
	fileName = from.stem();
	fileExt  = from.extension();

	return true;
}

boost::filesystem::path File::getTempFileName()
{
#ifdef COMPILER_VISUAL_STUDIO
	const ruint BUFSIZE = 4096;
	tchar lpPathBuffer[BUFSIZE];

	if (::GetTempPath(BUFSIZE, lpPathBuffer) == 0)
	{
		return boost::filesystem::path();
	}
	tchar szTempName[MAX_PATH];
	if (::GetTempFileName(lpPathBuffer, NULL, 0, szTempName) == 0)
	{
		return boost::filesystem::path();
	}
	return boost::filesystem::path(rdo::locale::convertToWStr(szTempName));
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	boost::uuids::random_generator random_gen;
	boost::filesystem::path tempFileName;
	tempFileName /= "tmp";
	tempFileName /= "rdo_temp_file_num_" + boost::uuids::to_string(random_gen());
	create(tempFileName);
	return tempFileName;
#endif // COMPILER_GCC
}

boost::filesystem::path File::extractFilePath(CREF(boost::filesystem::path) name)
{
	boost::filesystem::path parentPath = boost::filesystem::path(name).make_preferred().parent_path();
	return parentPath.empty()
		? parentPath
		: parentPath / boost::filesystem::path("/").make_preferred();
}

rbool File::trimLeft(CREF(boost::filesystem::path) name)
{
	boost::filesystem::ifstream inputStream(name, std::ios::binary);
	std::stringstream sstream;

	if (!inputStream.good())
	{
		return false;
	}

	rbool empty = true;
	while (!inputStream.eof())
	{
		char byte;
		inputStream.get(byte);

		if (empty)
		{
			if (byte != ' ' && byte != '\t' && byte != '\n' && byte != '\r')
			{
				empty = false;
			}
		}

		if (!empty)
		{
			sstream.write(&byte, 1);
		}
	}

	inputStream.close();

	boost::filesystem::path to(name);

	try
	{
		if (!boost::filesystem::remove(to))
		{
			return false;
		}
		boost::filesystem::ofstream outStream(name, std::ios::binary);
		outStream << sstream.str();
	}
	catch (CREF(boost::system::error_code))
	{
		return false;
	}
	return true;
}

CLOSE_RDO_NAMESPACE
