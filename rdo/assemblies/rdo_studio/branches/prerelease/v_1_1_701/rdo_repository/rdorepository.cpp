#include "rdorepository.h"

#include <rdokernel.h>
#include <rdosimwin.h>
#include <rdofile.h>
#include <rdotime.h>

using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDOThreadRepository
// ----------------------------------------------------------------------------
RDOThreadRepository::RDOThreadRepository()
	: RDOThreadMT  (_T("RDOThreadRepository"))
	, modelName    (_T("")                   )
	, modelPath    (_T("")                   )
	, hasModel     (false                    )
	, realOnlyInDlg(false                    )
{
	notifies.push_back(RT_STUDIO_MODEL_NEW                  );
	notifies.push_back(RT_STUDIO_MODEL_OPEN                 );
	notifies.push_back(RT_STUDIO_MODEL_CLOSE                );
	notifies.push_back(RT_STUDIO_MODEL_SAVE                 );
	notifies.push_back(RT_STUDIO_MODEL_SAVE_AS              );
	notifies.push_back(RT_REPOSITORY_MODEL_GET_FILEINFO     );
	notifies.push_back(RT_REPOSITORY_LOAD                   );
	notifies.push_back(RT_REPOSITORY_SAVE                   );
	notifies.push_back(RT_REPOSITORY_LOAD_BINARY            );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_BY_USER      );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_TRACE_STRING              );

	m_files.resize(11);
	m_files[rdoModelObjects::PAT].extention = _T(".pat");
	m_files[rdoModelObjects::RTP].extention = _T(".rtp");
	m_files[rdoModelObjects::RSS].extention = _T(".rss");
	m_files[rdoModelObjects::OPR].extention = _T(".opr");
	m_files[rdoModelObjects::FRM].extention = _T(".frm");
	m_files[rdoModelObjects::FUN].extention = _T(".fun");
	m_files[rdoModelObjects::DPT].extention = _T(".dpt");
	m_files[rdoModelObjects::SMR].extention = _T(".smr");
	m_files[rdoModelObjects::PMD].extention = _T(".pmd");
	m_files[rdoModelObjects::PMV].extention = _T(".pmv");
	m_files[rdoModelObjects::TRC].extention = _T(".trc");

//	m_files[rdoModelObjects::OPR].deleteifempty = true;
//	m_files[rdoModelObjects::DPT].deleteifempty = true;

	resetModelNames();

	after_constructor();
}

RDOThreadRepository::~RDOThreadRepository()
{
	trace_file.close();
}

