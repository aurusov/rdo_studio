/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2010
  \brief     Тест типов парсера
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <iostream>
#define BOOST_TEST_MODULE RDOParserType_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdortp.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOParserType_Test)

BOOST_AUTO_TEST_CASE(RDOParserType_Test_int)
{
	rdo::compiler::parser::LPIType pType = rdo::Factory<rdo::compiler::parser::RDOType__int>::create();
	BOOST_CHECK(pType);
}

BOOST_AUTO_TEST_CASE(RDOParserType_RTP_RDOValue)
{
	using namespace rdo::compiler::parser;
	const LPRDOParser parser = rdo::Factory<RDOParser>::create();
	BOOST_CHECK(parser);
	parser->init();
	const LPRDORTPResType rtp = rdo::Factory<RDORTPResType>::create(parser, RDOParserSrcInfo(), true);
	BOOST_CHECK(rtp);
	const LPRDORSSResource resource = rdo::Factory<RDORSSResource>::create(parser, RDOParserSrcInfo(), rtp);
	BOOST_CHECK(resource);
	const rdo::runtime::RDOValue value(rtp, resource);
	BOOST_CHECK(value.getPointerByType<RDORTPResType>() == resource);
}

BOOST_AUTO_TEST_SUITE_END()
