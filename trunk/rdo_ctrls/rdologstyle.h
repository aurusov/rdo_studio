#ifndef RDOLOGSTYLE_H
#define RDOLOGSTYLE_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOFont
// ----------------------------------------------------------------------------
enum RDOFontStyle {
	RDOFS_NONE      = 0x00,
	RDOFS_BOLD      = 0x01,
	RDOFS_ITALIC    = 0x02,
	RDOFS_UNDERLINE = 0x04
};

class RDOFont
{
public:
	RDOFont();
	virtual ~RDOFont();

	RDOFont& operator =( const RDOFont& font );
	bool operator ==( const RDOFont& font ) const;
	bool operator !=( const RDOFont& font ) const;

	CString      name;
	int          size;
	RDOFontStyle style;
	int          codepage;
	int          characterSet;
	
	void load();
	void save() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOLogStyle
// ----------------------------------------------------------------------------
class RDOLogStyle
{
public:
	RDOLogStyle();
	virtual ~RDOLogStyle();
	
	int vertBorder;
	int horzBorder;

	RDOFont font;

	virtual bool getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const;
	virtual bool getItemColors( const CString& item, COLORREF& textColor, COLORREF& backColor ) const;

	RDOLogStyle& operator =( const RDOLogStyle& style );
	bool operator ==( const RDOLogStyle& style ) const;
	bool operator !=( const RDOLogStyle& style ) const;

	static RDOLogStyle getDefaultStyle();

	virtual void load();
	virtual void save() const;
};

#endif // RDOLOGSTYLE_H
