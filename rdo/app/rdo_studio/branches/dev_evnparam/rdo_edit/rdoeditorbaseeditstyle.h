#ifndef RDOEDITORBASEEDITSTYLE_H
#define RDOEDITORBASEEDITSTYLE_H
#pragma once

#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditTheme
// --------------------------------------------------------------------------------
class RDOEditorBaseEditTheme: public rdoEditCtrl::RDOBaseEditTheme
{
public:
	RDOEditorBaseEditTheme();
	virtual ~RDOEditorBaseEditTheme();

	RDOEditorBaseEditTheme& operator =( const RDOEditorBaseEditTheme& theme );
	bool operator ==( const RDOEditorBaseEditTheme& theme ) const;
	bool operator !=( const RDOEditorBaseEditTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF identifierColor;
	COLORREF keywordColor;
	COLORREF functionsColor;
	COLORREF traceColor;
	COLORREF colorColor;
	COLORREF commentColor;
	COLORREF numberColor;
	COLORREF stringColor;
	COLORREF operatorColor;

	rdoStyle::RDOStyleFont::style identifierStyle;
	rdoStyle::RDOStyleFont::style keywordStyle;
	rdoStyle::RDOStyleFont::style functionsStyle;
	rdoStyle::RDOStyleFont::style traceStyle;
	rdoStyle::RDOStyleFont::style colorStyle;
	rdoStyle::RDOStyleFont::style commentStyle;
	rdoStyle::RDOStyleFont::style numberStyle;
	rdoStyle::RDOStyleFont::style stringStyle;
	rdoStyle::RDOStyleFont::style operatorStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual std::string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOEditorBaseEditTheme getDefaultTheme();
	static RDOEditorBaseEditTheme getCppTheme();
	static RDOEditorBaseEditTheme getPascalTheme();
	static RDOEditorBaseEditTheme getHtmlTheme();
	static RDOEditorBaseEditTheme getClassicTheme();
	static RDOEditorBaseEditTheme getTwilightTheme();
	static RDOEditorBaseEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditStyle
// --------------------------------------------------------------------------------
class RDOEditorBaseEditStyle: public rdoEditCtrl::RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOEditorBaseEditStyle();
	virtual ~RDOEditorBaseEditStyle();

	RDOEditorBaseEditStyle& operator =( const RDOEditorBaseEditStyle& style );
	bool operator ==( const RDOEditorBaseEditStyle& style ) const;
	bool operator !=( const RDOEditorBaseEditStyle& style ) const;
};

}; // namespace rdoEditor

#endif // RDOEDITORBASEEDITSTYLE_H
