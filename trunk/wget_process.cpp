#include "wget_process.h"

#include <qprocess.h>
#include <qtextcodec.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qglobal.h>
#include <qdir.h>

// -------------------------------------------------------
// -------- WGProcessList
// -------------------------------------------------------
WGProcessList* WGProcess_s = NULL;

WGProcessList::WGProcessList( QObject* parent ):
	QObject( parent ),
	saveToDir( QDir::homeDirPath() ),
	doRestart( true ),
	dontUseSuffix( false ),
	dontRetrieveOldFile( false ),
	useUsernameAndPassword( false ),
	username( "" ),
	password( "" ),
	writeLog( false ),
	logDirectory( QDir::homeDirPath() ),
	retriesNumber( "" ),
	waitBetweenRetrievals( "" ),
	checkClipboard( true ),
	autostartClipboard( false )
{
	WGProcess_s = this;

	proc_list.setAutoDelete( true );

	readConfig();

	if ( !saveToDir.isEmpty() ) {
		QDir dir( saveToDir );
		if ( !dir.exists() ) saveToDir = "";
	}
	if ( !logDirectory.isEmpty() ) {
		QDir dir( logDirectory );
		if ( !dir.exists() ) logDirectory = "";
	}
}

WGProcessList::~WGProcessList()
{
	proc_list.clear();

	WGProcess_s = NULL;
}

WGProcess* WGProcessList::newProcess()
{
	WGProcess* proc = new WGProcess( this );
	proc_list.append( proc );
	return proc;
}

void WGProcessList::deleteProcess( WGProcess* proc )
{
	if ( proc ) {
		proc_list.remove( indexOf(proc) );
	}
}

WGProcess* WGProcessList::newWgetVersionProcess()
{
	return new WGProcess( this );
}

void WGProcessList::deleteWgetVersionProcess( WGProcess* proc )
{
	delete proc;
}

unsigned int WGProcessList::indexOf( const WGProcess* const proc )
{
	for ( unsigned int i = 0; i < proc_list.count(); i++ )
		if ( proc_list.at( i ) == proc ) return i;

	return (unsigned int)-1;
}

unsigned int WGProcessList::count() const
{
	return proc_list.count();
}

WGProcess* WGProcessList::at( const unsigned int i )
{
	return proc_list.at( i );
}

WGProcess* WGProcessList::current()
{
	return proc_list.current();
}

WGProcess* WGProcessList::first()
{
	return proc_list.first();
}

WGProcess* WGProcessList::last()
{
	return proc_list.last();
}

WGProcess* WGProcessList::next()
{
	return proc_list.next();
}

WGProcess* WGProcessList::prev()
{
	return proc_list.prev();
}

#ifdef Q_OS_UNIX

QString getConfigValue( QString str, const char* search, int& pos )
{
	QString res = "";
	pos = str.find( search );
	if ( pos != -1 ) {
		QString s = search;
		str.remove( 0, s.length() );
		s = s.stripWhiteSpace();
		str = str.stripWhiteSpace();
		if ( s.findRev('=') != (int)(s.length()-1) && str.find('=') == 0 ) {
			str.remove( 0, 1 );
			str = str.stripWhiteSpace();
		}
		if ( str.find('"') == 0 && str.findRev('"') == (int)(str.length()-1) ) {
			str.remove( str.length()-1, 1 );
			str.remove( 0, 1 );
		}
		res = str;
	}
	return res;
}

void WGProcessList::readConfig()
{
	QDir dir( QDir::homeDirPath() );
	if ( dir.exists() ) {
		QFile file( dir.filePath(".winwget") );
		if ( file.open(IO_ReadOnly) ) {
			QTextStream stream( &file );
			QString line;
			QString value;
			int pos;
			while ( !stream.atEnd() ) {
				line = stream.readLine();

				value = getConfigValue( line, "saveToDir", pos );
				if ( pos == 0 ) saveToDir = value;

				value = getConfigValue( line, "doRestart", pos );
				if ( pos == 0 ) doRestart = value.toInt() ? true : false;

				value = getConfigValue( line, "dontUseSuffix", pos );
				if ( pos == 0 ) dontUseSuffix = value.toInt() ? true : false;

				value = getConfigValue( line, "dontRetrieveOldFile", pos );
				if ( pos == 0 ) dontRetrieveOldFile = value.toInt() ? true : false;

				value = getConfigValue( line, "writeLog", pos );
				if ( pos == 0 ) writeLog = value.toInt() ? true : false;

				value = getConfigValue( line, "logDirectory", pos );
				if ( pos == 0 ) logDirectory = value;

				value = getConfigValue( line, "retriesNumber", pos );
				if ( pos == 0 ) retriesNumber = value.toInt();

				value = getConfigValue( line, "waitBetweenRetrievals", pos );
				if ( pos == 0 ) waitBetweenRetrievals = value.toInt();

				value = getConfigValue( line, "checkClipboard", pos );
				if ( pos == 0 ) checkClipboard = value.toInt() ? true : false;

				value = getConfigValue( line, "autostartClipboard", pos );
				if ( pos == 0 ) autostartClipboard = value.toInt() ? true : false;
			}
		}
	}
}

