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
	rbool operator ==( const RDOEditorEditTheme& theme ) const;
	rbool operator !=( const RDOEditorEditTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	COLORREF foldFgColor;
	COLORREF foldBgColor;

	COLORREF errorBgColor;

	RDOFoldStyle foldStyle;
	rbool        commentFold;

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
	rbool operator ==( const RDOEditorEditAutoComplete& autoComplete ) const;
	rbool operator !=( const RDOEditorEditAutoComplete& autoComplete ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	rbool useAutoComplete;
	rbool showFullList;
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
	rbool operator ==( const RDOEditorEditBuffer& buffer ) const;
	rbool operator !=( const RDOEditorEditBuffer& buffer ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	rbool canClearBuffer;
	int   clearBufferDelay;
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
	rbool operator ==( const RDOEditorEditMargin& margin ) const;
	rbool operator !=( const RDOEditorEditMargin& margin ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	rbool fold;
	rbool bookmark;
	rbool lineNumber;
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
	rbool operator ==( const RDOEditorEditStyle& style ) const;
	rbool operator !=( const RDOEditorEditStyle& style ) const;

	virtual void  init( CREF(tstring) _regPath = "" );
	virtual rbool load();
	virtual rbool save() const;

	RDOEditorEditAutoComplete* autoComplete;
	RDOEditorEditBuffer*       buffer;
	RDOEditorEditMargin*       margin;
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDITSTYLE_H_
