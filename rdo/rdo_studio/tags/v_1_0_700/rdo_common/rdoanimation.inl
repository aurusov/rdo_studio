/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoanimation.inl
 * author   : Урусов Андрей
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_ANIMATION_NAMESPACE

inline FrameItem::FrameItem(Type type)
	: m_type(type)
{}

inline FrameItem::Type FrameItem::getType() const
{
	return m_type;
}

inline RDOColor::RDOColor()
	: m_transparent(true)
{}

inline RDOColor::RDOColor(CREF(RDOColor) color)
	: m_r          (color.m_r          )
	, m_g          (color.m_g          )
	, m_b          (color.m_b          )
	, m_transparent(color.m_transparent)
{}

inline RDOColor::RDOColor(rbyte r, rbyte g, rbyte b, rbool transparent)
	: m_r          (r          )
	, m_g          (g          )
	, m_b          (b          )
	, m_transparent(transparent)
{}

inline RDOColoredElement::RDOColoredElement(CREF(RDOColoredElement) color)
	: m_background(color.m_background)
	, m_foreground(color.m_foreground)
{}

inline RDOColoredElement::RDOColoredElement(CREF(RDOColor) background, CREF(RDOColor) foreground)
	: m_background(background)
	, m_foreground(foreground)
{}

inline RDOPoint::RDOPoint(CREF(RDOPoint) point)
	: m_x(point.m_x)
	, m_y(point.m_y)
{}

inline RDOPoint::RDOPoint(double x, double y)
	: m_x(x)
	, m_y(y)
{}

inline RDOSize::RDOSize()
	: m_width (0)
	, m_height(0)
{}

inline RDOSize::RDOSize(CREF(RDOSize) size)
	: m_width (size.m_width )
	, m_height(size.m_height)
{}

inline RDOSize::RDOSize(double width, double height)
	: m_width (width )
	, m_height(height)
{}

inline RDOBoundedElement::RDOBoundedElement(CREF(RDOBoundedElement) rect)
	: m_point(rect.m_point)
	, m_size (rect.m_size )
{}

inline RDOBoundedElement::RDOBoundedElement(CREF(RDOPoint) point, CREF(RDOSize) size)
	: m_point(point)
	, m_size (size )
{}

inline RDOTextElement::RDOTextElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color, CREF(tstring) text, TextAlign align)
	: FrameItem        (FIT_TEXT)
	, RDOBoundedElement(rect    )
	, RDOColoredElement(color   )
	, m_text           (text    )
	, m_align          (align   )
{}

inline RDOLineElement::RDOLineElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOColor) color)
	: FrameItem(FIT_LINE)
	, m_point1 (point1  )
	, m_point2 (point2  )
	, m_color  (color   )
{}

inline RDOTriangElement::RDOTriangElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOPoint) point3, CREF(RDOColoredElement) color)
	: FrameItem        (FIT_TRIANG)
	, RDOColoredElement(color     )
	, m_point1         (point1    )
	, m_point2         (point2    )
	, m_point3         (point3    )
{}

inline RDORectElement::RDORectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color)
	: FrameItem        (FIT_RECT)
	, RDOBoundedElement(rect    )
	, RDOColoredElement(color   )
{}

inline RDORRectElement::RDORRectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color)
	: FrameItem        (FIT_R_RECT)
	, RDOBoundedElement(rect      )
	, RDOColoredElement(color     )
{}

inline RDOEllipseElement::RDOEllipseElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color)
	: FrameItem        (FIT_ELLIPSE)
	, RDOBoundedElement(rect       )
	, RDOColoredElement(color      )
{}

inline RDOBmpElement::RDOBmpElement(CREF(RDOPoint) point, CREF(tstring) bmp_name, CREF(tstring) mask_name)
	: FrameItem  (FIT_BMP  )
	, m_point    (point    )
	, m_bmp_name (bmp_name )
	, m_mask_name(mask_name)
{}

inline rbool RDOBmpElement::hasMask() const
{
	return !m_mask_name.empty();
}

inline RDOSBmpElement::RDOSBmpElement(CREF(RDOBoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name)
	: FrameItem        (FIT_S_BMP)
	, RDOBoundedElement(rect     )
	, m_bmp_name       (bmp_name )
	, m_mask_name      (mask_name)
{}

inline rbool RDOSBmpElement::hasMask() const
{
	return !m_mask_name.empty();
}

inline RDOActiveElement::RDOActiveElement(CREF(RDOBoundedElement) rect, CREF(tstring) opr_name)
	: FrameItem        (FIT_ACTIVE)
	, RDOBoundedElement(rect      )
	, m_opr_name       (opr_name  )
{}

inline RDONullElement::RDONullElement()
	: FrameItem(FIT_NULL)
{}

inline RDOFrame::~RDOFrame()
{
	for (Elements::iterator it = m_elements.begin(); it != m_elements.end(); it++)
		delete (*it);

	m_elements.clear();
}

inline rbool RDOFrame::hasBgImage() const
{
	return !m_bgImageName.empty();
}

CLOSE_RDO_ANIMATION_NAMESPACE
