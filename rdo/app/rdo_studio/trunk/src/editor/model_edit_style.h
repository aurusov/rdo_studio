/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      model_edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_MODEL_EDIT_STYLE_H_
#define _RDO_STUDIO_EDITOR_MODEL_EDIT_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/signal.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- ModelTheme
// --------------------------------------------------------------------------------
class ModelTheme: public ParserTheme
{
public:
	enum Fold
	{
		F_NONE = 0,
		F_PLUS,
		F_PLUSCONNECTED,
		F_ARROW,
		F_ARROWCONNECTED,
		F_BOXCONNECTED,
		F_CIRCLECONNECTED
	};

	ModelTheme();
	virtual ~ModelTheme();

	ModelTheme& operator =( const ModelTheme& theme );
	rbool operator ==( const ModelTheme& theme ) const;
	rbool operator !=( const ModelTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor foldFgColor;
	QColor foldBgColor;
	QColor errorBgColor;
	Fold   foldStyle;
	rbool  commentFold;

	static ModelTheme getDefaultTheme();
	static ModelTheme getCppTheme();
	static ModelTheme getPascalTheme();
	static ModelTheme getHtmlTheme();
	static ModelTheme getClassicTheme();
	static ModelTheme getTwilightTheme();
	static ModelTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const ModelTheme& theme);
QSettings& operator>> (QSettings& settings,       ModelTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- ModelAutoComplete
// --------------------------------------------------------------------------------
class ModelAutoComplete
{
public:
	ModelAutoComplete();
	virtual ~ModelAutoComplete();

	ModelAutoComplete& operator =( const ModelAutoComplete& autoComplete );
	rbool operator ==( const ModelAutoComplete& autoComplete ) const;
	rbool operator !=( const ModelAutoComplete& autoComplete ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	rbool useAutoComplete;
	rbool showFullList;
};

QSettings& operator<< (QSettings& settings, const ModelAutoComplete& auto_complete);
QSettings& operator>> (QSettings& settings,       ModelAutoComplete& auto_complete);

// --------------------------------------------------------------------------------
// -------------------- ModelMargin
// --------------------------------------------------------------------------------
class ModelMargin
{
public:
	ModelMargin();
	virtual ~ModelMargin();

	ModelMargin& operator =( const ModelMargin& margin );
	rbool operator ==( const ModelMargin& margin ) const;
	rbool operator !=( const ModelMargin& margin ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	rbool fold;
	rbool bookmark;
	rbool lineNumber;
};

QSettings& operator<< (QSettings& settings, const ModelMargin& margin);
QSettings& operator>> (QSettings& settings,       ModelMargin& margin);

// --------------------------------------------------------------------------------
// -------------------- ModelStyle
// --------------------------------------------------------------------------------
class ModelStyle: public ParserStyle
{
protected:

public:

	ModelStyle();
	virtual ~ModelStyle();

	ModelStyle& operator =( const ModelStyle& style );
	rbool operator ==( const ModelStyle& style ) const;
	rbool operator !=( const ModelStyle& style ) const;

	void  init( CREF(QString) _groupName = "" );
	rbool load();
	rbool save() const;

	ModelAutoComplete* autoComplete;
	ModelMargin*       margin;

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
	typedef boost::signal<void (const ModelStyle&)> SubscriberList;
	SubscriberList m_subscriberList;
};

DECLARE_POINTER(ModelStyle)

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_MODEL_EDIT_STYLE_H_
