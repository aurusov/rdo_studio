#ifndef INT_VALIDATOR_H
#define INT_VALIDATOR_H

#include <qvalidator.h>
#include <qstring.h>
#include <qwidget.h>

// -------------------------------------------------------
// -------- WGIntValidator
// -------------------------------------------------------
class WGIntValidator: public QIntValidator
{ Q_OBJECT

protected:
	virtual QValidator::State validate( QString& input, int& pos ) const;

public:
	WGIntValidator( QWidget* parent = NULL, const char* name = NULL );
	~WGIntValidator();

};

#endif // INT_VALIDATOR_H
