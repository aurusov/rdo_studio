#ifndef RDOREPOSITORYFILE_H
#define RDOREPOSITORYFILE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdothread.h>
#include <rdostream.h>
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
	struct FileData
	{
		rdoModelObjects::RDOFileType  m_type;
		REF(rdo::stream)              m_stream;

		FileData(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream)
			: m_type  (type  )
			, m_stream(stream)
		{}
	};
	struct BinaryFile
	{
		tstring           m_name;
		REF(rdo::stream)  m_stream;

		BinaryFile(CREF(tstring) name, REF(rdo::stream) stream)
			: m_name  (name  )
			, m_stream(stream)
		{}
	};
	struct OpenFile
	{
		tstring  m_name;
		rbool    m_readonly;
		rbool    m_result;

		OpenFile(CREF(tstring) name = _T(""), rbool readonly = false)
			: m_name    (name    )
			, m_readonly(readonly)
			, m_result  (false   )
		{}
	};
	struct NewModel
	{
		tstring  m_name;
		tstring  m_path;

		NewModel(CREF(tstring) name = _T(""), CREF(tstring) path = _T(""))
			: m_name(name)
			, m_path(path)
		{}
	};
	struct FileInfo
	{
		rdoModelObjects::RDOFileType  m_type;
		tstring                       m_name;
		tstring                       m_full_name;
		tstring                       m_extention;
		rbool                         m_readonly;
		rbool                         m_described;

		FileInfo(  rdoModelObjects::RDOFileType type      = rdoModelObjects::SMR
		         , CREF(tstring)                name      = _T("")
		         , CREF(tstring)                full_name = _T("")
		         , CREF(tstring)                extention = _T("smr")
		         , rbool                        readonly  = false
		         , rbool                        described = false
		)
		: m_type     (type     )
		, m_name     (name     )
		, m_full_name(full_name)
		, m_extention(extention)
		, m_readonly (readonly )
		, m_described(described)
		{}
	};

private:
	tstring modelName;
	tstring modelPath;
	rbool   hasModel;

	struct fileInfo {
		tstring filename;
		tstring extention;
		rbool   described;
		rbool   mustexist;
		rbool   deleteifempty;
		rbool   readonly;

		fileInfo(): extention( "" ), deleteifempty( false ), readonly( false ) {
			resetname();
		};

		void resetname() {
			filename  = "";
			described = false;
			mustexist = true;
		}
	};

	typedef std::vector<fileInfo> FileList;
	FileList m_files;

	void resetModelNames();

	enum FindModel {
		fm_ok,
		fm_smr_error,
		fm_smr_empty
	};
	FindModel updateModelNames();

	rbool realOnlyInDlg;

	rbool saveAsDlg();
	rbool canCloseModel();
	void realCloseModel();

	void extractName(CREF(tstring) fullname);

	void setName(CREF(tstring) str);

	void loadFile(CREF(tstring) filename, REF(rdo::stream) stream, rbool described, rbool mustExist, REF(rbool) reanOnly) const;
	void saveFile(CREF(tstring) filename, REF(rdo::stream) stream, rbool deleteIfEmpty = false) const;

//	void changeLastModelPath();

	std::ofstream trace_file;
	void beforeModelStart();
	void stopModel();
	void trace( CREF(tstring) str );
	void writeModelFilesInfo(REF(std::ofstream) stream) const;

protected:
	virtual ~RDOThreadRepository(); // Чтобы нельзя было удалить через delete
	virtual void proc(REF(RDOMessageInfo) msg);

	void newModel   (CPTRC(NewModel) data                  );
	rbool openModel  (CREF(tstring)   modelFileName = _T(""));
	void closeModel ();
	rbool saveModel  ();
	void saveAsModel();

	void load( rdoModelObjects::RDOFileType type, REF(rdo::stream) stream );
	void save( rdoModelObjects::RDOFileType type, REF(rdo::stream) stream ) const;

	tstring getFileName    (rdoModelObjects::RDOFileType type) const { return m_files[type].filename;  }
	tstring getExtention   (rdoModelObjects::RDOFileType type) const { return m_files[type].extention; }
	tstring getFileExtName (rdoModelObjects::RDOFileType type) const { return m_files[type].filename + m_files[type].extention; }
	tstring getFullFileName(rdoModelObjects::RDOFileType type) const { return modelPath + getFileExtName(type);                 }
	rbool   isReadOnly     (rdoModelObjects::RDOFileType type) const { return m_files[type].readonly;  }
	rbool   isDescribed    (rdoModelObjects::RDOFileType type) const { return m_files[type].described; }
	rbool   isMustExist    (rdoModelObjects::RDOFileType type) const { return m_files[type].mustexist; }

	void loadBMP( CREF(tstring) name, REF(rdo::stream) stream ) const;

public:
	RDOThreadRepository();
};

}; // namespace rdoRepository

#endif // RDOREPOSITORYFILE_H
