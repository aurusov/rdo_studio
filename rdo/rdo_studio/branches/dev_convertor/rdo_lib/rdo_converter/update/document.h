/*
 * copyright: (c) RDO-Team, 2010
 * filename : document.h
 * author   : Урусов Андрей
 * date     : 01.11.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_DOCUMENT_H_
#define _CONVERTOR_DOCUMENT_H_

// ====================================================================== INCLUDES
#include <fstream>
#include <boost/shared_ptr.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/rdomacros.h"

#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocument
// ----------------------------------------------------------------------------
OBJECT(Document)
{
DECLARE_FACTORY(Document)
public:
	typedef rdoModelObjectsConvertor::RDOFileTypeOut Type;

	tstring   getName  (Type type) const;
	void      close    ();
	void      write    (Type type, CPTR(char) buffer, ruint size);

private:
	 Document(CREF(tstring) filePath, CREF(tstring) modelName);
	~Document();

	class MemoryStream
	{
	public:
		typedef std::vector<char> Buffer;

		void write(CPTR(char) buffer, ruint size);
		void write(REF(std::ofstream) stream) const;

	private:
		Buffer m_buffer;
	};

	typedef boost::shared_ptr<std::ofstream> LPFileStream;
	typedef boost::shared_ptr<MemoryStream>  LPMemoryStream;

	struct TypeItem
	{
		LPFileStream   m_pFileStream;
		LPMemoryStream m_pMemoryStream;
	};
	typedef std::map<Type, TypeItem> FileList;

	tstring  m_filePath;
	tstring  m_modelName;
	FileList m_fileList;

	REF(TypeItem)  getItem        (Type type);
	LPFileStream   getFileStream  (Type type);
	LPMemoryStream getMemoryStream(Type type);
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_DOCUMENT_H_
