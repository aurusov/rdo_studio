/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorepository.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOREPOSITORY_H_
#define _RDOREPOSITORY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "utils/src/stream/rdostream.h"
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace repository {

// --------------------------------------------------------------------------------
// -------------------- RDOThreadRepository
// --------------------------------------------------------------------------------
class RDOThreadRepository: public RDOThreadMT
{
public:
	RDOThreadRepository();

	struct FileData: public boost::noncopyable
	{
		rdoModelObjects::RDOFileType  m_type;
		REF(rdo::stream)              m_stream;

		FileData(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream)
			: m_type  (type  )
			, m_stream(stream)
		{}
	};
	struct BinaryFile: public boost::noncopyable
	{
		boost::filesystem::path  m_name;
		REF(rdo::stream)         m_stream;

		BinaryFile(CREF(boost::filesystem::path) name, REF(rdo::stream) stream)
			: m_name  (name  )
			, m_stream(stream)
		{}
	};
	struct CreateFileInfo: public boost::noncopyable
	{
		boost::filesystem::path          m_name;
		REF(boost::filesystem::ofstream) m_stream;

		CreateFileInfo(CREF(boost::filesystem::path) name, REF(boost::filesystem::ofstream) stream)
			: m_name  (name  )
			, m_stream(stream)
		{}
	};
	struct OpenFile
	{
		boost::filesystem::path  m_name;
		rbool                    m_readOnly;
		rbool                    m_result;

		OpenFile(CREF(boost::filesystem::path) name = boost::filesystem::path(), rbool readOnly = false)
			: m_name    (name    )
			, m_readOnly(readOnly)
			, m_result  (false   )
		{}
	};
	struct NewModel
	{
		boost::filesystem::path  m_name;
		boost::filesystem::path  m_path;

		NewModel(CREF(boost::filesystem::path) name = boost::filesystem::path(), CREF(boost::filesystem::path) path = boost::filesystem::path())
			: m_name(name)
			, m_path(path)
		{}
	};
	struct FileInfo
	{
		rdoModelObjects::RDOFileType  m_type;
		boost::filesystem::path       m_name;
		boost::filesystem::path       m_fullName;
		boost::filesystem::path       m_extention;
		rbool                         m_readOnly;
		rbool                         m_described;

		FileInfo(  rdoModelObjects::RDOFileType  type      = rdoModelObjects::SMR
		         , CREF(boost::filesystem::path) name      = boost::filesystem::path()
		         , CREF(boost::filesystem::path) fullName  = boost::filesystem::path()
		         , CREF(boost::filesystem::path) extention = "smr"
		         , rbool                         readOnly  = false
		         , rbool                         described = false
		)
			: m_type     (type     )
			, m_name     (name     )
			, m_fullName (fullName )
			, m_extention(extention)
			, m_readOnly (readOnly )
			, m_described(described)
		{}
	};

private:
	struct fileInfo
	{
		boost::filesystem::path  m_fileName;
		boost::filesystem::path  m_extention;
		rbool                    m_described;
		rbool                    m_mustExist;
		rbool                    m_deleteIfEmpty;
		rbool                    m_readOnly;

		fileInfo()
			: m_deleteIfEmpty(false)
			, m_readOnly     (false)
		{
			resetname();
		}

		void resetname()
		{
			m_fileName  = boost::filesystem::path();
			m_described = false;
			m_mustExist = true;
		}
	};
	typedef std::map<rdoModelObjects::RDOFileType, fileInfo> FileList;

	enum FindModel
	{
		fm_ok,
		fm_smr_error,
		fm_smr_empty
	};

	struct ProjectName
	{
		boost::filesystem::path  m_fullFileName;
		rbool                    m_rdox;

		ProjectName()
			: m_rdox(true)
		{}
	};

	typedef boost::posix_time::ptime SystemTime;

	boost::filesystem::path      m_modelName;
	boost::filesystem::path      m_modelPath;
	rbool                        m_hasModel;
	boost::filesystem::ofstream  m_traceFile;
	FileList                     m_files;
	rbool                        m_realOnlyInDlg;
	ProjectName                  m_projectName;
	SystemTime                   m_systemTime;

	void      resetModelNames ();
	FindModel updateModelNames();
	rbool     canCloseModel   ();
	void      realCloseModel  ();
	void      extractName     (CREF(boost::filesystem::path) fullName);
	void      setName         (CREF(boost::filesystem::path) name);
	void      createRDOX      ();

	void      loadFile(CREF(boost::filesystem::path) fileName, REF(rdo::stream) stream, rbool described, rbool mustExist, REF(rbool) reanOnly) const;
	void      saveFile(CREF(boost::filesystem::path) fileName, REF(rdo::stream) stream, rbool deleteIfEmpty = false) const;

	rbool     createFile(CREF(boost::filesystem::path) name, REF(boost::filesystem::ofstream) stream) const;

	void      beforeModelStart   ();
	void      stopModel          ();
	void      trace              (CREF(tstring) message);
	void      writeModelFilesInfo(REF(boost::filesystem::ofstream) stream) const;

protected:
	virtual ~RDOThreadRepository(); // Чтобы нельзя было удалить через delete
	virtual void proc(REF(RDOMessageInfo) msg);

	void  newModel  (CPTRC(NewModel) data);
	rbool openModel (CREF(boost::filesystem::path) modelFileName);
	void  closeModel();
	rbool saveModel ();

	void load(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream);
	void save(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream) const;

	boost::filesystem::path getFileName    (rdoModelObjects::RDOFileType type) const;
	boost::filesystem::path getExtention   (rdoModelObjects::RDOFileType type) const;
	boost::filesystem::path getFileExtName (rdoModelObjects::RDOFileType type) const;
	boost::filesystem::path getFullFileName(rdoModelObjects::RDOFileType type) const;
	rbool   isReadOnly     (rdoModelObjects::RDOFileType type) const;
	rbool   isDescribed    (rdoModelObjects::RDOFileType type) const;
	rbool   isMustExist    (rdoModelObjects::RDOFileType type) const;
	rbool   isDeleteIfEmpty(rdoModelObjects::RDOFileType type) const;

	void loadBMP(REF(boost::filesystem::path) name, REF(rdo::stream) stream) const;
};

}} // namespace rdo::repository

#endif // _RDOREPOSITORY_H_
