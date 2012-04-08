/*!
  \copyright (c) RDO-Team, 2011
  \file      document_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.01.2011
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_DOCUMENT_I_H_
#define _CONVERTOR_DOCUMENT_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/interface_ptr.h"

#include "converter/smr2rdox/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IDocument
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IDocument)
{
DECLARE_FACTORY(IDocument)
public:
	enum Type
	{
		UNDEFINED = 0,
		PAT, RTP, RSS, OPR, FRM,
		FUN, DPT, SMR, PMD, PMV,
		TRC, EVN, PRC
	};

	virtual void    insert(Type type, ruint to, CREF(tstring) value) = 0;
	virtual void    remove(Type type, ruint from, ruint to)          = 0;
	virtual tstring get   (Type type, ruint from, ruint to)          = 0;

protected:
	IDocument()
	{}
	virtual ~IDocument()
	{}
};
#define DECLARE_IDocument                                     \
	void    insert(Type type, ruint to, CREF(tstring) value); \
	void    remove(Type type, ruint from, ruint to);          \
	tstring get   (Type type, ruint from, ruint to);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_DOCUMENT_I_H_
