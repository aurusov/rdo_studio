#ifndef RDOEDITOREDITSTYLE_H
#define RDOEDITOREDITSTYLE_H
#pragma once

#include "../edit_ctrls/rdobaseeditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
class RDOEditorEditTheme: public rdoBaseEdit::RDOBaseEditTheme
{
public:
	RDOEditorEditTheme();
	virtual ~RDOEditorEditTheme();

	RDOEditorEditTheme& operator =( const RDOEditorEditTheme& theme );
	bool operator ==( const RDOEditorEditTheme& theme ) const;
	bool operator !=( const RDOEditorEditTheme& theme ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	COLORREF identifierColor;
	COLORREF keywordColor;
	COLORREF functionsColor;
	COLORREF traceColor;
	COLORREF commentColor;
	COLORREF numberColor;
	COLORREF stringColor;
	COLORREF operatorColor;

	COLORREF errorBgColor;

	rdoBaseEdit::RDOFontStyle identifierStyle;
	rdoBaseEdit::RDOFontStyle keywordStyle;
	rdoBaseEdit::RDOFontStyle functionsStyle;
	rdoBaseEdit::RDOFontStyle traceStyle;
	rdoBaseEdit::RDOFontStyle commentStyle;
	rdoBaseEdit::RDOFontStyle numberStyle;
	rdoBaseEdit::RDOFontStyle stringStyle;
	rdoBaseEdit::RDOFontStyle operatorStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOEditorEditTheme getDefaultTheme();
	static RDOEditorEditTheme getCppTheme();
	static RDOEditorEditTheme getPascalTheme();
	static RDOEditorEditTheme getHtmlTheme();
	static RDOEditorEditTheme getClassicTheme();
	static RDOEditorEditTheme getTwilightTheme();
	static RDOEditorEditTheme getOceanTheme();
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditAutoComplete
// ----------------------------------------------------------------------------
class RDOEditorEditAutoComplete
{
public:
	RDOEditorEditAutoComplete();
	virtual ~RDOEditorEditAutoComplete();

	RDOEditorEditAutoComplete& operator =( const RDOEditorEditAutoComplete& autoComplete );
	bool operator ==( const RDOEditorEditAutoComplete& autoComplete ) const;
	bool operator !=( const RDOEditorEditAutoComplete& autoComplete ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	bool useAutoComplete;
	bool showFullList;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
class RDOEditorEditStyle: public rdoBaseEdit::RDOBaseEditStyle
{
protected:
	virtual void initTheme();
	virtual void initAutoComplete();

public:
	RDOEditorEditStyle();
	virtual ~RDOEditorEditStyle();

	RDOEditorEditStyle& operator =( const RDOEditorEditStyle& style );
	bool operator ==( const RDOEditorEditStyle& style ) const;
	bool operator !=( const RDOEditorEditStyle& style ) const;

	virtual void init( const string& _regPath = "" );
	virtual void load();
	virtual void save() const;

	RDOEditorEditAutoComplete* autoComplete;
};

}; // namespace rdoEditor

#endif // RDOEDITOREDITSTYLE_H
