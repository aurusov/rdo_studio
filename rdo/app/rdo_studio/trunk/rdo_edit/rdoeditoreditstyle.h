/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoreditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDITSTYLE_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditTheme
// --------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditAutoComplete
// --------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditBuffer
// --------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditMargin
// --------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditStyle
// --------------------------------------------------------------------------------
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

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDITSTYLE_H_
