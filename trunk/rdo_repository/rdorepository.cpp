#include "rdorepository.h"
#include "../rdo_studio/resource.h"

#include <rdokernel.h>
#include <rdosimwin.h>

#include <afxdlgs.h>

using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDOThreadRepository
// ----------------------------------------------------------------------------
RDOThreadRepository::RDOThreadRepository():
	RDOThread( "RDOThreadRepository" ),
	modelName( "" ),
	modelPath( "" ),
	lastModelPath( "" ),
	realOnlyInDlg( false )
{
	notifies.push_back( RT_STUDIO_MODEL_NEW );
	notifies.push_back( RT_STUDIO_MODEL_OPEN );
	notifies.push_back( RT_STUDIO_MODEL_CLOSE );
	notifies.push_back( RT_STUDIO_MODEL_SAVE );
	notifies.push_back( RT_STUDIO_MODEL_SAVE_AS );
	notifies.push_back( RT_REPOSITORY_LOAD );
	notifies.push_back( RT_REPOSITORY_SAVE );
	notifies.push_back( RT_SIMULATOR_MODEL_START_BEFORE );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_SIMULATOR_TRACE_STRING );
//	notifies.push_back( RT_TEST1 );

	lastModelPath = AfxGetApp()->GetProfileString( "repository", "lastModelPath", "" );

	files.resize( 11 );
	files[ rdoModelObjects::PAT ].extention = ".pat";
	files[ rdoModelObjects::RTP ].extention = ".rtp";
	files[ rdoModelObjects::RSS ].extention = ".rss";
	files[ rdoModelObjects::OPR ].extention = ".opr";
	files[ rdoModelObjects::FRM ].extention = ".frm";
	files[ rdoModelObjects::FUN ].extention = ".fun";
	files[ rdoModelObjects::DPT ].extention = ".dpt";
	files[ rdoModelObjects::SMR ].extention = ".smr";
	files[ rdoModelObjects::PMD ].extention = ".pmd";
	files[ rdoModelObjects::PMV ].extention = ".pmv";
	files[ rdoModelObjects::TRC ].extention = ".trc";

	files[ rdoModelObjects::OPR ].deleteifempty = true;
	files[ rdoModelObjects::DPT ].deleteifempty = true;

	resetModelNames();

	after_constructor();
}

RDOThreadRepository::~RDOThreadRepository()
{
	trace_file.close();
}

