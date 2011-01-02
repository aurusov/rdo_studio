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
#include "rdo_lib/rdo_converter/update/document_i.h"
#include "rdo_lib/rdo_converter/update/update_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Document
// ----------------------------------------------------------------------------
OBJECT(Document)
	IS IMPLEMENTATION_OF(IDocument)
{
DECLARE_FACTORY(Document)
public:
	void    create      (CREF(tstring) filePath, CREF(tstring) modelName);
	void    init        (rdoModelObjectsConvertor::RDOFileTypeIn type, REF(std::ifstream) stream);
	void    insertUpdate(CREF(LPDocUpdate) pUpdate);
	void    convert     ();
	void    close       ();
	tstring getName     (Type type) const;

private:
	 Document();
	~Document();

	class MemoryStream
	{
	public:
		typedef std::vector<char> Buffer;

		void init  (REF(std::ifstream) stream);
		void get   (REF(std::ofstream) stream) const;

		void insert(ruint to, CREF(tstring) value);
		void remove(ruint from, ruint to);

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

	typedef std::pair<LPDocUpdate, rbool> Update;
	typedef std::list<Update>             UpdateContainer;

	tstring         m_filePath;
	tstring         m_modelName;
	FileList        m_fileList;
	UpdateContainer m_updateContainer;

	REF(TypeItem)  getItem        (Type type);
	LPFileStream   getFileStream  (Type type);
	LPMemoryStream getMemoryStream(Type type);

	DECLARE_IDocument;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_DOCUMENT_H_
