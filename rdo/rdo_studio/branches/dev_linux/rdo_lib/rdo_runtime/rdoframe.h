/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdoframe.h
 * author   : Урусов Андрей
 * date     : 07.12.2008
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOFRAME_H_
#define _RDOFRAME_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdoanimation.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDOFRMShow);
PREDECLARE_POINTER(RDOFRMItem);

OBJECT(RDOFRMFrame)
	IS  INSTANCE_OF(RDORuntimeObject)
	AND INSTANCE_OF(RDOSrcInfo      )
{
DECLARE_FACTORY(RDOFRMFrame)
public:
	typedef std::list<tstring> ImageNameList;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMPosition
	// ----------------------------------------------------------------------------
	OBJECT(RDOFRMPosition) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(RDOFRMPosition)
	public:
		enum PositionType
		{
			PT_ABSOLUTE,
			PT_DELTA,
			PT_GABARIT,
			PT_MULT,
			PT_RULET
		};

		PositionType getType() const
		{
			return m_type;
		}

		CREF(LPRDOCalc) getCalc() const
		{
			return m_pCalc;
		}

		int getX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = m_pCalc->calcValue(pRuntime);
			switch (m_type)
			{
				case RDOFRMPosition::PT_DELTA  : res += pFrame->m_lastX;                        break;
				case RDOFRMPosition::PT_GABARIT: res += pFrame->m_lastX + pFrame->m_lastWidth;  break;
				case RDOFRMPosition::PT_MULT   : res *= pFrame->m_lastX;                        break;
				case RDOFRMPosition::PT_RULET  : res += pFrame->getRuletX(pRuntime, m_ruletID); break;
			}
			return res.getInt();
		}
		int getY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = m_pCalc->calcValue(pRuntime);
			switch (m_type)
			{
				case RDOFRMPosition::PT_DELTA  : res += pFrame->m_lastY;                        break;
				case RDOFRMPosition::PT_GABARIT: res += pFrame->m_lastY + pFrame->m_lastHeight; break;
				case RDOFRMPosition::PT_MULT   : res *= pFrame->m_lastY;                        break;
				case RDOFRMPosition::PT_RULET  : res += pFrame->getRuletY(pRuntime, m_ruletID); break;
			}
			return res.getInt();
		}
		int getWidth(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = m_pCalc->calcValue(pRuntime);
			switch (m_type)
			{
				case RDOFRMPosition::PT_DELTA: res += pFrame->m_lastWidth; break;
				case RDOFRMPosition::PT_MULT : res *= pFrame->m_lastWidth; break;
				case RDOFRMPosition::PT_RULET: res += pFrame->getRuletX(pRuntime, m_ruletID); break;
			}
			return res.getInt();
		}
		int getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame)
		{
			RDOValue res = m_pCalc->calcValue(pRuntime);
			switch (m_type)
			{
				case RDOFRMPosition::PT_DELTA: res += pFrame->m_lastHeight; break;
				case RDOFRMPosition::PT_MULT : res *= pFrame->m_lastHeight; break;
				case RDOFRMPosition::PT_RULET: res += pFrame->getRuletY(pRuntime, m_ruletID); break;
			}
			return res.getInt();
		}
	private:
		RDOFRMPosition()
			: RDORuntimeObject()
			, m_type   (PT_ABSOLUTE)
			, m_ruletID(0          )
		{}

		RDOFRMPosition(CREF(LPRDOCalc) pCalc, PositionType type = PT_ABSOLUTE, int ruletID = 0)
			: RDORuntimeObject()
			, m_pCalc  (pCalc  )
			, m_type   (type   )
			, m_ruletID(ruletID)
		{}

		virtual ~RDOFRMPosition()
		{}

		LPRDOCalc    m_pCalc;
		PositionType m_type;
		int          m_ruletID;
	};
	friend class RDOFRMPosition;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMColor - объект-цвет
	// ----------------------------------------------------------------------------
	OBJECT(RDOFRMColor) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(RDOFRMColor)
	public:
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

		rdoAnimation::RDOColor getColor(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const;

		ColorType getType() const
		{
			return m_type;
		}
		void setType(ColorType type)
		{
			if (m_type == CT_NONE)
			{
				m_type = type;
			}
		}

	private:
		RDOFRMColor(ColorType type = CT_NONE);
		RDOFRMColor(int red, int green, int blue);
		RDOFRMColor(CREF(LPRDOCalc) pRedCalc, CREF(LPRDOCalc) pGreenCalc, CREF(LPRDOCalc) pBlueCalc);
		virtual ~RDOFRMColor();

		ColorType m_type;
		LPRDOCalc m_pRedCalc;
		LPRDOCalc m_pGreenCalc;
		LPRDOCalc m_pBlueCalc;
	};
	friend class RDOFRMColor;

	// ----------------------------------------------------------------------------
	// ---------- RDOFRMRulet
	// ----------------------------------------------------------------------------
	OBJECT(RDOFRMRulet)
		 IS  INSTANCE_OF(RDORuntimeObject)
		 AND INSTANCE_OF(RDOSrcInfo      )
	{
	DECLARE_FACTORY(RDOFRMRulet)
	public:
		ruint getIndex() const
		{
			return m_index;
		}
		CREF(LPRDOFRMPosition) getX() const
		{
			return m_pX;
		}
		CREF(LPRDOFRMPosition) getY() const
		{
			return m_pY;
		}

	private:
		RDOFRMRulet(CREF(RDOSrcInfo) src_info, ruint index, CREF(LPRDOFRMPosition) pX, CREF(LPRDOFRMPosition) pY)
			: RDOSrcInfo(src_info)
			, m_index   (index   )
			, m_pX      (pX      )
			, m_pY      (pY      )
		{}
		virtual ~RDOFRMRulet()
		{}

		ruint             m_index;
		LPRDOFRMPosition  m_pX;
		LPRDOFRMPosition  m_pY;
	};

