#ifndef RDOBASECTRLSTYLE_H
#define RDOBASECTRLSTYLE_H
#pragma once

namespace rdoEditCtrl {

// ----------------------------------------------------------------------------
// ---------- RDOBaseEditFont
// ----------------------------------------------------------------------------
enum RDOFontStyle {
	RDOFS_NONE      = 0x00,
	RDOFS_BOLD      = 0x01,
	RDOFS_ITALIC    = 0x02,
	RDOFS_UNDERLINE = 0x04
};

class RDOBaseCtrlFont
{
public:
	RDOBaseCtrlFont();
	virtual ~RDOBaseCtrlFont();

	RDOBaseCtrlFont& operator =( const RDOBaseCtrlFont& font );
	bool operator ==( const RDOBaseCtrlFont& font ) const;
	bool operator !=( const RDOBaseCtrlFont& font ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	std::string name;
	int         size;
	int         codepage;
	int         characterSet;
};

// ----------------------------------------------------------------------------
// ---------- RDOBaseCtrlStyle
// ----------------------------------------------------------------------------
class RDOBaseCtrlStyle
{
protected:
	std::string regPath;

	virtual void initFont();

public:
	RDOBaseCtrlStyle();
	virtual ~RDOBaseCtrlStyle();

	RDOBaseCtrlStyle& operator =( const RDOBaseCtrlStyle& style );
	bool operator ==( const RDOBaseCtrlStyle& style ) const;
	bool operator !=( const RDOBaseCtrlStyle& style ) const;

	virtual void init( const std::string& _regPath = "" );
	virtual bool load();
	virtual bool save() const;

	RDOBaseCtrlFont* font;
};

}; // namespace rdoEditCtrl

#endif // RDOBASECTRLSTYLE_H
