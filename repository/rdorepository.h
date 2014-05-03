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
#include <iostream>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
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
		rdo::model::FileType  m_type;
		std::stringstream&            m_stream;

		FileData(rdo::model::FileType type, std::stringstream& stream)
			: m_type  (type  )
			, m_stream(stream)
		{}
	};
	struct BinaryFile: public boost::noncopyable
	{
		boost::filesystem::path  m_name;
		std::stringstream&       m_stream;

		BinaryFile(const boost::filesystem::path& name, std::stringstream& stream)
			: m_name  (name  )
			, m_stream(stream)
		{}
	};
	struct CreateFileInfo: public boost::noncopyable
	{
		boost::filesystem::path m_name;
		boost::filesystem::ofstream& m_stream;

		CreateFileInfo(const boost::filesystem::path& name, boost::filesystem::ofstream& stream)
			: m_name  (name  )
			, m_stream(stream)
		{}
	};
	struct OpenFile
	{
		boost::filesystem::path m_name;
		bool m_readOnly;
		bool m_result;

		OpenFile(const boost::filesystem::path& name = boost::filesystem::path(), bool readOnly = false)
			: m_name    (name    )
			, m_readOnly(readOnly)
			, m_result  (false   )
		{}
	};
	struct NewModel
	{
		boost::filesystem::path  m_name;
		boost::filesystem::path  m_path;

		NewModel(const boost::filesystem::path& name = boost::filesystem::path(), const boost::filesystem::path& path = boost::filesystem::path())
			: m_name(name)
			, m_path(path)
		{}
	};
	struct FileInfo
	{
		rdo::model::FileType m_type;
		boost::filesystem::path m_name;
		boost::filesystem::path m_fullName;
		boost::filesystem::path m_extension;
		bool m_readOnly;
		bool m_described;

		FileInfo(rdo::model::FileType type = rdo::model::SMR
		       , const boost::filesystem::path& name = boost::filesystem::path()
		       , const boost::filesystem::path& fullName = boost::filesystem::path()
		       , const boost::filesystem::path& extension = "smr"
		       , bool readOnly  = false
		       , bool described = false
		)
			: m_type     (type     )
			, m_name     (name     )
			, m_fullName (fullName )
			, m_extension(extension)
			, m_readOnly (readOnly )
			, m_described(described)
		{}
	};

	static bool createRDOX(const boost::filesystem::path& fileName);

private:
	struct fileInfo
	{
		boost::filesystem::path m_fileName;
		boost::filesystem::path m_extension;
		bool m_described;
		bool m_mustExist;
		bool m_deleteIfEmpty;
		bool m_readOnly;

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
	typedef std::map<rdo::model::FileType, fileInfo> FileList;

	enum FindModel
	{
		fm_ok,
		fm_smr_error,
		fm_smr_empty
	};

	struct ProjectName
	{
		boost::filesystem::path m_fullFileName;
		bool m_rdox;

		ProjectName()
			: m_rdox(true)
		{}
	};

	typedef boost::posix_time::ptime SystemTime;

	boost::filesystem::path m_modelName;
	boost::filesystem::path m_modelPath;
	bool m_hasModel;
	boost::filesystem::ofstream m_traceFile;
	FileList m_files;
	bool m_realOnlyInDlg;
	ProjectName m_projectName;
	SystemTime m_systemTime;

	void resetModelNames();
	FindModel updateModelNames();
	bool canCloseModel();
	void realCloseModel();
	void extractName(const boost::filesystem::path& fullName);
	void setName(const boost::filesystem::path& name);
	void createRDOX();

	void loadFile(const boost::filesystem::path& fileName, std::ostream& stream, bool described, bool mustExist, bool& reanOnly) const;
	void saveFile(const boost::filesystem::path& fileName, const std::stringstream& stream, bool deleteIfEmpty = false) const;

	bool createFile(const boost::filesystem::path& name, boost::filesystem::ofstream& stream) const;

	void beforeModelStart();
	void stopModel();
	void trace(const std::string& message);
	void writeModelFilesInfo(boost::filesystem::ofstream& stream) const;

protected:
	virtual ~RDOThreadRepository(); // Чтобы нельзя было удалить через delete
	virtual void proc(RDOMessageInfo& msg);

	void newModel(const NewModel* const data);
	bool openModel(const boost::filesystem::path& modelFileName);
	void closeModel();
	bool saveModel();

	void load(rdo::model::FileType type, std::ostream& stream);
	void save(rdo::model::FileType type, const std::stringstream& stream) const;

	boost::filesystem::path getFileName(rdo::model::FileType type) const;
	boost::filesystem::path getExtension(rdo::model::FileType type) const;
	boost::filesystem::path getFileExtName(rdo::model::FileType type) const;
	boost::filesystem::path getFullFileName(rdo::model::FileType type) const;
	bool isReadOnly(rdo::model::FileType type) const;
	bool isDescribed(rdo::model::FileType type) const;
	bool isMustExist(rdo::model::FileType type) const;
	bool isDeleteIfEmpty(rdo::model::FileType type) const;

	void loadBMP(boost::filesystem::path& name, std::ostream& stream) const;
};

}} // namespace rdo::repository

#endif // _RDOREPOSITORY_H_
