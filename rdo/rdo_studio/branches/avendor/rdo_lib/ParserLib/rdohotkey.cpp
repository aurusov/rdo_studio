#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdohotkey.h"
#include "rdoparser.h"

namespace rdoParse 
{

RDOHotKeyToolkit rdoHotKeyToolkit;

RDOHotKeyToolkit::RDOHotKeyToolkit()
{

	keys.insert(map<string, int>::value_type("ESCAPE", RDOESCAPE));
	keys.insert(map<string, int>::value_type("TAB", RDOTAB));
	keys.insert(map<string, int>::value_type("SHIFT", RDOSHIFT));
	keys.insert(map<string, int>::value_type("CONTROL", RDOCONTROL));
	keys.insert(map<string, int>::value_type("BACK", RDOBACK));
	keys.insert(map<string, int>::value_type("RETURN", RDORETURN));
	keys.insert(map<string, int>::value_type("INSERT", RDOINSERT));
	keys.insert(map<string, int>::value_type("HOME", RDOHOME));
	keys.insert(map<string, int>::value_type("PRIOR", RDOPRIOR));
	keys.insert(map<string, int>::value_type("DELETE", RDODELETE));
	keys.insert(map<string, int>::value_type("END", RDOEND));
	keys.insert(map<string, int>::value_type("NEXT", RDONEXT));
	keys.insert(map<string, int>::value_type("UP", RDOUP));
	keys.insert(map<string, int>::value_type("LEFT", RDOLEFT));
	keys.insert(map<string, int>::value_type("DOWN", RDODOWN));
	keys.insert(map<string, int>::value_type("RIGHT", RDORIGHT));
	keys.insert(map<string, int>::value_type("DIVIDE", RDODIVIDE));
	keys.insert(map<string, int>::value_type("MULTIPLY", RDOMULTIPLY));
	keys.insert(map<string, int>::value_type("SUBTRACT", RDOSUBTRACT));
	keys.insert(map<string, int>::value_type("ADD", RDOADD));
	keys.insert(map<string, int>::value_type("CLEAR", RDOCLEAR));
	keys.insert(map<string, int>::value_type("SPACE", RDOSPACE));
	keys.insert(map<string, int>::value_type("DECIMAL", RDODECIMAL));
	keys.insert(map<string, int>::value_type("F2", RDOF2));
	keys.insert(map<string, int>::value_type("F3", RDOF3));
	keys.insert(map<string, int>::value_type("F4", RDOF4));
	keys.insert(map<string, int>::value_type("F5", RDOF5));
	keys.insert(map<string, int>::value_type("F6", RDOF6));
	keys.insert(map<string, int>::value_type("F7", RDOF7));
	keys.insert(map<string, int>::value_type("F8", RDOF8));
	keys.insert(map<string, int>::value_type("F9", RDOF9));
	keys.insert(map<string, int>::value_type("F10", RDOF10));
	keys.insert(map<string, int>::value_type("F11", RDOF11));
	keys.insert(map<string, int>::value_type("F12", RDOF12));
	keys.insert(map<string, int>::value_type("NUMPAD0", RDONUMPAD0));
	keys.insert(map<string, int>::value_type("NUMPAD1", RDONUMPAD1));
	keys.insert(map<string, int>::value_type("NUMPAD2", RDONUMPAD2));
	keys.insert(map<string, int>::value_type("NUMPAD3", RDONUMPAD3));
	keys.insert(map<string, int>::value_type("NUMPAD4", RDONUMPAD4));
	keys.insert(map<string, int>::value_type("NUMPAD5", RDONUMPAD5));
	keys.insert(map<string, int>::value_type("NUMPAD6", RDONUMPAD6));
	keys.insert(map<string, int>::value_type("NUMPAD7", RDONUMPAD7));
	keys.insert(map<string, int>::value_type("NUMPAD8", RDONUMPAD8));
	keys.insert(map<string, int>::value_type("NUMPAD9", RDONUMPAD9));


	for(char i = '0'; i < '9'; i++)
		keys.insert(map<string, int>::value_type(string(1, i), (int)i));

	for(i = 'A'; i < 'Z'; i++)
		keys.insert(map<string, int>::value_type(string(1, i), (int)i));

}

int RDOHotKeyToolkit::codeFromString(string *key)
{
	map<string, int>::iterator it = keys.find(*key);
	if(it == keys.end())
		currParser->error("Unknown key name: " + *key);

	return (*it).second;
}


}		// namespace rdoParse 