public:
	void          setBackgroundColor(CREF(LPRDOFRMColor) pBgColor   );
	void          setBackPicture    (CREF(tstring)       picFileName);
	void          setBackPicture    (int width, int height);
	void          startShow         (CREF(LPRDOCalc) pCalc = NULL);
	LPRDOFRMShow  getLastShow       () const                       { return !m_showList.empty() ? m_showList.back() : NULL; }
	void          addItem           (CREF(LPRDOFRMItem)  pItem   );
	void          addRulet          (CREF(LPRDOFRMRulet) pRulet  );
	rbool         checkCondition    (CREF(LPRDORuntime)  pRuntime);
	CREF(tstring) name              () const                       { return src_text(); }
	void          getBitmaps        (REF(ImageNameList) list) const;

	PTR(rdoAnimation::RDOFrame) createFrame(CREF(LPRDORuntime) pRuntime)
	{
		PTR(rdoAnimation::RDOFrame) pFrame = new rdoAnimation::RDOFrame();
		return prepareFrame(pFrame, pRuntime);
	}
	PTR(rdoAnimation::RDOFrame) prepareFrame(PTR(rdoAnimation::RDOFrame) pFrame, CREF(LPRDORuntime) pRuntime);

	void setColorLastBG    (RDOFRMColor::ColorType type, CREF(rdoAnimation::RDOColor) lastBg);
	void setColorLastFG    (RDOFRMColor::ColorType type, CREF(rdoAnimation::RDOColor) lastFg);
	void setColorLastBGText(RDOFRMColor::ColorType type, CREF(rdoAnimation::RDOColor) lastBgText);
	void setColorLastFGText(RDOFRMColor::ColorType type, CREF(rdoAnimation::RDOColor) lastFgText);
	void setLastXYWH       (double x, double y, double width, double height)
	{
		m_lastX      = x;
		m_lastY      = y;
		m_lastWidth  = width;
		m_lastHeight = height;
	}

	int getRuletX(CREF(LPRDORuntime) pRuntime, ruint ruletID) const
	{
		LPRDOFRMRulet pRulet = findRulet(ruletID);
		return pRulet ? pRulet->getX()->getCalc()->calcValue(pRuntime).getInt() : 0;
	}
	int getRuletY(CREF(LPRDORuntime) pRuntime, ruint ruletID) const
	{
		LPRDOFRMRulet pRulet = findRulet(ruletID);
		return pRulet ? pRulet->getY()->getCalc()->calcValue(pRuntime).getInt() : 0;
	}
	LPRDOFRMRulet findRulet(ruint ruletID) const
	{
		RuletList::const_iterator it = m_ruletList.find(ruletID);
		return it != m_ruletList.end() ? it->second : NULL;
	}

