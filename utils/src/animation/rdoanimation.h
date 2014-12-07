#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace animation {

struct FrameItem
{
    enum Type
    {
        FIT_NULL,
        FIT_TEXT,
        FIT_LINE,
        FIT_TRIANG,
        FIT_RECT,
        FIT_R_RECT,
        FIT_CIRCLE,
        FIT_ELLIPSE,
        FIT_BMP,
        FIT_S_BMP,
        FIT_ACTIVE
    };

    FrameItem(Type type);

    Type getType() const;

    bool operator== (const FrameItem& item) const
    {
        return m_type == item.m_type;
    }

private:
    Type m_type;
};

struct Color
{
    unsigned char m_r;
    unsigned char m_g;
    unsigned char m_b;
    bool m_transparent;

    Color();
    Color(const Color& color);
    Color(unsigned char r, unsigned char g, unsigned char b, bool transparent = false);

    bool operator== (const Color& color) const
    {
        return m_r == color.m_r && m_g == color.m_g && m_b == color.m_b && m_transparent == color.m_transparent;
    }
};

struct ColoredElement
{
    Color m_background;
    Color m_foreground;

    ColoredElement(const ColoredElement& color);
    ColoredElement(const Color& background, const Color& foreground);

    bool operator== (const ColoredElement& element) const
    {
        return m_background == element.m_background && m_foreground == element.m_foreground;
    }
};

struct Point
{
    double m_x;
    double m_y;

    Point(const Point& point);
    Point(double x, double y);

    bool operator== (const Point& point) const
    {
        return m_x == point.m_x && m_y == point.m_y;
    }
};

struct Size
{
    double m_width;
    double m_height;

    Size();
    Size(const Size& size);
    Size(double width, double height);

    bool operator== (const Size& size) const
    {
        return m_width == size.m_width && m_height == size.m_height;
    }
};

struct BoundedElement
{
    Point  m_point;
    Size   m_size;

    BoundedElement(const BoundedElement& rect);
    BoundedElement(const Point& point, const Size& size);

    bool operator== (const BoundedElement& element) const
    {
        return m_point == element.m_point && m_size == element.m_size;
    }
};

struct Radius
{
    double m_radius;

    Radius(const Radius& radius);
    Radius(double radius);

    bool operator== (const Radius& radius) const
    {
        return m_radius == radius.m_radius;
    }
};

struct TextElement
    : public FrameItem
    , public BoundedElement
    , public ColoredElement
{
    enum TextAlign
    {
        TETA_LEFT,
        TETA_RIGHT,
        TETA_CENTER
    };

    std::string m_text;
    TextAlign m_align;

    TextElement(const BoundedElement& rect, const ColoredElement& color, const std::string& text, TextAlign align);
};

struct LineElement: public FrameItem
{
    Point  m_point1;
    Point  m_point2;
    Color  m_color;

    LineElement(const Point& point1, const Point& point2, const Color& color);

    bool operator== (const LineElement& element) const
    {
        return m_point1 == element.m_point1 && m_point2 == element.m_point2 && m_color == element.m_color;
    }
};

struct TriangElement
    : public FrameItem
    , public ColoredElement
{
    Point  m_point1;
    Point  m_point2;
    Point  m_point3;

    TriangElement(const Point& point1, const Point& point2, const Point& point3, const ColoredElement& color);
};

struct RectElement
    : public FrameItem
    , public BoundedElement
    , public ColoredElement
{
    RectElement(const BoundedElement& rect, const ColoredElement& color);
};

struct RoundRectElement
    : public FrameItem
    , public BoundedElement
    , public ColoredElement
{
    RoundRectElement(const BoundedElement& rect, const ColoredElement& color);
};

struct CircleElement
    : public FrameItem
    , public ColoredElement
{
    Point  m_center;
    Radius m_radius;

    CircleElement(const Point& center, const Radius& radius, const ColoredElement& color);
};

struct EllipseElement
    : public FrameItem
    , public BoundedElement
    , public ColoredElement
{
    EllipseElement(const BoundedElement& rect, const ColoredElement& color);
};

struct BmpElement: public FrameItem
{
    Point m_point;
    std::string m_bmp_name;
    std::string m_mask_name;

    BmpElement(const Point& point, const std::string& bmp_name, const std::string& mask_name = "");

    bool hasMask() const;
};

struct ScaledBmpElement
    : public FrameItem
    , public BoundedElement
{
    std::string m_bmp_name;
    std::string m_mask_name;

    ScaledBmpElement(const BoundedElement& rect, const std::string& bmp_name, const std::string& mask_name = "");

    bool hasMask() const;
};

struct ActiveElement
    : public FrameItem
    , public BoundedElement
{
    std::string m_opr_name;

    ActiveElement(const BoundedElement& rect, const std::string& opr_name);
};

struct NullElement: public FrameItem
{
    NullElement();
};

struct Frame
{
    typedef std::vector<FrameItem*> Elements;

    Color m_bgColor;
    std::string m_bgImageName;
    Size m_size;
    Elements m_elements;

    ~Frame();

    bool hasBgImage() const;
};

}} // namespace rdo::animation
