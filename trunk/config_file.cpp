#include "config_file.h"

#ifdef Q_OS_UNIX

#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>
#define general_section "general"

#else

#include <qsettings.h>
#define winwget_register "winwget"

#endif

// -------------------------------------------------------
// -------- WGConfigFile
// -------------------------------------------------------
#ifdef Q_OS_UNIX
WGConfigFile::WGConfigFile(): save_config( false )
#else
WGConfigFile::WGConfigFile()
#endif
{
#ifdef Q_OS_UNIX
	config.clear();
	open();
#endif
}

WGConfigFile::~WGConfigFile()
{
#ifdef Q_OS_UNIX
	save();
	config.clear();
#endif
}

#ifdef Q_OS_UNIX // begin for Q_OS_UNIX systems

void WGConfigFile::open()
{
	QDir dir( QDir::homeDirPath() );
	if ( dir.exists() ) {
		QFile file( dir.filePath(".winwget") );
		if ( file.open(IO_ReadOnly) ) {
			QTextStream stream( &file );
			config.clear();
			QString section;
			section.sprintf( "[%s]", general_section );
			QString s;
			while ( !stream.atEnd() ) {
				s = stream.readLine().stripWhiteSpace();
				if ( s.find( '[' ) == 0 ) {
					int pos = s.find( ']' );
					if ( pos != -1 ) {
						section = s.left( pos + 1 );
					}
				} else {
					if ( !s.isEmpty() ) config.push_back( section + s );
				}
			}
			file.close();
			config.sort();
		}
	}
}

void WGConfigFile::save()
{
	if ( save_config ) {
		QDir dir( QDir::homeDirPath() );
		if ( dir.exists() ) {
			QFile file( dir.filePath(".winwget") );
			if ( file.open(IO_WriteOnly) ) {
				QTextStream stream( &file );
				config.sort();
				QString s       = "";
				QString section = "";
				for ( unsigned int i = 0; i < config.size(); i++ ) {
					s = config[i];
					int pos = s.find( ']' );
					s = s.left( pos + 1 );
					if ( s != section ) {
						section = s;
						if ( i != 0 ) stream << endl;
						stream << section << endl;
					}
					stream << config[i].right( config[i].length() - 1 - pos ) << endl;
				}
				file.close();
			}
		}
	}
}

QString WGConfigFile::getConfigValue( QString& name_value, bool& ok, const char* section )
{
	QString res = "";
	QString str = section ? section : general_section;
	QString sect;
	sect.sprintf( "[%s]", str.latin1() );
	QString str_section;
	name_value = name_value.stripWhiteSpace();
	for ( unsigned int i = 0; i < config.size(); i++ ) {
		str = config[i];
		int pos = str.find( ']' );
		str_section = str.left( pos + 1 );
		if ( sect == str_section ) {
			str = str.right( str.length() - 1 - pos );
			if ( str.find( name_value ) == 0 ) {
				str.remove( 0, name_value.length() + 1 ); // + 1 for removing '=' char after value name
				if ( str.find('"') == 0 && str.findRev('"') == (int)(str.length()-1) ) {
					str.remove( str.length()-1, 1 );
					str.remove( 0, 1 );
				}
				ok = true;
				return str;
			}
		}
	}
	ok = false;
	return res;
}

void WGConfigFile::setConfigValue( QString& name_value, QString value, const char* section )
{
	QString str = section ? section : general_section;
	QString sect;
	sect.sprintf( "[%s]", str.latin1() );
	QString str_section;
	name_value = name_value.stripWhiteSpace();
	bool found = false;
	for ( unsigned int i = 0; i < config.size(); i++ ) {
		str = config[i];
		int pos = str.find( ']' );
		str_section = str.left( pos + 1 );
		if ( sect == str_section ) {
			str = str.right( str.length() - 1 - pos );
			if ( str.find( name_value ) == 0 ) {
				pos += name_value.length() + 1; // + 1 for adding '=' char after value name
				str = config[i].left( pos + 1 ); 
				str += value;
				config[i] = str;
				found = true;
				break;
			}
		}
	}
	if ( !found ) {
		QString s;
		s.sprintf( "%s%s=", sect.latin1(), name_value.latin1() );
		s += value;
		config.push_back( s );
	}
	save_config = true;
}

QString WGConfigFile::read_str( QString& name_value, QString def_value, const char* section )
{
	bool ok;
	QString s =	getConfigValue( name_value, ok, section );
	return ok ? s : def_value;
}

