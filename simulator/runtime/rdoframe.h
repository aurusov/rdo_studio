/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoframe.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      07.12.2008
  \brief     Кадры РДО модели
  \indent    4T
*/

#ifndef _LIB_RUNTIME_FRAME_H_
#define _LIB_RUNTIME_FRAME_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/animation/rdoanimation.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFRMItem);

/*!
  \interface IRDOFRMItemGetBitmap
  \brief     Интерфейс запроса картинок у элемента анимации
*/
PREDECLARE_OBJECT_INTERFACE(IRDOFRMItemGetBitmap)
struct IRDOFRMItemGetBitmap: public rdo::RefCounter<IRDOFRMItemGetBitmap>
{
DECLARE_FACTORY(IRDOFRMItemGetBitmap)
public:
	typedef std::list<std::string> ImageNameList;

	virtual void getBitmaps(REF(ImageNameList) list) const = 0;

protected:
	IRDOFRMItemGetBitmap();
	virtual ~IRDOFRMItemGetBitmap();
};
#define DECLARE_IRDOFRMItemGetBitmap \
	virtual void getBitmaps(REF(ImageNameList) list) const;


/*!
  \class     RDOFRMSprite
  \brief     Спрайт. Владеет и запускает на исполнение основные команды анимации
*/
PREDECLARE_POINTER(RDOFRMSprite);
class RDOFRMSprite
	: public RDOCalc
	, public RDORuntimeObject
	, public RDOSrcInfo
	, public IRDOFRMItemGetBitmap
{
DECLARE_FACTORY(RDOFRMSprite)
public:
	/*!
	  \class     RDOFRMPosition
	  \brief     Позиция
	*/
	PREDECLARE_POINTER(RDOFRMPosition);
	class RDOFRMPosition
		: public rdo::counter_reference
		, public RDORuntimeObject
	{
	DECLARE_FACTORY(RDOFRMPosition)
	public:
		/*!
		  \enum      PositionType
		  \brief     Тип позици
		*/
		enum PositionType
		{
			PT_ABSOLUTE,
			PT_DELTA,
			PT_GABARIT,
			PT_MULT,
			PT_RULET
		};

		PositionType    getType() const;
		CREF(LPRDOCalc) getCalc() const;

		int getX     (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite);
		int getY     (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite);
		int getWidth (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite);
		int getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite);

	private:
		RDOFRMPosition(CREF(LPRDOCalc) pCalc, PositionType type = PT_ABSOLUTE, int ruletID = 0);
		RDOFRMPosition();
		virtual ~RDOFRMPosition();

		LPRDOCalc    m_pCalc;
		PositionType m_type;
		int          m_ruletID;
	};
	friend class RDOFRMPosition;

	/*!
	  \class     RDOFRMColor
	  \brief     Объект-цвет
	*/
	PREDECLARE_POINTER(RDOFRMColor);
	class RDOFRMColor
		: public rdo::counter_reference
		, public RDORuntimeObject
	{
	DECLARE_FACTORY(RDOFRMColor)
	public:
		//! @brief  Цвет фигуры
		enum ColorType
		{
			CT_NONE,
			CT_RGB,
			CT_TRANSPARENT,
			CT_LAST_BG,
			CT_LAST_FG,
			CT_LAST_BG_TEXT,
			CT_LAST_FG_TEXT
		};

		rdo::animation::Color getColor(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;

		ColorType getType() const;
		void setType(ColorType type);

	private:
		RDOFRMColor(ColorType type = CT_NONE);
		RDOFRMColor(rbyte red, rbyte green, rbyte blue, CREF(RDOSrcInfo) srcInfo);
		RDOFRMColor(CREF(LPRDOCalc) pRedCalc, CREF(LPRDOCalc) pGreenCalc, CREF(LPRDOCalc) pBlueCalc);
		virtual ~RDOFRMColor();

		ColorType m_type;
		LPRDOCalc m_pRedCalc;
		LPRDOCalc m_pGreenCalc;
		LPRDOCalc m_pBlueCalc;
	};
	friend class RDOFRMColor;

	/*!
	  \class     RDOFRMRulet
	  \brief     Рулетка. Используется для позиционирования
	*/
	PREDECLARE_POINTER(RDOFRMRulet);
	class RDOFRMRulet: public RDOCalc
	{
	DECLARE_FACTORY(RDOFRMRulet)
	public:
		ruint getIndex() const;
		CREF(LPRDOFRMPosition) getX() const;
		CREF(LPRDOFRMPosition) getY() const;

		//! @todo Добавлен для обратной совместимости
		CREF(RDOSrcInfo) src_info() const;

	private:
		RDOFRMRulet(CREF(RDOSrcInfo) src_info, ruint index, CREF(LPRDOFRMPosition) pX, CREF(LPRDOFRMPosition) pY);
		virtual ~RDOFRMRulet();

		ruint             m_index;
		LPRDOFRMPosition  m_pX;
		LPRDOFRMPosition  m_pY;

		DECLARE_ICalc;
	};

public:
	CREF(std::string) name() const;
	void insertItem(CREF(LPRDOCalc) pItem);
	void setSpriteCalc(CREF(LPRDOCalc) pSpriteCalc);

	void setColorLastBG    (RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastBg);
	void setColorLastFG    (RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastFg);
	void setColorLastBGText(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastBgText);
	void setColorLastFGText(RDOFRMColor::ColorType type, CREF(rdo::animation::Color) lastFgText);
	void setLastXY         (double x, double y);
	void setLastXYWH       (double x, double y, double width, double height);

	int getRuletX(CREF(LPRDORuntime) pRuntime, ruint ruletID) const;
	int getRuletY(CREF(LPRDORuntime) pRuntime, ruint ruletID) const;
	LPRDOFRMRulet findRulet(ruint ruletID) const;

protected:
	RDOFRMSprite(CREF(RDOSrcInfo) srcInfo);
	virtual ~RDOFRMSprite();

	RDOValue prepareFrame(CREF(LPRDORuntime) pRuntime);

	DECLARE_IRDOFRMItemGetBitmap;

private:
	typedef std::map<ruint, LPRDOFRMRulet>    RuletList;
	typedef std::list<LPIRDOFRMItemGetBitmap> GetBitmapList;

	LPRDOCalc              m_pSpriteCalc;
	rdo::animation::Color  m_colorLastBg;
	rdo::animation::Color  m_colorLastFg;
	rdo::animation::Color  m_colorLastBgText;
	rdo::animation::Color  m_colorLastFgText;
	double                 m_lastX;
	double                 m_lastY;
	double                 m_lastWidth;
	double                 m_lastHeight;
	RuletList              m_ruletList;
	GetBitmapList          m_getBitmapList;

	void insertGetBitmap(CREF(LPIRDOFRMItemGetBitmap) pGetBitmap);
	void insertRulet    (CREF(LPRDOFRMRulet)          pRulet);

	DECLARE_ICalc;
};
DECLARE_POINTER(RDOFRMSprite);

