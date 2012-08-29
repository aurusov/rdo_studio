#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

typedef std::string  tstring;
typedef unsigned int ruint;

void main(int paramCount, char* params[])
{
	if (paramCount != 3)
	{
		std::cerr << "using version.exe <mfc_res_filename.h> <string_filename.h>" << std::endl;
		return;
	}

	ruint rev;
	std::cin >> rev;
	const tstring versionFileName      = params[1];
	const tstring buildVersionFileName = params[2];

	std::ifstream ifile(versionFileName.c_str());
	if (ifile.is_open())
	{
		std::stringstream revStr;
		revStr << rev;
		std::stringstream ifileStr;
		while (true)
		{
			tstring str;
			ifile >> str;
			if (str.find(revStr.str()) != tstring::npos)
			{
				return;
			}
			if (ifile.eof())
			{
				break;
			}
		}
	}

	std::ofstream ofile;
	ofile.open(versionFileName.c_str());
	ofile << "#define FILEVER        1,1,0," << rev << std::endl;
	ofile << "#define PRODUCTVER     FILEVER" << std::endl;
	ofile << "#define STRCONVERT(x)  #x" << std::endl;
	ofile << "#define STR(x)         STRCONVERT(x)" << std::endl;
	ofile.close();

	std::ofstream ofileBuild;
	ofileBuild.open(buildVersionFileName.c_str());
	ofileBuild << "#ifndef _RDO_STUDIO_RES_BUILD_VERSION_H_" << std::endl;
	ofileBuild << "#define _RDO_STUDIO_RES_BUILD_VERSION_H_" << std::endl << std::endl;
	ofileBuild << "static const QString g_buildVersion(\"svn-" << rev << "\");" << std::endl << std::endl;
	ofileBuild << "#endif // _RDO_STUDIO_RES_BUILD_VERSION_H_" << std::endl;
	ofileBuild.close();
}