void RDOThreadRepository::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_STUDIO_MODEL_NEW: {
			newModel();
			break;
		}
		case RT_STUDIO_MODEL_OPEN: {
			msg.lock();
			std::string name = *static_cast<std::string*>(msg.param);
			msg.unlock();
			openModel( name );
//			broadcastMessage( RT_TEST1 );
			break;
		}
		case RT_TEST1: {
//			broadcastMessage( RT_TEST2 );
			break;
		}
		case RT_STUDIO_MODEL_CLOSE: {
			closeModel();
			break;
		}
		case RT_STUDIO_MODEL_SAVE: {
			bool res = saveModel();
			msg.lock();
			if ( msg.param ) *static_cast<bool*>(msg.param) = res;
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_SAVE_AS: {
			saveAsModel();
			break;
		}
		case RT_SIMULATOR_MODEL_START_BEFORE: {
			beforeModelStart();
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_OK           :
		case RT_SIMULATOR_MODEL_STOP_BY_USER      :
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: {
			stopModel();
			break;
		}
		case RT_SIMULATOR_TRACE_STRING: {
			msg.lock();
			trace( *static_cast<std::string*>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD: {
			msg.lock();
			FileData* fdata = static_cast<FileData*>(msg.param);
			load( fdata->type, fdata->stream );
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_SAVE: {
			msg.lock();
			FileData* fdata = static_cast<FileData*>(msg.param);
			save( fdata->type, fdata->stream );
			msg.unlock();
			break;
		}
		default: break;
	}
}

void RDOThreadRepository::resetModelNames()
{
	std::vector< fileInfo >::iterator it = files.begin();
	while ( it != files.end() ) {
		it->resetname();
		it++;
	}
}

bool RDOThreadRepository::updateModelNames()
{
	rdo::binarystream smrStream( std::ios::in | std::ios::out );
	loadFile( getFullFileName( rdoModelObjects::SMR ), smrStream, true, true, files[ rdoModelObjects::SMR ].readonly );
	rdoModelObjects::RDOSMRFileInfo fileInfo;
	kernel->simulator()->parseSMRFileInfo( smrStream, fileInfo );
	if ( !fileInfo.error ) {
		files[ rdoModelObjects::PAT ].filename = fileInfo.model_name.empty()     ? files[ rdoModelObjects::SMR ].filename : fileInfo.model_name;
 		files[ rdoModelObjects::RTP ].filename = fileInfo.model_name.empty()     ? files[ rdoModelObjects::SMR ].filename : fileInfo.model_name;
 		files[ rdoModelObjects::RSS ].filename = fileInfo.resource_file.empty()  ? files[ rdoModelObjects::SMR ].filename : fileInfo.resource_file;
 		files[ rdoModelObjects::OPR ].filename = fileInfo.oprIev_file.empty()    ? files[ rdoModelObjects::SMR ].filename : fileInfo.oprIev_file;
 		files[ rdoModelObjects::FRM ].filename = fileInfo.frame_file.empty()     ? files[ rdoModelObjects::SMR ].filename : fileInfo.frame_file;
 		files[ rdoModelObjects::FUN ].filename = fileInfo.model_name.empty()     ? files[ rdoModelObjects::SMR ].filename : fileInfo.model_name;
 		files[ rdoModelObjects::DPT ].filename = fileInfo.model_name.empty()     ? files[ rdoModelObjects::SMR ].filename : fileInfo.model_name;
 		files[ rdoModelObjects::PMD ].filename = fileInfo.statistic_file.empty() ? files[ rdoModelObjects::SMR ].filename : fileInfo.statistic_file;
 		files[ rdoModelObjects::PMV ].filename = fileInfo.results_file.empty()   ? files[ rdoModelObjects::SMR ].filename : fileInfo.results_file;
 		files[ rdoModelObjects::TRC ].filename = fileInfo.trace_file.empty()     ? files[ rdoModelObjects::SMR ].filename : fileInfo.trace_file;

		files[ rdoModelObjects::PAT ].described = !fileInfo.model_name.empty();
		files[ rdoModelObjects::RTP ].described = !fileInfo.model_name.empty();
		files[ rdoModelObjects::RSS ].described = !fileInfo.resource_file.empty();
		files[ rdoModelObjects::OPR ].described = !fileInfo.oprIev_file.empty() && rdo::isFileExists( getFullFileName( rdoModelObjects::OPR ) );
		files[ rdoModelObjects::FRM ].described = !fileInfo.frame_file.empty();
		files[ rdoModelObjects::FUN ].described = !fileInfo.model_name.empty();
		files[ rdoModelObjects::DPT ].described = !files[ rdoModelObjects::OPR ].described;
		files[ rdoModelObjects::SMR ].described = true;
		files[ rdoModelObjects::PMD ].described = !fileInfo.statistic_file.empty();
		files[ rdoModelObjects::PMV ].described = !fileInfo.results_file.empty();
		files[ rdoModelObjects::TRC ].described = !fileInfo.trace_file.empty();

//		files[ rdoModelObjects::PAT ].mustexist = !fileInfo.model_name.empty();
//		files[ rdoModelObjects::RTP ].mustexist = !fileInfo.model_name.empty();
//		files[ rdoModelObjects::RSS ].mustexist = !fileInfo.resource_file.empty();
//		files[ rdoModelObjects::OPR ].mustexist = !fileInfo.oprIev_file.empty();
		files[ rdoModelObjects::PAT ].mustexist = true;
		files[ rdoModelObjects::RTP ].mustexist = true;
		files[ rdoModelObjects::RSS ].mustexist = true;
		files[ rdoModelObjects::OPR ].mustexist = files[ rdoModelObjects::OPR ].described;
		files[ rdoModelObjects::FRM ].mustexist = !fileInfo.frame_file.empty();
		files[ rdoModelObjects::FUN ].mustexist = false;
		files[ rdoModelObjects::DPT ].mustexist = files[ rdoModelObjects::DPT ].described;
		files[ rdoModelObjects::SMR ].mustexist = true;
		files[ rdoModelObjects::PMD ].mustexist = !fileInfo.statistic_file.empty();
		files[ rdoModelObjects::PMV ].mustexist = false;
		files[ rdoModelObjects::TRC ].mustexist = false;
		return true;
	} else {
		std::vector< RDOThreadRepository::fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = files[ rdoModelObjects::SMR ].filename;
			it->described = true;
			it->mustexist = true;
			it++;
		}
		return false;
	}
}

void RDOThreadRepository::newModel()
{
	if ( canCloseModel() ) {
		realCloseModel();
		modelName   = "noname";
		modelPath   = "";
		std::vector< fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = modelName;
			it++;
		}
		broadcastMessage( RT_REPOSITORY_MODEL_NEW );
	} else {
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	}
}

bool RDOThreadRepository::openModel( const std::string& modelFileName )
{
	if ( canCloseModel() ) {

		realCloseModel();

		bool flag = true;
		realOnlyInDlg = false;
		std::vector< fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->readonly = false;
			it++;
		}
		if ( modelFileName.empty() ) {
			CString s;
			s.LoadString( ID_MODEL_FILETYPE );
			std::string defaultFileName = "";
			if ( rdo::isFileExists( lastModelPath ) ) {
				defaultFileName = lastModelPath;
			}
			CFileDialog dlg( true, "smr", defaultFileName.c_str(), 0, s, AfxGetMainWnd() );
			flag = dlg.DoModal() == IDOK;
			realOnlyInDlg = dlg.GetReadOnlyPref() == TRUE;
			it = files.begin();
			while ( it != files.end() ) {
				it->readonly = realOnlyInDlg;
				it++;
			}
			if ( flag ) {
				extractName( &dlg );
			}
		} else {
			extractName( modelFileName );
			flag = !modelName.empty();
		}

		if ( flag ) {
			if ( rdo::isFileExists( modelPath + modelName + files[ rdoModelObjects::SMR ].extention ) ) {

				files[ rdoModelObjects::SMR ].filename = modelName;

				if ( updateModelNames() ) {
					broadcastMessage( RT_REPOSITORY_MODEL_OPEN );
					return true;
				} else {
					broadcastMessage( RT_REPOSITORY_MODEL_OPEN );
					return false;
				}

			} else {
				AfxMessageBox( rdo::format( ID_MSG_MODELOPEN_ERROR, std::string(modelPath + modelName + files[ rdoModelObjects::SMR ].extention).c_str() ).c_str(), MB_ICONSTOP | MB_OK );
				setName( "" );
			}
		}

	} else {
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	}

	return false;
}

