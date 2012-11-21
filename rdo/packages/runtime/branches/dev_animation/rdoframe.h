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
#include "utils/rdoanimation.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFRMShow);
PREDECLARE_POINTER(RDOFRMItem);

/*!
  \class     RDOFRMSprite
  \brief     Спрайт. Реализует основные команды анимации
*/
OBJECT(RDOFRMSprite)
	IS  INSTANCE_OF(RDORuntimeObject)
	AND INSTANCE_OF(RDOSrcInfo      )
{
DECLARE_FACTORY(RDOFRMSprite)
public:
	typedef std::list<tstring> ImageNameList;

	/*!
	  \class     RDOFRMPosition
	  \brief     Позиция кадра
	*/
	OBJECT(RDOFRMPosition) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(RDOFRMPosition)
	public:
		/*!
		  \enum      PositionType
		  \brief     Тип позици кадра
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
	OBJECT(RDOFRMColor) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(RDOFRMColor)
	public:
		/*!
		  \enum      ColorType
		  \brief     Цвет фигуры
		*/
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
	  \brief     Объект-цвет
	*/
	OBJECT(RDOFRMRulet)
		 IS  INSTANCE_OF(RDORuntimeObject)
		 AND INSTANCE_OF(RDOSrcInfo      )
	{
	DECLARE_FACTORY(RDOFRMRulet)
	public:
		ruint getIndex() const;
		CREF(LPRDOFRMPosition) getX() const;
		CREF(LPRDOFRMPosition) getY() const;

	private:
		RDOFRMRulet(CREF(RDOSrcInfo) src_info, ruint index, CREF(LPRDOFRMPosition) pX, CREF(LPRDOFRMPosition) pY);
		virtual ~RDOFRMRulet();

		ruint             m_index;
		LPRDOFRMPosition  m_pX;
		LPRDOFRMPosition  m_pY;
	};

public:
	LPRDOFRMShow  getLastShow       () const;
	CREF(tstring) name              () const;

	void          setBackgroundColor(CREF(LPRDOFRMColor) pBgColor   );
	void          startShow         (CREF(LPRDOCalc) pCalc = NULL   );
	void          addItem           (CREF(LPRDOFRMItem)  pItem      );
	void          addRulet          (CREF(LPRDOFRMRulet) pRulet     );
	rbool         checkCondition    (CREF(LPRDORuntime)  pRuntime   );

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
	RDOFRMSprite(CREF(RDOSrcInfo) src_info, CREF(LPRDOCalc) pConditionCalc = NULL);
	virtual ~RDOFRMSprite();

private:
	typedef std::list<LPRDOFRMShow>        ShowList;
	typedef std::map<ruint, LPRDOFRMRulet> RuletList;

protected:
	LPRDOCalc              m_pConditionCalc;
	LPRDOFRMColor          m_pBgColor;

	ShowList               m_showList;
	rdo::animation::Color  m_colorLastBg;
	rdo::animation::Color  m_colorLastFg;
	rdo::animation::Color  m_colorLastBgText;
	rdo::animation::Color  m_colorLastFgText;
	double                 m_lastX;
	double                 m_lastY;
	double                 m_lastWidth;
	double                 m_lastHeight;
	RuletList              m_ruletList;
};

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
  \brief     Цветной объект
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
  \brief     Базовый для всех элементов
*/
OBJECT(RDOFRMItem) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMItem)
public:
	virtual PTR(rdo::animation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime) = 0;

	virtual void getBitmaps(REF(RDOFRMSprite::ImageNameList) list);

protected:
	RDOFRMItem(CREF(LPRDOFRMSprite) pSprite);
	virtual ~RDOFRMItem();

	CREF(LPRDOFRMSprite) getFrame() const;

private:
	LPRDOFRMSprite m_pFrame;
};

