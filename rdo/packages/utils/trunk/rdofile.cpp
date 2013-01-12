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
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

rbool File::read_only(CREF(tstring) name)
{
#ifdef COMPILER_VISUAL_STUDIO
#ifdef UNICODE
	return _waccess_s(name.c_str(), 04) == 0 && _waccess_s(name.c_str(), 06) == -1;
#else
	return _access(name.c_str(), 04) == 0 && _access(name.c_str(), 06) == -1;
#endif
#endif  // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	return access((char*)name.c_str(), R_OK) == 0 && access((char*)name.c_str(), W_OK) == -1;
#endif // COMPILER_GCC
}

rbool File::splitpath(CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt)
{
#if defined( COMPILER_VISUAL_STUDIO )
	tchar _drive[_MAX_DRIVE];
	tchar _dir  [_MAX_DIR  ];
	tchar _name [_MAX_FNAME];
	tchar _ext  [_MAX_EXT  ];

#ifdef UNICODE
	if (_wsplitpath_s(name.c_str(), _drive, _MAX_DRIVE, _dir, _MAX_DIR, _name, _MAX_FNAME, _ext, _MAX_EXT) != 0)
#else
	if (_splitpath_s(name.c_str(), _drive, _MAX_DRIVE, _dir, _MAX_DIR, _name, _MAX_FNAME, _ext, _MAX_EXT) != 0)
#endif
		return false;

	fileDir  = rdo::format(_T("%s%s"), _drive, _dir);
	fileName = _name;
	fileExt  = _ext;
#elif defined( COMPILER_GCC )
	boost::filesystem::path from(name);
#ifdef UNICODE
	fileDir = from.parent_path().wstring();
	fileName = from.stem().wstring();
	fileExt  = from.extension().wstring();
#else
	fileDir = from.parent_path().string();
	fileName = from.stem().string();
	fileExt  = from.extension().string();
#endif // UNICODE

	if(fileDir[fileDir.size() - 1] != _T('/'))
	{
		fileDir += _T("/");
	}

#endif // COMPILER_VISUAL_STUDIO
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
#ifdef UNICODE
	tstring tempFileName = tstring(_T("/tmp/rdo_temp_file_num_")) + boost::uuids::to_wstring(random_gen());
#else
	tstring tempFileName = tstring(_T("/tmp/rdo_temp_file_num_")) + boost::uuids::to_string(random_gen());
#endif // UNICODE
	create(tempFileName);
	return tempFileName;
#endif // COMPILER_GCC
}

rbool File::trimLeft(CREF(tstring) name)
{
	boost::filesystem::ifstream inputStream(name.c_str(), std::ios::binary);
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
			if (byte != _T(' ') && byte != _T('\t') && byte != _T('\n') && byte != _T('\r'))
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

	boost::filesystem::path to  (name);

	try
	{
		if (!boost::filesystem::remove(to))
		{
			return false;
		}
	boost::filesystem::ofstream outStream(name.c_str(), std::ios::binary);
		outStream << sstream.str();
	}
	catch (CREF(boost::system::error_code))
	{
		return false;
	}
	return true;
}

CLOSE_RDO_NAMESPACE