bool RDOThreadRepository::saveModel()
{
	bool flag = true;
	if ( modelPath.empty() ) {
		flag = saveAsDlg();
	}
	if ( flag ) {
		broadcastMessage( RT_REPOSITORY_MODEL_SAVE );
	}
	return flag;
}

void RDOThreadRepository::saveAsModel()
{
	if ( saveAsDlg() ) {
		broadcastMessage( RT_REPOSITORY_MODEL_SAVE );
	}
}

bool RDOThreadRepository::saveAsDlg()
{
	CString s;
	s.LoadString( ID_MODEL_FILETYPE );
	std::string defaultFileName = "";
	if ( rdo::isFileExists( lastModelPath ) ) {
		defaultFileName = lastModelPath;
	}
	CFileDialog dlg( false, "smr", defaultFileName.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, s );

	if ( dlg.DoModal() == IDOK ) {

		extractName( &dlg );

		if ( modelName.empty() ) return false;

		std::vector< fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = modelName;
			it++;
		}

		return true;

	}
	return false;
}

bool RDOThreadRepository::canCloseModel()
{
	bool res = true;
	broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE, &res );
	return res;
}

void RDOThreadRepository::realCloseModel()
{
	if ( !modelName.empty() ) {
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE );
		modelName   = "";
		modelPath   = "";
		resetModelNames();
	}
}

