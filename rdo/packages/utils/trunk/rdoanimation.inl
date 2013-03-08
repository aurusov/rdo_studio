/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoanimation.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      13.06.2009
  \brief     Описание объектов анимации, которые передаются от симулятора к GUI для отрисовки
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_ANIMATION_NAMESPACE

inline FrameItem::FrameItem(Type type)
	: m_type(type)
{}

inline FrameItem::Type FrameItem::getType() const
{
	return m_type;
}

inline Color::Color()
	: m_transparent(true)
{}

inline Color::Color(CREF(Color) color)
	: m_r          (color.m_r          )
	, m_g          (color.m_g          )
	, m_b          (color.m_b          )
	, m_transparent(color.m_transparent)
{}

inline Color::Color(rbyte r, rbyte g, rbyte b, rbool transparent)
	: m_r          (r          )
	, m_g          (g          )
	, m_b          (b          )
	, m_transparent(transparent)
{}

inline ColoredElement::ColoredElement(CREF(ColoredElement) color)
	: m_background(color.m_background)
	, m_foreground(color.m_foreground)
{}

inline ColoredElement::ColoredElement(CREF(Color) background, CREF(Color) foreground)
	: m_background(background)
	, m_foreground(foreground)
{}

inline Point::Point(CREF(Point) point)
	: m_x(point.m_x)
	, m_y(point.m_y)
{}

inline Point::Point(double x, double y)
	: m_x(x)
	, m_y(y)
{}

inline Size::Size()
	: m_width (0)
	, m_height(0)
{}

inline Size::Size(CREF(Size) size)
	: m_width (size.m_width )
	, m_height(size.m_height)
{}

inline Size::Size(double width, double height)
	: m_width (width )
	, m_height(height)
{}

inline BoundedElement::BoundedElement(CREF(BoundedElement) rect)
	: m_point(rect.m_point)
	, m_size (rect.m_size )
{}

inline BoundedElement::BoundedElement(CREF(Point) point, CREF(Size) size)
	: m_point(point)
	, m_size (size )
{}

inline Radius::Radius(CREF(Radius) radius)
	: m_radius(radius.m_radius)
{}

inline Radius::Radius(double radius)
	: m_radius(radius)
{}

inline TextElement::TextElement(CREF(BoundedElement) rect, CREF(ColoredElement) color, CREF(tstring) text, TextAlign align)
	: FrameItem     (FIT_TEXT)
	, BoundedElement(rect    )
	, ColoredElement(color   )
	, m_text        (text    )
	, m_align       (align   )
{}

inline LineElement::LineElement(CREF(Point) point1, CREF(Point) point2, CREF(Color) color)
	: FrameItem(FIT_LINE)
	, m_point1 (point1  )
	, m_point2 (point2  )
	, m_color  (color   )
{}

inline TriangElement::TriangElement(CREF(Point) point1, CREF(Point) point2, CREF(Point) point3, CREF(ColoredElement) color)
	: FrameItem     (FIT_TRIANG)
	, ColoredElement(color     )
	, m_point1      (point1    )
	, m_point2      (point2    )
	, m_point3      (point3    )
{}

inline RectElement::RectElement(CREF(BoundedElement) rect, CREF(ColoredElement) color)
	: FrameItem     (FIT_RECT)
	, BoundedElement(rect    )
	, ColoredElement(color   )
{}

inline RoundRectElement::RoundRectElement(CREF(BoundedElement) rect, CREF(ColoredElement) color)
	: FrameItem     (FIT_R_RECT)
	, BoundedElement(rect      )
	, ColoredElement(color     )
{}

inline CircleElement::CircleElement(CREF(Point) center, CREF(Radius) radius, CREF(ColoredElement) color)
	: FrameItem     (FIT_CIRCLE)
	, ColoredElement(color     )
	, m_center      (center    )
	, m_radius      (radius    )
{}

inline EllipseElement::EllipseElement(CREF(BoundedElement) rect, CREF(ColoredElement) color)
	: FrameItem     (FIT_ELLIPSE)
	, BoundedElement(rect       )
	, ColoredElement(color      )
{}

inline BmpElement::BmpElement(CREF(Point) point, CREF(tstring) bmp_name, CREF(tstring) mask_name)
	: FrameItem  (FIT_BMP  )
	, m_point    (point    )
	, m_bmp_name (bmp_name )
	, m_mask_name(mask_name)
{}

inline rbool BmpElement::hasMask() const
{
	return !m_mask_name.empty();
}

inline ScaledBmpElement::ScaledBmpElement(CREF(BoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name)
	: FrameItem     (FIT_S_BMP)
	, BoundedElement(rect     )
	, m_bmp_name    (bmp_name )
	, m_mask_name   (mask_name)
{}

inline rbool ScaledBmpElement::hasMask() const
{
	return !m_mask_name.empty();
}

inline ActiveElement::ActiveElement(CREF(BoundedElement) rect, CREF(tstring) opr_name)
	: FrameItem     (FIT_ACTIVE)
	, BoundedElement(rect      )
	, m_opr_name    (opr_name  )
{}

inline NullElement::NullElement()
	: FrameItem(FIT_NULL)
{}

inline Frame::~Frame()
{
	STL_FOR_ALL(m_elements, it)
		delete (*it);

	m_elements.clear();
}

inline rbool Frame::hasBgImage() const
{
	return !m_bgImageName.empty();
}

CLOSE_RDO_ANIMATION_NAMESPACE
