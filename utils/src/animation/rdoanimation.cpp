/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoanimation.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      13.06.2009
  \brief     Описание объектов анимации, которые передаются от симулятора к GUI для отрисовки
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/animation/rdoanimation.h"
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace animation {

FrameItem::FrameItem(Type type)
	: m_type(type)
{}

FrameItem::Type FrameItem::getType() const
{
	return m_type;
}

Color::Color()
	: m_transparent(true)
{}

Color::Color(const Color& color)
	: m_r          (color.m_r          )
	, m_g          (color.m_g          )
	, m_b          (color.m_b          )
	, m_transparent(color.m_transparent)
{}

Color::Color(unsigned char r, unsigned char g, unsigned char b, bool transparent)
	: m_r          (r          )
	, m_g          (g          )
	, m_b          (b          )
	, m_transparent(transparent)
{}

ColoredElement::ColoredElement(const ColoredElement& color)
	: m_background(color.m_background)
	, m_foreground(color.m_foreground)
{}

ColoredElement::ColoredElement(const Color& background, const Color& foreground)
	: m_background(background)
	, m_foreground(foreground)
{}

Point::Point(const Point& point)
	: m_x(point.m_x)
	, m_y(point.m_y)
{}

Point::Point(double x, double y)
	: m_x(x)
	, m_y(y)
{}

Size::Size()
	: m_width (0)
	, m_height(0)
{}

Size::Size(const Size& size)
	: m_width (size.m_width )
	, m_height(size.m_height)
{}

Size::Size(double width, double height)
	: m_width (width )
	, m_height(height)
{}

BoundedElement::BoundedElement(const BoundedElement& rect)
	: m_point(rect.m_point)
	, m_size (rect.m_size )
{}

BoundedElement::BoundedElement(const Point& point, const Size& size)
	: m_point(point)
	, m_size (size )
{}

Radius::Radius(const Radius& radius)
	: m_radius(radius.m_radius)
{}

Radius::Radius(double radius)
	: m_radius(radius)
{}

TextElement::TextElement(const BoundedElement& rect, const ColoredElement& color, const std::string& text, TextAlign align)
	: FrameItem     (FIT_TEXT)
	, BoundedElement(rect    )
	, ColoredElement(color   )
	, m_text        (text    )
	, m_align       (align   )
{}

LineElement::LineElement(const Point& point1, const Point& point2, const Color& color)
	: FrameItem(FIT_LINE)
	, m_point1 (point1  )
	, m_point2 (point2  )
	, m_color  (color   )
{}

TriangElement::TriangElement(const Point& point1, const Point& point2, const Point& point3, const ColoredElement& color)
	: FrameItem     (FIT_TRIANG)
	, ColoredElement(color     )
	, m_point1      (point1    )
	, m_point2      (point2    )
	, m_point3      (point3    )
{}

RectElement::RectElement(const BoundedElement& rect, const ColoredElement& color)
	: FrameItem     (FIT_RECT)
	, BoundedElement(rect    )
	, ColoredElement(color   )
{}

RoundRectElement::RoundRectElement(const BoundedElement& rect, const ColoredElement& color)
	: FrameItem     (FIT_R_RECT)
	, BoundedElement(rect      )
	, ColoredElement(color     )
{}

CircleElement::CircleElement(const Point& center, const Radius& radius, const ColoredElement& color)
	: FrameItem     (FIT_CIRCLE)
	, ColoredElement(color     )
	, m_center      (center    )
	, m_radius      (radius    )
{}

EllipseElement::EllipseElement(const BoundedElement& rect, const ColoredElement& color)
	: FrameItem     (FIT_ELLIPSE)
	, BoundedElement(rect       )
	, ColoredElement(color      )
{}

BmpElement::BmpElement(const Point& point, const std::string& bmp_name, const std::string& mask_name)
	: FrameItem  (FIT_BMP  )
	, m_point    (point    )
	, m_bmp_name (bmp_name )
	, m_mask_name(mask_name)
{}

bool BmpElement::hasMask() const
{
	return !m_mask_name.empty();
}

ScaledBmpElement::ScaledBmpElement(const BoundedElement& rect, const std::string& bmp_name, const std::string& mask_name)
	: FrameItem     (FIT_S_BMP)
	, BoundedElement(rect     )
	, m_bmp_name    (bmp_name )
	, m_mask_name   (mask_name)
{}

bool ScaledBmpElement::hasMask() const
{
	return !m_mask_name.empty();
}

ActiveElement::ActiveElement(const BoundedElement& rect, const std::string& opr_name)
	: FrameItem     (FIT_ACTIVE)
	, BoundedElement(rect      )
	, m_opr_name    (opr_name  )
{}

NullElement::NullElement()
	: FrameItem(FIT_NULL)
{}

Frame::~Frame()
{
	for (const auto& element: m_elements)
		delete element;

	m_elements.clear();
}

bool Frame::hasBgImage() const
{
	return !m_bgImageName.empty();
}

}} // namespace rdo::animation
