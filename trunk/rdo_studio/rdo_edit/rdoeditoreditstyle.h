#ifndef RDOEDITOREDITSTYLE_H
#define RDOEDITOREDITSTYLE_H
#pragma once

#include "../edit_ctrls/rdobaseeditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
class RDOEditorEditTheme: public rdoEditCtrl::RDOBaseEditTheme
{
public:
	RDOEditorEditTheme();
	virtual ~RDOEditorEditTheme();

	RDOEditorEditTheme& operator =( const RDOEditorEditTheme& theme );
	bool operator ==( const RDOEditorEditTheme& theme ) const;
	bool operator !=( const RDOEditorEditTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF identifierColor;
	COLORREF keywordColor;
	COLORREF functionsColor;
	COLORREF traceColor;
	COLORREF commentColor;
	COLORREF numberColor;
	COLORREF stringColor;
	COLORREF operatorColor;

	COLORREF errorBgColor;

	rdoEditCtrl::RDOFontStyle identifierStyle;
	rdoEditCtrl::RDOFontStyle keywordStyle;
	rdoEditCtrl::RDOFontStyle functionsStyle;
	rdoEditCtrl::RDOFontStyle traceStyle;
	rdoEditCtrl::RDOFontStyle commentStyle;
	rdoEditCtrl::RDOFontStyle numberStyle;
	rdoEditCtrl::RDOFontStyle stringStyle;
	rdoEditCtrl::RDOFontStyle operatorStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual std::string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

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

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	bool useAutoComplete;
	bool showFullList;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditBuffer
// ----------------------------------------------------------------------------
class RDOEditorEditBuffer
{
public:
	RDOEditorEditBuffer();
	virtual ~RDOEditorEditBuffer();

	RDOEditorEditBuffer& operator =( const RDOEditorEditBuffer& buffer );
	bool operator ==( const RDOEditorEditBuffer& buffer ) const;
	bool operator !=( const RDOEditorEditBuffer& buffer ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	bool canClearBuffer;
	int  clearBufferDelay;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseStyle
// ----------------------------------------------------------------------------
class RDOEditorBaseStyle: public rdoEditCtrl::RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOEditorBaseStyle();
	virtual ~RDOEditorBaseStyle();

	RDOEditorBaseStyle& operator =( const RDOEditorBaseStyle& style );
	bool operator ==( const RDOEditorBaseStyle& style ) const;
	bool operator !=( const RDOEditorBaseStyle& style ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorResultsStyle
// ----------------------------------------------------------------------------
class RDOEditorResultsStyle: public RDOEditorBaseStyle
{
public:
	RDOEditorResultsStyle();
	virtual ~RDOEditorResultsStyle();
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
class RDOEditorEditStyle: public RDOEditorBaseStyle
{
protected:
	virtual void initAutoComplete();
	virtual void initBuffer();

public:
	RDOEditorEditStyle();
	virtual ~RDOEditorEditStyle();

	RDOEditorEditStyle& operator =( const RDOEditorEditStyle& style );
	bool operator ==( const RDOEditorEditStyle& style ) const;
	bool operator !=( const RDOEditorEditStyle& style ) const;

	virtual void init( const std::string& _regPath = "" );
	virtual void load();
	virtual void save() const;

	RDOEditorEditAutoComplete* autoComplete;
	RDOEditorEditBuffer*       buffer;
};

}; // namespace rdoEditor

#endif // RDOEDITOREDITSTYLE_H
