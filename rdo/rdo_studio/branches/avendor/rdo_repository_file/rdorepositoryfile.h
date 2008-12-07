#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <strstream>

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

	bool readOnly;

	bool saveAsDlg();
	bool canCloseModel() const;
	void realCloseModel();

	void extractName( const CFileDialog* const dlg );
	void extractName( const string& fullname );
	static string extractFilePath( const string& fileName );
	static bool isFileExists( const string& fileName );

	void setName( const string& str );

	void loadFile( string& filename, strstream& stream ) const;
	void saveFile( string& filename, strstream& stream ) const;

	void changeLastModelPath();

public:
	RDORepositoryFile();
	virtual ~RDORepositoryFile();

	string getName() const;
	string getFullName() const;

	void newModel();
	bool openModel( const string& modelFileName = "" );
	void saveModel();
	void saveAsModel();
	void closeModel();

	bool isReadOnly() const;

	void loadPAT( strstream& stream ) const;
	void loadRTP( strstream& stream ) const;
	void loadRSS( strstream& stream ) const;
	void loadOPR( strstream& stream ) const;
	void loadFRM( strstream& stream ) const;
	void loadFUN( strstream& stream ) const;
	void loadDPT( strstream& stream ) const;
	void loadSMR( strstream& stream ) const;
	void loadPMD( strstream& stream ) const;
	void loadPMV( strstream& stream ) const;
	void loadTRC( strstream& stream ) const;

	void savePAT( strstream& stream ) const;
	void saveRTP( strstream& stream ) const;
	void saveRSS( strstream& stream ) const;
	void saveOPR( strstream& stream ) const;
	void saveFRM( strstream& stream ) const;
	void saveFUN( strstream& stream ) const;
	void saveDPT( strstream& stream ) const;
	void saveSMR( strstream& stream ) const;
	void savePMD( strstream& stream ) const;
	void savePMV( strstream& stream ) const;
	void saveTRC( strstream& stream ) const;
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
