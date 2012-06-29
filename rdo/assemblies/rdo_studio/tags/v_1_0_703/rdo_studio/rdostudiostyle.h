#ifndef RDOSTUDIOSTYLE_H
#define RDOSTUDIOSTYLE_H
#pragma once

namespace rdoStyle {

// ----------------------------------------------------------------------------
// ---------- RDOStyleFont
// ----------------------------------------------------------------------------
class RDOStyleFont
{
public:
	RDOStyleFont();
	virtual ~RDOStyleFont();

	RDOStyleFont& operator =( const RDOStyleFont& font );
	bool operator ==( const RDOStyleFont& font ) const;
	bool operator !=( const RDOStyleFont& font ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	enum style { NONE = 0x00, BOLD = 0x01, ITALIC = 0x02, UNDERLINE = 0x04 };

	std::string name;
	int         size;
	int         codepage;
	int         characterSet;

	static RDOStyleFont getDefaultFont();
	static RDOStyleFont getClassicFont();
	static RDOStyleFont getTracerLogFont();
	static RDOStyleFont getChartViewFont();
	static RDOStyleFont getFrameFont();
};

// ----------------------------------------------------------------------------
// ---------- RDOStyleTheme
// ----------------------------------------------------------------------------
class RDOStyleTheme
{
public:
	RDOStyleTheme();
	virtual ~RDOStyleTheme();

	RDOStyleTheme& operator =( const RDOStyleTheme& theme );
	bool operator ==( const RDOStyleTheme& theme ) const;
	bool operator !=( const RDOStyleTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF defaultColor;
	COLORREF backgroundColor;

	rdoStyle::RDOStyleFont::style defaultStyle;
};

// ----------------------------------------------------------------------------
// ---------- RDOStyle
// ----------------------------------------------------------------------------
class RDOStyle
{
protected:
	std::string regPath;

	virtual void initFont();

public:
	RDOStyle();
	virtual ~RDOStyle();

	RDOStyle& operator =( const RDOStyle& style );
	bool operator ==( const RDOStyle& style ) const;
	bool operator !=( const RDOStyle& style ) const;

	virtual void init( const std::string& _regPath = "" );
	virtual bool load();
	virtual bool save() const;

	RDOStyleFont* font;
};

// ----------------------------------------------------------------------------
// ---------- RDOStyleWithTheme
// ----------------------------------------------------------------------------
class RDOStyleWithTheme: public RDOStyle
{
protected:
	virtual void initTheme();

public:
	RDOStyleWithTheme();
	virtual ~RDOStyleWithTheme();

	RDOStyleWithTheme& operator =( const RDOStyleWithTheme& style );
	bool operator ==( const RDOStyleWithTheme& style ) const;
	bool operator !=( const RDOStyleWithTheme& style ) const;

	virtual void init( const std::string& _regPath = "" );
	virtual bool load();
	virtual bool save() const;

	RDOStyleTheme* theme;
};

}; // namespace rdoStyle

#endif // RDOSTUDIOSTYLE_H
