#ifndef UNICODE_LOG_H
#define UNICODE_LOG_H

#include <qstringlist.h>
#include <qstring.h>

// -------------------------------------------------------
// -------- WGUnicodeLog
// -------------------------------------------------------
class WGUnicodeLog: public QStringList
{

private:
	QStringList native;

public:
	WGUnicodeLog();
	~WGUnicodeLog();

	unsigned int unicode_count() const;
	void unicode_append( QString str );
	void unicode_append( const char* str );
	void unicode_remove_last();
	void unicode_clear();
	QString& unicode_at( unsigned int index );
	QString& unicode_last();
	QString& native_at( unsigned int index );
	QString& native_last();
};

#endif // UNICODE_LOG_H