/*!
  \def       DECLARE_RDOFRMIItem
  \brief     Декларация метода \a createElement
*/
#define DECLARE_RDOFRMIItem \
private:                    \
	PTR(rdo::animation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

/*!
  \def       RDOFRM_ITEM(A)
  \brief     Декларация наследника \a RDOFRMItem
*/
#define RDOFRM_ITEM(A) \
PREDECLARE_POINTER(A); \
CLASS(A): INSTANCE_OF(RDOFRMItem)

/*!
  \class     RDOFRMText
  \brief     Текстовая анимация
*/
RDOFRM_ITEM(RDOFRMText)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
{
DECLARE_FACTORY(RDOFRMText)
public:
	typedef rdo::animation::TextElement::TextAlign Align;

	void setText(Align align, CREF(LPRDOCalc) pValue);
	void setText(Align align, CREF(tstring)   text  );

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

	Align        m_align;
	LPRDOCalc    m_pValue;
	tstring      m_text;
	rbool        m_isTextString;

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMBitmapBase
  \brief     Базовый класс для картинок в анимации
*/
RDOFRM_ITEM(RDOFRMBitmapBase)
{
protected:
	virtual void getBitmaps(REF(RDOFRMSprite::ImageNameList) list);

	tstring m_pictFilename;
	tstring m_maskFilename;

protected:
	RDOFRMBitmapBase(CREF(LPRDOFRMSprite) pSprite, CREF(tstring) pictFilename, CREF(tstring) maskFilename = _T(""));
	virtual ~RDOFRMBitmapBase();
};

/*!
  \class     RDOFRMBitmap
  \brief     Картинка в анимации
*/
CLASS(RDOFRMBitmap): INSTANCE_OF(RDOFRMBitmapBase)
{
DECLARE_FACTORY(RDOFRMBitmap)
private:
	RDOFRMBitmap(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename = _T("")
	);
	virtual ~RDOFRMBitmap();

	RDOFRMSprite::LPRDOFRMPosition m_pX;
	RDOFRMSprite::LPRDOFRMPosition m_pY;

	DECLARE_RDOFRMIItem;
};

DECLARE_POINTER(RDOFRMBitmap)

/*!
  \class     RDOFRMBitmapStretch
  \brief     Растягивающаяся картинка в анимации
  \todo      что это?
*/
CLASS(RDOFRMBitmapStretch):
		INSTANCE_OF(RDOFRMBitmapBase  )
	AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMBitmapStretch)
private:
	RDOFRMBitmapStretch(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename = _T("")
	);
	virtual ~RDOFRMBitmapStretch();

	DECLARE_RDOFRMIItem;
};

DECLARE_POINTER(RDOFRMBitmapStretch);

/*!
  \class     RDOFRMRect
  \brief     Прямоугольник для анимации
*/
RDOFRM_ITEM(RDOFRMRect)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMRectRound
  \brief     Прямоугольник со скругленными углами для анимации
*/
RDOFRM_ITEM(RDOFRMRectRound)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMCircle
  \brief     Окружность для анимации
*/
RDOFRM_ITEM(RDOFRMCircle) IS INSTANCE_OF(RDOFRMColoredItem)
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMEllipse
  \brief     Эллипс для анимации
*/
RDOFRM_ITEM(RDOFRMEllipse)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMLine
  \brief     Линия для анимации
*/
RDOFRM_ITEM(RDOFRMLine) AND INSTANCE_OF(RDOFRMBoundingItem)
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMTriang
  \brief     Треугольник для анимации
*/
RDOFRM_ITEM(RDOFRMTriang) AND INSTANCE_OF(RDOFRMColoredItem)
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMActive
  \brief     Активный кадр
  \todo      что это?
*/
RDOFRM_ITEM(RDOFRMActive) AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMActive)
private:
	RDOFRMActive(
		CREF(LPRDOFRMSprite)                 pSprite,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pX,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pY,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMSprite::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       operName
	);
	virtual ~RDOFRMActive();

	tstring m_operName;

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMSpace
  \brief     Пробел
  \todo      что это?
*/
RDOFRM_ITEM(RDOFRMSpace) AND INSTANCE_OF(RDOFRMBoundingItem)
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

	DECLARE_RDOFRMIItem;
};

/*!
  \class     RDOFRMShow
  \brief     Показ кадра
  \todo      что это?
*/
OBJECT(RDOFRMShow) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMShow)
public:
	typedef std::list<LPRDOFRMItem> ItemList;

	rbool         isShowIf      () const;

	REF(ItemList) getItemList   ();
	void          insertItem    (CREF(LPRDOFRMItem) pItem            );
	rbool         checkCondition(CREF(LPRDORuntime) pRuntime         );
	virtual void  getBitmaps    (REF(RDOFRMSprite::ImageNameList) list);

private:
	RDOFRMShow(CREF(LPRDOCalc) pConditionCalc);
	virtual ~RDOFRMShow();

	ItemList  m_itemList;
	LPRDOCalc m_pConditionCalc;
};

/*!
  \class     RDOFRMFrame
  \brief     Фрейм. Формирует кадр анимации
*/
CLASS(RDOFRMFrame) : 
	INSTANCE_OF(RDOFRMSprite)
 {
 DECLARE_FACTORY(RDOFRMFrame)
 public:
	void          getBitmaps        (REF(ImageNameList) list) const;
	void          setBackPicture    (CREF(tstring)       picFileName);
	void          setBackPicture    (int width, int height          );
	PTR(rdo::animation::Frame) createFrame (CREF(LPRDORuntime) pRuntime);
	PTR(rdo::animation::Frame) prepareFrame(PTR(rdo::animation::Frame) pFrame, CREF(LPRDORuntime) pRuntime);
 private:
         RDOFRMFrame(CREF(RDOSrcInfo) src_info, CREF(LPRDOCalc) pConditionCalc = NULL);
         virtual ~RDOFRMFrame();
         tstring                m_picFileName;
         ruint                  m_width;
         ruint                  m_height;
};

DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdoframe.inl"

#endif // _LIB_RUNTIME_FRAME_H_