void RDOThreadRepository::closeModel()
{
	if ( canCloseModel() ) {
		realCloseModel();
	} else {
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	}
}

void RDOThreadRepository::extractName( const CFileDialog* const dlg )
{
	if ( !dlg ) return;

	std::string fullname  = dlg->GetPathName();
	std::string filename  = dlg->GetFileName();
	std::string extention = dlg->GetFileExt();

	modelPath = rdo::extractFilePath( fullname );

	if ( extention.empty() ) {
		setName( filename );
	} else {
		std::string name( filename.begin(), filename.length() - extention.length() - 1 );
		setName( name );
	}
}

void RDOThreadRepository::extractName( const std::string& fullname )
{
	modelPath = rdo::extractFilePath( fullname );

	std::string name = fullname;
	std::string::size_type pos = name.find_last_of( '.' );
	if ( pos != std::string::npos ) {
		std::string s;
		s.assign( name.begin(), pos );
		name = s;
	}
	static char szDelims[] = " \t\n\r";
	name.erase( 0, name.find_first_not_of( szDelims ) );
	name.erase( name.find_last_not_of( szDelims ) + 1, std::string::npos );
	pos = name.find_last_of( '\\' );
	if ( pos == std::string::npos ) {
		pos = name.find_last_of( '/' );
	}
	if ( pos != std::string::npos ) {
		std::string s( name, pos + 1, name.length() - pos );
		setName( s );
	} else {
		setName( "" );
	}
}

bool RDOThreadRepository::isFileReadOnly( const std::string& fileName )
{
	CFileFind finder;
	if ( finder.FindFile( fileName.c_str() ) ) {
		finder.FindNextFile();
		return finder.IsReadOnly() ? true : false;
	}
	return false;
}

void RDOThreadRepository::changeLastModelPath()
{
	std::string name = !modelName.empty() ? modelName + files[ rdoModelObjects::SMR ].extention : "*" + files[ rdoModelObjects::SMR ].extention;
	if ( !modelPath.empty() ) {
		lastModelPath = modelPath + name;
	} else {
		TCHAR buff[ MAX_PATH ];
		::GetCurrentDirectory( MAX_PATH, buff );
		lastModelPath = buff;
		if ( !lastModelPath.empty() ) {
			if ( lastModelPath[ lastModelPath.length()-1 ] != '\\' && lastModelPath[ lastModelPath.length()-1 ] != '/' ) {
				lastModelPath += "\\";
			}
		}
		lastModelPath += name;
	}
	AfxGetApp()->WriteProfileString( "repository", "lastModelPath", lastModelPath.c_str() );
}

void RDOThreadRepository::setName( const std::string& str )
{
	modelName = str;
	static char szDelims[] = " \t\n\r";
	modelName.erase( 0, modelName.find_first_not_of( szDelims ) );
	modelName.erase( modelName.find_last_not_of( szDelims ) + 1, std::string::npos );
	if ( modelName.empty() ) {
		modelPath = "";
		resetModelNames();
	}
	changeLastModelPath();
}

