#include "int_validator.h"

// -------------------------------------------------------
// -------- WGIntValidator
// -------------------------------------------------------
WGIntValidator::WGIntValidator( QWidget* parent, const char* name ): QIntValidator( 0, INT_MAX, parent, name )
{
}

WGIntValidator::~WGIntValidator()
{
}

QValidator::State WGIntValidator::validate( QString& input, int& pos ) const
{
	QValidator::State res = QIntValidator::validate( input, pos );
	if ( res == QValidator::Intermediate && !input.isEmpty() ) return QValidator::Invalid;
	return res;
}