void WGProcessList::writeConfig()
{
	QDir dir( QDir::homeDirPath() );
	if ( dir.exists() ) {
		QFile file( dir.filePath(".winwget") );
		if ( file.open(IO_WriteOnly) ) {
			QTextStream stream( &file );
			stream << "saveToDir=\""            << saveToDir             << "\"" << endl;
			stream << "doRestart="              << doRestart             << endl;
			stream << "dontUseSuffix="          << dontUseSuffix         << endl;
			stream << "dontRetrieveOldFile="    << dontRetrieveOldFile   << endl;
			stream << "writeLog="               << writeLog              << endl;
			stream << "logDirectory=\""         << logDirectory          << "\"" << endl;
			stream << "retriesNumber="          << retriesNumber         << endl;
			stream << "waitBetweenRetrievals="  << waitBetweenRetrievals << endl;
			stream << "checkClipboard="         << checkClipboard        << endl;
			stream << "autostartClipboard="     << autostartClipboard    << endl;
		}
	}
}

#else

#include <qsettings.h>

void WGProcessList::readConfig()
{
	QSettings reg;
	QString empty_str = "";
	saveToDir             = reg.readEntry( "/winwget/saveToDir", empty_str );
	doRestart             = reg.readBoolEntry( "/winwget/doRestart", true );
	dontUseSuffix         = reg.readBoolEntry( "/winwget/dontUseSuffix", false );
	dontRetrieveOldFile   = reg.readBoolEntry( "/winwget/dontRetrieveOldFile", false );
	writeLog              = reg.readBoolEntry( "/winwget/writeLog", false );
	logDirectory          = reg.readEntry( "/winwget/logDirectory", empty_str );
	retriesNumber         = reg.readEntry( "/winwget/retriesNumber", empty_str );
	waitBetweenRetrievals = reg.readEntry( "/winwget/waitBetweenRetrievals", empty_str );
	checkClipboard        = reg.readBoolEntry( "/winwget/checkClipboard", true );
	autostartClipboard    = reg.readBoolEntry( "/winwget/autostartClipboard", false );
}

void WGProcessList::writeConfig()
{
	QSettings reg;
	reg.writeEntry( "/winwget/saveToDir", saveToDir );
	reg.writeEntry( "/winwget/doRestart", doRestart );
	reg.writeEntry( "/winwget/dontUseSuffix", dontUseSuffix );
	reg.writeEntry( "/winwget/dontRetrieveOldFile", dontRetrieveOldFile );
	reg.writeEntry( "/winwget/writeLog", writeLog );
	reg.writeEntry( "/winwget/logDirectory", logDirectory );
	reg.writeEntry( "/winwget/retriesNumber", retriesNumber );
	reg.writeEntry( "/winwget/waitBetweenRetrievals", waitBetweenRetrievals );
	reg.writeEntry( "/winwget/checkClipboard", checkClipboard );
	reg.writeEntry( "/winwget/autostartClipboard", autostartClipboard );
}

#endif


