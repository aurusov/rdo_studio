#include "stdafx.h"
#include "rdorepositoryfile.h"
#include "../rdo_studio/resource.h"

#include <rdokernel.h>
#include <rdosimwin.h>

#include <fstream>
#include <afxdlgs.h>

using namespace std;
using namespace rdoRepository;

// ----------------------------------------------------------------------------
// ---------- RDORepositoryFile
// ----------------------------------------------------------------------------
RDORepositoryFile::RDORepositoryFile():
	modelName( "" ),
	modelPath( "" ),
	lastModelPath( "" ),
	patFileName( "" ),
	rtpFileName( "" ),
	rssFileName( "" ),
	oprFileName( "" ),
	frmFileName( "" ),
	funFileName( "" ),
	dptFileName( "" ),
	smrFileName( "" ),
	pmdFileName( "" ),
	pmvFileName( "" ),
	trcFileName( "" ),
	readOnly( false )
{
}

RDORepositoryFile::~RDORepositoryFile()
{
}

void RDORepositoryFile::newModel()
{
	if ( canCloseModel() ) {
		realCloseModel();
		modelName   = "model";
		modelPath   = "";
		patFileName = "";
		rtpFileName = "";
		rssFileName = "";
		oprFileName = "";
		frmFileName = "";
		funFileName = "";
		dptFileName = "";
		smrFileName = "";
		pmdFileName = "";
		pmvFileName = "";
		trcFileName = "";
		changeLastModelPath();
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
		readOnly = false;
		if ( modelFileName.empty() ) {
			CString s;
			s.LoadString( ID_MODEL_FILETYPE );
			CFileDialog dlg( true, "smr", lastModelPath.c_str(), 0, s, AfxGetMainWnd() );
			flag = dlg.DoModal() == IDOK;
			readOnly = dlg.GetReadOnlyPref() == TRUE;
			if ( flag ) {
				extractName( &dlg );
			}
		} else {
			extractName( modelFileName );
			flag = !modelName.empty();
		}

		if ( flag ) {
			if ( isFileExists( modelPath + modelName + ".smr" ) ) {

				smrFileName = modelPath + modelName + ".smr";
				stringstream smrStream;
				loadFile( smrFileName, smrStream );
				rdoModelObjects::RDOSMRFileInfo fileInfo;
				kernel.getSimulator()->parseSMRFileInfo( smrStream, fileInfo );

				patFileName = modelPath + fileInfo.model_name     + ".pat";
				rtpFileName = modelPath + fileInfo.model_name     + ".rtp";
				rssFileName = modelPath + fileInfo.resource_file  + ".rss";
				oprFileName = modelPath + fileInfo.oprIev_file    + ".opr";
				frmFileName = modelPath + fileInfo.frame_file     + ".frm";
				funFileName = modelPath + fileInfo.model_name     + ".fun";
				dptFileName = modelPath + fileInfo.model_name     + ".dpt";
				pmdFileName = modelPath + fileInfo.statistic_file + ".pmd";
				pmvFileName = modelPath + fileInfo.results_file   + ".pmv";
				trcFileName = modelPath + fileInfo.trace_file     + ".trc";

				kernel.notify( RDOKernel::openModel );
				return true;

			} else {
				CString s;
				s.LoadString( ID_MSG_MODELOPEN_ERROR );
				AfxMessageBox( s, MB_ICONSTOP | MB_OK );
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
	CFileDialog dlg( false, "smr", lastModelPath.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, s );

	if ( dlg.DoModal() == IDOK ) {

		extractName( &dlg );

		if ( modelName.empty() ) return false;

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
	kernel.notify( RDOKernel::closeModel );
	modelName   = "";
	modelPath   = "";
	patFileName = "";
	rtpFileName = "";
	rssFileName = "";
	oprFileName = "";
	frmFileName = "";
	funFileName = "";
	dptFileName = "";
	smrFileName = "";
	pmdFileName = "";
	pmvFileName = "";
	trcFileName = "";
	changeLastModelPath();
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

	modelPath = extractFilePath( fullname );

	if ( extention.empty() ) {
		setName( filename );
	} else {
		string name( filename.begin(), filename.length() - extention.length() - 1 );
		setName( name );
	}
}

void RDORepositoryFile::extractName( const string& fullname )
{
	modelPath = extractFilePath( fullname );

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

string RDORepositoryFile::extractFilePath( const string& fileName )
{
	string s;
	string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos != string::npos && pos < fileName.length() - 1 ) {
		s.assign( fileName.begin(), pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

bool RDORepositoryFile::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

void RDORepositoryFile::changeLastModelPath()
{
	string name = !modelName.empty() ? modelName + ".smr" : "*.smr";
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
}

string RDORepositoryFile::getName() const
{
	return modelName;
}

string RDORepositoryFile::getFullName() const
{
	return smrFileName;
}

void RDORepositoryFile::setName( const string& str )
{
	modelName = str;
	static char szDelims[] = " \t\n\r";
	modelName.erase( 0, modelName.find_first_not_of( szDelims ) );
	modelName.erase( modelName.find_last_not_of( szDelims ) + 1, string::npos );
	if ( modelName.empty() ) {
		modelPath   = "";
		patFileName = "";
		rtpFileName = "";
		rssFileName = "";
		oprFileName = "";
		frmFileName = "";
		funFileName = "";
		dptFileName = "";
		smrFileName = "";
		pmdFileName = "";
		pmvFileName = "";
		trcFileName = "";
	}
	changeLastModelPath();
}

bool RDORepositoryFile::isReadOnly() const
{
	return readOnly;
}

void RDORepositoryFile::loadFile( const string& filename, stringstream& stream ) const
{
	if ( isFileExists( filename ) ) {
		ifstream file( filename.c_str() );
		stream << file.rdbuf();
		file.close();
	}
}

void RDORepositoryFile::saveFile( const string& filename, stringstream& stream ) const
{
	if ( stream.str().length() ) {
		ofstream file( filename.c_str() );
		file << stream.str();
		file.close();
	} else {
		if ( isFileExists( filename ) ) {
			CFile::Remove( filename.c_str() );
		}
	}
}

void RDORepositoryFile::loadPAT( stringstream& stream ) const
{
	loadFile( patFileName, stream );
}

void RDORepositoryFile::loadRTP( stringstream& stream ) const
{
	loadFile( rtpFileName, stream );
}

void RDORepositoryFile::loadRSS( stringstream& stream ) const
{
	loadFile( rssFileName, stream );
}

void RDORepositoryFile::loadOPR( stringstream& stream ) const
{
	loadFile( oprFileName, stream );
}

void RDORepositoryFile::loadFRM( stringstream& stream ) const
{
	loadFile( frmFileName, stream );
}

void RDORepositoryFile::loadFUN( stringstream& stream ) const
{
	loadFile( funFileName, stream );
}

void RDORepositoryFile::loadDPT( stringstream& stream ) const
{
	loadFile( dptFileName, stream );
}

void RDORepositoryFile::loadSMR( stringstream& stream ) const
{
	loadFile( smrFileName, stream );
}

void RDORepositoryFile::loadPMD( stringstream& stream ) const
{
	loadFile( pmdFileName, stream );
}

void RDORepositoryFile::loadPMV( stringstream& stream ) const
{
	loadFile( pmvFileName, stream );
}

void RDORepositoryFile::loadTRC( stringstream& stream ) const
{
	loadFile( trcFileName, stream );
}

void RDORepositoryFile::savePAT( stringstream& stream ) const
{
	saveFile( patFileName, stream );
}

void RDORepositoryFile::saveRTP( stringstream& stream ) const
{
	saveFile( rtpFileName, stream );
}

void RDORepositoryFile::saveRSS( stringstream& stream ) const
{
	saveFile( rssFileName, stream );
}

void RDORepositoryFile::saveOPR( stringstream& stream ) const
{
	saveFile( oprFileName, stream );
}

void RDORepositoryFile::saveFRM( stringstream& stream ) const
{
	saveFile( frmFileName, stream );
}

void RDORepositoryFile::saveFUN( stringstream& stream ) const
{
	saveFile( funFileName, stream );
}

void RDORepositoryFile::saveDPT( stringstream& stream ) const
{
	saveFile( dptFileName, stream );
}

void RDORepositoryFile::saveSMR( stringstream& stream ) const
{
	saveFile( smrFileName, stream );
}

void RDORepositoryFile::savePMD( stringstream& stream ) const
{
	saveFile( pmdFileName, stream );
}

void RDORepositoryFile::savePMV( stringstream& stream ) const
{
	saveFile( pmvFileName, stream );
}

void RDORepositoryFile::saveTRC( stringstream& stream ) const
{
	saveFile( trcFileName, stream );
}