private:
	RDOFRMFrame(CREF(RDOSrcInfo) src_info, CREF(LPRDOCalc) pConditionCalc = NULL);
	virtual ~RDOFRMFrame();

	typedef std::list<LPRDOFRMShow>        ShowList;
	typedef std::map<ruint, LPRDOFRMRulet> RuletList;

	LPRDOCalc               m_pConditionCalc;
	LPRDOFRMColor           m_pBgColor;
	tstring                 m_picFileName;
	ruint                   m_width;
	ruint                   m_height;
	ShowList                m_showList;
	rdoAnimation::RDOColor  m_colorLastBg;
	rdoAnimation::RDOColor  m_colorLastFg;
	rdoAnimation::RDOColor  m_colorLastBgText;
	rdoAnimation::RDOColor  m_colorLastFgText;
	double                  m_lastX;
	double                  m_lastY;
	double                  m_lastWidth;
	double                  m_lastHeight;
	RuletList               m_ruletList;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBoundingItem - объект-четырехугольник
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMBoundingItem
{
protected:
	RDOFRMBoundingItem(CREF(RDOFRMFrame::LPRDOFRMPosition) pX, CREF(RDOFRMFrame::LPRDOFRMPosition) pY, CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth, CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight)
		: m_pX     (pX     )
		, m_pY     (pY     )
		, m_pWidth (pWidth )
		, m_pHeight(pHeight)
	{}
	virtual ~RDOFRMBoundingItem()
	{}

	int getX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pX->getX(pRuntime, pFrame);
	}
	int getY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pY->getY(pRuntime, pFrame);
	}
	int getWidth(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pWidth->getWidth(pRuntime, pFrame);
	}
	int getHeight(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pHeight->getHeight(pRuntime, pFrame);
	}
	int getWidthAsX(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pWidth->getX(pRuntime, pFrame);
	}
	int getHeightAsY(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pHeight->getY(pRuntime, pFrame);
	}

private:
	RDOFRMFrame::LPRDOFRMPosition m_pX;
	RDOFRMFrame::LPRDOFRMPosition m_pY;
	RDOFRMFrame::LPRDOFRMPosition m_pWidth;
	RDOFRMFrame::LPRDOFRMPosition m_pHeight;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMColoredItem - цветной объект
// ----------------------------------------------------------------------------
// В парсере не создается
// ----------------------------------------------------------------------------
class RDOFRMColoredItem
{
public:
	CREF(RDOFRMFrame::LPRDOFRMColor) getBgColor() const { return m_pBgColor; }
	CREF(RDOFRMFrame::LPRDOFRMColor) getFgColor() const { return m_pFgColor; }

	rdoAnimation::RDOColor getBg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pBgColor->getColor(pRuntime, pFrame);
	}
	rdoAnimation::RDOColor getFg(CREF(LPRDORuntime) pRuntime, CREF(LPRDOFRMFrame) pFrame) const
	{
		return m_pFgColor->getColor(pRuntime, pFrame);
	}

protected:
	RDOFRMColoredItem(CREF(RDOFRMFrame::LPRDOFRMColor) pBgColor, CREF(RDOFRMFrame::LPRDOFRMColor) pFgColor)
		: m_pBgColor(pBgColor)
		, m_pFgColor(pFgColor)
	{}
	virtual ~RDOFRMColoredItem()
	{}

private:
	RDOFRMFrame::LPRDOFRMColor m_pBgColor;
	RDOFRMFrame::LPRDOFRMColor m_pFgColor;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMItem - базовый для всех элементов
// ----------------------------------------------------------------------------
OBJECT(RDOFRMItem) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMItem)
public:
	virtual PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime) = 0;

	virtual void getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
	{}

protected:
	RDOFRMItem(CREF(LPRDOFRMFrame) pFrame)
		: m_pFrame(pFrame)
	{}
	virtual ~RDOFRMItem()
	{}

	CREF(LPRDOFRMFrame) getFrame() const { return m_pFrame; }

private:
	LPRDOFRMFrame m_pFrame;
};

#define DECLARE_RDOFRMIItem \
private:                    \
	PTR(rdoAnimation::FrameItem) createElement(CREF(LPRDORuntime) pRuntime);

#define RDOFRM_ITEM(A) \
PREDECLARE_POINTER(A); \
CLASS(A): INSTANCE_OF(RDOFRMItem)