// -------------------------------------------------------
// -------- WGProcess
// -------------------------------------------------------
WGProcess::WGProcess( QObject* parent ):
	QObject( parent),
	new_line( true ),
	logFileName( "" ),
	localFileName( "" ),
	url( "" ),
	time_download_begin( 0, 0, 0, 0 ),
	time_download_end( 0, 0, 0, 0 ),
	time_last_speed_check( 0, 0, 0, 0 ),
	lastFileSize( 0 ),
	lastFileSizeFull( 0 ),
	fileSize( 0 ),
	fileCurrentSize( 0 ),
	shot_fileCurrentSize_prev( 0 ),
	shot_fileCurrentSize_prev_line( -1 ),
	found_local_file_name_line( -1 ),
	found_connect_line( -1 ),
	found_login_line( -1 ),
	found_file_size_line( -1 ),
	found_file_progress_begin_line( -1 ),
	found_file_progress_end_line( -1 ),
	found_file_saved_line( -1 ),
	current_line( 0 ),
	status( psNone ),
	saveToDir( "" ),
	doRestart( true ),
	dontUseSuffix( false ),
	dontRetrieveOldFile( false ),
	useUsernameAndPassword( true ),
	username( "" ),
	password( "" ),
	writeLog( false ),
	logDirectory( "" ),
	retriesNumber( "" ),
	waitBetweenRetrievals( "" )
{
	proc = new QProcess( this );
	proc->addArgument( "wget" );
	proc->addArgument( "-v" );
	connect( proc, SIGNAL(readyReadStdout()), this, SLOT(readStdout()) );
	connect( proc, SIGNAL(readyReadStderr()), this, SLOT(readStderr()) );
	connect( proc, SIGNAL(processExited()),   this, SLOT(procExit()) );
/*
	log.clear();
	QFile f( "test.log" );
	if ( f.open(IO_ReadOnly) ) {
		QTextStream t( &f );
		QString s;
		int n = 1;
		while ( !t.eof() ) {
			s = t.readLine();
			log.unicode_append( s );
		}
		f.close();
	}
*/
}

WGProcess::~WGProcess()
{
	proc->kill();
	emit signal_close( this );
}

bool WGProcess::start( const bool without_arguments )
{
	if ( !url.isEmpty() ) {
		if ( !without_arguments ) {
			if ( doRestart ) {
				proc->addArgument( "-c" );
			} else {
				if ( dontUseSuffix       ) proc->addArgument( "-nc" );
				if ( dontRetrieveOldFile ) proc->addArgument( "-N" );
			}
			if ( useUsernameAndPassword && ( !username.isEmpty() || !password.isEmpty() )) {
				QString s = url.stripWhiteSpace();
				if ( s.find( "ftp://" ) != 0 ) s = "ftp://" + s;
				QString anonymous = "anonymous";
				QString user_name = !username.isEmpty() ? username : anonymous;
				if ( !password.isEmpty() ) {
					s.insert( 6, user_name + ":" + password + "@" );
				} else {
					s.insert( 6, user_name + "@" );
				}
				url = s;
			}
			if ( !retriesNumber.isEmpty()         ) proc->addArgument( "-t" + retriesNumber );
			if ( !waitBetweenRetrievals.isEmpty() ) proc->addArgument( "-w" + waitBetweenRetrievals );
			if ( !saveToDir.isEmpty() ) {
				int length = (int)saveToDir.length();
				if ( saveToDir.find( "/", length-1 ) == length-1 || saveToDir.find( "\\", length-1 ) == length-1 ) saveToDir.remove( length-1, 1 );
				proc->addArgument( "-P" + saveToDir );
			}
			proc->addArgument( url );
		}
		if ( proc->start() ) {
			if ( !url.isEmpty() ) {
				fileSize         = 0;
				fileCurrentSize  = 0;
				lastFileSize     = 0;
				lastFileSizeFull = 0;
				shot_fileCurrentSize_prev      = 0;
				shot_fileCurrentSize_prev_line = -1;
				found_local_file_name_line     = -1;
				found_connect_line             = -1;
				found_login_line               = -1;
				found_file_size_line           = -1;
				found_file_progress_begin_line = -1;
				found_file_progress_end_line   = -1;
				found_file_saved_line          = -1;
				current_line = 0;
				status = psNone;
				emit signal_start( this );
				setStatus( psStart );
			}
			return true;
		} else {
			emit signal_start( this );
			addToLog( "wget not found" );
			addToLog( "" );
			setStatus( psWgetNotFound );
			return false;
		}
	} else {
		emit signal_start( this );
		addToLog( "url not found" );
		addToLog( "" );
		setStatus( psURLNotFound );
		return false;
	}
}

bool WGProcess::version()
{
	proc->addArgument( "--version" );
	if ( !proc->start() ) {
		addToLog( "wget not found" );
		return false;
	}
	return true;
}

