#include "rdorepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <rdokernel.h>
#include <rdosimwin.h>

using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDOThreadRepository
// ----------------------------------------------------------------------------
RDOThreadRepository::RDOThreadRepository():
	RDOThreadMT( "RDOThreadRepository" ),
	modelName( "" ),
	modelPath( "" ),
	hasModel( false ),
	realOnlyInDlg( false )
{
	notifies.push_back( RT_STUDIO_MODEL_NEW );
	notifies.push_back( RT_STUDIO_MODEL_OPEN );
	notifies.push_back( RT_STUDIO_MODEL_CLOSE );
	notifies.push_back( RT_STUDIO_MODEL_SAVE );
	notifies.push_back( RT_STUDIO_MODEL_SAVE_AS );
	notifies.push_back( RT_REPOSITORY_MODEL_GET_FILEINFO );
	notifies.push_back( RT_REPOSITORY_LOAD );
	notifies.push_back( RT_REPOSITORY_SAVE );
	notifies.push_back( RT_REPOSITORY_LOAD_BINARY );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_RUNTIME_MODEL_START_BEFORE );
	notifies.push_back( RT_RUNTIME_TRACE_STRING );

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

//	files[ rdoModelObjects::OPR ].deleteifempty = true;
//	files[ rdoModelObjects::DPT ].deleteifempty = true;

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
			msg.lock();
			NewModel* data = static_cast<NewModel*>(msg.param);
			newModel( data );
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_OPEN: {
			msg.lock();
			OpenFile* data = static_cast<OpenFile*>(msg.param);
			data->result = openModel( data->name );
			msg.unlock();
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
		case RT_RUNTIME_MODEL_START_BEFORE: {
			beforeModelStart();
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_OK           :
		case RT_SIMULATOR_MODEL_STOP_BY_USER      :
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: {
			stopModel();
			break;
		}
		case RT_RUNTIME_TRACE_STRING: {
			msg.lock();
			trace( *static_cast<std::string*>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_MODEL_GET_FILEINFO: {
			msg.lock();
			FileInfo* data = static_cast<FileInfo*>(msg.param);
			data->name      = getFileName( data->type );
			data->full_name = getFullFileName( data->type );
			data->extention = getExtention( data->type );
			data->described = isDescribed( data->type );
			data->readonly  = isReadOnly( data->type );
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
		case RT_REPOSITORY_LOAD_BINARY: {
			msg.lock();
			BinaryFile* data = static_cast<BinaryFile*>(msg.param);
			loadBMP( data->name, data->stream );
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

RDOThreadRepository::FindModel RDOThreadRepository::updateModelNames()
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
		files[ rdoModelObjects::OPR ].described = !fileInfo.oprIev_file.empty(); // && rdo::isFileExists( getFullFileName( rdoModelObjects::OPR ) );
		files[ rdoModelObjects::FRM ].described = !fileInfo.frame_file.empty();
		files[ rdoModelObjects::FUN ].described = !fileInfo.model_name.empty();
		files[ rdoModelObjects::DPT ].described = !fileInfo.model_name.empty(); // !files[ rdoModelObjects::OPR ].described;
		files[ rdoModelObjects::SMR ].described = true;
		files[ rdoModelObjects::PMD ].described = !fileInfo.statistic_file.empty();
		files[ rdoModelObjects::PMV ].described = !fileInfo.results_file.empty();
		files[ rdoModelObjects::TRC ].described = !fileInfo.trace_file.empty();

		files[ rdoModelObjects::PAT ].mustexist = true;
		files[ rdoModelObjects::RTP ].mustexist = true;
		files[ rdoModelObjects::RSS ].mustexist = true;
		files[ rdoModelObjects::OPR ].mustexist = files[ rdoModelObjects::OPR ].described;
		files[ rdoModelObjects::FRM ].mustexist = !fileInfo.frame_file.empty();
		files[ rdoModelObjects::FUN ].mustexist = false;
		files[ rdoModelObjects::DPT ].mustexist = false; // files[ rdoModelObjects::DPT ].described;
		files[ rdoModelObjects::SMR ].mustexist = true;
		files[ rdoModelObjects::PMD ].mustexist = !fileInfo.statistic_file.empty();
		files[ rdoModelObjects::PMV ].mustexist = false;
		files[ rdoModelObjects::TRC ].mustexist = false;
		return fm_ok;
	} else {
		if ( fileInfo.model_name.empty() ) return fm_smr_empty;
		std::vector< RDOThreadRepository::fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = files[ rdoModelObjects::SMR ].filename;
			it->described = true;
			it->mustexist = true;
			it++;
		}
		return fm_smr_error;
	}
}

void RDOThreadRepository::newModel( const NewModel* const data )
{
	if ( canCloseModel() ) {
		realCloseModel();
		if ( data ) {
			std::string path = data->path;
			std::string::size_type pos = path.find_last_of( '\\' );
			if ( pos == std::string::npos ) {
				pos = path.find_last_of( '/' );
			}
			if ( pos != path.length() - 1 ) {
				path += '\\';
			}
			extractName( path + data->name + ".smr" );
			if ( !rdo::isFileExists( path ) ) {
				::CreateDirectory( path.c_str(), NULL );
			}
		} else {
			modelName = "noname";
			modelPath = "";
		}
		std::vector< fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = modelName;
			it++;
		}
		hasModel = true;
		broadcastMessage( RT_REPOSITORY_MODEL_NEW );
	} else {
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	}
}

bool RDOThreadRepository::openModel( const std::string& modelFileName )
{
	if ( canCloseModel() ) {

		realCloseModel();

		bool can_open = true;
		realOnlyInDlg = false;
		if ( modelFileName.empty() ) {
			OpenFile data;
			broadcastMessage( RT_REPOSITORY_MODEL_OPEN_GET_NAME, &data, true );
			if ( data.result ) {
				realOnlyInDlg = data.readonly;
				extractName( data.name );
			} else {
				can_open = false;
			}
		} else {
			extractName( modelFileName );
			can_open = !modelName.empty();
		}

		if ( can_open ) {
			std::vector< fileInfo >::iterator it = files.begin();
			while ( it != files.end() ) {
				it->readonly = realOnlyInDlg;
				it++;
			}
			if ( rdo::isFileExists( modelPath + modelName + files[ rdoModelObjects::SMR ].extention ) ) {

				files[ rdoModelObjects::SMR ].filename = modelName;
				hasModel = true;
				switch ( updateModelNames() ) {
					case fm_ok       : broadcastMessage( RT_REPOSITORY_MODEL_OPEN ); return true;
					case fm_smr_error: broadcastMessage( RT_REPOSITORY_MODEL_OPEN ); return false;
					case fm_smr_empty: return false;
				}

			} else {
				broadcastMessage( RT_REPOSITORY_MODEL_OPEN_ERROR, &std::string(modelPath + modelName + files[ rdoModelObjects::SMR ].extention) );
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
	OpenFile data;
	broadcastMessage( RT_REPOSITORY_MODEL_SAVE_GET_NAME, &data, true );
	if ( data.result ) {
		extractName( data.name );
		if ( modelName.empty() ) return false;
		std::vector< fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = modelName;
			it++;
		}
		return true;
	} else {
		return false;
	}
}

bool RDOThreadRepository::canCloseModel()
{
	if ( hasModel ) {
		bool res = true;
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE, &res, true );
		return res;
	} else {
		return true;
	}
}

void RDOThreadRepository::realCloseModel()
{
	if ( hasModel ) {
//	if ( !modelName.empty() ) {
		hasModel = false;
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

void RDOThreadRepository::extractName( const std::string& fullname )
{
	modelPath = rdo::extractFilePath( fullname );

	std::string name = fullname;
	std::string::size_type pos = name.find_last_of( '.' );
	if ( pos != std::string::npos ) {
		std::string s;
		s.assign( &name[0], pos );
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
			if ( stream.isBinary() )
			{
				std::ifstream file( filename.c_str(), std::ios::in | std::ios::binary );
				stream << file.rdbuf();
				file.close();
			} else {
				std::ifstream file( filename.c_str() );
				stream << file.rdbuf();
				file.close();
			}
/*
			if ( stream.getOpenMode() & std::ios::binary* ) {
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
*/
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
		if ( !stream.str().empty() || ( file_exist && !deleteIfEmpty ) ) {
			if ( stream.isBinary() ) {
				std::ofstream file( filename.c_str(), std::ios::out | std::ios::binary );
				file << stream.rdbuf();
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
		stream << file.rdbuf();
		file.close();
	} else {
		stream.setstate( std::ios_base::badbit );
	}
}

void RDOThreadRepository::writeModelFilesInfo( std::ofstream& stream ) const
{
	stream << "Results_file   = " << getFileExtName( rdoModelObjects::PMV ) << "    " << static_cast<LPCTSTR>(CTime::GetCurrentTime().Format( "%a %b %d %H:%M:%S %Y" )) << std::endl;
	stream << "Run_file       = " << getFileExtName( rdoModelObjects::SMR ) << std::endl;
	stream << "Model_name     = " << files[ rdoModelObjects::SMR ].filename << std::endl;
	stream << "Resource_file  = " << files[ rdoModelObjects::RSS ].filename << files[ rdoModelObjects::RSS ].extention << std::endl;
	stream << "OprIev_file    = " << files[ rdoModelObjects::OPR ].filename << files[ rdoModelObjects::OPR ].extention << std::endl;
}

void RDOThreadRepository::beforeModelStart()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
	if ( files[ rdoModelObjects::TRC ].described ) {
		trace_file.open( getFullFileName( rdoModelObjects::TRC ).c_str(), std::ios::out | std::ios::binary );
		if ( trace_file.is_open() ) {
			writeModelFilesInfo( trace_file );
			std::stringstream model_structure;
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure );
			trace_file << std::endl << model_structure.str() << std::endl;
			trace_file << "$Tracing" << std::endl;
		}
	}
}

void RDOThreadRepository::stopModel()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
	if ( files[ rdoModelObjects::PMV ].described ) {
		std::ofstream results_file;
		results_file.open( getFullFileName( rdoModelObjects::PMV ).c_str(), std::ios::out | std::ios::binary );
		if ( results_file.is_open() ) {
			writeModelFilesInfo( results_file );
			std::stringstream stream;
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS_INFO, &stream );
			results_file << std::endl << stream.str() << std::endl;
			stream.str("");
			stream.clear();
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &stream );
			results_file << std::endl << stream.str() << std::endl;
		}
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
