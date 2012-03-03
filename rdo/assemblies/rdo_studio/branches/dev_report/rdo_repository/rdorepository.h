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

namespace rdoRepository {

// ----------------------------------------------------------------------------
// ---------- RDOThreadRepository
// ----------------------------------------------------------------------------
class RDOThreadRepository: public RDOThreadMT
{
public:
	struct FileData {
		rdoModelObjects::RDOFileType type;
		rdo::binarystream&           stream;
		FileData( rdoModelObjects::RDOFileType _type, rdo::binarystream& _stream ):
			type( _type ),
			stream( _stream )
		{
		};
	};
	struct BinaryFile {
		std::string        name;
		rdo::binarystream& stream;
		BinaryFile( const std::string& _name, rdo::binarystream& _stream ):
			name( _name ),
			stream( _stream )
		{
		};
	};
	struct OpenFile {
		std::string name;
		bool        readonly;
		bool        result;
		OpenFile( const std::string& _name = "", bool _readonly = false ):
			name( _name ),
			readonly( _readonly ),
			result( false )
		{
		}
	};
	struct NewModel {
		std::string name;
		std::string path;
		NewModel( const std::string& _name = "", const std::string& _path = "" ):
			name( _name ),
			path( _path )
		{
		}
	};
	struct FileInfo {
		rdoModelObjects::RDOFileType type;
		std::string name;
		std::string full_name;
		std::string extention;
		bool        readonly;
		bool        described;
		FileInfo( rdoModelObjects::RDOFileType _type = rdoModelObjects::SMR, const std::string& _name = "", const std::string& _full_name = "", const std::string& _extention = "smr", bool _readonly = false, bool _described = false ):
			type( _type ),
			name( _name ),
			full_name( _full_name ),
			extention( _extention ),
			readonly( _readonly ),
			described( _described )
		{
		}
	};

private:
	std::string modelName;
	std::string modelPath;
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

	enum FindModel {
		fm_ok,
		fm_smr_error,
		fm_smr_empty
	};
	FindModel updateModelNames();

	bool realOnlyInDlg;

	bool saveAsDlg();
	bool canCloseModel();
	void realCloseModel();

	void extractName( const std::string& fullname );
	static bool isFileReadOnly( const std::string& fileName );

	void setName( const std::string& str );

	void loadFile( const std::string& filename, rdo::binarystream& stream, const bool described, const bool mustExist, bool& reanOnly ) const;
	void saveFile( const std::string& filename, rdo::binarystream& stream, const bool deleteIfEmpty = false ) const;

//	void changeLastModelPath();

	std::ofstream trace_file;
	void beforeModelStart();
	void stopModel();
	void trace( const std::string& str );
	void writeModelFilesInfo( std::ofstream& stream ) const;

protected:
	virtual ~RDOThreadRepository(); // Чтобы нельзя было удалить через delete
	virtual void proc( RDOMessageInfo& msg );

	void newModel( const NewModel* const data );
	bool openModel( const std::string& modelFileName = "" );
	void closeModel();
	bool saveModel();
	void saveAsModel();

	void load( rdoModelObjects::RDOFileType type, rdo::binarystream& stream );
	void save( rdoModelObjects::RDOFileType type, rdo::binarystream& stream ) const;

	std::string getFileName( rdoModelObjects::RDOFileType type ) const     { return files[ type ].filename;  }
	std::string getExtention( rdoModelObjects::RDOFileType type ) const    { return files[ type ].extention; }
	std::string getFileExtName( rdoModelObjects::RDOFileType type ) const  { return files[ type ].filename + files[ type ].extention; }
	std::string getFullFileName( rdoModelObjects::RDOFileType type ) const { return modelPath + getFileExtName( type );               }
	bool isReadOnly( rdoModelObjects::RDOFileType type ) const             { return files[ type ].readonly;  }
	bool isDescribed( rdoModelObjects::RDOFileType type ) const            { return files[ type ].described; }
	bool isMustExist( rdoModelObjects::RDOFileType type ) const            { return files[ type ].mustexist; }

	void loadBMP( const std::string& name, rdo::binarystream& stream ) const;

public:
	RDOThreadRepository();
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
