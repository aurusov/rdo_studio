#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <sstream>

using namespace std;

class CFileDialog;

namespace rdoRepository {

// ----------------------------------------------------------------------------
// ---------- RDORepositoryFile
// ----------------------------------------------------------------------------
class RDORepositoryFile
{
private:
	string modelName;
	string modelPath;
	string lastModelPath;
	string patFileName;
	string rtpFileName;
	string rssFileName;
	string oprFileName;
	string frmFileName;
	string funFileName;
	string dptFileName;
	string smrFileName;
	string pmdFileName;
	string pmvFileName;
	string trcFileName;

	bool readOnly;

	bool saveAsDlg();
	bool canCloseModel() const;
	void realCloseModel();

	void extractName( const CFileDialog* const dlg );
	void extractName( const string& fullname );
	static string extractFilePath( const string& fileName );
	static bool isFileExists( const string& fileName );

	void setName( const string& str );

	void loadFile( const string& filename, stringstream& stream ) const;
	void saveFile( const string& filename, stringstream& stream ) const;

	void changeLastModelPath();

public:
	RDORepositoryFile();
	virtual ~RDORepositoryFile();

	string getName() const;
	string getFullName() const;

	void newModel();
	bool openModel( const string& modelFileName = "" );
	bool saveModel();
	void saveAsModel();
	void closeModel();

	bool isReadOnly() const;

	void loadPAT( stringstream& stream ) const;
	void loadRTP( stringstream& stream ) const;
	void loadRSS( stringstream& stream ) const;
	void loadOPR( stringstream& stream ) const;
	void loadFRM( stringstream& stream ) const;
	void loadFUN( stringstream& stream ) const;
	void loadDPT( stringstream& stream ) const;
	void loadSMR( stringstream& stream ) const;
	void loadPMD( stringstream& stream ) const;
	void loadPMV( stringstream& stream ) const;
	void loadTRC( stringstream& stream ) const;

	void savePAT( stringstream& stream ) const;
	void saveRTP( stringstream& stream ) const;
	void saveRSS( stringstream& stream ) const;
	void saveOPR( stringstream& stream ) const;
	void saveFRM( stringstream& stream ) const;
	void saveFUN( stringstream& stream ) const;
	void saveDPT( stringstream& stream ) const;
	void saveSMR( stringstream& stream ) const;
	void savePMD( stringstream& stream ) const;
	void savePMV( stringstream& stream ) const;
	void saveTRC( stringstream& stream ) const;
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
