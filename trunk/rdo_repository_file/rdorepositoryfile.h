#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <sstream>
#include <vector>

class CFileDialog;

namespace rdoRepository {

// ----------------------------------------------------------------------------
// ---------- RDORepositoryFile
// ----------------------------------------------------------------------------
class RDORepositoryFile
{
private:
	std::string modelName;
	std::string modelPath;
	std::string lastModelPath;
	std::string patFileName;
	std::string rtpFileName;
	std::string rssFileName;
	std::string oprFileName;
	std::string frmFileName;
	std::string funFileName;
	std::string dptFileName;
	std::string smrFileName;
	std::string pmdFileName;
	std::string pmvFileName;
	std::string trcFileName;
	bool patDescribed;
	bool rtpDescribed;
	bool rssDescribed;
	bool oprDescribed;
	bool frmDescribed;
	bool funDescribed;
	bool dptDescribed;
	bool smrDescribed;
	bool pmdDescribed;
	bool pmvDescribed;
	bool trcDescribed;
	bool patMustExist;
	bool rtpMustExist;
	bool rssMustExist;
	bool oprMustExist;
	bool frmMustExist;
	bool funMustExist;
	bool dptMustExist;
	bool smrMustExist;
	bool pmdMustExist;
	bool pmvMustExist;
	bool trcMustExist;
	void resetModelNames();

	bool readOnly;

	bool saveAsDlg();
	bool canCloseModel() const;
	void realCloseModel();

	void extractName( const CFileDialog* const dlg );
	void extractName( const std::string& fullname );
	static std::string extractFilePath( const std::string& fileName );
	static bool isFileExists( const std::string& fileName );

	void setName( const std::string& str );

	void loadFile( const std::string& filename, std::stringstream& stream, const bool described, const bool mustExist ) const;
	void saveFile( const std::string& filename, std::stringstream& stream, const bool deleteIfEmpty = false ) const;

	void loadFile( const std::string& filename, std::vector< char >& vec, const bool described, const bool mustExist ) const;
	void saveFile( const std::string& filename, std::vector< char >& vec, const bool deleteIfEmpty = false ) const;

	void changeLastModelPath();

public:
	RDORepositoryFile();
	virtual ~RDORepositoryFile();

	std::string getName() const;
	std::string getFullName() const;

	void newModel();
	bool openModel( const std::string& modelFileName = "" );
	bool saveModel();
	void saveAsModel();
	void closeModel();

	bool isReadOnly() const;

	void loadPAT( std::stringstream& stream ) const;
	void loadRTP( std::stringstream& stream ) const;
	void loadRSS( std::stringstream& stream ) const;
	void loadOPR( std::stringstream& stream ) const;
	void loadFRM( std::stringstream& stream ) const;
	void loadFUN( std::stringstream& stream ) const;
	void loadDPT( std::stringstream& stream ) const;
	void loadSMR( std::stringstream& stream ) const;
	void loadPMD( std::stringstream& stream ) const;
	void loadPMV( std::stringstream& stream ) const;
	void loadTRC( std::stringstream& stream ) const;

	void savePAT( std::stringstream& stream ) const;
	void saveRTP( std::stringstream& stream ) const;
	void saveRSS( std::stringstream& stream ) const;
	void saveOPR( std::stringstream& stream ) const;
	void saveFRM( std::stringstream& stream ) const;
	void saveFUN( std::stringstream& stream ) const;
	void saveDPT( std::stringstream& stream ) const;
	void saveSMR( std::stringstream& stream ) const;
	void savePMD( std::stringstream& stream ) const;
	void savePMV( std::stringstream& stream ) const;
	void saveTRC( std::stringstream& stream ) const;

	void loadBMP( const std::string& name, std::iostream& stream ) const;

	void loadPAT( std::vector< char >& vec ) const;
	void loadRTP( std::vector< char >& vec ) const;
	void loadRSS( std::vector< char >& vec ) const;
	void loadOPR( std::vector< char >& vec ) const;
	void loadFRM( std::vector< char >& vec ) const;
	void loadFUN( std::vector< char >& vec ) const;
	void loadDPT( std::vector< char >& vec ) const;
	void loadSMR( std::vector< char >& vec ) const;
	void loadPMD( std::vector< char >& vec ) const;
	void loadPMV( std::vector< char >& vec ) const;
	void loadTRC( std::vector< char >& vec ) const;
/*
	void savePAT( std::vector< char >& vec ) const;
	void saveRTP( std::vector< char >& vec ) const;
	void saveRSS( std::vector< char >& vec ) const;
	void saveOPR( std::vector< char >& vec ) const;
	void saveFRM( std::vector< char >& vec ) const;
	void saveFUN( std::vector< char >& vec ) const;
	void saveDPT( std::vector< char >& vec ) const;
	void saveSMR( std::vector< char >& vec ) const;
	void savePMD( std::vector< char >& vec ) const;
	void savePMV( std::vector< char >& vec ) const;
	void saveTRC( std::vector< char >& vec ) const;
*/
	void loadBMP( const std::string& name, std::vector< char >& vec ) const;
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
