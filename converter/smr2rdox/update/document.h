#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"

#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
#include "converter/smr2rdox/update/document_i.h"
#include "converter/smr2rdox/update/update_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Document
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(Document);
class Document
	: public rdo::counter_reference
	, public IDocument
{
DECLARE_FACTORY(Document)
public:
	typedef rdo::converter::smr2rdox::RDOFileTypeOut TypeOut;

	void  create      (const boost::filesystem::path& filePath, const boost::filesystem::path& modelName);
	void  init        (rdo::converter::smr2rdox::RDOFileTypeIn type, std::ifstream& stream);
	void  insertUpdate(const LPDocUpdate& pUpdate);
	void  convert     ();
	void  close       ();
	boost::filesystem::path getName(TypeOut typeOut) const;

private:
	 Document();
	~Document();

	class MemoryStream
	{
	public:
		typedef std::vector<char> Buffer;

		void init(std::ifstream& stream);
		void get(std::ofstream& stream) const;

		void insert(std::size_t to, const std::string& value);
		void remove(std::size_t from, std::size_t to);

		std::string get(std::size_t from, std::size_t to);

	private:
		Buffer m_buffer;
	};

	typedef boost::shared_ptr<MemoryStream>  LPMemoryStream;
	typedef boost::shared_ptr<std::ofstream> LPFileStream;
	typedef std::map<Type, LPMemoryStream>   MemoryFileList;
	typedef std::map<TypeOut, LPFileStream>  StreamFileList;

	typedef std::pair<LPDocUpdate, bool> Update;
	typedef std::list<Update> UpdateContainer;

	boost::filesystem::path  m_filePath;
	boost::filesystem::path  m_modelName;
	MemoryFileList           m_memoryFileList;
	StreamFileList           m_streamFileList;
	UpdateContainer          m_updateContainer;

	LPMemoryStream getMemoryStream(Type    type);
	LPFileStream   getFileStream  (TypeOut type);

	TypeOut typeToOut(const Type& typeIn) const;

	DECLARE_IDocument;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
