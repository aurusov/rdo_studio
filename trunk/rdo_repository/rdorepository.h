#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdothread.h>
#include <rdobinarystream.h>
#include <rdocommon.h>
#include <string>
#include <fstream>

class CFileDialog;

namespace rdoRepository {

// ----------------------------------------------------------------------------
// ---------- RDOThreadRepository
// ----------------------------------------------------------------------------
class RDOThreadRepository: public RDOThreadMT
{
private:
	std::string modelName;
	std::string modelPath;
	std::string lastModelPath;
	bool hasModel;

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
	bool canCloseModel();
	void realCloseModel();

	void extractName( const CFileDialog* const dlg );
	void extractName( const std::string& fullname );
	static bool isFileReadOnly( const std::string& fileName );

	void setName( const std::string& str );

	void loadFile( const std::string& filename, rdo::binarystream& stream, const bool described, const bool mustExist, bool& reanOnly ) const;
	void saveFile( const std::string& filename, rdo::binarystream& stream, const bool deleteIfEmpty = false ) const;

	void changeLastModelPath();

	std::ofstream trace_file;
	void beforeModelStart();
	void stopModel();
	void trace( const std::string& str );

protected:
	virtual ~RDOThreadRepository(); // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );

	void newModel();
	bool openModel( const std::string& modelFileName = "" );
	void closeModel();
	bool saveModel();
	void saveAsModel();

	void load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream );
	void save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const;

public:
	RDOThreadRepository();

	std::string getName() const      { return modelName;                                          }
	std::string getFullName() const  { return modelPath + getFileExtName( rdoModelObjects::SMR ); }

	std::string getFileName( rdoModelObjects::RDOFileType type ) const     { return files[ type ].filename;  }
	std::string getExtention( rdoModelObjects::RDOFileType type ) const    { return files[ type ].extention; }
	std::string getFileExtName( rdoModelObjects::RDOFileType type ) const  { return files[ type ].filename + files[ type ].extention; }
	std::string getFullFileName( rdoModelObjects::RDOFileType type ) const { return modelPath + getFileExtName( type );               }
	bool isReadOnly( rdoModelObjects::RDOFileType type ) const             { return files[ type ].readonly;  }
	bool isDescribed( rdoModelObjects::RDOFileType type ) const            { return files[ type ].described; }
	bool isMustExist( rdoModelObjects::RDOFileType type ) const            { return files[ type ].mustexist; }

	struct FileData {
		rdoModelObjects::RDOFileType type;
		rdo::binarystream&           stream;
		FileData( rdoModelObjects::RDOFileType _type, rdo::binarystream& _stream ): type( _type ), stream( _stream ) {};
	};
	struct OpenFile {
		std::string name;
		bool&       result;
		OpenFile( const std::string& _name, bool& _result ): name( _name ), result( _result ) {}
	};

	void loadBMP( const std::string& name, rdo::binarystream& stream ) const;
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