void RDOThreadRepository::loadFile( const std::string& filename, rdo::binarystream& stream, const bool described, const bool mustExist, bool& reanOnly ) const
{
	if ( described ) {
		if ( rdo::isFileExists( filename ) ) {
			if ( !realOnlyInDlg ) {
				reanOnly = isFileReadOnly( filename );
			} else {
				reanOnly = true;
			}
			if ( stream.getOpenMode() & std::ios::binary ) {
				std::ifstream file( filename.c_str(), std::ios::in | std::ios::binary );
				file.seekg( 0, std::ios::end );
				int len = file.tellg();
				file.seekg( 0, std::ios::beg );
				stream.resize( len );
				file.read( stream.data(), len );
				file.close();
			} else {
				std::ifstream file( filename.c_str() );
				stream << file.rdbuf();
				file.close();
			}
		} else {
			stream.setstate( std::ios_base::badbit );
			if ( mustExist ) stream.setstate( stream.rdstate() | std::ios_base::failbit );
		}
	} else {
		stream.setstate( std::ios_base::badbit );
	}
}

void RDOThreadRepository::saveFile( const std::string& filename, rdo::binarystream& stream, const bool deleteIfEmpty ) const
{
	if ( !filename.empty() ) {
		bool file_exist = rdo::isFileExists( filename );
		if ( stream.size() || ( file_exist && !deleteIfEmpty ) ) {
			if ( stream.getOpenMode() & std::ios::binary ) {
				std::ofstream file( filename.c_str(), std::ios::out | std::ios::binary );
				file.write( stream.data(), stream.size() );
				file.close();
			} else {
				std::ofstream file( filename.c_str() );
				file << stream.rdbuf();
				file.close();
			}
		} else {
			if ( file_exist && deleteIfEmpty ) {
				CFile::Remove( filename.c_str() );
			}
		}
	}
}

void RDOThreadRepository::load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream )
{
	loadFile( getFullFileName( type ), stream, files[ type ].described, files[ type ].mustexist, files[ type ].readonly );
}

void RDOThreadRepository::save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const
{
	saveFile( getFullFileName( type ), stream, files[ type ].deleteifempty );
	if ( type == rdoModelObjects::SMR ) {
		const_cast<RDOThreadRepository*>(this)->updateModelNames();
	}
}

void RDOThreadRepository::loadBMP( const std::string& name, rdo::binarystream& stream ) const
{
	std::string file_name = modelPath + name + ".bmp";
	if ( rdo::isFileExists( file_name ) ) {
		std::ifstream file( file_name.c_str(), std::ios::in | std::ios::binary );
		file.seekg( 0, std::ios::end );
		int len = file.tellg();
		file.seekg( 0, std::ios::beg );
		stream.resize( len );
		file.read( stream.data(), len );
		file.close();
	} else {
		stream.setstate( std::ios_base::badbit );
	}
}

void RDOThreadRepository::beforeModelStart()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
	if ( files[ rdoModelObjects::TRC ].described ) {
		trace_file.open( getFullFileName( rdoModelObjects::TRC ).c_str(), std::ios::out | std::ios::binary );
		if ( trace_file.is_open() ) {
			trace_file << "Results_file   = " << getFileExtName( rdoModelObjects::PMV ) << "    " << static_cast<LPCTSTR>(CTime::GetCurrentTime().Format( "%a %b %d %H:%M:%S %Y" )) << std::endl;
			trace_file << "Run_file       = " << getFileExtName( rdoModelObjects::SMR ) << std::endl;
			trace_file << "Model_name     = " << files[ rdoModelObjects::SMR ].filename << std::endl;
			trace_file << "Resource_file  = " << files[ rdoModelObjects::RSS ].filename << files[ rdoModelObjects::RSS ].extention << std::endl;
			trace_file << "OprIev_file    = " << files[ rdoModelObjects::OPR ].filename << files[ rdoModelObjects::OPR ].extention << std::endl;
//!			trace_file << kernel.getSimulator()->getModelStructure().str() << std::endl;
			trace_file << "$Tracing" << std::endl;
		}
	}
}

void RDOThreadRepository::stopModel()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
}

void RDOThreadRepository::trace( const std::string& str )
{
	if ( trace_file.is_open() ) {
		trace_file.write( str.c_str(), str.length() );
		trace_file.write( "\n", 1 );
		trace_file.flush();
	}
}