int WGConfigFile::read_int( QString& name_value, const int def_value, const char* section )
{
	bool ok;
	QString s =	getConfigValue( name_value, ok, section );
	int i;
	if ( ok ) i = s.toInt( &ok );
	return ok ? i : def_value;
}

bool WGConfigFile::read_bool( QString& name_value, const bool def_value, const char* section )
{
	bool ok;
	QString s =	getConfigValue( name_value, ok, section );
	bool b;
	if ( ok ) b = s.toInt( &ok ) ? true : false;
	return ok ? b : def_value;
}

void WGConfigFile::write( QString& name_value, QString value, const char* section )
{
	QString s = "\"";
	s += value;
	s += "\"";
	setConfigValue( name_value, s, section );
}

void WGConfigFile::write( QString& name_value, const int value, const char* section )
{
	setConfigValue( name_value, QString::number( value ), section );
}

void WGConfigFile::write( QString& name_value, const bool value, const char* section )
{
	QString s;
	if ( value ) {
		s = "1";
	} else {
		s = "0";
	}
	setConfigValue( name_value, s, section );
}

#else // begin for no Q_OS_UNIX systems

QString WGConfigFile::read_str( QString& name_value, QString def_value, const char* section )
{
	QSettings reg;
	QString s;
	if ( section ) {
		s.sprintf( "/%s/%s/%s", winwget_register, section, name_value.latin1() );
	} else {
		s.sprintf( "/%s/%s", winwget_register, name_value.latin1() );
	}
	return reg.readEntry( s, def_value );
}

int WGConfigFile::read_int( QString& name_value, const int def_value, const char* section )
{
	QSettings reg;
	QString s;
	if ( section ) {
		s.sprintf( "/%s/%s/%s", winwget_register, section, name_value.latin1() );
	} else {
		s.sprintf( "/%s/%s", winwget_register, name_value.latin1() );
	}
	return reg.readNumEntry( s, def_value );
}

bool WGConfigFile::read_bool( QString& name_value, const bool def_value, const char* section )
{
	QSettings reg;
	QString s;
	if ( section ) {
		s.sprintf( "/%s/%s/%s", winwget_register, section, name_value.latin1() );
	} else {
		s.sprintf( "/%s/%s", winwget_register, name_value.latin1() );
	}
	return reg.readBoolEntry( s, def_value );
}

void WGConfigFile::write( QString& name_value, QString value, const char* section )
{
	QSettings reg;
	QString s;
	if ( section ) {
		s.sprintf( "/%s/%s/%s", winwget_register, section, name_value.latin1() );
	} else {
		s.sprintf( "/%s/%s", winwget_register, name_value.latin1() );
	}
	reg.writeEntry( s, value );
}

void WGConfigFile::write( QString& name_value, const int value, const char* section )
{
	QSettings reg;
	QString s;
	if ( section ) {
		s.sprintf( "/%s/%s/%s", winwget_register, section, name_value.latin1() );
	} else {
		s.sprintf( "/%s/%s", winwget_register, name_value.latin1() );
	}
	reg.writeEntry( s, value );
}

void WGConfigFile::write( QString& name_value, const bool value, const char* section )
{
	QSettings reg;
	QString s;
	if ( section ) {
		s.sprintf( "/%s/%s/%s", winwget_register, section, name_value.latin1() );
	} else {
		s.sprintf( "/%s/%s", winwget_register, name_value.latin1() );
	}
	reg.writeEntry( s, value );
}

#endif // endif for no Q_OS_UNIX systems

QString WGConfigFile::read_str( const char* name_value, QString def_value, const char* section )
{
	QString n = name_value;
	QString d = def_value;
	return read_str( n, d, section );
}

int WGConfigFile::read_int( const char* name_value, const int def_value, const char* section )
{
	QString n = name_value;
	return read_int( n, def_value, section );
}

bool WGConfigFile::read_bool( const char* name_value, const bool def_value, const char* section )
{
	QString n = name_value;
	return read_bool( n, def_value, section );
}

void WGConfigFile::write( const char* name_value, QString value, const char* section )
{
	QString n = name_value;
	write( n, value, section );
}

void WGConfigFile::write( const char* name_value, const int value, const char* section )
{
	QString n = name_value;
	write( n, value, section );
}

void WGConfigFile::write( const char* name_value, const bool value, const char* section )
{
	QString n = name_value;
	write( n, value, section );
}