WGUnicodeLog WGProcess::getStrListFromByteArray( const QByteArray& array )
{
	WGUnicodeLog list;
	QString s = "";
	int count = (int)array.count();
	for ( int i = 0; i < count; i++ ) {
		char c = array[i];
		if ( c == 0x0D || c == 0x0A ) {
			list.unicode_append( s );
			s = "";
			if ( i < count-1 ) {
				c = array[i+1];
				if ( c == 0x0D || c == 0x0A ) i++;
			}
		} else {
			s += c;
		}
	}
	new_line = false;
	if ( count ) {
		list.unicode_append( s );
//		char c = array[ count-1 ];
//		new_line = c == 0x0D || c == 0x0A;
	}
	return list;
}

void WGProcess::readStdout()
{
	bool nl = new_line;
	current_log = getStrListFromByteArray( proc->readStdout() );
	QString s;
	unsigned int count = current_log.unicode_count();
	for ( unsigned int i = 0; i < count; i++ ) {
		if ( !nl && i == 0 && log.unicode_count() ) {
			s = log.native_last() + current_log.native_at(i);
			log.unicode_remove_last();
			log.unicode_append( s );
			saveToLogStream( current_log.native_at(i), false );
		} else {
			s = current_log.native_at(i);
			log.unicode_append( s );
			saveToLogStream( s );
		}
	}
	logParser();
	emit signal_log_change( current_log, new_line );
}

void WGProcess::readStderr()
{
	bool nl = new_line;
	current_log = getStrListFromByteArray( proc->readStderr() );
	QString s;
	unsigned int count = current_log.unicode_count();
	for ( unsigned int i = 0; i < count; i++ ) {
		if ( !nl && i == 0 && log.unicode_count() ) {
			s = log.native_last() + current_log.native_at(i);
			log.unicode_remove_last();
			log.unicode_append( s );
			saveToLogStream( current_log.native_at(i), false );
		} else {
			s = current_log.native_at(i);
			log.unicode_append( s );
			saveToLogStream( s );
		}
	}
	logParser();
	emit signal_log_change( current_log, new_line );
}

#define local_file_name_str            "=>"
#define local_file_name_pos            11
#define connect_str                    "Connecting to"
#define connect_pos                    0
#define login_str                      "Logging in as"
#define login_pos                      0
#define file_size_str                  "Length"
#define file_size_pos                  0
#define file_size_index                2
#define file_size_todo_str             "["
#define file_size_todo_index           3
#define done_str                       "done"
#define progress_str1                  " 0K"
#define progress_str2                  "->"
#define progress_str3                  "["
#define progress_pos1                  3
#define progress_pos2                  7
#define progress_pos3                  65
#define file_saved_str                 "saved"
#define file_saved_index               7
#define host_not_found_str             "Host not found"
#define invalid_host_name_str          "Invalid host name"
#define file_already_exist_str         "already there, not retrieving"
#define no_such_file_or_directory_str  "No such file or directory"
#define no_such_file                   "No such file"
#define no_matches_on_pattern_str      "No matches on pattern"
#define invalid_port_specification     "Invalid port specification"
#define unknown_unsupported_protocol   "Unknown/unsupported protocol"
#define unknown_error_str              "Unknown error"

