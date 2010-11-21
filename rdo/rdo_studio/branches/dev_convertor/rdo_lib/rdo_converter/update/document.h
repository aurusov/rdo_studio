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

	typedef std::map<Type, PTR(std::ofstream)> FileList;

	tstring  m_filePath;
	tstring  m_modelName;
	FileList m_fileList;

	REF(std::ofstream) getStream(Type type);
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_DOCUMENT_H_
