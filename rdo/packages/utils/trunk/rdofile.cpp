/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      07.11.2020
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
#   include <Windows.h>
#   include <io.h>
#else
#   include <unistd.h>
#endif // COMPILER_VISUAL_STUDIO
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "utils/rdocommon.h"
#include "utils/rdolocale.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

rbool File::create(CREF(tstring) name)
{
	return create(name, "");
}

rbool File::create(CREF(tstring) name, CREF(tstring) content)
{
	boost::filesystem::fstream file(rdo::locale::convertToWStr(name), std::ios::out | std::ios::binary);
	file << content << std::endl;
	file.close();
	return true;
}

rbool File::exist(CREF(tstring) name)
{
	return  boost::filesystem::exists(rdo::locale::convertToWStr(name));
}

rbool File::read_only(CREF(tstring) name)
{
#ifdef COMPILER_VISUAL_STUDIO
	std::string fileName = rdo::locale::convertToCLocale(name);
	return _access(fileName.c_str(), 04) == 0 && _access(fileName.c_str(), 06) == -1;
#endif  // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	return access(name.c_str(), R_OK) == 0 && access(name.c_str(), W_OK) == -1;
#endif // COMPILER_GCC
}

rbool File::unlink(CREF(tstring) name)
{
	return boost::filesystem::remove(rdo::locale::convertToWStr(name));
}

rbool File::splitpath(CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt)
{
	boost::filesystem::path from(name);
	boost::filesystem::path parentDir(from.parent_path());
	boost::filesystem::path rootName      = parentDir.root_name();
	boost::filesystem::path rootDirectory = parentDir.root_directory();
	if ((rootName.empty() && rootDirectory.empty()) || parentDir != (rootName / rootDirectory))
	{
		parentDir /= boost::filesystem::path("/");
	}

	fileDir  = parentDir.make_preferred().string();
	fileName = from.stem().string();
	fileExt  = from.extension().string();

	return true;
}

tstring File::getTempFileName()
{
#ifdef COMPILER_VISUAL_STUDIO
	const ruint BUFSIZE = 4096;
	tchar lpPathBuffer[BUFSIZE];

	if (::GetTempPath(BUFSIZE, lpPathBuffer) == 0)
	{
		return tstring();
	}
	tchar szTempName[MAX_PATH];
	if (::GetTempFileName(lpPathBuffer, NULL, 0, szTempName) == 0)
	{
		return tstring();
	}
	return szTempName;
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	boost::uuids::random_generator random_gen;
	tstring tempFileName = tstring("/tmp/rdo_temp_file_num_") + boost::uuids::to_string(random_gen());
	create(tempFileName);
	return tempFileName;
#endif // COMPILER_GCC
}

tstring File::extractFilePath(CREF(tstring) fileName)
{
	boost::filesystem::path fullFileName(fileName);
	tstring result = (fullFileName.make_preferred().parent_path() / boost::filesystem::path("/").make_preferred()).string();
	return result;
}

rbool File::trimLeft(CREF(tstring) name)
{
	boost::filesystem::ifstream inputStream(rdo::locale::convertToWStr(name), std::ios::binary);
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
