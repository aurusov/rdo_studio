#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <qstring.h>
#include <qglobal.h>

#ifdef Q_OS_UNIX
#include <qstringlist.h>
#endif

// -------------------------------------------------------
// -------- WGConfigFile
// -------------------------------------------------------
class WGConfigFile
{

#ifdef Q_OS_UNIX
private:
	QStringList config;
	bool save_config;
	void open();
	void save();
	QString getConfigValue( QString& name_value, bool& ok, const char* section = NULL );
	void setConfigValue( QString& name_value, QString value, const char* section = NULL );
#endif

public:
	WGConfigFile();
	~WGConfigFile();

	QString read_str( QString& name_value, QString def_value, const char* section = NULL );
	QString read_str( const char* name_value, QString def_value, const char* section = NULL );
	int read_int( QString& name_value, const int def_value, const char* section = NULL );
	int read_int( const char* name_value, const int def_value, const char* section = NULL );
	bool read_bool( QString& name_value, const bool def_value, const char* section = NULL );
	bool read_bool( const char* name_value, const bool def_value, const char* section = NULL );

	void write( QString& name_value, QString value, const char* section = NULL );
	void write( const char* name_value, QString value, const char* section = NULL );
	void write( QString& name_value, const int value, const char* section = NULL );
	void write( const char* name_value, const int value, const char* section = NULL );
	void write( QString& name_value, const bool value, const char* section = NULL );
	void write( const char* name_value, const bool value, const char* section = NULL );
};

#endif // CONFIG_FILE_H