void WGProcess::logParser()
{

	if ( status == psPause ) return;
	int count = (int)log.unicode_count();
	for ( int line = current_line; line < count; line++ ) {
		if ( current_line != line ) current_line = line;
		QString str = log.native_at( line );
		int i;

		if ( str.find( host_not_found_str ) != -1 ) {
			setStatus( psHostNotFound );
		}
		if ( str.find( invalid_host_name_str ) != -1 ) {
			setStatus( psInvalidHostName );
		}
		if ( str.find( file_already_exist_str ) != -1 ) {
			setStatus( psFileAlreadyExist );
		}
		if ( str.find( no_such_file_or_directory_str ) != -1 ) {
			setStatus( psNoSuchFileOrDirectory );
		} else {
			if ( str.find( no_such_file ) != -1 ) {
				setStatus( psNoSuchFile );
			}
		}
		if ( str.find( no_matches_on_pattern_str ) != -1 ) {
			setStatus( psNoMatchesOnPattern );
		}
		if ( str.find( invalid_port_specification ) != -1 ) {
			setStatus( psInvalidPortSpecification );
		}
		if ( str.find( unknown_unsupported_protocol ) != -1 ) {
			setStatus( psUnknownUnsupportedProtocol );
		}
		if ( str.find( unknown_error_str ) != -1 ) {
			setStatus( psUnknownError );
		}

		// local file name
		if ( found_local_file_name_line < line && str.find( local_file_name_str ) == local_file_name_pos ) {
			found_local_file_name_line = line;
			found_connect_line             = -1;
			found_login_line               = -1;
			found_file_size_line           = -1;
			found_file_progress_begin_line = -1;
			found_file_progress_end_line   = -1;
			found_file_saved_line          = -1;
			if ( fileSize ) {
				fileSize         = 0;
				fileCurrentSize  = 0;
				lastFileSize     = 0;
				lastFileSizeFull = 0;
				shot_fileCurrentSize_prev = 0;
				emit signal_file_size( this, fileSize );
				emit signal_file_progress( this );
			}
			QString fn = str;
			fn.remove( 0, 15 );
			fn.remove( fn.findRev( '\'' ), 1 );
			setLocalFileName( fn );
			saveToLogStream( "" );
			setStatus( psConnect );
		}

		// connect
		if ( found_local_file_name_line != -1 && found_connect_line < line && str.find( connect_str ) == connect_pos ) {
			found_connect_line = line;
			found_login_line               = -1;
			found_file_size_line           = -1;
			found_file_progress_begin_line = -1;
			found_file_progress_end_line   = -1;
			found_file_saved_line          = -1;
			setStatus( psConnect );
		}

		// login
		if ( found_connect_line != -1 && found_login_line < line && str.find( login_str ) == login_pos ) {
			found_login_line = line;
			found_file_size_line           = -1;
			found_file_progress_begin_line = -1;
			found_file_progress_end_line   = -1;
			found_file_saved_line          = -1;
			setStatus( psLogin );
		}

		// done ( before download )
		if ( found_login_line != -1 && str.find( done_str ) != -1 ) {
			time_download_begin   = QTime::currentTime();
			time_last_speed_check = time_download_begin;
		}

		// file size
		if ( found_login_line != -1 && found_file_size_line < line && str.find( file_size_str ) == file_size_pos ) {
			found_file_size_line = line;
			found_file_progress_begin_line = -1;
			found_file_progress_end_line   = -1;
			found_file_saved_line          = -1;
			QString fs = pickOutStr( str, file_size_index );
			for ( i = fs.length()-1; i >= 0; i-- )
				if ( fs.find(",", i) == i ) fs.remove( i, 1 );
			unsigned long int step = 1;
			fileSize = 0;
			for ( i = fs.length()-1; i >= 0; i-- ) {
				fileSize += (fs[i].latin1() - '0') * step;
				step *= 10;
				fs.remove( i, 1 );
			}
			emit signal_file_size( this, fileSize );
			if ( str.find( file_size_todo_str ) != - 1 ) {
				fs = pickOutStr( str, file_size_todo_index );
				fs.remove( 0, 1 );
				for ( i = fs.length()-1; i >= 0; i-- )
					if ( fs.find(",", i) == i ) fs.remove( i, 1 );
				step = 1;
				fileCurrentSize = 0;
				for ( i = fs.length()-1; i >= 0; i-- ) {
					fileCurrentSize += (fs[i].latin1() - '0') * step;
					step *= 10;
					fs.remove( i, 1 );
				}
				fileCurrentSize = fileSize - fileCurrentSize;
				emit signal_file_progress( this );
			}
		}

		// file progress
		if ( found_file_size_line != -1 && found_file_progress_begin_line < line &&
		     ( str.find( progress_str1 ) == progress_pos1 ) &&
		     ( str.find( progress_str2 ) == progress_pos2 ) ) {
			found_file_progress_begin_line = line;
			found_file_progress_end_line   = -1;
			found_file_saved_line          = -1;
			time_download_begin   = QTime::currentTime();
			time_last_speed_check = time_download_begin;
			setStatus( psDownload );
		}
		if ( found_file_progress_begin_line != -1 && str.find( progress_str2 ) == progress_pos2 && line >= shot_fileCurrentSize_prev_line ) {
			int shot_size = 0;
			int length = (int)str.length();
			for ( i = 0; i < length; i++ )
				if ( str[i] == '.' ) shot_size += 1024;
			
			if ( shot_fileCurrentSize_prev_line == line ) {
				shot_size -= shot_fileCurrentSize_prev;
			} else {
				shot_fileCurrentSize_prev = 0;
				shot_fileCurrentSize_prev_line = line;
			}
			unsigned long int fs_prev = fileCurrentSize;
			fileCurrentSize  += shot_size;
			lastFileSize     += shot_size;
			lastFileSizeFull += shot_size;
			if ( fileCurrentSize + 1024 >= fileSize && str.find( progress_str3 ) == progress_pos3 ) {
				fileCurrentSize = fileSize;
				found_file_progress_end_line = line;
				found_file_saved_line = -1;
				setStatus( psFinish );
			}
			if ( fs_prev != fileCurrentSize ) {
				shot_fileCurrentSize_prev += shot_size;
				emit signal_file_progress( this );
			}
		}

		// file saved
		if ( ( found_file_progress_end_line != -1 || found_login_line != -1 ) &&
		       found_file_saved_line < line && str.find( file_saved_str ) != -1 ) {
			found_file_saved_line = line;
			if ( found_file_progress_begin_line == -1 ) {
				if ( !fileSize ) {
					QString fs = pickOutStr( str, file_saved_index );
					fs.remove( 0, 1 );
					fs.remove( fs.length()-1, 1 );
					for ( i = fs.length()-1; i >= 0; i-- )
						if ( fs.find(",", i) == i ) fs.remove( i, 1 );
					unsigned long int step = 1;
					for ( i = fs.length()-1; i >= 0; i-- ) {
						fileSize += (fs[i].latin1() - '0') * step;
						step *= 10;
						fs.remove( i, 1 );
					}
					emit signal_file_size( this, fileSize );
				}
				fileCurrentSize = fileSize;
				emit signal_file_progress( this );
				found_file_progress_end_line = line;
				setStatus( psFinish );
			}
			setStatus( psSaved );
		}
	}
}

