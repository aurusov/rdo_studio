#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <fstream>
#include <rdobinarystream.h>
#include <rdocommon.h>

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

	struct fileInfo {
		std::string filename;
		std::string extention;
		bool described;
		bool mustexist;
		bool deleteifempty;
		bool readonly;

		fileInfo(): extention( "" ), deleteifempty( false ), readonly( false ) {
			resetname();
		};

		void resetname() {
			filename  = "";
			described = false;
			mustexist = true;
		}
	};

	std::vector< fileInfo > files;

	void resetModelNames();
	bool updateModelNames();

	bool realOnlyInDlg;

	bool saveAsDlg();
	bool canCloseModel() const;
	void realCloseModel();

	void extractName( const CFileDialog* const dlg );
	void extractName( const std::string& fullname );
	static bool isFileReadOnly( const std::string& fileName );

	void setName( const std::string& str );

	void loadFile( const std::string& filename, rdo::binarystream& stream, const bool described, const bool mustExist, bool& reanOnly ) const;
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

	std::string getName() const      { return modelName;                                          }
	std::string getFullName() const  { return modelPath + getFileExtName( rdoModelObjects::SMR ); }

	void newModel();
	bool openModel( const std::string& modelFileName = "" );
	bool saveModel();
	void saveAsModel();
	void closeModel();

	std::string getFileName( rdoModelObjects::RDOFileType type ) const     { return files[ type ].filename;  }
	std::string getExtention( rdoModelObjects::RDOFileType type ) const    { return files[ type ].extention; }
	std::string getFileExtName( rdoModelObjects::RDOFileType type ) const  { return files[ type ].filename + files[ type ].extention; }
	std::string getFullFileName( rdoModelObjects::RDOFileType type ) const { return modelPath + getFileExtName( type );               }
	bool isReadOnly( rdoModelObjects::RDOFileType type ) const             { return files[ type ].readonly;  }
	bool isDescribed( rdoModelObjects::RDOFileType type ) const            { return files[ type ].described; }
	bool isMustExist( rdoModelObjects::RDOFileType type ) const            { return files[ type ].mustexist; }

	void load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream );
	void save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const;

	void loadBMP( const std::string& name, rdo::binarystream& stream ) const;
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
