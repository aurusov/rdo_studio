/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoreditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITOREDITSTYLE_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITOREDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/signal.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditorbaseeditstyle.h"
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

	QColor foldFgColor;
	QColor foldBgColor;

	QColor errorBgColor;

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
	RDOEditorEditMargin*       margin;

	template <class CallbackFun>
	void attachSubscriber(const CallbackFun& subscriber)
	{
		m_subscriberList.connect(subscriber);
		subscriber(*this);
	}

	template <class CallbackFun>
	void detachSubscriber(const CallbackFun& subscriber)
	{
		m_subscriberList.disconnect(subscriber);
	}

private:
	typedef boost::signal<void (const RDOEditorEditStyle&)> SubscriberList;
	SubscriberList m_subscriberList;
};

DECLARE_POINTER(RDOEditorEditStyle)

} // namespace rdoEditor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITOREDITSTYLE_H_
