#ifndef RDOEDITOREDITSTYLE_H
#define RDOEDITOREDITSTYLE_H
#pragma once

#include "rdoeditorbaseeditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
enum RDOFoldStyle {
	RDOFOLDS_NONE = 0,
	RDOFOLDS_PLUS,
	RDOFOLDS_PLUSCONNECTED,
	RDOFOLDS_ARROW,
	RDOFOLDS_ARROWCONNECTED,
	RDOFOLDS_BOXCONNECTED,
	RDOFOLDS_CIRCLECONNECTED
};

class RDOEditorEditTheme: public RDOEditorBaseEditTheme
{
public:
	RDOEditorEditTheme();
	virtual ~RDOEditorEditTheme();

	RDOEditorEditTheme& operator =( const RDOEditorEditTheme& theme );
	bool operator ==( const RDOEditorEditTheme& theme ) const;
	bool operator !=( const RDOEditorEditTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF foldFgColor;
	COLORREF foldBgColor;

	COLORREF errorBgColor;

	RDOFoldStyle foldStyle;
	bool         commentFold;

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
// ---------- RDOEditorEditMargin
// ----------------------------------------------------------------------------
class RDOEditorEditMargin
{
public:
	RDOEditorEditMargin();
	virtual ~RDOEditorEditMargin();

	RDOEditorEditMargin& operator =( const RDOEditorEditMargin& margin );
	bool operator ==( const RDOEditorEditMargin& margin ) const;
	bool operator !=( const RDOEditorEditMargin& margin ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	bool fold;
	bool bookmark;
	bool lineNumber;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
class RDOEditorEditStyle: public RDOEditorBaseEditStyle
{
protected:
	virtual void initTheme();
	virtual void initAutoComplete();
	virtual void initBuffer();
	virtual void initMargin();

public:
	RDOEditorEditStyle();
	virtual ~RDOEditorEditStyle();

	RDOEditorEditStyle& operator =( const RDOEditorEditStyle& style );
	bool operator ==( const RDOEditorEditStyle& style ) const;
	bool operator !=( const RDOEditorEditStyle& style ) const;

	virtual void init( const std::string& _regPath = "" );
	virtual bool load();
	virtual bool save() const;

	RDOEditorEditAutoComplete* autoComplete;
	RDOEditorEditBuffer*       buffer;
	RDOEditorEditMargin*       margin;
};

}; // namespace rdoEditor

#endif // RDOEDITOREDITSTYLE_H
