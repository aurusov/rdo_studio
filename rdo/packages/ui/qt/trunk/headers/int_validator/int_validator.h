/*!
  \copyright (c) RDO-Team, 2013
  \file      int_validator.h
  \author    Романов Ярослав (rdo@rk9.bmstu.ru)
  \date      25.02.2013
  \brief     QIntValidator для настроек
  \indent    4T
*/

#ifndef _UI_QT_INTVALIDATOR_H_
#define _UI_QT_INTVALIDATOR_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qvalidator>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class IntValidator: public QIntValidator
{
public:
	IntValidator(int bottom, int top, PTR(QObject) pParent)
		: QIntValidator(bottom, top, pParent)
	{}

	virtual QValidator::State validate(QString& input, int& pos) const
	{
		QIntValidator::State result(QIntValidator::validate(input, pos));
		if (result == QValidator::Intermediate)
		{
			result = QValidator::Invalid;
		}
		return result;
	}
};

#endif // _UI_QT_INTVALIDATOR_H_
