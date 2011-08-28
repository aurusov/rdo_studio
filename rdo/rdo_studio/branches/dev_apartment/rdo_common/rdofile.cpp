/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.11.2020
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <Windows.h>
#include <boost/filesystem.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdofile.h"
#include "rdo_common/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

rbool File::splitpath(CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt)
{
	char _drive[_MAX_DRIVE];
	char _dir  [_MAX_DIR  ];
	char _name [_MAX_FNAME];
	char _ext  [_MAX_EXT  ];

	if (_splitpath_s(name.c_str(), _drive, _MAX_DRIVE, _dir, _MAX_DIR, _name, _MAX_FNAME, _ext, _MAX_EXT) != 0)
		return false;

	fileDir  = rdo::format(_T("%s%s"), _drive, _dir);
	fileName = _name;
	fileExt  = _ext;
	return true;
}

tstring File::getTempFileName()
{
	const ruint BUFSIZE = 4096;
	char lpPathBuffer[BUFSIZE];

	if (::GetTempPath(BUFSIZE, lpPathBuffer) == 0)
	{
		return tstring();
	}

	char szTempName[MAX_PATH];
	if (::GetTempFileName(lpPathBuffer, NULL, 0, szTempName) == 0)
	{
		return tstring();
	}

	return szTempName;
}

rbool File::trimLeft(CREF(tstring) name)
{
	tstring tempFileName = getTempFileName();

	std::ofstream tempStream(tempFileName.c_str(), std::ios::trunc | std::ios::binary);
	std::ifstream fileStream(name.c_str(), std::ios::binary);

	if (!tempStream.good() || !fileStream.good())
	{
		return false;
	}

	rbool empty = true;
	while (!fileStream.eof())
	{
		char byte;
		fileStream.get(byte);

		if (empty)
		{
			if (byte != _T(' ') && byte != _T('\t') && byte != _T('\n') && byte != _T('\r'))
			{
				empty = false;
			}
		}

		if (!empty)
		{
			tempStream.write(&byte, 1);
		}
	}

	tempStream.close();
	fileStream.close();

	boost::filesystem::path from(tempFileName);
	boost::filesystem::path to  (name);

	try
	{
		if (!boost::filesystem::remove(to))
		{
			return false;
		}
		boost::filesystem::rename(from, to);
	}
	catch (CREF(boost::system::error_code))
	{
		return false;
	}

	return true;
}

CLOSE_RDO_NAMESPACE
