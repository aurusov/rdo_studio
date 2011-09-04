#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

typedef std::string  tstring;
typedef unsigned int ruint;

void main(int paramCount, char* params[])
{
	//if (paramCount != 2)
	//{
	//	std::cerr << "using version.exe <rev.number>" << std::endl;
	//	return;
	//}

	//std::stringstream revStr;
	//revStr << params[1];
	//ruint rev;
	//revStr >> rev;
	//const tstring versionFileName = "C:/rdo/rdo_studio/rdo_studio/res/version.h";

	ruint rev;
	std::cin >> rev;
	const tstring versionFileName = "../app/rdo_studio_mfc/res/version.h";

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
}
