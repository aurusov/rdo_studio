#ifndef RDOFRAME_H
#define RDOFRAME_H

#include "rdocalc.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOFRMBoundingItem - объект-четырехугольник
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMBoundingItem
{
protected:
	RDOCalc* x;
	RDOCalc* y;
	RDOCalc* width;
	RDOCalc* height;

	RDOFRMBoundingItem( RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height ):
		x( _x ),
		y( _y ),
		width( _width ),
		height( _height )
	{
	}
	virtual ~RDOFRMBoundingItem()
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMColor - объект-цвет
// ----------------------------------------------------------------------------
// Создается в парсере, в конструкторе графического объекта создается объект-копия,
// а не указатель, после чего созданный в парсере объек-цвет удаляется
// ----------------------------------------------------------------------------
class RDOFRMColor: public RDORuntimeObject
{
protected:
	bool isTransparent;
	int  red;
	int  green;
	int  blue;

public:
	RDOFRMColor( RDORuntimeParent* _parent ):
		RDORuntimeObject( _parent ),
		isTransparent( true ),
		red( 0 ),
		green( 0 ),
		blue( 0 )
	{
	}
	RDOFRMColor( RDORuntimeParent* _parent, int _red, int _green, int _blue ):
		RDORuntimeObject( _parent ),
		isTransparent( false ),
		red( _red ),
		green( _green ),
		blue( _blue )
	{
	}

	rdoSimulator::RDOColor getColor() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMColoredItem - цветной объект
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMColoredItem
{
protected:
	RDOFRMColor* fgColor;
	RDOFRMColor* bgColor;

	RDOFRMColoredItem( RDOFRMColor* _bgColor, RDOFRMColor* _fgColor ):
		bgColor( _bgColor ),
		fgColor( _fgColor )
	{
	}
	virtual ~RDOFRMColoredItem() {}

	void color_reparent( RDORuntimeParent* _parent ) {
		fgColor->reparent( _parent );
		bgColor->reparent( _parent );
	}

public:
	rdoSimulator::RDOColor getFg() { return fgColor->getColor(); }
	rdoSimulator::RDOColor getBg() { return bgColor->getColor(); }
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMItem - базовый для всех элементов
// ----------------------------------------------------------------------------
class RDOFRMItem: public RDORuntimeParent
{
friend class RDOFRMShow;
friend class RDOFRMFrame;

protected:
	RDOFRMItem( RDORuntimeParent* _parent ): RDORuntimeParent( _parent ) {}
	virtual ~RDOFRMItem()                                                {}

	virtual void getBitmaps( std::list< std::string >& list )            {}
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim ) = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
class RDORTPEnum;

class RDOFRMText: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
private:
	int                  align;
	RDOCalc* value;
	RDORTPEnum*          enu;
	std::string          txt;
	bool                 isTextString;

protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMText( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color );
	void setText( int _align, RDOCalc* _value, RDORTPEnum* _enu );
	void setText( int _align, std::string* _txt );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
class RDOFRMBitmap: public RDOFRMItem
{
private:
	RDOCalc* x;
	RDOCalc* y;
	std::string picFileName;
	std::string mask;

protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );
	virtual void getBitmaps( std::list< std::string >& list );

public:
	RDOFRMBitmap( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, std::string* _picFileName, std::string* _mask = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMS_bmp
// ----------------------------------------------------------------------------
class RDOFRMS_bmp: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	std::string picFileName;
	std::string mask;

protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );
	virtual void getBitmaps( std::list< std::string >& list );

public:
	RDOFRMS_bmp( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, std::string* _picFileName, std::string* _mask = NULL );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
class RDOFRMRect: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMRect( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMR_rect
// ----------------------------------------------------------------------------
class RDOFRMR_rect: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMR_rect( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
class RDOFRMLine: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	RDOFRMColor* color;

protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMLine( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* _color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
class RDOFRMEllipse: public RDOFRMItem, public RDOFRMBoundingItem, public RDOFRMColoredItem
{
protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMEllipse( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, RDOFRMColor* bgColor, RDOFRMColor* color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
class RDOFRMTriang: public RDOFRMItem, public RDOFRMColoredItem
{
private:
	RDOCalc* x1;
	RDOCalc* y1;
	RDOCalc* x2;
	RDOCalc* y2;
	RDOCalc* x3;
	RDOCalc* y3;

protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMTriang( RDORuntimeParent* _parent, RDOCalc* _x1, RDOCalc* _y1, RDOCalc* _x2, RDOCalc* _y2, RDOCalc* _x3, RDOCalc* _y3, RDOFRMColor* bgColor, RDOFRMColor* color );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
class RDOFRMActive: public RDOFRMItem, public RDOFRMBoundingItem
{
private:
	std::string operName;

protected:
	virtual rdoSimulator::RDOFrameElement* createElement( RDORuntime* sim );

public:
	RDOFRMActive( RDORuntimeParent* _parent, RDOCalc* _x, RDOCalc* _y, RDOCalc* _width, RDOCalc* _height, std::string* _operName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
// В списке потомков RDOFRMShow должны находится только RDOFRMItem.
// Такова текущая логика добавления объектов к RDOFRMShow.
// Приведение RDORuntimeObject к RDOFRMItem происходит без проверок.
// ----------------------------------------------------------------------------
class RDOFRMShow: public RDORuntimeParent
{
friend class RDOFRMFrame;

private:
	RDOCalc* conditionCalc;

public:
	RDOFRMShow( RDORuntimeParent* _parent, RDOCalc* _conditionCalc );
	virtual ~RDOFRMShow();

	bool checkCondition( RDORuntime* sim );
	virtual void getBitmaps( std::list< std::string >& list );
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
class RDOFRMFrame: public RDORuntimeParent
{
private:
	std::string name;
	RDOCalc* conditionCalc;
	int r, g, b;                 // background
	bool hasBackPicture;
	std::string picFileName;     // back picture
	int width, height;           // frame size
	std::list< RDOFRMShow* > shows;

public:
	RDOFRMFrame( RDORuntime* _runtime, std::string* _name, RDOCalc* _conditionCalc = NULL );
	virtual ~RDOFRMFrame();
	void setBackground( int _r = 0, int _g = 100, int _b = 0 );
	void setBackPicture( std::string* _picFileName );
	void setBackPicture( int _width, int _height );
	void startShow( RDOCalc* calc = NULL );
	void addItem( RDOFRMItem* item );
	void end();
	bool checkCondition( RDORuntime* sim );
	rdoSimulator::RDOFrame* createFrame( RDORuntime* sim ) {
		rdoSimulator::RDOFrame* frame = new rdoSimulator::RDOFrame();
		return prepareFrame( frame, sim );
	}
	rdoSimulator::RDOFrame* prepareFrame( rdoSimulator::RDOFrame* frame, RDORuntime* sim );
	std::string getName() { return name; }
	virtual void getBitmaps( std::list< std::string >& list );
};

} // namespace rdoRuntime

#endif // RDOFRAME_H
