#ifndef WGET_PROCESS_H
#define WGET_PROCESS_H

#include "unicode_log.h"

#include <qobject.h>
#include <qcstring.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qptrlist.h>

// -------------------------------------------------------
// -------- WGProcessList
// -------------------------------------------------------
class WGProcess;

class WGProcessList: public QObject
{ Q_OBJECT

private:
	QPtrList< WGProcess > proc_list;

public:
	WGProcessList( QObject* parent = NULL );
	~WGProcessList();

	WGProcess* newProcess();
	void deleteProcess( WGProcess* proc );

	WGProcess* newWgetVersionProcess();
	void deleteWgetVersionProcess( WGProcess* proc );

	unsigned int count() const;
	unsigned int indexOf( const WGProcess* const proc );
	WGProcess* at( const unsigned int i );
	WGProcess* current();
	WGProcess* first();
	WGProcess* last();
	WGProcess* next();
	WGProcess* prev();

	QString saveToDir;
	bool doRestart;
	bool dontUseSuffix;
	bool dontRetrieveOldFile;
	bool useUsernameAndPassword;
	QString username;
	QString password;
	bool writeToLog;
	QString logDirectory;
	QString retriesNumber;
	QString waitBetweenRetrievals;
	bool checkClipboard;
	bool autostartClipboard;
};

// -------------------------------------------------------
// -------- WGProcess
// -------------------------------------------------------
class QProcess;

typedef enum WGProcessStatus {
	psNone                       =  0,
	psStart                      =  1,
	psConnect                    =  2,
	psLogin                      =  3,
	psDownload                   =  4,
	psFinish                     =  5,
	psSaved                      =  6,
	psPause                      =  7,
	psWgetNotFound               =  8,
	psURLNotFound                =  9,
	psHostNotFound               = 10,
	psInvalidHostName            = 11,
	psFileAlreadyExist           = 12,
	psNoSuchFileOrDirectory      = 13,
	psNoSuchFile                 = 14,
	psNoMatchesOnPattern         = 15,
	psInvalidPortSpecification   = 16,
	psUnknownUnsupportedProtocol = 17,
	psUnknownError               = 18
};

class WGProcess: public QObject
{ Q_OBJECT

friend class WGProcessList;
friend class QPtrList< WGProcess >;

private:
	WGProcess( QObject* parent = NULL );
	~WGProcess();

	WGUnicodeLog log;
	WGUnicodeLog current_log;
	bool new_line;
	void addToLog( const char* str );

	QString logFileName;
	void saveToLogStream( QString& str, const bool newline = true );
	void saveToLogStream( const char* str, const bool newline = true );

	QProcess* proc;
	QString localFileName;
	QString url;

	QTime time_download_begin;
	QTime time_download_end;
	QTime time_last_speed_check;
	unsigned long int lastFileSize;
	unsigned long int lastFileSizeFull;

	unsigned long int fileSize;
	unsigned long int fileCurrentSize;
	int shot_fileCurrentSize_prev;
	int shot_fileCurrentSize_prev_line;

	int found_local_file_name_line;
	int found_connect_line;
	int found_login_line;
	int found_file_size_line;
	int found_file_progress_begin_line;
	int found_file_progress_end_line;
	int found_file_saved_line;
	int current_line;
	void logParser();
	QString pickOutStr( QString str, const int index ) const;

	WGUnicodeLog getStrListFromByteArray( const QByteArray& array );

	WGProcessStatus status;
	void setStatus( WGProcessStatus value );

protected slots:
	void readStdout();
	void readStderr();
	void procExit();

public:
	bool start( const bool without_arguments = false );

	bool version();

	WGUnicodeLog& getFullLog();

	QString& getLocalFileName();
	void setLocalFileName( QString& str );
	void setLocalFileName( const char* str );

	QString& getURL();
	QString getURLWithHidenPassword() const;
	void setURL( QString& str );
	void setURL( const char* str );

	unsigned long int getFileSize() const;
	unsigned long int getFileCurrentSize() const;
	double getProgress() const;

	unsigned long int getLastSpeed();

	bool getPause() const;
	void setPause( const bool value );

	WGProcessStatus getStatus() const;

	QString saveToDir;
	bool doRestart;
	bool dontUseSuffix;
	bool dontRetrieveOldFile;
	bool useUsernameAndPassword;
	QString username;
	QString password;
	bool writeToLog;
	QString logDirectory;
	QString retriesNumber;
	QString waitBetweenRetrievals;

signals:
	void signal_start( WGProcess* proc );
	void signal_close( WGProcess* proc );
	void signal_log_change( WGUnicodeLog& new_log, const bool new_line = true );
	void signal_local_fine_name_change( WGProcess* proc );
	void signal_file_size( WGProcess* proc, const unsigned long int file_size );
	void signal_file_progress( WGProcess* proc );
	void signal_change_status( WGProcess* proc, WGProcessStatus status );
};

// -------------------------------------------------------
extern WGProcessList* WGProcess_s;
// -------------------------------------------------------

#endif // WGET_PROCESS_H