void RDOThreadRepository::proc( REF(RDOMessageInfo) msg )
{
	switch ( msg.message ) {
		case RT_STUDIO_MODEL_NEW: {
			msg.lock();
			PTR(NewModel) data = static_cast<PTR(NewModel)>(msg.param);
			newModel( data );
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_OPEN: {
			msg.lock();
			PTR(OpenFile) data = static_cast<PTR(OpenFile)>(msg.param);
			data->m_result = openModel(data->m_name);
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_CLOSE: {
			closeModel();
			break;
		}
		case RT_STUDIO_MODEL_SAVE: {
			rbool res = saveModel();
			msg.lock();
			if (msg.param) *static_cast<PTR(rbool)>(msg.param) = res;
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
			trace( *static_cast<PTR(tstring)>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_MODEL_GET_FILEINFO: {
			msg.lock();
			PTR(FileInfo) data = static_cast<PTR(FileInfo)>(msg.param);
			data->m_name      = getFileName    (data->m_type);
			data->m_full_name = getFullFileName(data->m_type);
			data->m_extention = getExtention   (data->m_type);
			data->m_described = isDescribed    (data->m_type);
			data->m_readonly  = isReadOnly     (data->m_type);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD: {
			msg.lock();
			PTR(FileData) fdata = static_cast<PTR(FileData)>(msg.param);
			load(fdata->m_type, fdata->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_SAVE: {
			msg.lock();
			PTR(FileData) fdata = static_cast<PTR(FileData)>(msg.param);
			save(fdata->m_type, fdata->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD_BINARY: {
			msg.lock();
			PTR(BinaryFile) data = static_cast<PTR(BinaryFile)>(msg.param);
			loadBMP(data->m_name, data->m_stream);
			msg.unlock();
			break;
		}
		default: break;
	}
}

void RDOThreadRepository::resetModelNames()
{
	FileList::iterator it = m_files.begin();
	while ( it != m_files.end() ) {
		it->resetname();
		it++;
	}
}

RDOThreadRepository::FindModel RDOThreadRepository::updateModelNames()
{
	rdo::textstream smrStream;
	loadFile( getFullFileName( rdoModelObjects::SMR ), smrStream, true, true, m_files[rdoModelObjects::SMR].readonly );
	rdoModelObjects::RDOSMRFileInfo fileInfo;
	kernel->simulator()->parseSMRFileInfo(smrStream, fileInfo);
	if ( !fileInfo.error ) {
		m_files[rdoModelObjects::PAT].filename = fileInfo.model_name.empty()     ? m_files[rdoModelObjects::SMR].filename : fileInfo.model_name;
 		m_files[rdoModelObjects::RTP].filename = fileInfo.model_name.empty()     ? m_files[rdoModelObjects::SMR].filename : fileInfo.model_name;
 		m_files[rdoModelObjects::RSS].filename = fileInfo.resource_file.empty()  ? m_files[rdoModelObjects::SMR].filename : fileInfo.resource_file;
 		m_files[rdoModelObjects::OPR].filename = fileInfo.oprIev_file.empty()    ? m_files[rdoModelObjects::SMR].filename : fileInfo.oprIev_file;
 		m_files[rdoModelObjects::FRM].filename = fileInfo.frame_file.empty()     ? m_files[rdoModelObjects::SMR].filename : fileInfo.frame_file;
 		m_files[rdoModelObjects::FUN].filename = fileInfo.model_name.empty()     ? m_files[rdoModelObjects::SMR].filename : fileInfo.model_name;
 		m_files[rdoModelObjects::DPT].filename = fileInfo.model_name.empty()     ? m_files[rdoModelObjects::SMR].filename : fileInfo.model_name;
 		m_files[rdoModelObjects::PMD].filename = fileInfo.statistic_file.empty() ? m_files[rdoModelObjects::SMR].filename : fileInfo.statistic_file;
 		m_files[rdoModelObjects::PMV].filename = fileInfo.results_file.empty()   ? m_files[rdoModelObjects::SMR].filename : fileInfo.results_file;
 		m_files[rdoModelObjects::TRC].filename = fileInfo.trace_file.empty()     ? m_files[rdoModelObjects::SMR].filename : fileInfo.trace_file;

		m_files[rdoModelObjects::PAT].described = !fileInfo.model_name.empty();
		m_files[rdoModelObjects::RTP].described = !fileInfo.model_name.empty();
		m_files[rdoModelObjects::RSS].described = !fileInfo.resource_file.empty();
		m_files[rdoModelObjects::OPR].described = !fileInfo.oprIev_file.empty(); // && rdo::isFileExists( getFullFileName( rdoModelObjects::OPR ) );
		m_files[rdoModelObjects::FRM].described = !fileInfo.frame_file.empty();
		m_files[rdoModelObjects::FUN].described = !fileInfo.model_name.empty();
		m_files[rdoModelObjects::DPT].described = !fileInfo.model_name.empty(); // !m_files[rdoModelObjects::OPR].described;
		m_files[rdoModelObjects::SMR].described = true;
		m_files[rdoModelObjects::PMD].described = !fileInfo.statistic_file.empty();
		m_files[rdoModelObjects::PMV].described = !fileInfo.results_file.empty();
		m_files[rdoModelObjects::TRC].described = !fileInfo.trace_file.empty();

		m_files[rdoModelObjects::PAT].mustexist = true;
		m_files[rdoModelObjects::RTP].mustexist = true;
		m_files[rdoModelObjects::RSS].mustexist = true;
		m_files[rdoModelObjects::OPR].mustexist = m_files[rdoModelObjects::OPR].described;
		m_files[rdoModelObjects::FRM].mustexist = !fileInfo.frame_file.empty();
		m_files[rdoModelObjects::FUN].mustexist = false;
		m_files[rdoModelObjects::DPT].mustexist = false; // m_files[rdoModelObjects::DPT].described;
		m_files[rdoModelObjects::SMR].mustexist = true;
		m_files[rdoModelObjects::PMD].mustexist = !fileInfo.statistic_file.empty();
		m_files[rdoModelObjects::PMV].mustexist = false;
		m_files[rdoModelObjects::TRC].mustexist = false;
		return fm_ok;
	} else {
		if ( fileInfo.model_name.empty() ) return fm_smr_empty;
		std::vector< RDOThreadRepository::fileInfo >::iterator it = m_files.begin();
		while ( it != m_files.end() ) {
			it->filename = m_files[rdoModelObjects::SMR].filename;
			it->described = true;
			it->mustexist = true;
			it++;
		}
		return fm_smr_error;
	}
}

void RDOThreadRepository::newModel(CPTRC(NewModel) data)
{
	if (canCloseModel())
	{
		realCloseModel();
		if (data)
		{
			tstring path = data->m_path;
			tstring::size_type pos = path.find_last_of(_T('\\'));
			if (pos == tstring::npos)
			{
				pos = path.find_last_of(_T('/'));
			}
			if (pos != path.length() - 1)
			{
				path += _T('\\');
			}
			extractName(path + data->m_name + _T(".smr"));
			if (!rdo::File::exist( path ))
			{
				::CreateDirectory(path.c_str(), NULL);
			}
		}
		else
		{
			modelName = _T("noname");
			modelPath = _T("");
		}
		FileList::iterator it = m_files.begin();
		while (it != m_files.end())
		{
			it->filename = modelName;
			++it;
		}
		hasModel = true;
		broadcastMessage(RT_REPOSITORY_MODEL_NEW);
	}
	else
	{
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}
}

rbool RDOThreadRepository::openModel(CREF(tstring) modelFileName)
{
	if ( canCloseModel() ) {

		realCloseModel();

		rbool can_open = true;
		realOnlyInDlg = false;
		if ( modelFileName.empty() ) {
			OpenFile data;
			broadcastMessage(RT_REPOSITORY_MODEL_OPEN_GET_NAME, &data, true);
			if (data.m_result)
			{
				realOnlyInDlg = data.m_readonly;
				extractName(data.m_name);
			}
			else
			{
				can_open = false;
			}
		} else {
			extractName( modelFileName );
			can_open = !modelName.empty();
		}

		if ( can_open ) {
			FileList::iterator it = m_files.begin();
			while ( it != m_files.end() ) {
				it->readonly = realOnlyInDlg;
				it++;
			}
			if ( rdo::File::exist( modelPath + modelName + m_files[rdoModelObjects::SMR].extention ) ) {

				m_files[rdoModelObjects::SMR].filename = modelName;
				hasModel = true;
				switch ( updateModelNames() ) {
					case fm_ok       : broadcastMessage( RT_REPOSITORY_MODEL_OPEN ); return true;
					case fm_smr_error: broadcastMessage( RT_REPOSITORY_MODEL_OPEN ); return false;
					case fm_smr_empty: return false;
				}

			} else {
				broadcastMessage( RT_REPOSITORY_MODEL_OPEN_ERROR, &tstring(modelPath + modelName + m_files[rdoModelObjects::SMR].extention) );
				setName(_T(""));
			}
		}

	} else {
		broadcastMessage( RT_REPOSITORY_MODEL_CLOSE_ERROR );
	}

	return false;
}

rbool RDOThreadRepository::saveModel()
{
	rbool flag = true;
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

rbool RDOThreadRepository::saveAsDlg()
{
	OpenFile data;
	broadcastMessage(RT_REPOSITORY_MODEL_SAVE_GET_NAME, &data, true);
	if (data.m_result)
	{
		extractName(data.m_name);
		if (modelName.empty())
			return false;
		FileList::iterator it = m_files.begin();
		while (it != m_files.end())
		{
			it->filename = modelName;
			++it;
		}
		return true;
	}
	else
	{
		return false;
	}
}

rbool RDOThreadRepository::canCloseModel()
{
	if ( hasModel ) {
		rbool res = true;
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
		modelName   = _T("");
		modelPath   = _T("");
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

void RDOThreadRepository::extractName( CREF(tstring) fullname )
{
	modelPath = rdo::extractFilePath( fullname );

	tstring name = fullname;
	tstring::size_type pos = name.find_last_of( '.' );
	if ( pos != tstring::npos ) {
		tstring s;
		s.assign( &name[0], pos );
		name = s;
	}
	static char szDelims[] = _T(" \t\n\r");
	name.erase( 0, name.find_first_not_of( szDelims ) );
	name.erase( name.find_last_not_of( szDelims ) + 1, tstring::npos );
	pos = name.find_last_of( '\\' );
	if ( pos == tstring::npos ) {
		pos = name.find_last_of( '/' );
	}
	if ( pos != tstring::npos ) {
		tstring s( name, pos + 1, name.length() - pos );
		setName( s );
	} else {
		setName(_T(""));
	}
}

void RDOThreadRepository::setName( CREF(tstring) str )
{
	modelName = str;
	static char szDelims[] = _T(" \t\n\r");
	modelName.erase( 0, modelName.find_first_not_of( szDelims ) );
	modelName.erase( modelName.find_last_not_of( szDelims ) + 1, tstring::npos );
	if ( modelName.empty() ) {
		modelPath = _T("");
		resetModelNames();
	}
}

void RDOThreadRepository::loadFile(CREF(tstring) filename, REF(rdo::stream) stream, rbool described, rbool mustExist, REF(rbool) reanOnly) const
{
	if ( described ) {
		if ( rdo::File::exist( filename ) ) {
			if ( !realOnlyInDlg ) {
				reanOnly = rdo::File::read_only(filename);
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

void RDOThreadRepository::saveFile( CREF(tstring) filename, REF(rdo::stream) stream, rbool deleteIfEmpty ) const
{
	if ( !filename.empty() ) {
		rbool file_exist = rdo::File::exist(filename);
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
			if (file_exist && deleteIfEmpty)
			{
				rdo::File::unlink(filename);
			}
		}
	}
}

void RDOThreadRepository::load( rdoModelObjects::RDOFileType type, REF(rdo::stream) stream )
{
	loadFile( getFullFileName( type ), stream, m_files[type].described, m_files[type].mustexist, m_files[type].readonly );
}

void RDOThreadRepository::save( rdoModelObjects::RDOFileType type, REF(rdo::stream) stream ) const
{
	saveFile( getFullFileName( type ), stream, m_files[type].deleteifempty );
	if ( type == rdoModelObjects::SMR ) {
		const_cast<PTR(RDOThreadRepository)>(this)->updateModelNames();
	}
}

void RDOThreadRepository::loadBMP( CREF(tstring) name, REF(rdo::stream) stream ) const
{
	tstring file_name = modelPath + name + _T(".bmp");
	if (rdo::File::exist(file_name))
	{
		std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
		stream << file.rdbuf();
		file.close();
	}
	else
	{
		stream.setstate(std::ios_base::badbit);
	}
}

void RDOThreadRepository::writeModelFilesInfo(REF(std::ofstream) stream) const
{
	stream << _T("Results_file   = ") << getFileExtName( rdoModelObjects::PMV ) << _T("    ") << rdo::Time::local().asString() << std::endl;
	stream << _T("Run_file       = ") << getFileExtName( rdoModelObjects::SMR ) << std::endl;
	stream << _T("Model_name     = ") << m_files[rdoModelObjects::SMR].filename << std::endl;
	stream << _T("Resource_file  = ") << m_files[rdoModelObjects::RSS].filename << m_files[rdoModelObjects::RSS].extention << std::endl;
	stream << _T("OprIev_file    = ") << m_files[rdoModelObjects::OPR].filename << m_files[rdoModelObjects::OPR].extention << std::endl;
}

void RDOThreadRepository::beforeModelStart()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
	if ( m_files[rdoModelObjects::TRC].described ) {
		trace_file.open( getFullFileName( rdoModelObjects::TRC ).c_str(), std::ios::out | std::ios::binary );
		if ( trace_file.is_open() ) {
			writeModelFilesInfo( trace_file );
			rdo::textstream model_structure;
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure );
			trace_file << std::endl << model_structure.str() << std::endl;
			trace_file << _T("$Tracing") << std::endl;
		}
	}
}

void RDOThreadRepository::stopModel()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
	if ( m_files[rdoModelObjects::PMV].described ) {
		std::ofstream results_file;
		results_file.open( getFullFileName( rdoModelObjects::PMV ).c_str(), std::ios::out | std::ios::binary );
		if ( results_file.is_open() ) {
			writeModelFilesInfo( results_file );
			rdo::textstream stream;
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS_INFO, &stream );
			results_file << std::endl << stream.str() << std::endl;
			stream.str("");
			stream.clear();
			sendMessage( kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &stream );
			results_file << std::endl << stream.str() << std::endl;
		}
	}
}

void RDOThreadRepository::trace( CREF(tstring) str )
{
	if ( trace_file.is_open() ) {
		trace_file.write(str.c_str(), str.length());
		trace_file.write(_T("\n"), 1);
		trace_file.flush();
	}
}