QString WGProcess::pickOutStr( QString str, const int index ) const
{
	str = str.stripWhiteSpace();
	int space;
	for ( int i = 0; i < index - 1; i++ ) {
		space = str.find( " " );
		if ( space != -1 ) {
			str.remove( 0, space + 1 );
			str = str.stripWhiteSpace();
		} else return "";
	}
	space = str.find( " " );
	if ( space != -1 ) {
		str.remove( space, str.length() - space + 1);
	}
	return str;
}

WGUnicodeLog& WGProcess::getFullLog()
{
	return log;
}

QString& WGProcess::getLocalFileName()
{
	return localFileName;
}

void WGProcess::setLocalFileName( QString& str )
{
	localFileName = str;
	emit signal_local_fine_name_change( this );
}

void WGProcess::setLocalFileName( const char* str )
{
	QString s( str );
	setLocalFileName( s );
}

QString& WGProcess::getURL()
{
	return url;
}

QString WGProcess::getURLWithHidenPassword() const
{
	QString s = url;
	int i = s.find( '/' );
	if ( i != -1 ) {
		int b = 0;
		int e = s.length() - 1;
		if ( s.find( '/', i+1 ) == i+1 ) {
			b = i+2;
			i = s.find( '/', b );
			if ( i != -1 ) {
				e = i-1;
			} else {
				e = b-1;
			}
		} else {
			e = i-1;
		}
		if ( b < e ) {
			i = s.find( '@', b );
			if ( i != -1 ) e = i-1;
			if ( b < e ) {
				i = s.find( ':', b );
				if ( i != -1 ) b = i+1;
				if ( b <= e ) {
					for ( i = b; i <= e; i++ ) {
						s.replace( i, 1, "x" );
					}
				}
			}
		}
	}
	return s;
}

void WGProcess::setURL( QString& str )
{
	url = str;
}

void WGProcess::setURL( const char* str )
{
	QString s( str );
	setURL( s );
}

unsigned long int WGProcess::getFileSize() const
{
	return fileSize;
}

unsigned long int WGProcess::getFileCurrentSize() const
{
	return fileCurrentSize;
}

double WGProcess::getProgress() const
{
	return fileSize ?  (double)((double)fileCurrentSize / (double)fileSize) : 0;
}

