#ifndef RDOEDITORSCILOGSTYLE_H
#define RDOEDITORSCILOGSTYLE_H
#pragma once

#include "rdoeditorscieditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLogTheme
// ----------------------------------------------------------------------------
class RDOEditorSciLogTheme: public RDOEditorSciEditTheme
{
public:
	RDOEditorSciLogTheme();
	virtual ~RDOEditorSciLogTheme();

	RDOEditorSciLogTheme& operator =( const RDOEditorSciLogTheme& theme );
	bool operator ==( const RDOEditorSciLogTheme& theme ) const;
	bool operator !=( const RDOEditorSciLogTheme& theme ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	COLORREF selectLineBgColor;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLogStyle
// ----------------------------------------------------------------------------
class RDOEditorSciLogStyle: public RDOEditorSciEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOEditorSciLogStyle();
	virtual ~RDOEditorSciLogStyle();

	RDOEditorSciLogStyle& operator =( const RDOEditorSciLogStyle& style );
	bool operator ==( const RDOEditorSciLogStyle& style ) const;
	bool operator !=( const RDOEditorSciLogStyle& style ) const;
};

}; // namespace rdoEditor

#endif // RDOEDITORSCILOGSTYLE_H
