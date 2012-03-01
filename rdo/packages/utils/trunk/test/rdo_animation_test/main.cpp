/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест элемента animation common-библиотеки
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOCommonAnimation_Test
#include <boost/test/included/unit_test.hpp>
#include <memory>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdoanimation.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOCommonAnimation_Test)

BOOST_AUTO_TEST_CASE(RDOCommonAnimationFrame)
{
	rdoAnimation::FrameItem::Type frameItemType = rdoAnimation::FrameItem::FIT_NULL;
	rdoAnimation::FrameItem frameItem(frameItemType);

	BOOST_CHECK(frameItem.getType() == frameItemType);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationColor)
{
	rdoAnimation::RDOColor color;

	BOOST_CHECK(color.m_transparent);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationColoredElement)
{
	rdoAnimation::RDOColor background;
	rdoAnimation::RDOColor foreground;
	rdoAnimation::RDOColoredElement elem(background, foreground);

	BOOST_CHECK(elem.m_background.m_transparent == background.m_transparent);
	BOOST_CHECK(elem.m_foreground.m_transparent == foreground.m_transparent);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationPoint)
{
	int x = 0;
	int y = 0;
	rdoAnimation::RDOPoint point(x,y);

	BOOST_CHECK(point.m_x == x);
	BOOST_CHECK(point.m_y == y);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationSize)
{
	int width = 10;
	int height = 10;
	rdoAnimation::RDOSize size(width, height);

	BOOST_CHECK(size.m_width == width);
	BOOST_CHECK(size.m_height == height);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationBoundedElement)
{
	int x = 0;
	int y = 5;

	int width = 10;
	int height = 20;

	rdoAnimation::RDOPoint point(x, y);
	rdoAnimation::RDOSize size(width, height);
	rdoAnimation::RDOBoundedElement boundedElement(point, size);

	BOOST_CHECK(boundedElement.m_point == point);
	BOOST_CHECK(boundedElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationRadius)
{
	double radiusSize = 100;
	rdoAnimation::RDORadius radius(radiusSize);
	
	BOOST_CHECK(radius.m_radius == radiusSize);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationTextElement)
{
	rdoAnimation::RDOColor background;
	rdoAnimation::RDOColor foreground;
	rdoAnimation::RDOColoredElement elem(background, foreground);

	rdoAnimation::RDOPoint point(0,0);
	rdoAnimation::RDOSize size(10, 10);
	rdoAnimation::RDOBoundedElement boundedElement(point, size);

	tstring text("RDO");

	rdoAnimation::RDOTextElement::TextAlign align = rdoAnimation::RDOTextElement::TETA_CENTER;

	rdoAnimation::RDOTextElement textElement(boundedElement, elem, text, align);

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

	rdoAnimation::RDOPoint point1(x1, y1);
	rdoAnimation::RDOPoint point2(x2, y2);

	rdoAnimation::RDOColor color;

	rdoAnimation::RDOLineElement lineElem(point1, point2, color);
	
	BOOST_CHECK(lineElem.m_color.m_transparent == color.m_transparent);
	BOOST_CHECK(lineElem.m_color == color);
	BOOST_CHECK(lineElem.m_point1 == point1);
	BOOST_CHECK(lineElem.m_point2 == point2);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationTriangElement)
{
	rdoAnimation::RDOPoint point1(0, 0);
	rdoAnimation::RDOPoint point2(10, 0);
	rdoAnimation::RDOPoint point3(0, 10);

	rdoAnimation::RDOColor background;
	rdoAnimation::RDOColor foreground;
	rdoAnimation::RDOColoredElement elem(background, foreground);

	rdoAnimation::RDOTriangElement triangElement(point1, point2, point3, elem);

	BOOST_CHECK(triangElement.m_background == background);
	BOOST_CHECK(triangElement.m_foreground == foreground);
	BOOST_CHECK(triangElement.m_point1 == point1);
	BOOST_CHECK(triangElement.m_point2 == point2);
	BOOST_CHECK(triangElement.m_point3 == point3);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationRectElements)
{
	rdoAnimation::RDOColor background;
	rdoAnimation::RDOColor foreground;
	rdoAnimation::RDOColoredElement elem(background, foreground);

	rdoAnimation::RDOPoint point(0,0);
	rdoAnimation::RDOSize size(10, 10);
	rdoAnimation::RDOBoundedElement boundedElement(point, size);

	rdoAnimation::RDORectElement rectElement(boundedElement, elem);

	BOOST_CHECK(rectElement.m_background == background);
	BOOST_CHECK(rectElement.m_foreground == foreground);
	BOOST_CHECK(rectElement.m_point == point);
	BOOST_CHECK(rectElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationCircleElement)
{
	rdoAnimation::RDOPoint point(0,0);
	rdoAnimation::RDORadius radius(10);

	rdoAnimation::RDOColor background;
	rdoAnimation::RDOColor foreground;
	rdoAnimation::RDOColoredElement elem(background, foreground);

	rdoAnimation::RDOCircleElement circle(point ,radius, elem);

	BOOST_CHECK(circle.m_center == point);
	BOOST_CHECK(circle.m_radius == radius);
	BOOST_CHECK(circle.m_background == background);
	BOOST_CHECK(circle.m_foreground == foreground);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationEllipseElement)
{
	rdoAnimation::RDOPoint point(0,0);
	rdoAnimation::RDOSize size(10, 10);
	rdoAnimation::RDOBoundedElement boundedElement(point, size);

	rdoAnimation::RDOColor background;
	rdoAnimation::RDOColor foreground;
	rdoAnimation::RDOColoredElement elem(background, foreground);

	rdoAnimation::RDOEllipseElement ellipse(boundedElement, elem);

	BOOST_CHECK(ellipse.m_background == background);
	BOOST_CHECK(ellipse.m_foreground == foreground);
	BOOST_CHECK(ellipse.m_point == point);
	BOOST_CHECK(ellipse.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationBmpElement)
{
	rdoAnimation::RDOPoint point(0,0);
	tstring bmp_name("rdo_bmp");
	tstring mask_name("mask");

	rdoAnimation::RDOBmpElement bmpElement(point, bmp_name, mask_name);

	BOOST_CHECK(bmpElement.m_point == point);
	BOOST_CHECK(bmpElement.m_bmp_name == bmp_name);
	BOOST_CHECK(bmpElement.m_mask_name == mask_name);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationSBmpElement)
{
	tstring bmp_name("rdo_bmp");
	tstring mask_name("mask");

	rdoAnimation::RDOPoint point(0, 0);
	rdoAnimation::RDOSize size(10, 10);
	rdoAnimation::RDOBoundedElement boundedElement(point, size);

	rdoAnimation::RDOSBmpElement sbmpElement(boundedElement, bmp_name, mask_name);

	BOOST_CHECK(sbmpElement.m_bmp_name == bmp_name);
	BOOST_CHECK(sbmpElement.m_mask_name == mask_name);
	BOOST_CHECK(sbmpElement.m_point == point);
	BOOST_CHECK(sbmpElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationActiveElement)
{
	tstring text("RDO");

	rdoAnimation::RDOPoint point(0, 0);
	rdoAnimation::RDOSize size(10, 10);
	rdoAnimation::RDOBoundedElement boundedElement(point, size);

	rdoAnimation::RDOActiveElement actionElement(boundedElement, text);

	BOOST_CHECK(actionElement.m_point == point);
	BOOST_CHECK(actionElement.m_size == size);
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationNullElement)
{
	std::auto_ptr<rdoAnimation::RDONullElement> nullElem(new rdoAnimation::RDONullElement);
	BOOST_CHECK(nullElem.get());
}

BOOST_AUTO_TEST_CASE(RDOCommonAnimationFramet)
{
	rdoAnimation::RDOFrame frame;
	BOOST_CHECK(!frame.hasBgImage());
}

BOOST_AUTO_TEST_SUITE_END() // RDOCommonAnimation_Test
