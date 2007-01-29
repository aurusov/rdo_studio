#ifndef RDOFRM_FRM
#define RDOFRM_FRM

#include "rdoparser_object.h"
#include <rdocommon.h>
#include <rdortp.h>

namespace rdoRuntime
{
class RDOCalc;
class RDORuntime;
}

namespace rdoParse 
{

int frmparse( void* lexer );
int frmlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void frmerror( char* mes );

class RDOFRMColor: public RDODeletable
{
public:
	bool isTransparent;
	int red, green, blue;
	RDOFRMColor();
	RDOFRMColor(int r, int g, int b);
};

class RDOFRMItem: public RDOParserObject
{
protected:
	RDOFRMItem( const const RDOParserObject* _parent ): RDOParserObject( _parent ) {}

public:
	virtual ~RDOFRMItem() {}
	virtual rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
	virtual void getAllBitmaps( std::list< std::string >& list ) {}
};

class RDOFRMBoundingItem: public virtual RDOFRMItem
{
protected:
	RDOFRMBoundingItem( const RDOParserObject* _parent, RDOFUNArithm* _x, RDOFUNArithm* _y, RDOFUNArithm* _width, RDOFUNArithm* _height );

public:
	rdoRuntime::RDOCalc *x, *y, *width, *height;
	virtual ~RDOFRMBoundingItem() {}
};

class RDOFRMColoredItem: public virtual RDOFRMItem
{
protected:
	RDOFRMColoredItem( const RDOParserObject* _parent, RDOFRMColor* _bgColor, RDOFRMColor* _color );

public:
	RDOFRMColor bgColor;
	RDOFRMColor color;
	virtual ~RDOFRMColoredItem() {}
	rdoSimulator::RDOColor getBg();
	rdoSimulator::RDOColor getFg();
};

class RDORTPEnum;
class RDOFRMText: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
private:
	int                       align;
	rdoRuntime::RDOCalc*      value;
	std::string*              txt;
	bool                      isTextString;
	RDORTPResParam::ParamType type;
	RDORTPEnum*               enu;

public:
	RDOFRMText( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, RDOFRMColor* bgColor, RDOFRMColor* color );
	void setText( int _align, RDOFUNArithm* _value );
	void setText( int _align, std::string* _txt );
	virtual rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMBitmap: public RDOFRMItem
{
private:
	rdoRuntime::RDOCalc *x;
	rdoRuntime::RDOCalc *y;
	std::string *picFileName;
	std::string *mask;

public:
	RDOFRMBitmap( const RDOParserObject* _parent, RDOFUNArithm* _x, RDOFUNArithm* _y, std::string* _picFileName, std::string* _mask = NULL );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
	virtual void getAllBitmaps( std::list< std::string >& list );
};

class RDOFRMS_bmp: public RDOFRMBoundingItem
{
private:
	std::string *picFileName;
	std::string *mask;

public:
	RDOFRMS_bmp( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, std::string* _picFileName, std::string* _mask = NULL );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
	virtual void getAllBitmaps( std::list< std::string >& list );
};

class RDOFRMRect: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
public:
	RDOFRMRect( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, RDOFRMColor* bgColor, RDOFRMColor* color );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMR_rect: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
public:
	RDOFRMR_rect( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, RDOFRMColor* bgColor, RDOFRMColor* color );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMLine: public RDOFRMBoundingItem
{
private:
	RDOFRMColor color;

public:
	RDOFRMLine( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, RDOFRMColor* _color );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMEllipse: public RDOFRMBoundingItem, public RDOFRMColoredItem
{
public:
	RDOFRMEllipse( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, RDOFRMColor* bgColor, RDOFRMColor* color );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMTriang: public RDOFRMColoredItem
{
private:
	rdoRuntime::RDOCalc *x1, *y1, *x2, *y2, *x3, *y3;

public:
	RDOFRMTriang( const RDOParserObject* _parent, RDOFUNArithm* _x1, RDOFUNArithm* _y1, RDOFUNArithm* _x2, RDOFUNArithm* _y2, RDOFUNArithm* _x3, RDOFUNArithm* _y3, RDOFRMColor* bgColor, RDOFRMColor* color );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMActive: public RDOFRMBoundingItem
{
private:
	std::string *operName;

public:
	RDOFRMActive( const RDOParserObject* _parent, RDOFUNArithm* x, RDOFUNArithm* y, RDOFUNArithm* width, RDOFUNArithm* height, std::string* _operName );
	rdoSimulator::RDOFrameElement* createElement( rdoRuntime::RDORuntime* sim );
};

class RDOFRMFrame;

class RDOFRMShow: public RDODeletable
{
private:
	friend RDOFRMFrame;
	rdoRuntime::RDOCalc *conditionCalc;
	std::vector<RDOFRMItem *> items;

public:
	RDOFRMShow(RDOFUNLogic* logic);
	virtual ~RDOFRMShow();
	bool checkCondition( rdoRuntime::RDORuntime* sim );
	virtual void getAllBitmaps( std::list< std::string >& list );
};

class RDOFRMFrame: public RDOParserObject
{
private:
	std::string *name;
	rdoRuntime::RDOCalc *conditionCalc;
	int r, g, b;	// background
	bool hasBackPicture;
	std::string* picFileName;	// back picture
	int width, height;	// frame size
	std::vector<RDOFRMShow *> shows;

public:
	RDOFRMFrame( RDOParser* _parser, std::string* _name, RDOFUNLogic* _logic = NULL );
	void setBackground(int _r = 0, int _g = 100, int _b = 0);
	void setBackPicture(std::string* _picFileName);
	void setBackPicture(int _width, int _height);
	void startShow(RDOFUNLogic* logic = NULL);
	void addItem(RDOFRMItem *item);
	void end();
	virtual ~RDOFRMFrame();
	bool checkCondition( rdoRuntime::RDORuntime* sim );
	rdoSimulator::RDOFrame* createFrame( rdoRuntime::RDORuntime* sim ) {
		rdoSimulator::RDOFrame* frame = new rdoSimulator::RDOFrame();
		return prepareFrame( frame, sim );
	}
	rdoSimulator::RDOFrame* prepareFrame( rdoSimulator::RDOFrame* frame, rdoRuntime::RDORuntime* sim );
	std::string *getName() { return name; }
	virtual void getAllBitmaps( std::list< std::string >& list );
};

} // namespace rdoParse 

#endif // RDOFRM_FRM
