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

namespace rdo { namespace gui { namespace editor {

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

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

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

QSettings& operator<< (QSettings& settings, const RDOEditorEditTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOEditorEditTheme& theme);

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

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	rbool useAutoComplete;
	rbool showFullList;
};

QSettings& operator<< (QSettings& settings, const RDOEditorEditAutoComplete& auto_complete);
QSettings& operator>> (QSettings& settings,       RDOEditorEditAutoComplete& auto_complete);

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

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	rbool fold;
	rbool bookmark;
	rbool lineNumber;
};

QSettings& operator<< (QSettings& settings, const RDOEditorEditMargin& margin);
QSettings& operator>> (QSettings& settings,       RDOEditorEditMargin& margin);

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditStyle
// --------------------------------------------------------------------------------
class RDOEditorEditStyle: public RDOEditorBaseEditStyle
{
protected:

public:

	RDOEditorEditStyle();
	virtual ~RDOEditorEditStyle();

	RDOEditorEditStyle& operator =( const RDOEditorEditStyle& style );
	rbool operator ==( const RDOEditorEditStyle& style ) const;
	rbool operator !=( const RDOEditorEditStyle& style ) const;

	void  init( CREF(QString) _groupName = "" );
	rbool load();
	rbool save() const;

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

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITOREDITSTYLE_H_
