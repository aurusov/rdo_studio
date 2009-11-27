/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoanimation.h
 * author   : Урусов Андрей
 * date     : 13.06.2009
 * bref     : Описание объектов анимации
 * indent   : 4T
 */

#ifndef _RDOANIMATION_H_
#define _RDOANIMATION_H_

// ====================================================================== INCLUDES
#include <vector>
// ====================================================================== SYNOPSIS
#include "namespace.h"
#include "rdotypes.h"
#include "rdomacros.h"
// ===============================================================================

OPEN_RDO_ANIMATION_NAMESPACE

	STRUCT(FrameItem)
	{
		enum Type
		{
			FIT_NULL   ,
			FIT_TEXT   ,
			FIT_LINE   ,
			FIT_TRIANG ,
			FIT_RECT   ,
			FIT_R_RECT ,
			FIT_ELLIPSE,
			FIT_BMP    ,
			FIT_S_BMP  ,
			FIT_ACTIVE
		};

		FrameItem(Type type);
		Type getType() const;

	private:
		Type m_type;
	};

	STRUCT(RDOColor)
	{
		rbyte m_r;
		rbyte m_g;
		rbyte m_b;
		rbool m_transparent;

		RDOColor();
		RDOColor(CREF(RDOColor) color);
		RDOColor(rbyte r, rbyte g, rbyte b, rbool transparent = false);
	};

	STRUCT(RDOColoredElement)
	{
		RDOColor m_background;
		RDOColor m_foreground;

		RDOColoredElement(CREF(RDOColoredElement) color);
		RDOColoredElement(CREF(RDOColor) background, CREF(RDOColor) foreground);
	};

	STRUCT(RDOPoint)
	{
		double m_x;
		double m_y;

		RDOPoint(CREF(RDOPoint) point);
		RDOPoint(double x, double y);
	};

	STRUCT(RDOSize)
	{
		double m_width;
		double m_height;

		RDOSize();
		RDOSize(CREF(RDOSize) size);
		RDOSize(double width, double height);
	};

	STRUCT(RDOBoundedElement)
	{
		RDOPoint  m_point;
		RDOSize   m_size;

		RDOBoundedElement(CREF(RDOBoundedElement) rect);
		RDOBoundedElement(CREF(RDOPoint) point, CREF(RDOSize) size);
	};

#define FRAME_ITEM(A) STRUCT_PARENT_OF(A, FrameItem)

	FRAME_ITEM(RDOTextElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                           AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		enum TextAlign { TETA_LEFT, TETA_RIGHT, TETA_CENTER };

		tstring    m_text;
		TextAlign  m_align;

		RDOTextElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color, CREF(tstring) text, TextAlign align);
	};

	FRAME_ITEM(RDOLineElement)
	{
		RDOPoint  m_point1;
		RDOPoint  m_point2;
		RDOColor  m_color;

		RDOLineElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOColor) color);
	};

	FRAME_ITEM(RDOTriangElement) IS IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDOPoint  m_point1;
		RDOPoint  m_point2;
		RDOPoint  m_point3;

		RDOTriangElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOPoint) point3, CREF(RDOColoredElement) color);
	};

	FRAME_ITEM(RDORectElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                           AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDORectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	FRAME_ITEM(RDORRectElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                            AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDORRectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	FRAME_ITEM(RDOEllipseElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                              AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDOEllipseElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	FRAME_ITEM(RDOBmpElement)
	{
		RDOPoint  m_point;
		tstring   m_bmp_name;
		tstring   m_mask_name;

		RDOBmpElement(CREF(RDOPoint) point, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));
		rbool hasMask() const;
	};

	FRAME_ITEM(RDOSBmpElement) IS IMPLEMENTATION_OF(RDOBoundedElement)
	{
		tstring  m_bmp_name;
		tstring  m_mask_name;

		RDOSBmpElement(CREF(RDOBoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));
		rbool hasMask() const;
	};

	FRAME_ITEM(RDOActiveElement) IS IMPLEMENTATION_OF(RDOBoundedElement)
	{
		tstring  m_opr_name;

		RDOActiveElement(CREF(RDOBoundedElement) rect, CREF(tstring) opr_name);
	};

	FRAME_ITEM(RDONullElement)
	{
		RDONullElement();
	};

	STRUCT(RDOFrame)
	{
		typedef  std::vector<PTR(FrameItem)>  Elements;

		RDOColor  m_bgColor;
		tstring   m_bgImageName;
		RDOSize   m_size;
		Elements  m_elements;

		~RDOFrame();
		rbool hasBgImage() const;
	};

CLOSE_RDO_ANIMATION_NAMESPACE

#include "rdoanimation.inl"

#endif //! _RDOANIMATION_H_
