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

	keys.insert(map<string, int>::value_type("ESCAPE", VK_ESCAPE));
	keys.insert(map<string, int>::value_type("TAB", VK_TAB));
	keys.insert(map<string, int>::value_type("SHIFT", VK_SHIFT));
	keys.insert(map<string, int>::value_type("CONTROL", VK_CONTROL));
	keys.insert(map<string, int>::value_type("BACK", VK_BACK));
	keys.insert(map<string, int>::value_type("RETURN", VK_RETURN));
	keys.insert(map<string, int>::value_type("INSERT", VK_INSERT));
	keys.insert(map<string, int>::value_type("HOME", VK_HOME));
	keys.insert(map<string, int>::value_type("PRIOR", VK_PRIOR));
	keys.insert(map<string, int>::value_type("DELETE", VK_DELETE));
	keys.insert(map<string, int>::value_type("END", VK_END));
	keys.insert(map<string, int>::value_type("NEXT", VK_NEXT));
	keys.insert(map<string, int>::value_type("UP", VK_UP));
	keys.insert(map<string, int>::value_type("LEFT", VK_LEFT));
	keys.insert(map<string, int>::value_type("DOWN", VK_DOWN));
	keys.insert(map<string, int>::value_type("RIGHT", VK_RIGHT));
	keys.insert(map<string, int>::value_type("DIVIDE", VK_DIVIDE));
	keys.insert(map<string, int>::value_type("MULTIPLY", VK_MULTIPLY));
	keys.insert(map<string, int>::value_type("SUBTRACT", VK_SUBTRACT));
	keys.insert(map<string, int>::value_type("ADD", VK_ADD));
	keys.insert(map<string, int>::value_type("CLEAR", VK_CLEAR));
	keys.insert(map<string, int>::value_type("SPACE", VK_SPACE));
	keys.insert(map<string, int>::value_type("DECIMAL", VK_DECIMAL));
	keys.insert(map<string, int>::value_type("F2", VK_F2));
	keys.insert(map<string, int>::value_type("F3", VK_F3));
	keys.insert(map<string, int>::value_type("F4", VK_F4));
	keys.insert(map<string, int>::value_type("F5", VK_F5));
	keys.insert(map<string, int>::value_type("F6", VK_F6));
	keys.insert(map<string, int>::value_type("F7", VK_F7));
	keys.insert(map<string, int>::value_type("F8", VK_F8));
	keys.insert(map<string, int>::value_type("F9", VK_F9));
	keys.insert(map<string, int>::value_type("F10", VK_F10));
	keys.insert(map<string, int>::value_type("F11", VK_F11));
	keys.insert(map<string, int>::value_type("F12", VK_F12));
	keys.insert(map<string, int>::value_type("NUMPAD0", VK_NUMPAD0));
	keys.insert(map<string, int>::value_type("NUMPAD1", VK_NUMPAD1));
	keys.insert(map<string, int>::value_type("NUMPAD2", VK_NUMPAD2));
	keys.insert(map<string, int>::value_type("NUMPAD3", VK_NUMPAD3));
	keys.insert(map<string, int>::value_type("NUMPAD4", VK_NUMPAD4));
	keys.insert(map<string, int>::value_type("NUMPAD5", VK_NUMPAD5));
	keys.insert(map<string, int>::value_type("NUMPAD6", VK_NUMPAD6));
	keys.insert(map<string, int>::value_type("NUMPAD7", VK_NUMPAD7));
	keys.insert(map<string, int>::value_type("NUMPAD8", VK_NUMPAD8));
	keys.insert(map<string, int>::value_type("NUMPAD9", VK_NUMPAD9));


	for(char i = '0'; i <= '9'; i++)
		keys.insert(map<string, int>::value_type(string(1, i), (int)i));

	for(i = 'A'; i <= 'Z'; i++)
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