/*!
  \class     RDOFRMBoundingItem
  \brief     Объект-четырехугольник
  \details   В парсере не создается
*/
class RDOFRMBoundingItem
{
protected:
	RDOFRMBoundingItem(CREF(RDOFRMSprite::LPRDOFRMPosition) pX, CREF(RDOFRMSprite::LPRDOFRMPosition) pY, CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth, CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight);
	virtual ~RDOFRMBoundingItem();

	int getX        (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;
	int getY        (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;
	int getWidth    (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;
	int getHeight   (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;
	int getWidthAsX (CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;
	int getHeightAsY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;

private:
	RDOFRMSprite::LPRDOFRMPosition m_pX;
	RDOFRMSprite::LPRDOFRMPosition m_pY;
	RDOFRMSprite::LPRDOFRMPosition m_pWidth;
	RDOFRMSprite::LPRDOFRMPosition m_pHeight;
};

/*!
  \class     RDOFRMColoredItem
  \brief     Объект с цветом
  \details   В парсере не создается
*/
class RDOFRMColoredItem
{
public:
	CREF(RDOFRMSprite::LPRDOFRMColor) getBgColor() const;
	CREF(RDOFRMSprite::LPRDOFRMColor) getFgColor() const;

	rdo::animation::Color getBg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;
	rdo::animation::Color getFg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMSprite) pSprite) const;

protected:
	RDOFRMColoredItem(CREF(RDOFRMSprite::LPRDOFRMColor) pBgColor, CREF(RDOFRMSprite::LPRDOFRMColor) pFgColor);
	virtual ~RDOFRMColoredItem();

private:
	RDOFRMSprite::LPRDOFRMColor m_pBgColor;
	RDOFRMSprite::LPRDOFRMColor m_pFgColor;
};

/*!
  \class     RDOFRMItem
  \brief     Базовый класс для всех элементов
*/
PREDECLARE_POINTER(RDOFRMItem);
class RDOFRMItem: public RDOCalc
{
DECLARE_FACTORY(RDOFRMItem)
protected:
	RDOFRMItem(CREF(LPRDOFRMSprite) pSprite);
	virtual ~RDOFRMItem();

	CREF(LPRDOFRMSprite) getFrame() const;

private:
	LPRDOFRMSprite m_pFrame;
};

/*!
  \class     RDOFRMText
  \brief     Текст
*/
PREDECLARE_POINTER(RDOFRMText);
class RDOFRMText
	: public RDOFRMItem
	, public RDOFRMBoundingItem
	, public RDOFRMColoredItem
{
DECLARE_FACTORY(RDOFRMText)
public:
	typedef rdo::animation::TextElement::TextAlign Align;

	void setText(Align align, CREF(LPRDOCalc) pValue);
	void setText(Align align, CREF(std::string) text);

private:
	RDOFRMText(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMText();

	Align m_align;
	LPRDOCalc m_pValue;
	std::string m_text;
	rbool m_isTextString;

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMBitmapBase
  \brief     Базовый класс для картинок
*/
PREDECLARE_POINTER(RDOFRMBitmapBase);
class RDOFRMBitmapBase
	: public RDOFRMItem
	, public IRDOFRMItemGetBitmap
{
protected:
	std::string m_pictFilename;
	std::string m_maskFilename;

	RDOFRMBitmapBase(CREF(LPRDOFRMSprite) pSprite, CREF(std::string) pictFilename, CREF(std::string) maskFilename = "");
	virtual ~RDOFRMBitmapBase();

private:
	DECLARE_IRDOFRMItemGetBitmap
};

/*!
  \class     RDOFRMBitmap
  \brief     Картинка
*/
class RDOFRMBitmap: public RDOFRMBitmapBase
{
DECLARE_FACTORY(RDOFRMBitmap)
private:
	RDOFRMBitmap(
		CREF(LPRDOFRMSprite) pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(std::string) pictFilename,
		CREF(std::string) maskFilename = ""
	);
	virtual ~RDOFRMBitmap();

	RDOFRMSprite::LPRDOFRMPosition m_pX;
	RDOFRMSprite::LPRDOFRMPosition m_pY;

	DECLARE_ICalc;
};

DECLARE_POINTER(RDOFRMBitmap)

/*!
  \class     RDOFRMBitmapStretch
  \brief     Масштабируемая картинка
*/
class RDOFRMBitmapStretch
	: public RDOFRMBitmapBase
	, public RDOFRMBoundingItem
{
DECLARE_FACTORY(RDOFRMBitmapStretch)
private:
	RDOFRMBitmapStretch(
		CREF(LPRDOFRMSprite) pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(std::string) pictFilename,
		CREF(std::string) maskFilename = ""
	);
	virtual ~RDOFRMBitmapStretch();

	DECLARE_ICalc;
};

DECLARE_POINTER(RDOFRMBitmapStretch);

/*!
  \class     RDOFRMRect
  \brief     Объект прямоугольник
*/
PREDECLARE_POINTER(RDOFRMRect);
class RDOFRMRect
	: public RDOFRMItem
	, public RDOFRMBoundingItem
	, public RDOFRMColoredItem
{
DECLARE_FACTORY(RDOFRMRect)
private:
	RDOFRMRect(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMRect();

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMRectRound
  \brief     Прямоугольник со скругленными углами
*/
PREDECLARE_POINTER(RDOFRMRectRound);
class RDOFRMRectRound
	: public RDOFRMItem
	, public RDOFRMBoundingItem
	, public RDOFRMColoredItem
{
DECLARE_FACTORY(RDOFRMRectRound)
private:
	RDOFRMRectRound(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMRectRound();

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMCircle
  \brief     Окружность
*/
PREDECLARE_POINTER(RDOFRMCircle);
class RDOFRMCircle
	: public RDOFRMItem
	, public RDOFRMColoredItem
{
DECLARE_FACTORY(RDOFRMCircle)
private:
	RDOFRMCircle(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pRadius,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMCircle();

	RDOFRMSprite::LPRDOFRMPosition m_pX;
	RDOFRMSprite::LPRDOFRMPosition m_pY;
	RDOFRMSprite::LPRDOFRMPosition m_pRadius;

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMEllipse
  \brief     Эллипс
*/
PREDECLARE_POINTER(RDOFRMEllipse);
class RDOFRMEllipse
	: public RDOFRMItem
	, public RDOFRMBoundingItem
	, public RDOFRMColoredItem
{
DECLARE_FACTORY(RDOFRMEllipse)
private:
	RDOFRMEllipse(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMEllipse();

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMLine
  \brief     Линия
*/
PREDECLARE_POINTER(RDOFRMLine);
class RDOFRMLine
	: public RDOFRMItem
	, public RDOFRMBoundingItem
{
DECLARE_FACTORY(RDOFRMLine)
private:
	RDOFRMLine(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX2,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY2,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pColor
	);
	virtual ~RDOFRMLine();

	RDOFRMSprite::LPRDOFRMColor m_pColor;

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMTriang
  \brief     Треугольник
*/
PREDECLARE_POINTER(RDOFRMTriang);
class RDOFRMTriang
	: public RDOFRMItem
	, public RDOFRMColoredItem
{
DECLARE_FACTORY(RDOFRMTriang)
private:
	RDOFRMTriang(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY1,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX2,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY2,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX3,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY3,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMSprite::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMTriang();

	RDOFRMSprite::LPRDOFRMPosition m_pX1;
	RDOFRMSprite::LPRDOFRMPosition m_pY1;
	RDOFRMSprite::LPRDOFRMPosition m_pX2;
	RDOFRMSprite::LPRDOFRMPosition m_pY2;
	RDOFRMSprite::LPRDOFRMPosition m_pX3;
	RDOFRMSprite::LPRDOFRMPosition m_pY3;

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMActive
  \brief     Активная область
*/
PREDECLARE_POINTER(RDOFRMActive);
class RDOFRMActive
	: public RDOFRMItem
	, public RDOFRMBoundingItem
{
DECLARE_FACTORY(RDOFRMActive)
private:
	RDOFRMActive(
		CREF(LPRDOFRMSprite) pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(std::string) operName
	);
	virtual ~RDOFRMActive();

	std::string m_operName;

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMSpace
  \brief     Пустота
*/
PREDECLARE_POINTER(RDOFRMSpace);
class RDOFRMSpace
	: public RDOFRMItem
	, public RDOFRMBoundingItem
{
DECLARE_FACTORY(RDOFRMSpace)
private:
	RDOFRMSpace(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight
	);
	virtual ~RDOFRMSpace();

	DECLARE_ICalc;
};

/*!
  \class     RDOFRMFrame
  \brief     Фрейм. Формирует кадр анимации
*/
class RDOFRMFrame: public RDOFRMSprite
{
DECLARE_FACTORY(RDOFRMFrame)
public:
	void setBackPicture(CREF(std::string) picFileName);
	void setBackPicture(int width, int height);

	void prepareFrame(PTR(rdo::animation::Frame) pFrame, CREF(LPRDORuntime) pRuntime);

	void setBackgroundColor(CREF(LPRDOFRMColor) pBgColor);

	//! @todo Нужно спрятатть в приват и поправить симулятор, из которого метод вызывается
	DECLARE_IRDOFRMItemGetBitmap;

private:
	RDOFRMFrame(CREF(RDOSrcInfo) srcInfo);
	virtual ~RDOFRMFrame();

	LPRDOFRMColor m_pBgColor;
	std::string m_picFileName;
	ruint m_width;
	ruint m_height;

	DECLARE_ICalc;
};
DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdoframe.inl"

#endif // _LIB_RUNTIME_FRAME_H_
