#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <fstream>
#include <rdobinarystream.h>

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
	void updateModelNames();

	bool readOnly;

	bool saveAsDlg();
	bool canCloseModel() const;
	void realCloseModel();

	void extractName( const CFileDialog* const dlg );
	void extractName( const std::string& fullname );
	static std::string extractFilePath( const std::string& fileName );
	static bool isFileExists( const std::string& fileName );

	void setName( const std::string& str );

	void loadFile( const std::string& filename, rdo::binarystream& stream, const bool described, const bool mustExist ) const;
	void saveFile( const std::string& filename, rdo::binarystream& stream, const bool deleteIfEmpty = false ) const;

	void changeLastModelPath();

	std::ofstream trace_file;
	static void beforeModelStartNotify();
	static void stopModelNotify();
	static void traceNotify( std::string str );
	void beforeModelStart();
	void stopModel();
	void trace( const std::string& str );

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

	void loadPAT( rdo::binarystream& stream ) const;
	void loadRTP( rdo::binarystream& stream ) const;
	void loadRSS( rdo::binarystream& stream ) const;
	void loadOPR( rdo::binarystream& stream ) const;
	void loadFRM( rdo::binarystream& stream ) const;
	void loadFUN( rdo::binarystream& stream ) const;
	void loadDPT( rdo::binarystream& stream ) const;
	void loadSMR( rdo::binarystream& stream ) const;
	void loadPMD( rdo::binarystream& stream ) const;
	void loadPMV( rdo::binarystream& stream ) const;
	void loadTRC( rdo::binarystream& stream ) const;

	void savePAT( rdo::binarystream& stream ) const;
	void saveRTP( rdo::binarystream& stream ) const;
	void saveRSS( rdo::binarystream& stream ) const;
	void saveOPR( rdo::binarystream& stream ) const;
	void saveFRM( rdo::binarystream& stream ) const;
	void saveFUN( rdo::binarystream& stream ) const;
	void saveDPT( rdo::binarystream& stream ) const;
	void saveSMR( rdo::binarystream& stream ) const;
	void savePMD( rdo::binarystream& stream ) const;
	void savePMV( rdo::binarystream& stream ) const;
	void saveTRC( rdo::binarystream& stream ) const;

	void loadBMP( const std::string& name, rdo::binarystream& stream ) const;
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
