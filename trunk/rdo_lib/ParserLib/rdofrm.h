#ifndef RDOFRM_FRM
#define RDOFRM_FRM

//#include "rdoStdFuncs.h"
//#include "rdortp.h"

#include <rdocommon.h>

namespace rdoRuntime
{
class RDOCalc;
class RDORuntime;
/*
class RDOCalcConst;
class RDOCalcIsEqual;
class RDOCalcFuncParam;
class RDOFunCalc;
class RDOCalcSeqInit;
class RDOCalcSeqNext;
*/
}

using namespace rdoRuntime;

namespace rdoParse 
{

int frmparse( void* lexer );
int frmlex( int* lpval, void* lexer );
void frmerror( char* mes );

class RDOFRMColor
{
public:
	bool isTransparent;
	int red, green, blue;
	RDOFRMColor();
	RDOFRMColor(int r, int g, int b);
};

class RDOFRMItem
{
protected:
	RDOFRMItem() {}
public:
	virtual ~RDOFRMItem() {}
	virtual RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
	virtual void getAllBitmaps(std::vector<const std::string *> &vect) {}
};

class RDOFRMBoundingItem: public virtual RDOFRMItem
{
protected:
	RDOFRMBoundingItem(RDOFUNArithm *_x, RDOFUNArithm *_y, RDOFUNArithm *_width, RDOFUNArithm *_height);
public:
	RDOCalc *x, *y, *width, *height;
	virtual ~RDOFRMBoundingItem() {}
};

class RDOFRMColoredItem: public virtual RDOFRMItem
{
protected:
	RDOFRMColoredItem(RDOFRMColor *_bgColor, RDOFRMColor *_color);
public:
	RDOFRMColor bgColor;
	RDOFRMColor color;
	virtual ~RDOFRMColoredItem() {}
	RDOSimulatorNS::RDOColor getBg();
	RDOSimulatorNS::RDOColor getFg();
};

struct RDORTPEnum;
class RDOFRMText: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
	int align;
	RDOCalc *value;
	std::string *txt;
	bool isTextString;
	int type;
	RDORTPEnum *enu;
public:
	RDOFRMText(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color);
	void setText(int _align, RDOFUNArithm *_value);
	void setText(int _align, std::string *_txt);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMBitmap: public RDOFRMItem
{
	RDOCalc *x;
	RDOCalc *y;
	std::string *picFileName;
	std::string *mask;
public:
	RDOFRMBitmap(RDOFUNArithm *_x, RDOFUNArithm *_y, std::string *_picFileName, std::string *_mask = NULL);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
	void getAllBitmaps(std::vector<const std::string *> &vect);
};

class RDOFRMS_bmp: public RDOFRMBoundingItem
{
	std::string *picFileName;
	std::string *mask;
public:
	RDOFRMS_bmp(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, std::string *_picFileName, std::string *_mask = NULL);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
	void getAllBitmaps(std::vector<const std::string *> &vect);
};

class RDOFRMRect: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
public:
	RDOFRMRect(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMR_rect: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
public:
	RDOFRMR_rect(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMLine: public RDOFRMBoundingItem
{
	RDOFRMColor color;
public:
	RDOFRMLine(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *_color);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMEllipse: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
public:
	RDOFRMEllipse(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, RDOFRMColor *bgColor, RDOFRMColor *color);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMTriang: public RDOFRMColoredItem
{
	RDOCalc *x1, *y1, *x2, *y2, *x3, *y3;
public:
	RDOFRMTriang(RDOFUNArithm *_x1, RDOFUNArithm *_y1, RDOFUNArithm *_x2, RDOFUNArithm *_y2, RDOFUNArithm *_x3, RDOFUNArithm *_y3, RDOFRMColor *bgColor, RDOFRMColor *color);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMActive: public RDOFRMBoundingItem
{
	std::string *operName;
public:
	RDOFRMActive(RDOFUNArithm *x, RDOFUNArithm *y, RDOFUNArithm *width, RDOFUNArithm *height, std::string *_operName);
	RDOSimulatorNS::RDOFrameElement* createElement(RDORuntime *sim);
};

class RDOFRMFrame;
class RDOFRMShow
{
	friend RDOFRMFrame;
	RDOCalc *conditionCalc;
	std::vector<RDOFRMItem *> items;
public:
	RDOFRMShow(RDOFUNLogic* logic);
	virtual ~RDOFRMShow();
	bool checkCondition(RDORuntime *sim);
	void getAllBitmaps(std::vector<const std::string *> &vect);
};

class RDOFRMFrame
{
	std::string *name;
	RDOCalc *conditionCalc;
	int r, g, b;	// background
	bool hasBackPicture;
	std::string* picFileName;	// back picture
	int width, height;	// frame size
	std::vector<RDOFRMShow *> shows;
public:
	RDOFRMFrame(std::string *_name, RDOFUNLogic *_logic = NULL);
	void setBackground(int _r = 0, int _g = 100, int _b = 0);
	void setBackPicture(std::string* _picFileName);
	void setBackPicture(int _width, int _height);
	void startShow(RDOFUNLogic* logic = NULL);
	void addItem(RDOFRMItem *item);
	void end();
	virtual ~RDOFRMFrame();
	bool checkCondition(RDORuntime *sim);
	RDOSimulatorNS::RDOFrame* createFrame(RDORuntime *sim);
	std::string *getName() { return name; }
	void getAllBitmaps(std::vector<const std::string *> &vect);
};



}		// namespace rdoParse 

#endif //RDOFRM_FRM
