#ifndef RDOHOT_KEY
#define RDOHOT_KEY

using namespace std;

namespace rdoParse 
{
/*
const int RDOESCAPE	= 0x101;
const int RDOTAB		= 0x10F;
const int RDOSHIFT	= 0x12A;		// Left
const int RDOCONTROL	= 0x11D;		// Left or Right
const int RDOBACK		= 0x10E;
const int RDORETURN	= 0x11C;
const int RDOINSERT	= 0x152;
const int RDOHOME		= 0x024;
const int RDOPRIOR	= 0x149;
const int RDODELETE	= 0x153;
const int RDOEND		= 0x14F;
const int RDONEXT		= 0x151;
const int RDOUP		= 0x026;
const int RDOLEFT		= 0x025;
const int RDODOWN		= 0x028;
const int RDORIGHT	= 0x027;
const int RDODIVIDE	= 0x135;
const int RDOMULTIPLY= 0x137;		// definetly Print Screen, if somebody know 'Gray*' scan code, tell me
const int RDOSUBTRACT= 0x06D;
const int RDOADD		= 0x06B;
const int RDOCLEAR	= 0x14C;		// I think so
const int RDOSPACE	= VK_SPACE;
const int RDODECIMAL	= 0x134;		// I hope...
const int RDOF2		= 0x13C;
const int RDOF3		= 0x13D;
const int RDOF4		= 0x13E;
const int RDOF5		= 0x13F;
const int RDOF6		= 0x140;
const int RDOF7		= 0x141;
const int RDOF8		= 0x142;
const int RDOF9		= 0x143;
const int RDOF10		= 0x144;
const int RDOF11		= 0x157;
const int RDOF12		= 0x158;
const int RDONUMPAD0	= 0x152;
const int RDONUMPAD1	= 0x14F;
const int RDONUMPAD2	= 0x150;
const int RDONUMPAD3	= 0x151;
const int RDONUMPAD4	= 0x14B;
const int RDONUMPAD5	= 0x14C;
const int RDONUMPAD6	= 0x14D;
const int RDONUMPAD7	= 0x147;
const int RDONUMPAD8	= 0x148;
const int RDONUMPAD9	= 0x149;
*/
class RDOHotKeyToolkit
{
	map<string, int> keys;
public:
	RDOHotKeyToolkit();
	int codeFromString(string *key);
};

extern RDOHotKeyToolkit rdoHotKeyToolkit;

}		// namespace rdoParse 

#endif //RDOHOT_KEY
				 