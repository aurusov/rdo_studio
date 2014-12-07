#ifndef _UTILS_RDOFILE_H_
#define _UTILS_RDOFILE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <boost/filesystem.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class File
{
public:
	static bool create(const boost::filesystem::path& name);
	static bool create(const boost::filesystem::path& name, const std::string& content);

	static bool exist(const boost::filesystem::path& name);
	static bool read_only(const boost::filesystem::path& name);
	static bool unlink(const boost::filesystem::path& name);
	static bool splitpath(const boost::filesystem::path& from, boost::filesystem::path& fileDir, boost::filesystem::path& fileName, boost::filesystem::path& fileExt);
	static boost::filesystem::path getTempFileName();
	static boost::filesystem::path extractFilePath(const boost::filesystem::path& name);
	static bool trimLeft(const boost::filesystem::path& name);
};

} // namespace rdo

#endif // _UTILS_RDOFILE_H_