// ----------------------------------------------------------------------------
// ---------- RDOFRMText
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMText)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
{
DECLARE_FACTORY(RDOFRMText)
public:
	typedef rdoAnimation::RDOTextElement::TextAlign Align;

	void setText(Align align, CREF(LPRDOCalc) pValue);
	void setText(Align align, CREF(tstring)   text  );

private:
	RDOFRMText(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMText();

	Align        m_align;
	LPRDOCalc    m_pValue;
	tstring      m_text;
	rbool        m_isTextString;

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapBase
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMBitmapBase)
{
protected:
	virtual void getBitmaps(REF(RDOFRMFrame::ImageNameList) list)
	{
		list.push_back(m_pictFilename);
		if (!m_maskFilename.empty())
		{
			list.push_back(m_maskFilename);
		}
	}

	tstring m_pictFilename;
	tstring m_maskFilename;

protected:
	RDOFRMBitmapBase(CREF(LPRDOFRMFrame) pFrame, CREF(tstring) pictFilename, CREF(tstring) maskFilename = _T(""))
		: RDOFRMItem    (pFrame      )
		, m_pictFilename(pictFilename)
		, m_maskFilename(maskFilename)
	{}
	virtual ~RDOFRMBitmapBase()
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmap
// ----------------------------------------------------------------------------
CLASS(RDOFRMBitmap): INSTANCE_OF(RDOFRMBitmapBase)
{
DECLARE_FACTORY(RDOFRMBitmap)
private:
	RDOFRMBitmap(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename = _T("")
	);
	virtual ~RDOFRMBitmap();

	RDOFRMFrame::LPRDOFRMPosition m_pX;
	RDOFRMFrame::LPRDOFRMPosition m_pY;

	DECLARE_RDOFRMIItem;
};
DECLARE_POINTER(RDOFRMBitmap)

// ----------------------------------------------------------------------------
// ---------- RDOFRMBitmapStretch
// ----------------------------------------------------------------------------
CLASS(RDOFRMBitmapStretch):
	    INSTANCE_OF(RDOFRMBitmapBase  )
	AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMBitmapStretch)
private:
	RDOFRMBitmapStretch(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       pictFilename,
		CREF(tstring)                       maskFilename = _T("")
	);
	virtual ~RDOFRMBitmapStretch();

	DECLARE_RDOFRMIItem;
};
DECLARE_POINTER(RDOFRMBitmapStretch);

// ----------------------------------------------------------------------------
// ---------- RDOFRMRect
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMRect)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
{
DECLARE_FACTORY(RDOFRMRect)
private:
	RDOFRMRect(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMRect();

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMRectRound
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMRectRound)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
{
DECLARE_FACTORY(RDOFRMRectRound)
private:
	RDOFRMRectRound(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMRectRound();

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMEllipse
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMEllipse)
	IS  INSTANCE_OF(RDOFRMBoundingItem)
	AND INSTANCE_OF(RDOFRMColoredItem )
{
DECLARE_FACTORY(RDOFRMEllipse)
private:
	RDOFRMEllipse(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMEllipse();

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMLine
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMLine) AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMLine)
private:
	RDOFRMLine(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX2,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY2,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pColor
	);
	virtual ~RDOFRMLine();

	RDOFRMFrame::LPRDOFRMColor m_pColor;

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMTriang
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMTriang) AND INSTANCE_OF(RDOFRMColoredItem)
{
DECLARE_FACTORY(RDOFRMTriang)
private:
	RDOFRMTriang(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY1,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX2,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY2,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX3,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY3,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pBgColor,
		CREF(RDOFRMFrame::LPRDOFRMColor)    pFgColor
	);
	virtual ~RDOFRMTriang();

	RDOFRMFrame::LPRDOFRMPosition m_pX1;
	RDOFRMFrame::LPRDOFRMPosition m_pY1;
	RDOFRMFrame::LPRDOFRMPosition m_pX2;
	RDOFRMFrame::LPRDOFRMPosition m_pY2;
	RDOFRMFrame::LPRDOFRMPosition m_pX3;
	RDOFRMFrame::LPRDOFRMPosition m_pY3;

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMActive
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMActive) AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMActive)
private:
	RDOFRMActive(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight,
		CREF(tstring)                       operName
	);
	virtual ~RDOFRMActive();

	tstring m_operName;

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMSpace
// ----------------------------------------------------------------------------
RDOFRM_ITEM(RDOFRMSpace) AND INSTANCE_OF(RDOFRMBoundingItem)
{
DECLARE_FACTORY(RDOFRMSpace)
private:
	RDOFRMSpace(
		CREF(LPRDOFRMFrame)                 pFrame,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pX,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pY,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pWidth,
		CREF(RDOFRMFrame::LPRDOFRMPosition) pHeight
	);
	virtual ~RDOFRMSpace();

	DECLARE_RDOFRMIItem;
};

// ----------------------------------------------------------------------------
// ---------- RDOFRMShow
// ----------------------------------------------------------------------------
OBJECT(RDOFRMShow) IS INSTANCE_OF(RDORuntimeObject)
{
DECLARE_FACTORY(RDOFRMShow)
public:
	typedef std::list<LPRDOFRMItem> ItemList;

	void          insertItem (CREF(LPRDOFRMItem) pItem);
	REF(ItemList) getItemList();

	rbool        isShowIf      () const                        { return m_pConditionCalc != NULL; }
	rbool        checkCondition(CREF(LPRDORuntime) pRuntime);
	virtual void getBitmaps    (REF(RDOFRMFrame::ImageNameList) list);

private:
	RDOFRMShow(CREF(LPRDOCalc) pConditionCalc);
	virtual ~RDOFRMShow();

	ItemList  m_itemList;
	LPRDOCalc m_pConditionCalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOFRAME_H_
