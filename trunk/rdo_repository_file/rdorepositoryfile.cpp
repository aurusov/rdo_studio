#include "stdafx.h"
#include "rdorepositoryfile.h"
#include "../rdo_studio/resource.h"

#include <rdokernel.h>
#include <rdosimwin.h>

#include <afxdlgs.h>

using namespace std;
using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDORepositoryFile
// ----------------------------------------------------------------------------
RDORepositoryFile* repository = NULL;

RDORepositoryFile::RDORepositoryFile():
	modelName( "" ),
	modelPath( "" ),
	lastModelPath( "" ),
	realOnlyInDlg( false )
{
	repository = this;

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, beforeModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::executeError, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::traceString, traceNotify );

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
}

RDORepositoryFile::~RDORepositoryFile()
{
	trace_file.close();
	repository = NULL;
}

void RDORepositoryFile::resetModelNames()
{
	std::vector< fileInfo >::iterator it = files.begin();
	while ( it != files.end() ) {
		it->resetname();
		it++;
	}
}

bool RDORepositoryFile::updateModelNames()
{
	rdo::binarystream smrStream( ios::in | ios::out );
	loadFile( getFullFileName( rdoModelObjects::SMR ), smrStream, true, true, files[ rdoModelObjects::SMR ].readonly );
	rdoModelObjects::RDOSMRFileInfo fileInfo;
	kernel.getSimulator()->parseSMRFileInfo( smrStream, fileInfo );
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
		std::vector< RDORepositoryFile::fileInfo >::iterator it = files.begin();
		while ( it != files.end() ) {
			it->filename = files[ rdoModelObjects::SMR ].filename;
			it->described = true;
			it->mustexist = true;
			it++;
		}
		return false;
	}
}

void RDORepositoryFile::newModel()
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
		kernel.notify( RDOKernel::newModel );
	} else {
		kernel.notify( RDOKernel::canNotCloseModel );
	}
}

bool RDORepositoryFile::openModel( const string& modelFileName )
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
			string defaultFileName = "";
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
					kernel.notify( RDOKernel::openModel );
					return true;
				} else {
					kernel.notify( RDOKernel::openModel );
					return false;
				}

			} else {
				AfxMessageBox( rdo::format( ID_MSG_MODELOPEN_ERROR, string(modelPath + modelName + files[ rdoModelObjects::SMR ].extention).c_str() ).c_str(), MB_ICONSTOP | MB_OK );
				setName( "" );
			}
		}

	} else {
		kernel.notify( RDOKernel::canNotCloseModel );
	}

	return false;
}

bool RDORepositoryFile::saveModel()
{
	bool flag = true;
	if ( modelPath.empty() ) {
		flag = saveAsDlg();
	}
	if ( flag ) {
		kernel.notify( RDOKernel::saveModel );
	}
	return flag;
}

void RDORepositoryFile::saveAsModel()
{
	if ( saveAsDlg() ) {
		kernel.notify( RDOKernel::saveModel );
	}
}

bool RDORepositoryFile::saveAsDlg()
{
	CString s;
	s.LoadString( ID_MODEL_FILETYPE );
	string defaultFileName = "";
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

bool RDORepositoryFile::canCloseModel() const
{
	return kernel.boolNotifyAnd( RDOKernel::canCloseModel );
}

void RDORepositoryFile::realCloseModel()
{
	if ( !modelName.empty() ) {
		kernel.notify( RDOKernel::closeModel );
		modelName   = "";
		modelPath   = "";
		resetModelNames();
	}
}

void RDORepositoryFile::closeModel()
{
	if ( canCloseModel() ) {
		realCloseModel();
	} else {
		kernel.notify( RDOKernel::canNotCloseModel );
	}
}

void RDORepositoryFile::extractName( const CFileDialog* const dlg )
{
	if ( !dlg ) return;

	string fullname  = dlg->GetPathName();
	string filename  = dlg->GetFileName();
	string extention = dlg->GetFileExt();

	modelPath = rdo::extractFilePath( fullname );

	if ( extention.empty() ) {
		setName( filename );
	} else {
		string name( filename.begin(), filename.length() - extention.length() - 1 );
		setName( name );
	}
}

void RDORepositoryFile::extractName( const string& fullname )
{
	modelPath = rdo::extractFilePath( fullname );

	string name = fullname;
	string::size_type pos = name.find_last_of( '.' );
	if ( pos != string::npos ) {
		string s;
		s.assign( name.begin(), pos );
		name = s;
	}
	static char szDelims[] = " \t\n\r";
	name.erase( 0, name.find_first_not_of( szDelims ) );
	name.erase( name.find_last_not_of( szDelims ) + 1, string::npos );
	pos = name.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = name.find_last_of( '/' );
	}
	if ( pos != string::npos ) {
		string s( name, pos + 1, name.length() - pos );
		setName( s );
	} else {
		setName( "" );
	}
}

bool RDORepositoryFile::isFileReadOnly( const string& fileName )
{
	CFileFind finder;
	if ( finder.FindFile( fileName.c_str() ) ) {
		finder.FindNextFile();
		return finder.IsReadOnly() ? true : false;
	}
	return false;
}

