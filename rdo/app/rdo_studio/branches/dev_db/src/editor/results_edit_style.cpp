/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      results_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/results_edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;
using namespace rdo::gui::editor;

ResultsStyle::ResultsStyle(): ParserStyle()
{}

ResultsStyle::~ResultsStyle()
{}

ResultsStyle ResultsStyle::getDefaultStyle()
{
	ResultsStyle style;
	return style;
}

ResultsStyle ResultsStyle::getCppStyle()
{
	ResultsStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getDefaultStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordColor    = QColor( 0x00, 0x00, 0xFF );
	style.functionsColor  = QColor( 0x00, 0x00, 0xFF );
	style.traceColor      = QColor( 0x00, 0x00, 0xFF );
	style.colorColor      = QColor( 0x00, 0x00, 0xFF );
	style.commentColor    = QColor( 0x00, 0x80, 0x00 );
	style.numberColor     = QColor( 0x00, 0x00, 0x00 );
	style.stringColor     = QColor( 0x00, 0x00, 0x00 );
	style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::NONE;
	style.functionsStyle  = StyleFont::NONE;
	style.traceStyle      = StyleFont::NONE;
	style.colorStyle      = StyleFont::NONE;
	style.commentStyle    = StyleFont::NONE;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ResultsStyle ResultsStyle::getPascalStyle()
{
	ResultsStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getDefaultStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	style.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	style.traceColor      = QColor( 0x00, 0x00, 0x00 );
	style.colorColor      = QColor( 0x00, 0x00, 0x00 );
	style.commentColor    = QColor( 0x00, 0x00, 0x80 );
	style.numberColor     = QColor( 0x00, 0x00, 0x00 );
	style.stringColor     = QColor( 0x00, 0x00, 0x00 );
	style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::BOLD;
	style.functionsStyle  = StyleFont::BOLD;
	style.traceStyle      = StyleFont::BOLD;
	style.colorStyle      = StyleFont::BOLD;
	style.commentStyle    = StyleFont::ITALIC;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ResultsStyle ResultsStyle::getHtmlStyle()
{
	ResultsStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getDefaultStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordColor    = QColor( 0x80, 0x00, 0x80 );
	style.functionsColor  = QColor( 0xFF, 0x00, 0x00 );
	style.traceColor      = QColor( 0xFF, 0x00, 0x00 );
	style.colorColor      = QColor( 0xFF, 0x00, 0x00 );
	style.commentColor    = QColor( 0x00, 0x80, 0x00 );
	style.numberColor     = QColor( 0x00, 0x00, 0xFF );
	style.stringColor     = QColor( 0x00, 0x00, 0xFF );
	style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::NONE;
	style.functionsStyle  = StyleFont::NONE;
	style.traceStyle      = StyleFont::NONE;
	style.colorStyle      = StyleFont::NONE;
	style.commentStyle    = StyleFont::NONE;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ResultsStyle ResultsStyle::getClassicStyle()
{
	ResultsStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getClassicStyle();

	style.identifierColor = QColor( 0xFF, 0xFF, 0x00 );
	style.keywordColor    = QColor( 0xFF, 0xFF, 0xFF );
	style.functionsColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.traceColor      = QColor( 0xFF, 0xFF, 0xFF );
	style.colorColor      = QColor( 0xFF, 0xFF, 0xFF );
	style.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	style.numberColor     = QColor( 0xFF, 0xFF, 0x00 );
	style.stringColor     = QColor( 0xFF, 0xFF, 0x00 );
	style.operatorColor   = QColor( 0xFF, 0xFF, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::NONE;
	style.functionsStyle  = StyleFont::NONE;
	style.traceStyle      = StyleFont::NONE;
	style.colorStyle      = StyleFont::NONE;
	style.commentStyle    = StyleFont::NONE;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ResultsStyle ResultsStyle::getTwilightStyle()
{
	ResultsStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getTwilightStyle();

	style.identifierColor = QColor( 0xFF, 0xFF, 0xFF );
	style.keywordColor    = QColor( 0x00, 0xFF, 0xFF );
	style.functionsColor  = QColor( 0xFF, 0xFF, 0x00 );
	style.traceColor      = QColor( 0x00, 0xFF, 0x00 );
	style.colorColor      = QColor( 0x00, 0xFF, 0x00 );
	style.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	style.numberColor     = QColor( 0xC0, 0xC0, 0xC0 );
	style.stringColor     = QColor( 0xFF, 0xFF, 0xFF );
	style.operatorColor   = QColor( 0xFF, 0xFF, 0xFF );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::BOLD;
	style.functionsStyle  = StyleFont::BOLD;
	style.traceStyle      = static_cast<StyleFont::style>(StyleFont::BOLD | StyleFont::ITALIC);
	style.colorStyle      = StyleFont::BOLD;
	style.commentStyle    = StyleFont::ITALIC;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ResultsStyle ResultsStyle::getOceanStyle()
{
	ResultsStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getOceanStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0xFF );
	style.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	style.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	style.traceColor      = QColor( 0x00, 0x00, 0x00 );
	style.colorColor      = QColor( 0x00, 0x00, 0x00 );
	style.commentColor    = QColor( 0x00, 0x80, 0x80 );
	style.numberColor     = QColor( 0x00, 0x00, 0xFF );
	style.stringColor     = QColor( 0x00, 0x00, 0xFF );
	style.operatorColor   = QColor( 0x00, 0x00, 0xFF );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::BOLD;
	style.functionsStyle  = StyleFont::BOLD;
	style.traceStyle      = StyleFont::BOLD;
	style.colorStyle      = StyleFont::BOLD;
	style.commentStyle    = StyleFont::ITALIC;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}