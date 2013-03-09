/*!
  \copyright (c) RDO-Team, 2011
  \file      document.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.11.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_DOCUMENT_H_
#define _CONVERTOR_DOCUMENT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"
#include "utils/rdomacros.h"

#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
#include "converter/smr2rdox/update/document_i.h"
#include "converter/smr2rdox/update/update_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Document
// --------------------------------------------------------------------------------
OBJECT(Document)
	IS IMPLEMENTATION_OF(IDocument)
{
DECLARE_FACTORY(Document)
public:
	typedef rdo::converter::smr2rdox::RDOFileTypeOut TypeOut;

	void    create      (CREF(tstring) filePath, CREF(tstring) modelName);
	void    init        (rdo::converter::smr2rdox::RDOFileTypeIn type, REF(std::ifstream) stream);
	void    insertUpdate(CREF(LPDocUpdate) pUpdate);
	void    convert     ();
	void    close       ();
	tstring getName     (TypeOut typeOut) const;

private:
	 Document();
	~Document();

	class MemoryStream
	{
	public:
		typedef std::vector<char> Buffer;

		void    init  (REF(std::ifstream) stream);
		void    get   (REF(std::ofstream) stream) const;

		void    insert(ruint to, CREF(tstring) value);
		void    remove(ruint from, ruint to);

		tstring get   (ruint from, ruint to);

	private:
		Buffer m_buffer;
	};

	typedef boost::shared_ptr<MemoryStream>  LPMemoryStream;
	typedef boost::shared_ptr<std::ofstream> LPFileStream;
	typedef std::map<Type, LPMemoryStream>   MemoryFileList;
	typedef std::map<TypeOut, LPFileStream>  StreamFileList;

	typedef std::pair<LPDocUpdate, rbool> Update;
	typedef std::list<Update>             UpdateContainer;

	tstring         m_filePath;
	tstring         m_modelName;
	MemoryFileList  m_memoryFileList;
	StreamFileList  m_streamFileList;
	UpdateContainer m_updateContainer;

	LPMemoryStream getMemoryStream(Type    type);
	LPFileStream   getFileStream  (TypeOut type);

	TypeOut typeToOut(CREF(Type) typeIn) const;

	DECLARE_IDocument;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_DOCUMENT_H_