void RDORepositoryFile::changeLastModelPath()
{
	string name = !modelName.empty() ? modelName + files[ rdoModelObjects::SMR ].extention : "*" + files[ rdoModelObjects::SMR ].extention;
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

void RDORepositoryFile::setName( const string& str )
{
	modelName = str;
	static char szDelims[] = " \t\n\r";
	modelName.erase( 0, modelName.find_first_not_of( szDelims ) );
	modelName.erase( modelName.find_last_not_of( szDelims ) + 1, string::npos );
	if ( modelName.empty() ) {
		modelPath = "";
		resetModelNames();
	}
	changeLastModelPath();
}

void RDORepositoryFile::loadFile( const string& filename, rdo::binarystream& stream, const bool described, const bool mustExist, bool& reanOnly ) const
{
	if ( described ) {
		if ( rdo::isFileExists( filename ) ) {
			if ( !realOnlyInDlg ) {
				reanOnly = isFileReadOnly( filename );
			} else {
				reanOnly = true;
			}
			if ( stream.getOpenMode() & ios::binary ) {
				ifstream file( filename.c_str(), ios::in | ios::binary );
				file.seekg( 0, ios::end );
				int len = file.tellg();
				file.seekg( 0, ios::beg );
				stream.resize( len );
				file.read( stream.data(), len );
				file.close();
			} else {
				ifstream file( filename.c_str() );
				stream << file.rdbuf();
				file.close();
			}
		} else {
			stream.setstate( ios_base::badbit );
			if ( mustExist ) stream.setstate( stream.rdstate() | ios_base::failbit );
		}
	} else {
		stream.setstate( ios_base::badbit );
	}
}

void RDORepositoryFile::saveFile( const string& filename, rdo::binarystream& stream, const bool deleteIfEmpty ) const
{
	if ( !filename.empty() ) {
		bool file_exist = rdo::isFileExists( filename );
		if ( stream.size() || ( file_exist && !deleteIfEmpty ) ) {
			if ( stream.getOpenMode() & ios::binary ) {
				ofstream file( filename.c_str(), ios::out | ios::binary );
				file.write( stream.data(), stream.size() );
				file.close();
			} else {
				ofstream file( filename.c_str() );
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

void RDORepositoryFile::load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream )
{
	loadFile( getFullFileName( type ), stream, files[ type ].described, files[ type ].mustexist, files[ type ].readonly );
}

void RDORepositoryFile::save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const
{
	saveFile( getFullFileName( type ), stream, files[ type ].deleteifempty );
	if ( type == rdoModelObjects::SMR ) {
		const_cast<RDORepositoryFile*>(this)->updateModelNames();
	}
}

void RDORepositoryFile::loadBMP( const string& name, rdo::binarystream& stream ) const
{
	string file_name = modelPath + name + ".bmp";
	if ( rdo::isFileExists( file_name ) ) {
		ifstream file( file_name.c_str(), ios::in | ios::binary );
		file.seekg( 0, ios::end );
		int len = file.tellg();
		file.seekg( 0, ios::beg );
		stream.resize( len );
		file.read( stream.data(), len );
		file.close();
	} else {
		stream.setstate( ios_base::badbit );
	}
}

void RDORepositoryFile::beforeModelStartNotify()
{
	repository->beforeModelStart();
}

void RDORepositoryFile::stopModelNotify()
{
	repository->stopModel();
}

void RDORepositoryFile::traceNotify( std::string str )
{
	repository->trace( str );
}

void RDORepositoryFile::beforeModelStart()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
	if ( files[ rdoModelObjects::TRC ].described ) {
		trace_file.open( getFullFileName( rdoModelObjects::TRC ).c_str(), ios::out | ios::binary );
		if ( trace_file.is_open() ) {
			trace_file << "Results_file   = " << getFileExtName( rdoModelObjects::PMV ) << "    " << static_cast<LPCTSTR>(CTime::GetCurrentTime().Format( "%a %b %d %H:%M:%S %Y" )) << endl;
			trace_file << "Run_file       = " << getFileExtName( rdoModelObjects::SMR ) << endl;
			trace_file << "Model_name     = " << files[ rdoModelObjects::SMR ].filename << endl;
			trace_file << "Resource_file  = " << files[ rdoModelObjects::RSS ].filename << files[ rdoModelObjects::RSS ].extention << endl;
			trace_file << "OprIev_file    = " << files[ rdoModelObjects::OPR ].filename << files[ rdoModelObjects::OPR ].extention << endl;
			trace_file << kernel.getSimulator()->getModelStructure().str() << endl;
			trace_file << "$Tracing" << endl;
		}
	}
}

void RDORepositoryFile::stopModel()
{
	if ( trace_file.is_open() ) {
		trace_file.close();
	}
}

void RDORepositoryFile::trace( const string& str )
{
	if ( trace_file.is_open() ) {
		trace_file.write( str.c_str(), str.length() );
		trace_file.write( "\n", 1 );
		trace_file.flush();
	}
}
