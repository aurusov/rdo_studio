// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOCommonAnimation_Test
#include "utils/src/common/warning_disable.h"
#include <memory>
#include <boost/test/included/unit_test.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/animation/rdoanimation.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOCommonAnimation_Test)

BOOST_AUTO_TEST_CASE(RDOCommonAnimationFrame)
{
    rdo::animation::FrameItem::Type frameItemType = rdo::animation::FrameItem::Type::NONE;
    rdo::animation::FrameItem frameItem(frameItemType);

    BOOST_CHECK(frameItem.getType() == frameItemType);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationColor)
{
    rdo::animation::Color color;

    BOOST_CHECK(color.m_transparent);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationColoredElement)
{
    rdo::animation::Color background;
    rdo::animation::Color foreground;
    rdo::animation::ColoredElement elem(background, foreground);

    BOOST_CHECK(elem.m_background.m_transparent == background.m_transparent);
    BOOST_CHECK(elem.m_foreground.m_transparent == foreground.m_transparent);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationPoint)
{
    int x = 0;
    int y = 0;
    rdo::animation::Point point(x,y);

    BOOST_CHECK(point.m_x == x);
    BOOST_CHECK(point.m_y == y);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationSize)
{
    int width = 10;
    int height = 10;
    rdo::animation::Size size(width, height);

    BOOST_CHECK(size.m_width == width);
    BOOST_CHECK(size.m_height == height);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationBoundedElement)
{
    int x = 0;
    int y = 5;

    int width = 10;
    int height = 20;

    rdo::animation::Point point(x, y);
    rdo::animation::Size size(width, height);
    rdo::animation::BoundedElement boundedElement(point, size);

    BOOST_CHECK(boundedElement.m_point == point);
    BOOST_CHECK(boundedElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationRadius)
{
    double radiusSize = 100;
    rdo::animation::Radius radius(radiusSize);

    BOOST_CHECK(radius.m_radius == radiusSize);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationTextElement)
{
    rdo::animation::Color background(0, 0, 0);
    rdo::animation::Color foreground(0, 0, 0);
    rdo::animation::ColoredElement elem(background, foreground);

    rdo::animation::Point point(0,0);
    rdo::animation::Size size(10, 10);
    rdo::animation::BoundedElement boundedElement(point, size);
    const std::string text("RDO");

    rdo::animation::TextElement::Align align = rdo::animation::TextElement::Align::CENTER;

    rdo::animation::TextElement textElement(boundedElement, elem, text, align);

    BOOST_CHECK(textElement.m_point == point);
    BOOST_CHECK(textElement.m_size == size);
    BOOST_CHECK(textElement.m_background == background);
    BOOST_CHECK(textElement.m_foreground == foreground);
    BOOST_CHECK(textElement.m_text == text);
    BOOST_CHECK(textElement.m_align == align);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationLineElement)
{
    int x1 = 1;
    int y1 = 3;
    int x2 = 5;
    int y2 = 7;

    rdo::animation::Point point1(x1, y1);
    rdo::animation::Point point2(x2, y2);

    rdo::animation::Color color;

    rdo::animation::LineElement lineElem(point1, point2, color);

    BOOST_CHECK(lineElem.m_color.m_transparent == color.m_transparent);
    BOOST_CHECK(lineElem.m_color == color);
    BOOST_CHECK(lineElem.m_point1 == point1);
    BOOST_CHECK(lineElem.m_point2 == point2);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationTriangElement)
{
    rdo::animation::Point point1(0, 0);
    rdo::animation::Point point2(10, 0);
    rdo::animation::Point point3(0, 10);

    rdo::animation::Color background;
    rdo::animation::Color foreground;
    rdo::animation::ColoredElement elem(background, foreground);

    rdo::animation::TriangElement triangElement(point1, point2, point3, elem);

    BOOST_CHECK(triangElement.m_background == background);
    BOOST_CHECK(triangElement.m_foreground == foreground);
    BOOST_CHECK(triangElement.m_point1 == point1);
    BOOST_CHECK(triangElement.m_point2 == point2);
    BOOST_CHECK(triangElement.m_point3 == point3);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationRectElements)
{
    rdo::animation::Color background;
    rdo::animation::Color foreground;
    rdo::animation::ColoredElement elem(background, foreground);

    rdo::animation::Point point(0,0);
    rdo::animation::Size size(10, 10);
    rdo::animation::BoundedElement boundedElement(point, size);

    rdo::animation::RectElement rectElement(boundedElement, elem);

    BOOST_CHECK(rectElement.m_background == background);
    BOOST_CHECK(rectElement.m_foreground == foreground);
    BOOST_CHECK(rectElement.m_point == point);
    BOOST_CHECK(rectElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationCircleElement)
{
    rdo::animation::Point point(0,0);
    rdo::animation::Radius radius(10);

    rdo::animation::Color background;
    rdo::animation::Color foreground;
    rdo::animation::ColoredElement elem(background, foreground);

    rdo::animation::CircleElement circle(point ,radius, elem);

    BOOST_CHECK(circle.m_center == point);
    BOOST_CHECK(circle.m_radius == radius);
    BOOST_CHECK(circle.m_background == background);
    BOOST_CHECK(circle.m_foreground == foreground);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationEllipseElement)
{
    rdo::animation::Point point(0,0);
    rdo::animation::Size size(10, 10);
    rdo::animation::BoundedElement boundedElement(point, size);

    rdo::animation::Color background;
    rdo::animation::Color foreground;
    rdo::animation::ColoredElement elem(background, foreground);

    rdo::animation::EllipseElement ellipse(boundedElement, elem);

    BOOST_CHECK(ellipse.m_background == background);
    BOOST_CHECK(ellipse.m_foreground == foreground);
    BOOST_CHECK(ellipse.m_point == point);
    BOOST_CHECK(ellipse.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationBmpElement)
{
    rdo::animation::Point point(0,0);
    const std::string bmp_name("rdo_bmp");
    const std::string mask_name("mask");

    rdo::animation::BmpElement bmpElement(point, bmp_name, mask_name);

    BOOST_CHECK(bmpElement.m_point == point);
    BOOST_CHECK(bmpElement.m_bmp_name == bmp_name);
    BOOST_CHECK(bmpElement.m_mask_name == mask_name);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationSBmpElement)
{
    const std::string bmp_name("rdo_bmp");
    const std::string mask_name("mask");

    rdo::animation::Point point(0, 0);
    rdo::animation::Size size(10, 10);
    rdo::animation::BoundedElement boundedElement(point, size);

    rdo::animation::ScaledBmpElement sbmpElement(boundedElement, bmp_name, mask_name);

    BOOST_CHECK(sbmpElement.m_bmp_name == bmp_name);
    BOOST_CHECK(sbmpElement.m_mask_name == mask_name);
    BOOST_CHECK(sbmpElement.m_point == point);
    BOOST_CHECK(sbmpElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationActiveElement)
{
    const std::string text("RDO");

    rdo::animation::Point point(0, 0);
    rdo::animation::Size size(10, 10);
    rdo::animation::BoundedElement boundedElement(point, size);

    rdo::animation::ActiveElement actionElement(boundedElement, text);

    BOOST_CHECK(actionElement.m_point == point);
    BOOST_CHECK(actionElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationNullElement)
{
    std::unique_ptr<rdo::animation::NullElement> nullElem(new rdo::animation::NullElement);
    BOOST_CHECK(nullElem.get());
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationFramet)
{
    rdo::animation::Frame frame;
    BOOST_CHECK(!frame.hasBgImage());
}

BOOST_AUTO_TEST_SUITE_END() // RDOCommonAnimation_Test