unsigned long int WGProcess::getLastSpeed()
{
	if ( !getPause() ) {

		unsigned long int sec_begin = time_last_speed_check.msec() +
		                              time_last_speed_check.second() * 1000 +
		                              time_last_speed_check.minute() * 1000 * 60 +
		                              time_last_speed_check.hour() * 1000 * 60 * 60;

		time_last_speed_check = QTime::currentTime();
		unsigned long int sec_now   = time_last_speed_check.msec() +
		                              time_last_speed_check.second() * 1000 +
		                              time_last_speed_check.minute() * 1000 * 60 +
		                              time_last_speed_check.hour() * 1000 * 60 * 60;

		unsigned long int fs = lastFileSize;
		lastFileSize = 0;

		return (double)(sec_now - sec_begin) / 1000.0 ? (unsigned long int)(fs / ((double)(sec_now - sec_begin) / 1000.0 )) : 0;

	} else {

		unsigned long int sec_begin = time_download_begin.msec() +
		                              time_download_begin.second() * 1000 +
		                              time_download_begin.minute() * 1000 * 60 +
		                              time_download_begin.hour() * 1000 * 60 * 60;

		unsigned long int sec_end   = time_download_end.msec() +
		                              time_download_end.second() * 1000 +
		                              time_download_end.minute() * 1000 * 60 +
		                              time_download_end.hour() * 1000 * 60 * 60;

		return (double)(sec_end - sec_begin) / 1000.0 ? (unsigned long int)(lastFileSizeFull / ((double)(sec_end - sec_begin) / 1000.0 )) : 0;

	}
}

bool WGProcess::getPause() const
{
	int pause = status == psPause || status == psSaved || status == psFinish ||
	            status == psWgetNotFound       || status == psURLNotFound    ||
	            status == psHostNotFound       || status == psInvalidHostName       ||
	            status == psFileAlreadyExist   || status == psNoSuchFileOrDirectory ||
	            status == psNoMatchesOnPattern || status == psUnknownError;
	return pause;
}

void WGProcess::setPause( const bool value )
{
	if ( getPause() != value ) {
		if ( value ) {
			proc->kill();
			setStatus( psPause );
			addToLog( "user paused" );
			addToLog( "" );
		} else {
			start( true );
		}
	}
}

WGProcessStatus WGProcess::getStatus() const
{
	return status;
}

void WGProcess::setStatus( WGProcessStatus value )
{
	if ( status != value ) {
		status = value;
		if ( getPause() ) time_download_end = QTime::currentTime();
		emit signal_change_status( this, status );
	}
}

void WGProcess::procExit()
{
	if ( status == psConnect ) setStatus( psUnknownError );
}

void WGProcess::addToLog( const char* str )
{
	current_log.unicode_clear();
	current_log.unicode_append( str );
	log.unicode_append( str );
	saveToLogStream( str );
	emit signal_log_change( current_log, true );
}

void WGProcess::saveToLogStream( QString& str, const bool newline )
{
	if ( writeLog && !localFileName.isEmpty() ) {

		bool save_all = false;

		if ( logFileName.isEmpty() ) {
			QString name( localFileName );
			QString dir( "" );
			int i = name.findRev( "/" );
			if ( i == -1 ) i = name.findRev( "\\" );
			if ( i != -1 ) {
				dir = name.left( i+1 );
				name.remove( 0, i+1 );
			}
			name += ".log";
			bool to_unicode = true;
			if ( !logDirectory.isEmpty() ) {
				dir = logDirectory;
				to_unicode = false;
			}
			if ( dir.findRev( "/" ) != (int)dir.length()-1 && dir.findRev( "\\" ) != (int)dir.length()-1 ) dir += "/";
			if ( to_unicode ) {
				logFileName = QTextCodec::codecForLocale()->toUnicode( dir + name );
			} else {
				logFileName = dir + name;
			}
			save_all = true;
		}

		QFile file( logFileName );
		if ( file.open( IO_WriteOnly | IO_Append ) ) {
			QTextStream stream( &file );
			if ( save_all ) {
				unsigned int count = log.unicode_count();
				for ( unsigned int i = 0; i < count; i++ ) {
					stream << log.native_at( i );
					if ( !(i == count-1 && !newline) ) stream << endl;
				}
			} else {
				if ( newline ) stream << endl;
				stream << str;
			}
			file.close();
		}
	}
}

void WGProcess::saveToLogStream( const char* str, const bool newline )
{
	QString s( str );
	saveToLogStream( s, newline );
}
